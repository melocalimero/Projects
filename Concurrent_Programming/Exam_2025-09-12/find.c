#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>

#define CAPIENZA 10

/**
 * STRUTTURE AUSILIARI
 */
typedef struct{
    DIR* dp;
    struct stat statbuf;
    struct dirent* entry;
    char* dirname;
}d_info_t;

typedef struct{
    int fd, dim;
    char* fname;
    unsigned char* fcontent;
}f_info_t;

/**
 * STRUTTURE BUFFER
 */
typedef struct{
    int is_poison, n_occorrenze;
    char* fname;
}record_t;

typedef struct{
    record_t coda[CAPIENZA];
    int in, out;
    sem_t liberi, pieni;
    pthread_mutex_t mtx;
}queue_t;

/**
 * STRUTTURA CONDIVISA
 */
typedef struct{
    queue_t intermedia, finale;
    int n_lettori;
    pthread_mutex_t mtx_n_lettori;
}shared_t;

/**
 * STRUTTURE THREAD
 */
typedef struct{
    // parte condivisa
    shared_t* s;

    // parte privata
    int tid;
    d_info_t curr_d;
    record_t curr_r;
}dir_t;

typedef struct{
    // parte condivisa
    shared_t* s;

    // parte privata
    char* word;
    f_info_t f;
    record_t curr_r;
}search_t;

/**
 * INIZIALIZZAZIONE
 */
void init_record_data(record_t* r){
    r->is_poison = r->n_occorrenze = 0;
}

void init_queue_data(queue_t* q){
    pthread_mutex_init(&q->mtx, NULL);
    sem_init(&q->liberi, 0, CAPIENZA);
    sem_init(&q->pieni, 0, 0);
    q->in = q->out = 0;
}

void init_shared_data(shared_t* s, int n){
    pthread_mutex_init(&s->mtx_n_lettori, NULL);
    s->n_lettori = n;
    init_queue_data(&s->finale);
    init_queue_data(&s->intermedia);
}

void init_dir_data(dir_t* d, shared_t* s, char* dirname, int id){
    d->curr_d.dirname = dirname;
    d->s = s;
    d->tid = id;
    init_record_data(&d->curr_r);
}

void init_search_data(search_t* se, shared_t* s, char* word){
    se->s = s;
    se->word = word;
}

/**
 * CHIUSURA
 */
void delete_queue(queue_t* q){
    pthread_mutex_destroy(&q->mtx);
    sem_destroy(&q->liberi);
    sem_destroy(&q->pieni);
}

void delete_shared(shared_t* s){
    pthread_mutex_destroy(&s->mtx_n_lettori);
    delete_queue(&s->finale);
    delete_queue(&s->intermedia);
}

/**
 * GESTIONE CODA
 */
void add_rec(queue_t* q, record_t r){
    sem_wait(&q->liberi);
    pthread_mutex_lock(&q->mtx);
    q->coda[q->in] = r;
    q->in = (q->in + 1) % CAPIENZA;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->pieni);
}

record_t get_rec(queue_t* q){
    record_t r;
    sem_wait(&q->pieni);
    pthread_mutex_lock(&q->mtx);
    r = q->coda[q->out];
    q->out = (q->out + 1) % CAPIENZA;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->liberi);
    return r;
}

/**
 * APERTURA FILE E DIRECTORY
 */
DIR* open_dir(char* dirname){
    DIR* dp = opendir(dirname);
    if(dp == NULL){
        perror("[!]");
        exit(1);
    }

    return dp;
}

void map_file(search_t* se){
    int fd = open(se->f.fname, O_RDONLY);
    if(fd < 0){
        perror("[!]");
        exit(1);
    }

    struct stat statbuf;
    if(fstat(fd, &statbuf) < 0){
        perror("[!]");
        exit(1);
    }

    int dim = statbuf.st_size;
    unsigned char* fcontent = mmap(NULL, dim, PROT_READ, MAP_PRIVATE, fd, 0);
    if(fcontent == MAP_FAILED){
        perror("[!]");
        exit(1);
    }

    se->f.dim = dim;
    se->f.fcontent = fcontent;
    se->f.fd = fd;
}

