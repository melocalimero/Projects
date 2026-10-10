#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define CAPIENZA_RAW 10
#define CAPIENZA_FILTERED 5
#define MAX_TIMESTAMP 20
#define MAX_MESSAGE 200

/**
 * STRUTTURE AUSILIARI
 */
typedef enum{
    DEBUG = 0, 
    INFO,
    WARNING,
    ERROR,
    CRITICAL
}sev_t;

typedef struct{
    FILE* fd;
    char* filename;
}f_info_t;

/**
 * STRUTTURE CODA
 */
typedef struct{
    int is_poison;
    sev_t sev;
    char timestamp[MAX_TIMESTAMP], message[MAX_MESSAGE];
    char* filename;
}record_t;

typedef struct{
    record_t* coda;
    int in, out, size;
    sem_t liberi, pieni;
    pthread_mutex_t mtx;
}queue_t;

/**
 * STRUTTURA CONDIVISA
 */
typedef struct{
    queue_t raw, filtered;
    int n_lettori, n_filtri, n_log;
    pthread_mutex_t mtx_n_lettori, mtx_n_filtri,mtx_n_log;
}shared_t;

/**
 * STRUTTURE THREAD
 */
typedef struct{
    // parte condivisa
    shared_t* s;

    // parte privata
    record_t curr_r;
    f_info_t f;
    int tid, n_letti;
}reader_t;

typedef struct{
    // parte condivisa
    shared_t* s;

    // parte privata
    record_t curr_r;
    sev_t limite_sev;
    int tid, n_ver;
}filter_t;

/**
 * FUNZIONI AUSILIARIE FONDAMENTALI
 */
sev_t string_to_sev(char* sev){
    if(strcmp(sev, "DEBUG") == 0) return DEBUG;
    if(strcmp(sev, "INFO") == 0) return INFO;
    if(strcmp(sev, "WARNING") == 0) return WARNING;
    if(strcmp(sev, "ERROR") == 0) return ERROR;
    if(strcmp(sev, "CRITICAL") == 0) return CRITICAL;
    perror("[!]ERROR severity's string not supported");
    exit(1);
}

char* sev_to_string(sev_t sev){
    switch(sev){
        case 0:
            return "DEBUG";
        case 1:
            return "INFO";
        case 2:
            return "WARNING";
        case 3:
            return "ERROR";
        case 4:
            return "CRITICAL";
        default:
            return "[!]Severity not supported";
    }
}


/**
 * INIZIALIZZAZIONE
 */
void init_record_data(record_t* r){
    r->is_poison = 0;
}

void init_queue_data(queue_t* q, int size){
    pthread_mutex_init(&q->mtx, NULL);
    sem_init(&q->liberi, 0, size);
    sem_init(&q->pieni, 0, 0);
    q->in = q->out = 0;
    q->size = size;
    q->coda = malloc(sizeof(record_t) * size);
}

void init_shared_data(shared_t* s, int n, int r){
    pthread_mutex_init(&s->mtx_n_filtri, NULL);
    pthread_mutex_init(&s->mtx_n_lettori, NULL);
    pthread_mutex_init(&s->mtx_n_log, NULL);
    s->n_filtri = r;
    s->n_lettori = n;
    s->n_log = 0;
    init_queue_data(&s->filtered, CAPIENZA_FILTERED);
    init_queue_data(&s->raw, CAPIENZA_RAW);
}

void init_reader_data(reader_t* r, shared_t* s, char* fname, int id){
    r->f.filename = fname;
    r->s = s;
    r->tid = id;
    r->n_letti = 0;
    init_record_data(&r->curr_r);
}

void init_filter_data(filter_t* f, shared_t* s, char* lim_sev, int id){
    f->n_ver = 0;
    f->s = s;
    f->tid = id;
    f->limite_sev = string_to_sev(lim_sev);
}

/**
 * DEALLOCAZIONE E DISTRUZIONE SEM E MTX
 */
