#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define N_THREAD 3
#define CAPIENZA 20
#define MAX_VALUE 100

typedef struct{
    int poison, round, size;
    int* arr;
}record_t;

typedef struct{
    record_t queue[CAPIENZA];
    pthread_mutex_t mtx;
    sem_t frees, fulls;
    int in, out;
}queue_t;

typedef struct{
    queue_t proposte, scartati;
    int n_vettori_da_gen, n_vettori_gen;
    pthread_mutex_t mtx_n_vettori;
}shared_t;

typedef struct{
    int n_vett_gen;
    record_t* r;
    shared_t* s;
}generatore_t;

typedef struct{
    int tid;
    record_t curr_r;
    shared_t* s;
}verificatore_t;

typedef struct{
    int tid, slot, old_v;
    record_t curr_r;
    shared_t* s;
}riparatore_t;

void init_record_data(record_t* r, int m){
    r->poison = r->round = 0;
    r->arr = malloc(sizeof(int) * m);
    r->size = m;
}

void init_queue_data(queue_t* q){
    pthread_mutex_init(&q->mtx, NULL);
    sem_init(&q->frees, 0, CAPIENZA);
    sem_init(&q->fulls, 0, 0);
    q->in = q->out = 0;
}

void init_shared_data(shared_t* s, int n){
    pthread_mutex_init(&s->mtx_n_vettori, NULL);
    s->n_vettori_da_gen = n;
    s->n_vettori_gen = 0;
    init_queue_data(&s->proposte);
    init_queue_data(&s->scartati);
}

void init_generatore_data(generatore_t* g, shared_t* s, int n, int m){
    g->n_vett_gen = 0;
    g->s = s;
    g->r = malloc(sizeof(record_t) * n);
    for(int i = 0; i < n; i++) init_record_data(&g->r[i], m);
}

void init_verificatore_data(verificatore_t* v, shared_t* s, int id){
    v->s = s;
    v->tid = id;
}

void init_riparatore_data(riparatore_t* r, shared_t* s, int id){
    r->s = s;
    r->tid = id;
}

void destroy_queue_sem_mtx(queue_t* q){
    pthread_mutex_destroy(&q->mtx);
    sem_destroy(&q->frees);
    sem_destroy(&q->fulls);
}

void destroy_shared_sem_mtx(shared_t* s){
    pthread_mutex_destroy(&s->mtx_n_vettori);
    destroy_queue_sem_mtx(&s->proposte);
    destroy_queue_sem_mtx(&s->scartati);
}

void enqueue_rec(queue_t* q, record_t r){
    sem_wait(&q->frees);
    pthread_mutex_lock(&q->mtx);
    q->queue[q->in] = r;
    q->in = (q->in + 1) % CAPIENZA;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->fulls);
}

record_t dequeue_rec(queue_t* q){
    record_t r;
    sem_wait(&q->fulls);
    pthread_mutex_lock(&q->mtx);
    r = q->queue[q->out]; 
    q->out = (q->out + 1) % CAPIENZA;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->frees);
    return r;
}

int is_mirror(int* arr, int size){
    int f_half = 0;
    int s_half = 0;
    for(int i = 0; i < size; i++){
        if(i < size/2){
            f_half += arr[i];
        }
        else{
            s_half += arr[i];
        }
    }
    return f_half == s_half;
}

void print_array(int* arr, int size){
    for(int i = 0; i < size - 1; i++){
        printf("%d, ", arr[i]);
    }
    printf("%d\n", arr[size - 1]);
}

void* routine_generatore(void* arg){
    generatore_t* g = (generatore_t*) arg;
    for(int i = 0; i < g->s->n_vettori_da_gen; i++){
        for(int j = 0; j < g->r->size; j++){
            g->r[i].arr[j] = rand() % MAX_VALUE; 
        }
        g->n_vett_gen++;

        enqueue_rec(&g->s->proposte, g->r[i]);
        flockfile(stdout);
        printf("GEN] vettore candidato n.%d: ", g->n_vett_gen);
        print_array(g->r[i].arr, g->r[i].size);
        funlockfile(stdout);
    }

    printf("[GEN] terminato\n");
    free(g->r);
    pthread_exit(NULL);
}