/**
 * CONTROLLO TERMINAZIONE
 */
void check_termination(dir_t* d){
    pthread_mutex_lock(&d->s->mtx_n_lettori);
    d->s->n_lettori--;
    if(d->s->n_lettori == 0){
        d->curr_r.is_poison = 1;
        add_rec(&d->s->intermedia, d->curr_r);
    }
    pthread_mutex_unlock(&d->s->mtx_n_lettori);
}

/**
 * CONTEGGIO OCCORRENZE
 */
int count_occurences(unsigned char* fcontent, char* word){
    int count = 0;
    int length = strlen(word);

    char* ptr = fcontent;
    while((ptr = strcasestr(ptr, word)) != NULL){
        count++;
        ptr += length;
    }

    return count;
}

/**
 * ROUTINES
 */
void* routine_dir(void* arg){
    dir_t* d = (dir_t*) arg;
    printf("[DIR-%d] direcotry '%s'\n", d->tid, d->curr_d.dirname);
    d->curr_d.dp = open_dir(d->curr_d.dirname);
    while((d->curr_d.entry = readdir(d->curr_d.dp)) != NULL){
        if(d->curr_d.entry->d_type == DT_REG){
            char path_completo[4096];
            snprintf(path_completo, sizeof(path_completo), "%s/%s", d->curr_d.dirname, d->curr_d.entry->d_name);

            char* global_path = realpath(path_completo, NULL);
            printf("[DIR-%d] trovato file '%s' in '%s'\n",d->tid, d->curr_d.entry->d_name, d->curr_d.dirname);
            d->curr_r.fname = global_path;
            add_rec(&d->s->intermedia, d->curr_r);
        }
    }

    check_termination(d);
    pthread_exit(NULL);
}

void* routine_search(void* arg){
    search_t* se = (search_t*) arg;
    printf("[SEARCH] parola da trovare :'%s'\n", se->word);
    for(;;){
        se->curr_r = get_rec(&se->s->intermedia);
        if(se->curr_r.is_poison) break;

        se->f.fname = se->curr_r.fname;
        map_file(se);
        se->curr_r.n_occorrenze = count_occurences(se->f.fcontent, se->word);
        if(se->curr_r.n_occorrenze > 0){
            printf("[SEARCH] il file '%s' contiene %d occorrenze\n", se->curr_r.fname, se->curr_r.n_occorrenze);
            add_rec(&se->s->finale, se->curr_r);
        }
        close(se->f.fd);
    }

    add_rec(&se->s->finale, se->curr_r);
    pthread_exit(NULL);

}

/**
 * MAIN
 * Usage: find <word> <dir-1> <dir-2> ... <dir-n>
 */
int main(int argc, char* argv[]){
    if(argc < 3){
        fprintf(stderr,"Usage: %s <word> <dir-1> <dir-2> ... <dir-n>\n", argv[0]);
        exit(1);
    }

    int n = argc - 2;
    int totale = 0;

    record_t r;
    shared_t s;
    dir_t d[n];
    search_t se;

    init_shared_data(&s, n);
    for(int i = 0; i < n; i++) init_dir_data(&d[i], &s, argv[i + 2], i + 1);
    init_search_data(&se, &s, argv[1]);

    pthread_t thread_dir[n];
    pthread_t thread_search;

    for(int i = 0; i < n; i++){
        if(pthread_create(&thread_dir[i], NULL, routine_dir, &d[i]) != 0){
            perror("[!]");
            exit(1);
        }
    }
    if(pthread_create(&thread_search, NULL, routine_search, &se) != 0){
        perror("[!]");
        exit(1);
    }

    for(;;){
        r = get_rec(&s.finale);
        if(r.is_poison) break;

        totale += r.n_occorrenze;
        printf("[MAIN] con il file '%s' il totale parziale è ora di %d\n", r.fname, totale);
    }

    for(int i = 0; i < n; i++) pthread_join(thread_dir[i], NULL);
    pthread_join(thread_search, NULL);
    delete_shared(&s);
    printf("[MAIN] terminazione con totale finale di %d occorrenze\n", totale);
    return 0;
}