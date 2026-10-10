#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define INTERMEDIATE_CAPACITY 10
#define N_CHECKERS 3
#define N_ELEMENTS 12

/**
 * FILE INFO STRUCT
 */
typedef struct{
    int fd, size;
    char* filename;
    unsigned char* file_content;
}file_info_t;

/**
 * RECORD STRUCT
 */
typedef struct{
    int arr[N_ELEMENTS];
    u_int8_t poison;
    int size;
}record_t;

/**
 * SHARED STRUCT
 */
typedef struct{
    record_t intermediate[INTERMEDIATE_CAPACITY];
    record_t final;
    sem_t empty_int, empty_fin, full_int, full_fin;
    pthread_mutex_t mtx_int, mtx_fin, mtx_n_readers, mtx_n_checkers;
    int int_in, int_out, n_active_readers, n_active_checkers;
}shared_t;

/**
 * READERS STRUCT
 */
typedef struct{
    int tid, n_arrays_checked;
    file_info_t f_info;
    record_t curr_rec;
    shared_t* s;
}reader_t;

/**
 * CHECKER STRUCT
 */
typedef struct{
    record_t curr_rec;
    int tid, curr_odd_sum, curr_even_sum, n_verified_arr;
    shared_t* s;
}checker_t;

/**
 * FUNCTION TO INITIALIZE
 */
void init_record_data(record_t* r){
    r->poison = 0;
    r->size = N_ELEMENTS;
}

void init_shared_data(shared_t* s, int n_readers){
    pthread_mutex_init(&s->mtx_fin, NULL);
    pthread_mutex_init(&s->mtx_int, NULL);
    pthread_mutex_init(&s->mtx_n_checkers, NULL);
    pthread_mutex_init(&s->mtx_n_readers, NULL);

    sem_init(&s->empty_fin, 0, 1);
    sem_init(&s->empty_int, 0, INTERMEDIATE_CAPACITY);
    sem_init(&s->full_fin, 0, 0);
    sem_init(&s->full_int, 0, 0);

    s->int_in = 0;
    s->int_out = 0;
    s->n_active_checkers = N_CHECKERS;
    s->n_active_readers = n_readers;
}

void init_reader_data(reader_t* r,shared_t* s, char* filename, int id){
    r->s = s;
    r->n_arrays_checked = 0;
    r->tid = id;
    r->f_info.filename = filename;
    init_record_data(&r->curr_rec);
}

void init_checker_data(checker_t* c, shared_t* s, int id){
    c->curr_even_sum = 0;
    c->curr_odd_sum = 0;
    c->s = s;
    c->tid = id;
    c->n_verified_arr = 0;
}

/**
 * FUNCTION TO DESTROY MUTEX AND SEM
 */
void shut_down(shared_t* s){
    pthread_mutex_destroy(&s->mtx_fin);
    pthread_mutex_destroy(&s->mtx_int);
    pthread_mutex_destroy(&s->mtx_n_readers);
    pthread_mutex_destroy(&s->mtx_n_checkers);

    sem_destroy(&s->empty_fin);
    sem_destroy(&s->empty_int);
    sem_destroy(&s->full_fin);
    sem_destroy(&s->full_int);
}

/**
 * FUNCTION TO ADD REC IN INTERMEDIATE
 */
void add_rec_in_intermediate(shared_t* s, record_t r){
    sem_wait(&s->empty_int);
    pthread_mutex_lock(&s->mtx_int);
    s->intermediate[s->int_in] = r;
    s->int_in = (s->int_in + 1) % INTERMEDIATE_CAPACITY;
    pthread_mutex_unlock(&s->mtx_int);
    sem_post(&s->full_int);
}

/**
 * FUNCTION TO ADD REC IN FINAL
 */
void add_rec_in_final(shared_t* s, record_t r){
    sem_wait(&s->empty_fin);
    pthread_mutex_lock(&s->mtx_fin);
    s->final = r;
    pthread_mutex_unlock(&s->mtx_fin);
    sem_post(&s->full_fin);
}

