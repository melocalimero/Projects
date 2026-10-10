#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#define NUMERI 6
#define MAX_VALUE 91
#define CAPIENZA_CAND 10
#define CAPIENZA_VINC 5
#define ID_MAX_LENGTH 20 


/**
 * ENUM DEI PREMI
 */
typedef enum{
    JACKPOT, SECONDO, TERZO, QUARTO, NESSUNO
}premio_data_t;

/*
 * STRUTTURA RECORD 
 */
typedef struct{
    char identificativo[ID_MAX_LENGTH];
    int numeri_biglietto[NUMERI];
    char nome_file[256];
    premio_data_t premio;
}record_data_t;

/**
 * STRUTTURA CONDIVISA
 */
typedef struct{
    record_data_t candidati[CAPIENZA_CAND];
    record_data_t vincenti[CAPIENZA_VINC];
    int candidati_in, candidati_out, vincenti_in, vincenti_out, n_lettori_attivi;
    pthread_mutex_t mtx_candidati, mtx_vincenti, mtx_lettori_attivi;
    sem_t candidati_aggiornato, vincenti_aggiornato, spazio_in_candidati, spazio_in_vincenti;
}shared_data_t;

/**
 * STRUTTURA LETTORI
 */
typedef struct{
    shared_data_t* s;
    int id, n_biglietti_letti;
    record_data_t r;
}lettore_data_t;

/**
 * STRUTTURA VERIFICAOTI
 */
typedef struct{
    shared_data_t* s;
    int id;
    record_data_t r_da_verificare;
    int* numeri_vincenti;
}verificatore_data_t;

/**
 * FUNZIONE PER INIZIALIZZARE
 */
void init_shared_data(shared_data_t* s, int n){
    pthread_mutex_init(&s->mtx_candidati, NULL);
    pthread_mutex_init(&s->mtx_vincenti, NULL);
    pthread_mutex_init(&s->mtx_lettori_attivi, NULL);
    
    s->candidati_in = 0;
    s->candidati_out = 0;
    s->n_lettori_attivi = n;
    s->vincenti_in = 0;
    s->vincenti_out = 0;
    sem_init(&s->candidati_aggiornato, 0, 0);
    sem_init(&s->spazio_in_candidati, 0, CAPIENZA_CAND);
    sem_init(&s->spazio_in_vincenti, 0, CAPIENZA_VINC);
    sem_init(&s->vincenti_aggiornato, 0, 0);
}

void init_lettore_data(lettore_data_t* l, shared_data_t* s, int id){
    l->id = id;
    l->n_biglietti_letti = 0;
    l->s = s;
}

void init_verificatore_data(verificatore_data_t* v, shared_data_t* s, int id, int* numeri_vinc){
    v->id = id;
    v->s = s;
    v->numeri_vincenti = numeri_vinc;
}

/**
 * FUNZIONI PER CHIUDERE
 */
void shut_down_shared_data(shared_data_t* s){
    pthread_mutex_destroy(&s->mtx_candidati);
    pthread_mutex_destroy(&s->mtx_lettori_attivi);
    pthread_mutex_destroy(&s->mtx_vincenti);
    sem_destroy(&s->candidati_aggiornato);
    sem_destroy(&s->spazio_in_candidati);
    sem_destroy(&s->spazio_in_vincenti);
    sem_destroy(&s->vincenti_aggiornato);
}

/**
 * FUNZIONE PER LEGGERE DAL FILE winning.txt
 */
int* leggi_file_vincente(char* filename){
    FILE* f = fopen(filename, "r");
    char buffer[ID_MAX_LENGTH];
    fgets(buffer, sizeof(buffer), f);
    int* numeri = malloc(sizeof(int) * ID_MAX_LENGTH);
    int j = 0;
    
    return numeri;
}

/**
 * FUNZIONE LETTORE
 */
void* lettura(void* arg){}

/**
 * FUNZIONE VERIFICATORE
 */
void* verifica(void* arg){}

/**
 * FUNZIONE MAIN
 */
int main(int argc, char* argv[]){
    if(argc < 4){
        fprintf(stderr, "Uso: %s <file vincitore> <M verificatori> <file-biglietto1.txt> ... <file-bigliettoN.txt>\n", argv[0]);
        exit(1);
    }

    const int n = argc - 3; //# file e di lettori
    const int m = atoi(argv[2]); //# verificatori 

    int* numeri_vincenti = leggi_file_vincente(argv[1]);
    for(int i = 0; numeri_vincenti[i] != 0; i++){
        printf("%d ,", numeri_vincenti[i]);
    }
    puts("");
}