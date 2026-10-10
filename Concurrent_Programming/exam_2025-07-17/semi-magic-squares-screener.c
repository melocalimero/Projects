#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define CAPIENZA 5
#define MAX_SIZE 16
#define MIN_SIZE 3

/**
 * STRUTTURA FILE INFO
 */
typedef struct{
    char* filename;
    int fd, dimension;
    char* f_content;
}f_info_t;

/**
 * STRUTTURA RECORD
 */
typedef struct{
    int poison, size;
    int mat[MAX_SIZE][MAX_SIZE];
}record_t;

/**
 * STRUTTURA CODA
 */
typedef struct{
    record_t queue[CAPIENZA];
    int out, in;
    sem_t free_slots, full_slots;
    pthread_mutex_t mtx;
}queue_t;

/**
 * STRUTTURA CONDIVISA
 */
typedef struct{
    queue_t intermedia;
    record_t finale;
    pthread_mutex_t mtx_finale, mtx_n_lettori;
    sem_t free_finale, full_finale;
    int n_lettori_attivi;
}shared_t;

/**
 * STRUTTURA LETTORI
 */
typedef struct{
    int tid;
    f_info_t f;
    int n_quadrati_letti;
    record_t curr_rec;
    shared_t* s;
}lettore_t;

/**
 * STRUTTURA VERIFICATORE
 */
typedef struct{
    record_t curr_rec;
    shared_t* s;
}verificatore_t;

/**
 * FUNZIONI PER INIZIALIZZARE
 */
void init_record_data(record_t* r, int n){
    r->poison = 0;
    r->size = n;
}

void init_queue_data(queue_t* q){
    pthread_mutex_init(&q->mtx, NULL);
    sem_init(&q->free_slots, 0, CAPIENZA);
    sem_init(&q->full_slots, 0, 0);
    q->in = q->out = 0;
}

void init_shared_data(shared_t* s, int n){
    pthread_mutex_init(&s->mtx_n_lettori, NULL);
    pthread_mutex_init(&s->mtx_finale, NULL);
    init_queue_data(&s->intermedia);
    sem_init(&s->free_finale, 0, 1);
    sem_init(&s->full_finale, 0, 0);
    s->n_lettori_attivi = n;
}

void init_lettore_data(lettore_t* l, shared_t* s, char* filename, int id, int n){
    l->s = s;
    l->f.filename = filename;
    l->n_quadrati_letti = 0;
    l->tid = id;
    init_record_data(&l->curr_rec, n);
}

void init_verificatore_data(verificatore_t* v, shared_t* s){
    v->s = s;
}

/**
 * FUNZIONI PER DISTRUGGERE SEMAFORI E MUTEX
 */
void destroy_queue_sem_mtx(queue_t* q){
    pthread_mutex_destroy(&q->mtx);
    sem_destroy(&q->free_slots);
    sem_destroy(&q->full_slots);
}

void destroy_shared_sem_mtx(shared_t* s){
    pthread_mutex_destroy(&s->mtx_finale);
    pthread_mutex_destroy(&s->mtx_n_lettori);
    sem_destroy(&s->free_finale);
    sem_destroy(&s->full_finale);
    destroy_queue_sem_mtx(&s->intermedia);
}

/**
 * FUNZIONE PER ACCODARE E DECODARE
 */
void enqueue_record(queue_t* q, record_t r){
    sem_wait(&q->free_slots);
    pthread_mutex_lock(&q->mtx);
    q->queue[q->in] = r;
    q->in = (q->in + 1) % CAPIENZA;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->full_slots);
}

record_t dequeue_record(queue_t* q){
    record_t r;
    sem_wait(&q->full_slots);
    pthread_mutex_lock(&q->mtx);
    r = q->queue[q->out];
    q->out = (q->out + 1) % CAPIENZA;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->free_slots);
    return r;
}

/**
 * FUNZIONI PER AGGIUNGERE ED ESTRARRE RECORD FINALE
 */
void add_rec_finale(shared_t* s, record_t r){
    sem_wait(&s->free_finale);
    pthread_mutex_lock(&s->mtx_finale);
    s->finale = r;
    pthread_mutex_unlock(&s->mtx_finale);
    sem_post(&s->full_finale);
}

record_t extract_rec_finale(shared_t* s){
    record_t r;
    sem_wait(&s->full_finale);
    pthread_mutex_lock(&s->mtx_finale);
    r = s->finale;
    pthread_mutex_unlock(&s->mtx_finale);
    sem_post(&s->free_finale);
    return r;
}

/**
 * FUNZIONE PER VERIFICARE PROPRIERTÀ
 */
int is_semi_magic(int arr[][MAX_SIZE], int size){
    int first_sum = 0;
    for(int i = 0; i < size; i++){
        first_sum += arr[0][i];
    }

    for(int i = 0; i < size; i++){
        int other_row_sum = 0;
        int other_col_sum = 0;
        for(int j = 0; j < size; j++){
            other_row_sum += arr[i][j];
            other_col_sum += arr[j][i];
        }
        if(other_row_sum != first_sum || other_col_sum != first_sum){
            return 0;
        }
    }
    return 1;
}

/**
 * FUNZIONE PER MAPPARE FILE
 */
void map_file(char* filename, lettore_t* l){
    int fd = open(filename, O_RDONLY);
    if(fd < 0){
        perror("[!]");
        exit(1);
    }

    struct stat f_stat;
    if(fstat(fd, &f_stat) < 0){
        perror("[!]");
        exit(1);
    }

    int dimension = f_stat.st_size;
    char* f_content = mmap(NULL, dimension, PROT_READ, MAP_PRIVATE, fd, 0);
    if(f_content == MAP_FAILED){
        perror("[!]");
        exit(1);
    }

    l->f.dimension = dimension;
    l->f.f_content = f_content;
    l->f.fd = fd;
}

