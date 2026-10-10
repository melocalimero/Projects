#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_REQUEUSTS 5 // capienza buffer

/**
 * STRUTTURA FILE
 */
typedef struct{
    FILE* fd;
    char* fname;
}f_info_t;

/**
 * STRUTTURA RECORD
 */
typedef struct{
    int is_poison, id; 
    long long oper1, oper2;
    char op;
}record_t;

/**
 * STRUTTURA BUFFER CIRCOLARE
 */
typedef struct{
    record_t buff[MAX_REQUEUSTS];
    int in, out;
    sem_t liberi, pieni;
    pthread_mutex_t mtx;
}buffer_t;

/**
 * STRUTTURA CONDIVISA
 */
typedef struct{
    buffer_t richieste;
    long long* risposte;
    int n_lettori_attivi, n_verifiche;
    pthread_mutex_t mtx_risposte, mtx_n_lettori, mtx_n_verifiche;
    sem_t *liberi_risp, *pieni_risp;
}shared_t;

/**
 * STRUTTURA LETTORI
 */
typedef struct{
    shared_t* s;
    record_t curr_r;
    f_info_t f;
    long long tmp_risposta, risposta_finale;
    int tid;
}reader_t;

/**
 * STRUTTURA CALCOLATORE
 */
typedef struct{
    shared_t* s;
    record_t curr_r;
    long long risp;
}calc_t;

/**
 * INIZIALIZZAZIONE
 */
void init_record_data(record_t* r, int id){
    r->is_poison = 0;
    r->id = id;
}

void init_buffer_data(buffer_t* b){
    pthread_mutex_init(&b->mtx, NULL);
    sem_init(&b->liberi, 0, MAX_REQUEUSTS);
    sem_init(&b->pieni, 0, 0);
    b->in = b->out = 0;
}

void init_shared_data(shared_t* s, int n){
    pthread_mutex_init(&s->mtx_n_lettori, NULL);
    pthread_mutex_init(&s->mtx_n_verifiche, NULL);
    pthread_mutex_init(&s->mtx_risposte, NULL);
    s->liberi_risp = malloc(sizeof(sem_t) * n);
    s->pieni_risp = malloc(sizeof(sem_t) * n);
    for(int i = 0; i < n; i++){
        sem_init(&s->liberi_risp[i], 0, n);
        sem_init(&s->pieni_risp[i], 0, 0);
    }
    s->n_lettori_attivi = n;
    s->n_verifiche = 0;
    s->risposte = malloc(sizeof(long long) * n);
    init_buffer_data(&s->richieste);
}

void init_reader_data(reader_t* r, shared_t* s, char* fname, int id){
    r->f.fname = fname;
    r->s = s;
    r->tid = id;
    init_record_data(&r->curr_r, r->tid - 1);
}

void init_calc_data(calc_t* c, shared_t* s){
    c->s = s;
}

/**
 * CHIUSURA
 */
void destroy_buffer(buffer_t* b){
    pthread_mutex_destroy(&b->mtx);
    sem_destroy(&b->liberi);
    sem_destroy(&b->pieni);
}

void destroy_shared(shared_t* s, int n){
    pthread_mutex_destroy(&s->mtx_n_lettori);
    pthread_mutex_destroy(&s->mtx_n_verifiche);
    pthread_mutex_destroy(&s->mtx_risposte);
    for(int i = 0; i < n; i++){
        sem_destroy(&s->liberi_risp[i]);
        sem_destroy(&s->pieni_risp[i]);
    }
    free(s->risposte);
    destroy_buffer(&s->richieste);
}

/**
 * GESTIONE BUFFER
 */
void add_rec(buffer_t* b, record_t r){
    sem_wait(&b->liberi);
    pthread_mutex_lock(&b->mtx);
    b->buff[b->in] = r;
    b->in = (b->in + 1) % MAX_REQUEUSTS;
    pthread_mutex_unlock(&b->mtx);
    sem_post(&b->pieni);
}

record_t get_rec(buffer_t* b){
    record_t r;
    sem_wait(&b->pieni);
    pthread_mutex_lock(&b->mtx);
    r = b->buff[b->out];
    b->out = (b->out + 1) % MAX_REQUEUSTS;
    pthread_mutex_unlock(&b->mtx);
    sem_post(&b->liberi);
    return r;
}

/**
 * GESTIONE VETTORE RISPOSTE
 */
void add_response(shared_t* s, int id, long long resp){
    sem_wait(&s->liberi_risp[id]);
    pthread_mutex_lock(&s->mtx_risposte);
    s->risposte[id] = resp;
    pthread_mutex_unlock(&s->mtx_risposte);
    sem_post(&s->pieni_risp[id]);
}

long long get_response(shared_t* s, int id){
    long long r;
    sem_wait(&s->pieni_risp[id]);
    pthread_mutex_lock(&s->mtx_risposte);
    r = s->risposte[id];
    pthread_mutex_unlock(&s->mtx_risposte);
    sem_post(&s->liberi_risp[id]);
    return r;
}