/**
 * FUNCTION TO EXTRACT REC FROM INTERMEDIATE
 */
record_t extract_rec_from_intermediate(shared_t* s){
    record_t r;
    sem_wait(&s->full_int);
    pthread_mutex_lock(&s->mtx_int);
    r = s->intermediate[s->int_out];
    s->int_out = (s->int_out + 1) % INTERMEDIATE_CAPACITY;
    pthread_mutex_unlock(&s->mtx_int);
    sem_post(&s->empty_int);
    return r;
}

/**
 * FUNCTION TO EXTRACT REC FROM FINAL
 */
record_t extract_rec_from_final(shared_t* s){
    record_t r;
    sem_wait(&s->full_fin);
    pthread_mutex_lock(&s->mtx_fin);
    r = s->final;
    pthread_mutex_unlock(&s->mtx_fin);
    sem_post(&s->empty_fin);
    return r;
}

/**
 * FUNCTION TO VERIFY PROPERTY
 */
void has_equisum(checker_t* c){
    int even_sum = 0;
    int odd_sum = 0;
    for(int i = 0; i < c->curr_rec.size; i++){
        if(i % 2 == 0){
            even_sum += c->curr_rec.arr[i];
        }
        else{
            odd_sum += c->curr_rec.arr[i];
        }
    }
    c->curr_even_sum = even_sum;
    c->curr_odd_sum = odd_sum;
}

/**
 * FUNCTION TO OPEN FILE WITH MMAP
 */
void open_file_with_mmap(reader_t* r){
    int fd = open(r->f_info.filename, O_RDONLY);
    if(fd < 0){
        perror("[!]ERROR occured while opening file");
        exit(1);
    }

    struct stat f_stat;
    if(fstat(fd, &f_stat) < 0){
        perror("[!]ERROR occured while calling fstat");
        exit(1);
    }

    int dimension = f_stat.st_size;
    unsigned char* f_content = mmap(NULL, dimension, PROT_READ, MAP_PRIVATE, fd, 0);
    if(f_content == MAP_FAILED){
        perror("[!]ERROR occured while mapping file in memory");
        exit(1);
    }

    r->f_info.fd = fd;
    r->f_info.size = dimension;
    r->f_info.file_content = f_content;
    close(fd);
}

/**
 * READER ROUTINE
 */
void* reader_routine(void* arg){
    reader_t* r = (reader_t*) arg;
    printf("[READER-%d] file '%s'\n", r->tid, r->f_info.filename);
    open_file_with_mmap(r);
    for(int i = 0; i < r->f_info.size; i += r->curr_rec.size){
        for(int j = 0; j < r->curr_rec.size; j++){
            r->curr_rec.arr[j] = r->f_info.file_content[i + j];
        }
        add_rec_in_intermediate(r->s, r->curr_rec);
        r->n_arrays_checked++;
        flockfile(stdout);
        printf("[READER-%d] vettore candidato n.%d:", r->tid, r->n_arrays_checked);
        for(int j = 0; j < r->curr_rec.size - 1; j++){
            printf(" %d,", r->curr_rec.arr[j]);
        }
        printf(" %d\n", r->curr_rec.arr[r->curr_rec.size - 1]);
        funlockfile(stdout);
    }
    pthread_mutex_lock(&r->s->mtx_n_readers);
    r->s->n_active_readers--;
    if(r->s->n_active_readers == 0){
        r->curr_rec.poison = 1;
        pthread_mutex_lock(&r->s->mtx_n_checkers);
        int n_act_check = r->s->n_active_checkers;
        pthread_mutex_unlock(&r->s->mtx_n_checkers);
        for(int i = 0; i < n_act_check; i++){
            add_rec_in_intermediate(r->s, r->curr_rec);
        }
    }
    pthread_mutex_unlock(&r->s->mtx_n_readers);
    munmap(r->f_info.file_content, r->f_info.size);
    printf("[READER-%d] terminazione con %d vettori letti\n", r->tid, r->n_arrays_checked);
    pthread_exit(NULL);
}

