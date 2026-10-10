#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

#define LUNGHEZZA_LATO 3
#define CAPIENZA_INTERMEDIA 10
#define CAPIENZA_FINALE 3

typedef struct{
    FILE* fd;
    char* filename;
}f_info_t;

typedef struct{
    int poison;
    int mat[LUNGHEZZA_LATO][LUNGHEZZA_LATO];
}record_t;   

typedef struct{
    record_t* coda;
    int size, in, out;
    sem_t liberi, pieni;
    pthread_mutex_t mtx;
}queue_t;

typedef struct{
    queue_t intermedia, finale;
    int n_lettori, n_verificatori;
    pthread_mutex_t mtx_n_lettori, mtx_n_verificatori;
}shared_t;

typedef struct{
    shared_t* s;
    int n_letti, tid;
    record_t curr_rec;
    f_info_t f;
}reader_t;

typedef struct{
    shared_t* s;
    int tid;
    record_t curr_rec;
}verifier_t;

void init_record_data(record_t* r){
    r->poison = 0;
}

void init_queue_data(queue_t* c, int size){
    c->coda = malloc(sizeof(record_t) * size);
    c->in = c->out = 0;
    c->size = size;
    pthread_mutex_init(&c->mtx, NULL);
    sem_init(&c->liberi, 0, size);
    sem_init(&c->pieni, 0, 0);
}

void init_shared_data(shared_t* s, int n, int m){
    s->n_lettori = n;
    s->n_verificatori = m;
    pthread_mutex_init(&s->mtx_n_lettori, NULL);
    pthread_mutex_init(&s->mtx_n_verificatori, NULL);
    init_queue_data(&s->finale, CAPIENZA_FINALE);
    init_queue_data(&s->intermedia, CAPIENZA_INTERMEDIA);
}

void init_reader_data(reader_t* r, shared_t* s, char* fname, int id){
    init_record_data(&r->curr_rec);
    r->n_letti = 0;
    r->s = s;
    r->tid = id;
    r->f.filename = fname;
}

void init_verifier_data(verifier_t* v, shared_t* s, int id){
    v->s = s;
    v->tid = id;
}

void destroy_queue(queue_t* q){
    pthread_mutex_destroy(&q->mtx);
    sem_destroy(&q->liberi);
    sem_destroy(&q->pieni);
    free(q->coda);
}

void destroy_shared(shared_t* s){
    pthread_mutex_destroy(&s->mtx_n_lettori);
    pthread_mutex_destroy(&s->mtx_n_verificatori);
    destroy_queue(&s->finale);
    destroy_queue(&s->intermedia);
}

void add_rec(queue_t* q, record_t r){
    sem_wait(&q->liberi);
    pthread_mutex_lock(&q->mtx);
    q->coda[q->in] = r;
    q->in = (q->in + 1) % q->size;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->pieni);
}

record_t extract_rec(queue_t* q){
    record_t r;
    sem_wait(&q->pieni);
    pthread_mutex_lock(&q->mtx);
    r = q->coda[q->out];
    q->out = (q->out + 1) % q->size;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->liberi);
    return r;
}

FILE* open_file(char* filename){
    FILE* fd = fopen(filename, "r");
    if(fd == NULL){
        perror("[!]");
        exit(1);
    }

    return fd;
}

int is_magic(int mat[][LUNGHEZZA_LATO]){
    int first_sum = 0;
    for(int i = 0; i < LUNGHEZZA_LATO; i++){
        first_sum += mat[i][0];
    }

    for(int i = 0; i < LUNGHEZZA_LATO; i++){
        int other_col_sum = 0;
        int other_row_sum = 0;
        for(int j = 0; j < LUNGHEZZA_LATO; j++){
            other_row_sum += mat[i][j];
            other_col_sum += mat[j][i];
        }
        if(other_col_sum != first_sum || other_row_sum != first_sum){
            return 0;
        }
    }

    int prim_diagonale = 0;
    int sec_diagonale = 0;
    for(int i = 0; i < LUNGHEZZA_LATO; i++){
        prim_diagonale += mat[i][i];
        sec_diagonale += mat[i][LUNGHEZZA_LATO - 1 - i];
    }
    if(sec_diagonale != first_sum || prim_diagonale != first_sum){
        return 0;
    }
    return 1;
}

void print_matrix(int arr[][LUNGHEZZA_LATO]){
    for(int i = 0; i < LUNGHEZZA_LATO; i++){
        printf("(");
        for(int j = 0; j < LUNGHEZZA_LATO - 1; j++){
            printf("%d, ", arr[i][j]);
        }
        printf("%d) ", arr[i][LUNGHEZZA_LATO - 1]); 
    }
    printf("\n");
}