void* routine_verificatore(void* arg){
    verificatore_t* v = (verificatore_t*) arg;
    for(;;){
        v->curr_r = dequeue_rec(&v->s->proposte);

        if(v->curr_r.poison == 1){
            break;
        }

        flockfile(stdout);
        printf("[VER-%d] estratto un vettore candidato con round pari a %d: ", v->tid, v->curr_r.round);
        print_array(v->curr_r.arr, v->curr_r.size);
        funlockfile(stdout);

        if(is_mirror(v->curr_r.arr, v->curr_r.size)) {
            pthread_mutex_lock(&v->s->mtx_n_vettori);
            v->s->n_vettori_gen++;
            printf("[VER-%d] vettore verificato e accettato dopo %d round di aggiustamenti (%d di %d)\n", v->tid, v->curr_r.round, v->s->n_vettori_gen, v->s->n_vettori_da_gen);
            if(v->s->n_vettori_da_gen == v->s->n_vettori_gen){
                v->curr_r.poison = 1;
                for(int i = 0; i < N_THREAD; i++){
                    enqueue_rec(&v->s->scartati, v->curr_r);
                }
                for(int i = 0; i < N_THREAD - 1; i++){
                    enqueue_rec(&v->s->proposte, v->curr_r);
                }
                printf("[VER] terminato con %d vettori specchio trovati\n", v->s->n_vettori_gen);
                pthread_mutex_unlock(&v->s->mtx_n_vettori);

                break;
            }
            pthread_mutex_unlock(&v->s->mtx_n_vettori);
            free(v->curr_r.arr);
        }

        else {
            printf("[VER-%d] vettore non verificato e rigettato\n", v->tid);
            enqueue_rec(&v->s->scartati, v->curr_r);
        }

    }
    pthread_exit(NULL);
}

void* routine_riparatore(void* arg){
    riparatore_t* r = (riparatore_t*) arg;
    for(;;){
        r->curr_r = dequeue_rec(&r->s->scartati);

        if(r->curr_r.poison == 1) {
            break;
        }

        flockfile(stdout);
        printf("[REP-%d] estratto un vettore da riparare: ", r->tid);
        print_array(r->curr_r.arr, r->curr_r.size);
        funlockfile(stdout);

        r->curr_r.round++;
        r->slot = rand() % r->curr_r.size;
        r->old_v = r->curr_r.arr[r->slot];
        r->curr_r.arr[r->slot] = rand() % MAX_VALUE;

        enqueue_rec(&r->s->proposte, r->curr_r);

        flockfile(stdout);
        printf("[REP-%d] reinserito vettore riparato (%d->%d) con round pari a %d: ", r->tid, r->old_v, r->curr_r.arr[r->slot], r->curr_r.round);
        print_array(r->curr_r.arr, r->curr_r.size);
        funlockfile(stdout);
    }
    printf("[REP-%d] terminato\n", r->tid);
    pthread_exit(NULL);
}

//Usage: mirror-vector-generator <N> <M>
int main(int argc, char* argv[]){
    srand(time(NULL));

    if(argc != 3){
        fprintf(stderr,"Usage: %s <N> <M>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]); // numero di vettori
    int m = atoi(argv[2]); // numero di elementi per ogni vettore

    if(m % 2 != 0){
        fprintf(stderr, "Usage: <M> must be even\n");
        exit(1);
    }

    shared_t s;
    generatore_t g;
    verificatore_t v[N_THREAD];
    riparatore_t r[N_THREAD];

    init_shared_data(&s, n);
    init_generatore_data(&g, &s, n, m);
    for(int i = 0; i < N_THREAD; i++){
        init_verificatore_data(&v[i], &s, i + 1);
        init_riparatore_data(&r[i], &s, i + 1);
    }

    pthread_t generatore;
    pthread_t verificatore[N_THREAD];
    pthread_t riparatore[N_THREAD];

    printf("[MAIN] creazione di un thread generator, di %d thread verificatore e di %d thread riparatori\n", N_THREAD, N_THREAD);
    if(pthread_create(&generatore, NULL, routine_generatore, &g) != 0){
        perror("[!]");
        exit(1);
    }

    for(int i = 0; i < N_THREAD; i++){

        if(pthread_create(&verificatore[i], NULL, routine_verificatore, &v[i]) != 0){
            perror("[!]");
            exit(1);
        }

        if(pthread_create(&riparatore[i], NULL, routine_riparatore, &r[i]) != 0){
            perror("[!]");
            exit(1);
        }
    }

    pthread_join(generatore, NULL);
    for(int i = 0; i < N_THREAD; i++){
        pthread_join(verificatore[i], NULL);
        pthread_join(riparatore[i], NULL);
    }

    destroy_shared_sem_mtx(&s);
    printf("[MAIN] terminato\n");
    return 0;
}