/**
 * CHECKER ROUTINE
 */
void* checker_routine(void* arg){
    checker_t* c = (checker_t*) arg;
    for(;;){
        c->curr_rec = extract_rec_from_intermediate(c->s);
        if(c->curr_rec.poison == 1){
            break;
        }
        flockfile(stdout);
        printf("[VERIF-%d] verifico vettore: ", c->tid);
        for(int i = 0; i < c->curr_rec.size - 1; i++){
            printf(" %d,", c->curr_rec.arr[i]);
        }
        printf(" %d\n", c->curr_rec.arr[c->curr_rec.size - 1]);
        funlockfile(stdout);
        has_equisum(c);
        if(c->curr_even_sum == c->curr_odd_sum){
            add_rec_in_final(c->s, c->curr_rec);
            printf("[VERIF-%d] si tratta di un vettore equisomma con somma %d!\n", c->tid, c->curr_even_sum);
        }
        else{
            printf("[VERIF-%d] non è un vettore equisomma (somma pari %d vs. dispari %d)\n", c->tid, c->curr_even_sum, c->curr_odd_sum);
        }
        c->n_verified_arr++;
    }
    pthread_mutex_lock(&c->s->mtx_n_checkers);
    c->s->n_active_checkers--;
    if(c->s->n_active_checkers == 0){
        c->curr_rec.poison = 1;
        add_rec_in_final(c->s, c->curr_rec);
    }
    pthread_mutex_unlock(&c->s->mtx_n_checkers);
    printf("[VERIF-%d] terminazione con %d vettori verificati\n", c->tid, c->n_verified_arr);
    pthread_exit(NULL);
}

/**
 * MAIN FUNCTION
 * Usage: equisum-array-screener <file-bin-1> <file-bin-2> ... <file-bin-N>
 */
int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s <file-bin-1> <file-bin-2> ... <file-bin-N>\n", argv[0]);
        exit(1);
    }

    int n = argc - 1;
    int n_equisum_arr = 0;

    record_t curr_rec;
    shared_t s;
    reader_t r[n];
    checker_t c[N_CHECKERS];

    init_shared_data(&s, n);
    for(int i = 0; i < n; i++) init_reader_data(&r[i], &s, argv[i + 1], i + 1);
    for(int i = 0; i < N_CHECKERS; i++) init_checker_data(&c[i], &s, i + 1);

    pthread_t reader_thread[n];
    pthread_t checker_thread[N_CHECKERS];
    printf("[MAIN] creazione di %d thread lettori e %d thread verificatori\n", n, N_CHECKERS);

    for(int i = 0; i < n; i++){
        if(pthread_create(&reader_thread[i], NULL, reader_routine, &r[i]) != 0){
            perror("[!]ERROR occured while creating reader thread");
            exit(1);
        }
    }

    for(int i = 0; i < N_CHECKERS; i++){
        if(pthread_create(&checker_thread[i], NULL, checker_routine, &c[i]) != 0){
            perror("[!]ERROR occured while creating checker thread");
            exit(1);
        }
    }

    for(;;){
        curr_rec = extract_rec_from_final(&s);
        if(curr_rec.poison == 1){
            break;
        }
        flockfile(stdout);
        printf("[MAIN] ricevuto nuovo vettore equisomma:");
        for(int i = 0; i < curr_rec.size - 1; i++){
            printf(" %d,", curr_rec.arr[i]);
        }
        printf(" %d\n", curr_rec.arr[curr_rec.size - 1]);
        funlockfile(stdout);
        n_equisum_arr++;
    }

    for(int i = 0; i < n; i++) pthread_join(reader_thread[i], NULL);
    for(int i = 0; i < N_CHECKERS; i++) pthread_join(checker_thread[i], NULL);

    printf("[MAIN] terminazione con %d vettori equisomma trovati\n", n_equisum_arr);
    shut_down(&s);   
    return 0;
}