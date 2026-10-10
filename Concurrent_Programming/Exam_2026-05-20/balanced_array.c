#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_VALUE 100
#define N_VER 2
#define N_FIX 3
#define CAPACITY 15

/**
 * STRUCT RECORD
 */
typedef struct{
    int poison, round, size;
    int* arr;
}record_t;

/**
 * STRUCT CODA
 */
typedef struct{
    record_t queue[CAPACITY];
    int in, out;
    sem_t free_slots, full_slots;
    pthread_mutex_t mtx;
}queue_t;

/**
 * STRUCT CONDIVISA
 */
typedef struct{
    queue_t proposte, scartati;
    int n_vettori_da_gen, n_vettori_gen;
    pthread_mutex_t mtx_n_vettori_gen;
}shared_t;

/**
 * STRUCT GENERATORI
 */
typedef struct{
    record_t* records;
    int n_vettori_gen, n_vettori_da_gen;
    shared_t* s;
}generatore_t;

/**
 * STRUCT VERIFICATORI
 */
typedef struct{
    record_t curr_rec;
    int n_pari, n_dispari, tid;
    shared_t* s;
}verificatore_t;

/**
 * STRUCT RIPARATORI
 */
typedef struct{
    record_t curr_rec;
    int tid, old_value, slot_to_change;
    shared_t* s;
}riparatore_t;

/**
 * FUNZIONI PER INIZIALIZZARE
 */
void init_record_data(record_t* r, int m){
    r->arr = malloc(sizeof(int) * m);
    r->poison = r->round = 0;
    r->size = m;
}

void init_queue_data(queue_t* q){
    pthread_mutex_init(&q->mtx, NULL);
    sem_init(&q->free_slots, 0, CAPACITY);
    sem_init(&q->full_slots, 0, 0);
    q->in = q->out = 0;
}

void init_shared_data(shared_t* s, int n){
    pthread_mutex_init(&s->mtx_n_vettori_gen, NULL);
    init_queue_data(&s->proposte);
    init_queue_data(&s->scartati);
    s->n_vettori_da_gen = n;
    s->n_vettori_gen = 0;
}

void init_generatore_data(generatore_t* g, shared_t* s, int n, int m){
    g->n_vettori_gen = 0;
    g->n_vettori_da_gen = n;
    g->records = malloc(sizeof(record_t) * n);
    for(int i = 0; i < n; i++) init_record_data(&g->records[i], m);
    g->s = s;
}

void init_verificatore_data(verificatore_t* v, shared_t* s, int id){
    v->n_dispari = 0;
    v->n_pari = 0;
    v->tid = id;
    v->s = s;

}

void init_riparatore_data(riparatore_t* r, shared_t* s, int id){
    r->old_value = r->slot_to_change = -1;
    r->s = s;
    r->tid = id;
}

/**
 * FUNZIONE PER DISTRUGGERE MUTEX E SEMAFORI
 */
void delete_queue_sem_mtx(queue_t* q){
    pthread_mutex_destroy(&q->mtx);
    sem_destroy(&q->free_slots);
    sem_destroy(&q->full_slots);
}

void delete_shared_sem_mtx(shared_t* s){
    pthread_mutex_destroy(&s->mtx_n_vettori_gen);
    delete_queue_sem_mtx(&s->scartati);
    delete_queue_sem_mtx(&s->proposte);
}

/**
 * FUNZIONE PER LIBERARE LA MEMORIA DINAMICA
 */
void free_generatore_data(generatore_t* g) {
    if (g->records != NULL) {
        for (int i = 0; i < g->n_vettori_da_gen; i++) {
            free(g->records[i].arr); // Libera l'array di interi
        }
        free(g->records); // Libera l'array delle struct record_t
    }
}

/**
 * FUNZIONE PER AGGIUNGERE RECORD NELLE CODE
 */
void enqueue(queue_t* q, record_t r){
    sem_wait(&q->free_slots);
    pthread_mutex_lock(&q->mtx);
    q->queue[q->in] = r;
    q->in = (q->in + 1) % CAPACITY;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->full_slots);
}

/**
 * FUNZIONE PER ESTRARRE RECORD DALLE CODE
 */
record_t dequeue(queue_t* q){
    record_t r;
    sem_wait(&q->full_slots);
    pthread_mutex_lock(&q->mtx);
    r = q->queue[q->out];
    q->out = (q->out + 1) % CAPACITY;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->free_slots);
    return r;
}

/**
 * FUNZIONE PER VERIFICARE PROPRIETÀ
 */
int is_balanced(verificatore_t* v){
    v->n_dispari = 0;
    v->n_pari = 0;
    for(int i = 0; i < v->curr_rec.size; i++){
        if(v->curr_rec.arr[i] % 2 == 0){
            v->n_pari++;
        }
        else{
            v->n_dispari++;
        }
    }
    return v->n_pari == v->n_dispari;
}

/**
 * FUNZIONE PER STAMPARE ARRAY
 */
void print_array(int* arr, int size){
    for(int i = 0; i < size - 1; i++){
        printf("%d, ", arr[i]);
    }
    printf("%d\n", arr[size - 1]);
}

/**
 * ROUTINE GENERATORE
 */