void destroy_queue(queue_t* q){
    pthread_mutex_destroy(&q->mtx);
    sem_destroy(&q->liberi);
    sem_destroy(&q->pieni);
    free(q->coda);
}

void destroy_shared(shared_t* s){
    pthread_mutex_destroy(&s->mtx_n_filtri);
    pthread_mutex_destroy(&s->mtx_n_lettori);
    destroy_queue(&s->filtered);
    destroy_queue(&s->raw);
}

/**
 * GESTIONE CODE
 */
void add_rec(queue_t* q, record_t r){
    sem_wait(&q->liberi);
    pthread_mutex_lock(&q->mtx);
    q->coda[q->in] = r;
    q->in = (q->in + 1) % q->size;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->pieni); 
}

record_t get_rec(queue_t* q){
    record_t r;
    sem_wait(&q->pieni);
    pthread_mutex_lock(&q->mtx);
    r = q->coda[q->out];
    q->out = (q->out + 1) % q->size;
    pthread_mutex_unlock(&q->mtx);
    sem_post(&q->liberi);
    return r;
}

/**
 * GESTIONE FILE
 */
FILE* open_file(char* filename){
    FILE* fd = fopen(filename, "r");
    if(fd == NULL){
        perror("[!] ERROR opening file");
        exit(1);
    }
    return fd;
}

/**
 * STAMPE
 */
void print_record(const record_t r){
    printf("%s|%s|%s\n", r.timestamp, sev_to_string(r.sev), r.message);
}

/**
 * CONTROLLI DI TERMINAZIONE
 */
void check_reader_termination(reader_t* r){
    fclose(r->f.fd);

    pthread_mutex_lock(&r->s->mtx_n_lettori);
    r->s->n_lettori--;
    if(r->s->n_lettori == 0){
        r->curr_r.is_poison = 1;
        pthread_mutex_lock(&r->s->mtx_n_filtri);
        int n = r->s->n_filtri;
        pthread_mutex_unlock(&r->s->mtx_n_filtri);
        for(int i = 0; i < n; i++){
            add_rec(&r->s->raw, r->curr_r);
        }
    }
    pthread_mutex_unlock(&r->s->mtx_n_lettori);

    pthread_mutex_lock(&r->s->mtx_n_log);
    r->s->n_log += r->n_letti;
    pthread_mutex_unlock(&r->s->mtx_n_log);
}

void check_filter_termination(filter_t* f){
    pthread_mutex_lock(&f->s->mtx_n_filtri);
    f->s->n_filtri--;
    if(f->s->n_filtri == 0){
        if(f->curr_r.is_poison != 1) f->curr_r.is_poison = 1;
        add_rec(&f->s->filtered, f->curr_r);
    }

    pthread_mutex_unlock(&f->s->mtx_n_filtri);
}

/**
 * ROUTINES
 */
void* routine_reader(void* arg){
    reader_t* r = (reader_t*) arg;
    printf("[READER-%d] file '%s'\n", r->tid, r->f.filename);
    r->f.fd = open_file(r->f.filename);

    char line[4096];
    char sev[20];
    while(fgets(line, sizeof(line), r->f.fd) != NULL){
        if(sscanf(line, "%19[^|]|%[^|]|%199[^\n]", r->curr_r.timestamp, sev, r->curr_r.message) != 3){
            perror("[!] ERROR parsing data");
            exit(1);
        }

        r->curr_r.sev = string_to_sev(sev);
        r->curr_r.filename = r->f.filename;
        add_rec(&r->s->raw, r->curr_r);
        r->n_letti++;
        flockfile(stdout);
        printf("[READER-%d] riga di log n.%d: ", r->tid, r->n_letti);
        print_record(r->curr_r);
        funlockfile(stdout);
    }

    check_reader_termination(r);
    printf("[READER-%d] terminazione con %d righe lette\n", r->tid, r->n_letti);
    pthread_exit(NULL);

}

