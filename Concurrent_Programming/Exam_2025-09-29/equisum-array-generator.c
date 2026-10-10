#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define CAPIENZA 5
#define MAX_VAL 100

typedef struct{
    int poison, round, size;
    int* arr;
}record_t;

typedef struct{
    record_t coda[CAPIENZA];
    int in, out;
    sem_t pieni, liberi;
    pthread_mutex_t mtx;
}queue_t;

typedef struct{
    queue_t proposte, scartati;
    int n_vett_gen, n_da_gen, n_fixer;
    pthread_mutex_t mtx_n_vett, mtx_n_fixer;
}shared_t;

typedef struct{
    shared_t* s;
    int n_vett, n_da_gen;
    record_t* r;
}generator_t;

typedef struct{
    shared_t* s;
    record_t curr_rec;
}verifier_t;

typedef struct{
    shared_t* s;
    record_t curr_rec;
    int tid, old_val, slot;
}fixer_t;

void init_record_data(record_t* r, int n){
    r->arr = malloc(sizeof(int) * n);
    r->poison = r->round = 0;
    r->size = n;
}

void init_queue_data(queue_t* q){
    q->in = q->out = 0;
    pthread_mutex_init(&q->mtx, NULL);
    sem_init(&q->liberi, 0, CAPIENZA);
    sem_init(&q->pieni, 0, 0);
}

void init_shared_data(shared_t* s, int t, int r){
    s->n_da_gen = t;
    s->n_vett_gen = 0;
    s->n_fixer = r;
    pthread_mutex_init(&s->mtx_n_vett, NULL);
    pthread_mutex_init(&s->mtx_n_fixer, NULL);
    init_queue_data(&s->proposte);
    init_queue_data(&s->scartati);
}

void init_generator_data(generator_t* g, shared_t* s, int t, int n){
    g->n_vett = 0;
    g->s = s;
    g->n_da_gen = s->n_da_gen;
    g->r = malloc(sizeof(record_t) * t);
    for(int i = 0; i < t; i++) init_record_data(&g->r[i], n);
}

void init_verifier_data(verifier_t* v, shared_t* s){
    v->s = s;
}

void init_fixer_data(fixer_t* f, shared_t* s, int id){
    f->old_val = f->slot = -1;
    f->tid = id;
    f->s = s;
}

void shutdown_record(record_t* r){
    free(r->arr);
}

void shutdown_generator(generator_t* g, int t){
    for(int i = 0; i < t; i++){
        shutdown_record(&g->r[i]);
    }
    free(g->r);
}

void destroy_queue_sem_mtx(queue_t* q){
    pthread_mutex_destroy(&q->mtx);
    sem_destroy(&q->liberi);
    sem_destroy(&q->pieni);
}

void destroy_shared_sem_mtx(shared_t* s){
    pthread_mutex_destroy(&s->mtx_n_vett);
    destroy_queue_sem_mtx(&s->proposte);
    destroy_queue_sem_mtx(&s->scartati);
}

void add_rec(queue_t* q, record_t r){
    sem_wait(&q->liberi);
    pthread_mutex_lock(&q->mtx);
    q->coda[q->in] = r;
    q->in = (q->in + 1) % CAPIENZA;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->pieni);
}

record_t extract_rec(queue_t* q){
    record_t r;
    sem_wait(&q->pieni);
    pthread_mutex_lock(&q->mtx);
    r = q->coda[q->out];
    q->out = (q->out + 1) % CAPIENZA;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->liberi);
    return r;
}

void print_array(record_t r){
    for(int i = 0; i < r.size - 1; i++){
        printf("%d, ", r.arr[i]);
    }
    printf("%d\n", r.arr[r.size - 1]);
}

int is_equisum(int* arr, int size){
    int somma_pari = 0;
    int somma_dispari = 0;

    for(int i = 0; i < size; i++){
        if(i % 2 == 0){
            somma_pari += arr[i]; 
        }
        else{
            somma_dispari += arr[i]; 
        }
    }

    return somma_pari == somma_dispari;
}

