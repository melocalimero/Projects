/*Scrivere un programma in C che: 
A. Prenda un input da tastiera (argomenti della funzione main) 
costituito dal nome di un file di testo di input (ES: input.txt) 
ed il nome di un file binario di output (ES: elab.bin). Il file 
di input conterra’ del testo in italiano, allegato al compito 
e riportato in questo documento; si suppone che la lunghezza di 
ogni stringa contenuta all’interno del file sia non superiore ai 
20 caratteri. 

B. Legga tutte le stringhe contenute nel file di input e le 
memorizzi in un array di record (struct), in cui ogni record 
contiene un campo “key” che contiene la parola, ed un campo 
“length” che contiene la lunghezza della stringa. Si allochi, a 
tale scopo, un array di 200 record; i record dell’array in 
eccesso saranno inizializzati (e riconoscibili) mediante length=-1.  

C. Elabori l’array di record eliminando tutti i caratteri di 
punteggiatura ‘,’, ‘;’, ‘:’, ‘.’ (virgola, punto e virgola, due 
punti e punto) eventualmente presenti nelle parole contenute nei 
record ed aggiornando di conseguenza il campo length dei record. 

D. Elabori l’array di record ``eliminando’’ tutti record duplicati 
tranne uno. Ad esempio se l’array contiene due record 
{“navigazione”; 12}, uno dei due record dovra’ essere modificato 
in {“navigazione”; -1}.  

E. Stampi sullo standard output tutti i record ``non vuoti’’ (cioe’ 
il cui campo length sia un numero maggiore di zero) presenti 
nell’array, dal primo all’ultimo.  

F. Memorizzi i record in un file binario con nome specificato come 
secondo argomento del main, mediante la funzione fwrite().  

G. Legga il file binario salvato al punto precedente e stampi sullo 
standard output i record letti, uno per volta.  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 21
#define ARRAY_LENGTH 200

typedef struct parameters{
    char* input_filename;
    char* output_filename;
}Parameters;

typedef struct record{
    char key[STRING_LENGTH];
    int length;
}Record;


Parameters* readInput(int argc, char* argv[]);
void buildArray(Parameters*p, Record* array);
void filter(Record* array);
void createSet(Record* array);
void printArray(Record* array);
void save(Parameters* p, Record* array);
void read(Parameters* p);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc, argv);
    Record* array=malloc(sizeof(Record)*ARRAY_LENGTH);
    buildArray(p, array);
    filter(array);
    createSet(array);
    printArray(array);
    save(p, array);
    read(p);

    

    free(p);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc!=3){
        fprintf(stderr,"Usage: <./a.out> <string> <string>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4,".txt")!=0){
        fprintf(stderr,"First string extension MUST be .txt\n");
        exit(1);
    }
    if(strcmp(argv[2]+strlen(argv[2])-4,".bin")!=0){
        fprintf(stderr,"Second string extension MUST be .bin\n");
        exit(1);
    }

    Parameters*p=malloc(sizeof(Parameters));
    p->input_filename=argv[1];
    p->output_filename=argv[2];

    return p;
}

void buildArray(Parameters*p, Record* array){
    FILE* fp=fopen(p->input_filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    int i=0;
    while(!(feof(fp))&&i<ARRAY_LENGTH){
        fscanf(fp, "%s", array[i].key);
        array[i].length=strlen(array[i].key);
        if(array[i].length==0){
            array[i].length=-1;
        } 
        
        i++;
    }
    
    if(feof(fp)){
        while(i<ARRAY_LENGTH){
            array[i].length=-1;
            i++;
        }
    }
    fclose(fp);
    
}

void filter(Record* array){
    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        for(unsigned j=0; j<STRING_LENGTH; j++){
            switch(array[i].key[j]){
                case '.':
                case ';':
                case ':':
                case ',':
                    array[i].key[j]='\0';
                    array[i].length--;
                    break;
            }
        }
    }
}

void createSet(Record* array){
    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        for(unsigned j=i+1; j<ARRAY_LENGTH; j++){
            if(strcmp(array[j].key, array[i].key)==0){
                array[j].length=-1;
            }
        }
    }
}

void printArray(Record* array){
    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        if(array[i].length>0)
            fprintf(stdout,"word:%s length:%d\n", array[i].key, array[i].length);
    }
}

void save(Parameters* p, Record* array){
    FILE* fp=fopen(p->output_filename, "wb");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        if(array[i].length>0){
            fwrite(&array[i], sizeof(Record), 1, fp);
        }
    }
    fclose(fp);
}

void read(Parameters* p){
    FILE* fp=fopen(p->output_filename, "rb");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    while(!feof(fp)){
        Record* temp=malloc(sizeof(Record));
        fread(temp, sizeof(Record), 1, fp);
        printf("[word:%s] [length:%d]\n",temp->key, temp->length );
    }
}