/**
 * FUNZIONE PER STAMPARE MATRICE
 */
void print_matrix(int mat[][MAX_SIZE], int size){
    for(int i = 0; i < size; i++){
        printf("(");
        for(int j = 0; j < size - 1; j++){
            printf("%d, ", mat[i][j]);
        }
        printf("%d)", mat[i][size - 1]);
    }
    printf("\n");
}

/**
 * ROUTINE LETTORI
 */
void* routine_lettore(void* arg){
    lettore_t* l = (lettore_t*) arg;
    printf("[READER-%d] file '%s'\n", l->tid, l->f.filename);
    map_file(l->f.filename, l);
    for(int i = 0; i + (l->curr_rec.size * l->curr_rec.size) <= l->f.dimension; i += l->curr_rec.size * l->curr_rec.size){
        for(int j = 0; j < l->curr_rec.size; j++){
            for(int k = 0; k < l->curr_rec.size; k++){
                l->curr_rec.mat[j][k] = l->f.f_content[i + (j * l->curr_rec.size) + k];
            }
        }
        l->n_quadrati_letti++;

        enqueue_record(&l->s->intermedia, l->curr_rec);
        flockfile(stdout);
        printf("[READER-2] quadrato candidato n.%d: ", l->n_quadrati_letti);
        print_matrix(l->curr_rec.mat, l->curr_rec.size);
        funlockfile(stdout);
    }

    pthread_mutex_lock(&l->s->mtx_n_lettori);
    l->s->n_lettori_attivi--;
    if(l->s->n_lettori_attivi == 0){
        l->curr_rec.poison = 1;
        enqueue_record(&l->s->intermedia, l->curr_rec);
    }
    pthread_mutex_unlock(&l->s->mtx_n_lettori);
    printf("[READER-%d] terminazione con %d quadrati letti\n", l->tid, l->n_quadrati_letti);
    pthread_exit(NULL);
}

/**
 * ROUTINE VERIFICATORE
 */
void* routine_verificatore(void* arg){
    verificatore_t* v = (verificatore_t*) arg;
    for(;;){
        v->curr_rec = dequeue_record(&v->s->intermedia);
        if(v->curr_rec.poison == 1){
            break;
        }
        flockfile(stdout);
        printf("[VERIF] verifico quadrato: ");
        print_matrix(v->curr_rec.mat, v->curr_rec.size);
        funlockfile(stdout);
        if(is_semi_magic(v->curr_rec.mat, v->curr_rec.size)){
            printf("[VERIF] trovato quadrato semi-magico!\n");
            add_rec_finale(v->s, v->curr_rec);
        }
    }
    printf("[VERIF] terminazione\n");
    add_rec_finale(v->s, v->curr_rec);
    pthread_exit(NULL);
}

/**
 * MAIN
 * Usage: semi-magic-squares-screener <M-square-size> <bin-file-1> ...  <bin-file-N>
 */
int main(int argc, char* argv[]){
    if(argc < 3){
        fprintf(stderr, "Usage: %s <M-square-size> <bin-file-1> ...  <bin-file-N>\n", argv[0]);
        exit(1);
    }

    int m = atoi(argv[1]);
    if(m < 3 || m > 16){
        fprintf(stderr, "Usage: <M> must stay between %d and %d\n", MIN_SIZE, MAX_SIZE);
        exit(1);
    }
    int n = argc - 2;
    int n_quadrati = 0;

    record_t r;
    shared_t s;
    lettore_t l[n];
    verificatore_t v;

    init_shared_data(&s, n);
    init_verificatore_data(&v, &s);
    for(int i = 0; i < n; i++) init_lettore_data(&l[i], &s, argv[i + 2], i + 1, m);

    pthread_t thread_lettore[n];
    pthread_t thread_verificatore;

    printf("[MAIN] creazione di %d thread lettori e 1 thread verificatore\n", n);
    for(int i = 0; i < n; i++){
        if(pthread_create(&thread_lettore[i], NULL, routine_lettore, &l[i]) != 0){
            perror("[!]ERROR occured while creating reader threads");
        }
    }

    if(pthread_create(&thread_verificatore, NULL, routine_verificatore, &v) != 0){
        perror("[!]ERROR occured while creating verifier threads");
    }

    for(;;){
        r = extract_rec_finale(&s);
        if(r.poison == 1){
            break;
        }

        n_quadrati++;
        int totale = 0;
        for(int i = 0; i < r.size; i++){
            totale += r.mat[i][0];
        }
        flockfile(stdout);
        printf("[MAIN] quadrato semi-magico trovato:\n");
        for(int i = 0; i < r.size; i++){
            printf("  (");
            for(int j = 0; j < r.size - 1; j++){
                printf("%d, ", r.mat[i][j]);
            }
            printf("%d)\n", r.mat[i][r.size - 1]);
        }
        printf("  totale semi-magico: %d\n", totale);
        funlockfile(stdout);
    }

    for(int i = 0; i < n; i++) pthread_join(thread_lettore[i], NULL);
    pthread_join(thread_verificatore, NULL);

    destroy_shared_sem_mtx(&s);
    printf("[MAIN] terminazione con %d quadrati semi-magici trovati\n", n_quadrati);
    return 0;
}