void* routine_lettori(void* arg){
    reader_t* r = (reader_t*) arg;
    printf("[READER-%d] file '%s'\n", r->tid, r->f.filename);
    r->f.fd = open_file(r->f.filename);
    while(fscanf(r->f.fd, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", &r->curr_rec.mat[0][0], &r->curr_rec.mat[0][1], &r->curr_rec.mat[0][2], 
                                                        &r->curr_rec.mat[1][0], &r->curr_rec.mat[1][1], &r->curr_rec.mat[1][2], 
                                                        &r->curr_rec.mat[2][0], &r->curr_rec.mat[2][1], &r->curr_rec.mat[2][2]) == 9){
        add_rec(&r->s->intermedia, r->curr_rec);
        r->n_letti++;
        flockfile(stdout);
        printf("[READER-%d] quadrato candidato n.%d: ", r->tid, r->n_letti);
        print_matrix(r->curr_rec.mat);
        funlockfile(stdout);
    }

    pthread_mutex_lock(&r->s->mtx_n_lettori);
    r->s->n_lettori--;
    if(r->s->n_lettori == 0){
        r->curr_rec.poison = 1;
        pthread_mutex_lock(&r->s->mtx_n_verificatori);
        int n = r->s->n_verificatori;
        pthread_mutex_unlock(&r->s->mtx_n_verificatori);
        for(int i = 0; i < n; i++){
            add_rec(&r->s->intermedia, r->curr_rec);
        }
    }
    pthread_mutex_unlock(&r->s->mtx_n_lettori);
    printf("[READER-%d] terminazione\n", r->tid);
    fclose(r->f.fd);
    pthread_exit(NULL);
}

void* routine_verificatori(void* arg){
    verifier_t* v = (verifier_t*) arg;
    for(;;){
        v->curr_rec = extract_rec(&v->s->intermedia);
        if(v->curr_rec.poison){
            break;
        }

        flockfile(stdout);
        printf("[VERIF-%d] verifico quadrato: ", v->tid);
        print_matrix(v->curr_rec.mat);
        funlockfile(stdout);

        if(is_magic(v->curr_rec.mat)){
            add_rec(&v->s->finale, v->curr_rec);
            printf("[VERIF-%d] trovato quadrato magico!\n", v->tid);
        }
    }
    pthread_mutex_lock(&v->s->mtx_n_verificatori);
    v->s->n_verificatori--;
    if(v->s->n_verificatori == 0){
        add_rec(&v->s->finale, v->curr_rec);
    }
    pthread_mutex_unlock(&v->s->mtx_n_verificatori);
    printf("[VER-%d] terminazione\n", v->tid);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    if(argc < 3){
        fprintf(stderr, "Usage: %s <M-verifiers> <file-1> ...  <file-N>\n", argv[0]);
        exit(1);
    }

    int m = atoi(argv[1]);
    int n = argc - 2;

    record_t rec;
    shared_t s;
    reader_t r[n];
    verifier_t v[m];

    init_shared_data(&s, n, m);
    for(int i = 0; i < n; i++) init_reader_data(&r[i], &s, argv[i + 2], i + 1);
    for(int i = 0; i < m; i++) init_verifier_data(&v[i], &s, i + 1);

    pthread_t lettore[n];
    pthread_t verificatore[m];

    printf("[MAIN] creazione di %d thread lettori e %d thread verificatori\n", n, m);

    for(int i = 0; i < n; i++){
        if(pthread_create(&lettore[i], NULL, routine_lettori, &r[i]) != 0){
            perror("[!]");
            exit(1);
        }
    }

    for(int i = 0; i < m; i++){
        if(pthread_create(&verificatore[i], NULL, routine_verificatori, &v[i]) != 0){
            perror("[!]");
            exit(1);
        }
    }

    for(;;){
        rec = extract_rec(&s.finale);
        if(rec.poison){
            break;
        }
        int somma = 0;
        for(int i = 0; i < LUNGHEZZA_LATO; i++){
            somma += rec.mat[0][i];
        }

        flockfile(stdout);
        printf("[MAIN] quadrato magico trovato:\n");
        for(int i = 0; i < LUNGHEZZA_LATO; i++){
            printf("(");
            for(int j = 0; j < LUNGHEZZA_LATO - 1; j++){
                printf("%d, ", rec.mat[i][j]);
            }
            printf("%d)\n", rec.mat[i][LUNGHEZZA_LATO - 1]); 
        }
        printf("totale: %d\n", somma);
        funlockfile(stdout);
    }

    for(int i = 0; i < n; i++) pthread_join(lettore[i], NULL);
    for(int i = 0; i < m; i++) pthread_join(verificatore[i], NULL);

    printf("[MAIN] terminazione\n");
    destroy_shared(&s);
    return 0;
}