void* routine_generatore(void* arg){
    generator_t* g = (generator_t*) arg;
    for(int i = 0; i < g->n_da_gen; i++){
        for(int j = 0; j < g->r[i].size; j++){
            g->r[i].arr[j] = rand() % MAX_VAL;
        }
        add_rec(&g->s->proposte, g->r[i]);
        g->n_vett++;
        flockfile(stdout);
        printf("[GEN] vettore candidato n.%d: ", g->n_vett);
        print_array(g->r[i]);
        funlockfile(stdout);
    }

    printf("[GEN] terminato\n");
    pthread_exit(NULL);
}

void* routine_verificatore(void* arg){
    verifier_t* v = (verifier_t*) arg;
    for(;;){
        v->curr_rec = extract_rec(&v->s->proposte);
        flockfile(stdout);
        printf("[VER] estratto un vettore candidato con round pari a %d: ", v->curr_rec.round);
        print_array(v->curr_rec);
        funlockfile(stdout);

        if(is_equisum(v->curr_rec.arr, v->curr_rec.size)){
            pthread_mutex_lock(&v->s->mtx_n_vett);
            v->s->n_vett_gen++;
            printf("[VER] vettore verificato e accettato dopo %d round di aggiustamenti (%d di %d)\n", v->curr_rec.round, v->s->n_vett_gen, v->s->n_da_gen);
            if(v->s->n_da_gen == v->s->n_vett_gen){
                v->curr_rec.poison = 1;
                pthread_mutex_lock(&v->s->mtx_n_fixer);
                int n_f = v->s->n_fixer;
                pthread_mutex_unlock(&v->s->mtx_n_fixer);
                for(int i = 0; i < n_f; i++){
                    add_rec(&v->s->scartati, v->curr_rec);
                }

                pthread_mutex_unlock(&v->s->mtx_n_vett);
                break;
            }
            pthread_mutex_unlock(&v->s->mtx_n_vett);
        }
        else{
            printf("[VER] vettore non verificato e rigettato\n");
            add_rec(&v->s->scartati, v->curr_rec);
        }
    }

    printf("[VER] terminato\n");
    pthread_exit(NULL);
}

void* routine_riparatore(void* arg){
    fixer_t* f = (fixer_t*) arg;
    for(;;){
        f->curr_rec = extract_rec(&f->s->scartati);

        if(f->curr_rec.poison == 1) break;

        flockfile(stdout);
        printf("[REP-%d] estratto un vettore da riparare: ", f->tid);
        print_array(f->curr_rec);
        funlockfile(stdout);

        f->curr_rec.round++;
        f->slot = rand() % f->curr_rec.size;
        f->old_val = f->curr_rec.arr[f->slot];
        f->curr_rec.arr[f->slot] = rand() % MAX_VAL;

        add_rec(&f->s->proposte, f->curr_rec);

        flockfile(stdout);
        printf("[REP-%d] reinserito vettore riparato (%d->%d) con round pari a %d: ", f->tid, f->old_val, f->curr_rec.arr[f->slot], f->curr_rec.round);
        print_array(f->curr_rec);
        funlockfile(stdout);

    }

    printf("[REP-%d] terminato\n", f->tid);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    if(argc != 4 ){
        fprintf(stderr,"Usage: %s <N> <T> <R>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]); // numero di elementi per array
    int t = atoi(argv[2]); // numero di array
    int r = atoi(argv[3]); // numero di riparatori

    shared_t s;
    generator_t g;
    verifier_t v;
    fixer_t f[r];

    init_shared_data(&s, t, r);
    init_generator_data(&g, &s, t, n);
    init_verifier_data(&v, &s);
    for(int i = 0; i < r; i++) init_fixer_data(&f[i], &s, i + 1);

    pthread_t generatore, verificatore;
    pthread_t riparatore[r];

    if(pthread_create(&generatore, NULL, routine_generatore, &g) != 0){
        perror("[!]");
        exit(1);
    }

    if(pthread_create(&verificatore, NULL, routine_verificatore, &v) != 0){
        perror("[!]");
        exit(1);
    }

    for(int i = 0; i < r; i++){
        if(pthread_create(&riparatore[i], NULL, routine_riparatore, &f[i]) != 0){
            perror("[!]");
            exit(1);
        }
    }

    pthread_join(generatore, NULL);
    pthread_join(verificatore, NULL);
    for(int i = 0; i < r; i++) pthread_join(riparatore[i], NULL);

    printf("[MAIN] terminato\n");
    shutdown_generator(&g, t);
    destroy_shared_sem_mtx(&s);
    return 0;
}