void* routine_filter(void* arg){
    filter_t* f = (filter_t*) arg;
    for(;;){
        f->curr_r = get_rec(&f->s->raw);
        if(f->curr_r.is_poison) break;

        f->n_ver++;

        flockfile(stdout);
        printf("[FILTER-%d] verifico riga: ", f->tid);
        print_record(f->curr_r);
        funlockfile(stdout);

        if(f->curr_r.sev >= f->limite_sev){
            printf("[FILTER-%d] severità %s uguale o superiore alla soglia, riga accettata\n", f->tid, sev_to_string(f->curr_r.sev));
            add_rec(&f->s->filtered, f->curr_r);
        }
        else{
            printf("[FILTER-%d] severità %s inferiore alla soglia, riga scartata\n", f->tid, sev_to_string(f->curr_r.sev));
        }
    }

    check_filter_termination(f);
    printf("[FILTER-%d] terminazione con %d righe verificate\n", f->tid, f->n_ver);
    pthread_exit(NULL);
}

/**
 * MAIN
 * Usage: log-analyzer <severity> <R-filters> <file-1> <file-2> ... <file-N>
 */
int main(int argc, char* argv[]){
    if(argc < 4){
        fprintf(stderr, "Usage: %s <severity> <R-filters> <file-1> <file-2> ... <file-N>\n", argv[0]);
        exit(1);
    }

    int n_filters = atoi(argv[2]);
    int n_readers = argc - 3;
    int n_debug = 0;
    int n_info = 0;
    int n_warning = 0;
    int n_error = 0;
    int n_critical = 0;
    int n_total = 0;

    record_t rec;
    shared_t s;
    reader_t r[n_readers];
    filter_t f[n_filters];

    init_shared_data(&s, n_readers, n_filters);
    for(int i = 0; i < n_readers; i++) init_reader_data(&r[i], &s, argv[ i + 3], i + 1);
    for(int i = 0; i < n_filters; i++) init_filter_data(&f[i], &s, argv[1], i + 1);

    pthread_t reader[n_readers], filter[n_filters];

    printf("[MAIN] creazione di %d thread lettori e %d thread filtro\n", n_readers, n_filters);
    printf("[MAIN] soglia di severità: %s\n", argv[1]);

    for(int i = 0; i < n_readers; i++){
        if(pthread_create(&reader[i], NULL, routine_reader, &r[i]) != 0){
            perror("[!]ERROR creating reader threads");
            exit(1);
        }
    }
    for(int i = 0; i < n_filters; i++){
        if(pthread_create(&filter[i], NULL, routine_filter, &f[i]) != 0){
            perror("[!]ERROR creating filter threads");
            exit(1);
        }
    }

    for(;;){
        rec = get_rec(&s.filtered);

        if(rec.is_poison) break;

        printf("[MAIN] ricevuta riga %s dal file '%s'\n", sev_to_string(rec.sev), rec.filename);

        switch(rec.sev){
            case DEBUG:
                n_debug++;
                break;
            case INFO:
                n_info++;
                break;
            case WARNING:
                n_warning++;
                break;
            case ERROR:
                n_error++;
                break;
            case CRITICAL:
                n_critical++;
                break;
        }
    }

    n_total = (n_debug + n_info + n_warning + n_error + n_critical);
    for(int i = 0; i < n_readers; i++) pthread_join(reader[i], NULL);
    for(int i = 0; i < n_filters; i++) pthread_join(filter[i], NULL);

    printf("[MAIN] riepilogo finale (soglia: %s):\n", argv[1]);
    if(n_debug > 0) printf("[MAIN]   DEBUG:  %d righe\n", n_debug);
    if(n_info > 0) printf("[MAIN]   INFO:  %d righe\n", n_info);
    if(n_warning > 0) printf("[MAIN]   WARNING:  %d righe\n", n_warning);
    if(n_error > 0) printf("[MAIN]   ERROR:  %d righe\n", n_error);
    if(n_critical > 0) printf("[MAIN]   CRITICAL:  %d righe\n", n_critical);
    printf("[MAIN]   totale: %d righe filtrate su %d totali\n", n_total, s.n_log);
    printf("[MAIN] terminazione\n");
    destroy_shared(&s);
    return 0;

}