void* routine_generatore(void* arg){
    generatore_t* g = (generatore_t*) arg;
    for(int i = 0; i < g->n_vettori_da_gen; i++){
        for(int j = 0; j < g->records[i].size; j++){
            g->records[i].arr[j] = rand() % MAX_VALUE;
        }

        enqueue(&g->s->proposte, g->records[i]);
        g->n_vettori_gen++;
        flockfile(stdout);
        printf("[GEN] vettore candidato n.%d: ", g->n_vettori_gen);
        print_array(g->records[i].arr, g->records[i].size);
        funlockfile(stdout);
    }
    printf("[GEN] terminato\n");
    pthread_exit(NULL);
}

/**
 * ROUTINE VERIFICATORE
 */
void* routine_verificatore(void* arg){
    verificatore_t* v = (verificatore_t*) arg;
    for(;;){
        v->curr_rec = dequeue(&v->s->proposte);
        if(v->curr_rec.poison == 1){
            break;
        }
        flockfile(stdout);
        printf("VER-%d] estratto un vettore candidato con round pari a %d: ", v->tid, v->curr_rec.round);
        print_array(v->curr_rec.arr, v->curr_rec.size);
        funlockfile(stdout);
        if(is_balanced(v)){
            pthread_mutex_lock(&v->s->mtx_n_vettori_gen);
            v->s->n_vettori_gen++;
            printf("[VER-%d] vettore verificato e accettato dopo %d round di aggiustamenti (%d di %d)\n", v->tid, v->curr_rec.round, v->s->n_vettori_gen, v->s->n_vettori_da_gen);
            if(v->s->n_vettori_gen == v->s->n_vettori_da_gen){
                v->curr_rec.poison = 1;

                for(int i = 0; i < N_FIX; i++) enqueue(&v->s->scartati, v->curr_rec);
                for(int i = 0; i < N_VER - 1; i++) enqueue(&v->s->proposte, v->curr_rec);
                
                printf("[VER] terminato con %d vettori bilanciati trovati\n", v->s->n_vettori_gen);
                pthread_mutex_unlock(&v->s->mtx_n_vettori_gen);
                break;
            }
            pthread_mutex_unlock(&v->s->mtx_n_vettori_gen);
        }
        else{
            printf("[VER-%d] vettore non verificato e rigettato (%d pari vs. %d dispari)\n", v->tid, v->n_pari, v->n_dispari);
            enqueue(&v->s->scartati, v->curr_rec);
        }
    }
    pthread_exit(NULL);
}

/**
 * ROUTINE RIPARATORE
 */
void* routine_riparatore(void* arg){
    riparatore_t* r = (riparatore_t*) arg;
    for(;;){
        r->curr_rec = dequeue(&r->s->scartati);
        if(r->curr_rec.poison == 1){
            break;
        }
        flockfile(stdout);
        printf("[REP-%d] estratto un vettore da riparare: ", r->tid);
        print_array(r->curr_rec.arr, r->curr_rec.size);
        funlockfile(stdout);
        r->slot_to_change = rand() % r->curr_rec.size;
        r->old_value = r->curr_rec.arr[r->slot_to_change];
        r->curr_rec.arr[r->slot_to_change] = rand() % MAX_VALUE;
        r->curr_rec.round++;
        enqueue(&r->s->proposte, r->curr_rec);
        flockfile(stdout);
        printf("[REP-%d] reinserito vettore riparato (%d->%d) con round pari a %d: ", r->tid, r->old_value, r->curr_rec.arr[r->slot_to_change], r->curr_rec.round);
        print_array(r->curr_rec.arr, r->curr_rec.size);
        funlockfile(stdout);
    }
    printf("[REP-%d] terminato\n", r->tid);
    pthread_exit(NULL);
}

/**
 * FUNZIONE MAIN
 * Usage: ./a.out <N> <M>
 */
int main(int argc, char* argv[]){
    srand(time(NULL));

    if(argc != 3){
        fprintf(stderr,"Usage: %s, <N> <M>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    if(m % 2 != 0){
        fprintf(stderr,"<M> must be even\n");
        exit(1);
    }

    shared_t s;
    generatore_t g;
    verificatore_t v[N_VER];
    riparatore_t r[N_FIX];

    init_shared_data(&s, n);
    init_generatore_data(&g, &s, n, m);
    for(int i = 0; i < N_VER; i++) init_verificatore_data(&v[i], &s, i);
    for(int i = 0; i < N_FIX; i++) init_riparatore_data(&r[i], &s, i);

    pthread_t thread_generatore;
    pthread_t thread_verificatore[N_VER];
    pthread_t thread_riparatore[N_FIX];

    printf("[MAIN] creazione di un thread generator, di %d thread verificatore e di %d thread riparatori\n", N_VER, N_FIX);

    if(pthread_create(&thread_generatore, NULL, routine_generatore, &g) != 0){
        perror("[!]ERROR occured while creating generator thread");
        exit(1);
    }
    for(int i = 0; i < N_VER; i++){
        if(pthread_create(&thread_verificatore[i], NULL, routine_verificatore, &v[i]) != 0){
            perror("[!]ERROR occured while creating checker thread");
            exit(1);
        }
    }
    for(int i = 0; i < N_FIX; i++){
        if(pthread_create(&thread_riparatore[i], NULL, routine_riparatore, &r[i]) != 0){
            perror("[!]ERROR occured while creating fixer thread");
            exit(1);
        }
    }

    pthread_join(thread_generatore, NULL);
    for(int i = 0; i < N_VER; i++) pthread_join(thread_verificatore[i], NULL);
    for(int i = 0; i < N_FIX; i++) pthread_join(thread_riparatore[i], NULL);

    delete_shared_sem_mtx(&s);
    free_generatore_data(&g);
    printf("MAIN] terminato\n");
    return 0;

}