/**
 * GESTIONE TERMINAZIONE
 */
void check_termination(reader_t* r){
    pthread_mutex_lock(&r->s->mtx_n_lettori);
    r->s->n_lettori_attivi--;
    if(r->s->n_lettori_attivi == 0){
        r->curr_r.is_poison = 1;
        add_rec(&r->s->richieste, r->curr_r);
    }
    pthread_mutex_unlock(&r->s->mtx_n_lettori);
}

/**
 * GESTIONE PARSING
 */
int is_last(char* l, record_t* r){
    char space;
    int pars = sscanf(l, "%c%c%lld", &r->op, &space, &r->oper2);
    if(pars == 3 && (r->op == '+' || r->op == '-' || r->op == 'x') && space == ' '){
        return 0;
    }
    return 1;
}

/**
 * ROUTINE LETTORI
 */
void* routine_reader(void* arg){
    reader_t* r = (reader_t*) arg;
    printf("[FILE-%d] file da verificare: '%s'\n", r->tid, r->f.fname);
    r->f.fd = fopen(r->f.fname, "r");
    if(r->f.fd == NULL){
        perror("[!]");
        exit(1);
    }

    char line[256];

    if(fgets(line, sizeof(line), r->f.fd) != NULL){
        sscanf(line, "%lld", &r->tmp_risposta);
    }

    printf("[FILE-%d] valore iniziale della computazione: %lld\n", r->tid, r->tmp_risposta);

    while(fgets(line, sizeof(line), r->f.fd) != NULL){
        if(!is_last(line, &r->curr_r)){

            r->curr_r.oper1 = r->tmp_risposta;
            add_rec(&r->s->richieste, r->curr_r);
            printf("[FILE-%d] prossima operazione: '%c %lld'\n", r->tid, r->curr_r.op, r->curr_r.oper2);

            r->tmp_risposta = get_response(r->s, r->tid - 1);

            printf("[FILE-%d] risultato ricevuto: %lld\n", r->tid, r->tmp_risposta);
        }
        else{
            break;
        }
    }

    if(sscanf(line, "%lld", &r->risposta_finale) != 1){
        perror("[!]");
        exit(1);
    }

    if(r->risposta_finale == r->tmp_risposta){
        pthread_mutex_lock(&r->s->mtx_n_verifiche);
        r->s->n_verifiche++;
        printf("[FILE-%d] computazione terminata in modo corretto: %lld\n", r->tid, r->risposta_finale);
        pthread_mutex_unlock(&r->s->mtx_n_verifiche);
    }
    else{
        printf("[FILE-%d] computazione terminata in modo errato: ottenuto %lld ma risposta giusta: %lld\n", r->tid, r->tmp_risposta, r->risposta_finale);
    }

    check_termination(r);
    pthread_exit(NULL);

}

/**
 * ROUTINE CALCOLATORE
 */
void* routine_calc(void* arg){
    calc_t* c = (calc_t*) arg;
    for(;;){
        c->curr_r = get_rec(&c->s->richieste);
        if(c->curr_r.is_poison) break;

        switch(c->curr_r.op){
            case '+':
                c->risp = c->curr_r.oper1 + c->curr_r.oper2;
                break;
            case '-':
                c->risp = c->curr_r.oper1 - c->curr_r.oper2;
                break;
            case 'x':
                c->risp = c->curr_r.oper1 * c->curr_r.oper2;
                break;
            default:
                perror("[!]ERROR, operation not supported");
                exit(1);
        }
        printf("[CALC] calcolo effettuato: %lld %c %lld = %lld\n", c->curr_r.oper1, c->curr_r.op, c->curr_r.oper2, c->risp);

        add_response(c->s, c->curr_r.id, c->risp);
    }
}

/**
 * MAIN
 * Usage: calc-verifier-3 <calc-file-1> <calc-file-2> ... <calc-file-n>
 */
int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage: %s <calc-file-1> <calc-file-2> ... <calc-file-n>\n", argv[0]);
        exit(1);
    }

    int n = argc - 1;

    shared_t s;
    reader_t r[n];
    calc_t c;

    init_shared_data(&s, n);
    for(int i = 0; i < n; i++) init_reader_data(&r[i], &s, argv[i + 1], i + 1);
    init_calc_data(&c, &s);

    pthread_t lettore[n];
    pthread_t calcolatore;

    for(int i = 0; i < n; i++){
        if(pthread_create(&lettore[i], NULL, routine_reader, &r[i]) != 0){
            perror("[!]");
            exit(1);
        }
    }

    if(pthread_create(&calcolatore, NULL, routine_calc, &c) != 0){
        perror("[!]");
        exit(1);
    }

    for(int i = 0; i < n; i++) pthread_join(lettore[i], NULL);
    pthread_join(calcolatore, NULL);

    printf("[MAIN] verifiche completate con successo: %d/%d\n", s.n_verifiche, n);
    destroy_shared(&s, n);
    return 0;
}