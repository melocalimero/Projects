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
eccesso saranno inizializzati (e riconoscibili) mediante length=
1.  

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
void buildArray(Record* array, Parameters p);
void filter(Record* array);
void createSet(Record* array);
void printArray(Record* array);
void save(Record* array, Parameters* p);
void read(Parameters *p);


int main(int argc, char* argv[]){
    Parameters* p=readInput(argc, argv);
    if(p==NULL){
        perror("Error passing the values to p");
        return -1;
    }
    Record* array=malloc(sizeof(Record)*ARRAY_LENGTH);

    buildArray(array, *p);
    filter(array);
    createSet(array);
    printArray(array);
    save(array, p);
    read(p);


    free(array);
}



Parameters* readInput(int argc, char* argv[]){
    if(argc!=3){
        fprintf(stderr, "[ERROR]: invalid usage\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr, "il nome del file di input deve avere come estensione 'txt' \n");
        exit(1);
    }

    if(strcmp(argv[2]+strlen(argv[2])-4, ".bin")!=0){
        fprintf(stderr, "il nome del file di input deve avere come estensione 'bin' \n");
        exit(1);
    }
    
        Parameters* p=malloc(sizeof(Parameters));
        p->input_filename=argv[1];
        p->output_filename=argv[2];

        return p;
        
    
}

void buildArray(Record* array, Parameters p){
    
    FILE*fp=fopen(p.input_filename, "r");
    if(!fp){
        perror("Error opening file");
        return;
    }
    int count=0;
    for(unsigned i=0; i<ARRAY_LENGTH; i++){

        fscanf(fp, "%s", array[i].key);
        
        array[i].length=strlen(array[i].key);
        
        count++;
    }

    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        if(array[i].length==0){
            array[i].length=-1;
        }
    }

}

void filter(Record*array){
    
    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        for(unsigned j=0; j<STRING_LENGTH; j++){
            int count=0;
                switch(array[i].key[j]){
                    case '.':
                    case ',':
                    case ';':
                    case ':':
                        array[i].key[j]='\0';
                        count++;
                        break;
                
            }
            array[i].length=array[i].length-count;
        }
        
    }

    
    
}

//non riconosce un "Cristoforo"
void createSet(Record* array){

    int k=0;
    int i=0;
    int count=0;
    char* check=malloc(sizeof(char)*STRING_LENGTH);
    while(k<ARRAY_LENGTH){
        strcpy(check, array[k].key);
        while(i<ARRAY_LENGTH){
            i++;
            if(strcmp(check, array[i].key)==0){
                count++;
                if(count>1){
                    array[i].length=-1;
                }
            }
        }
        count=0;
        i=0;
        k++;
        
    }

    free(check);

    
}

void printArray(Record* array){
    int count=0;
    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        if(array[i].length>0){
            
            fprintf(stdout,"PrintArray():[%u]\t",count);
            fprintf(stdout,"[word: %s]\t[length:%d]\n",array[i].key, array[i].length);
            count++;
        }
    }
}

void save(Record* array, Parameters* p){
    FILE* fp=fopen(p->output_filename, "wb");
    if(!fp){
        perror("Error opening the file");
        exit(1);
    }

    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        if(array[i].length>0){
            if(fwrite(&array[i], sizeof(Record), 1, fp)!=1){
                fprintf(stderr,"Error writing the %u element on file\n", i);
                break;
            }
        }
    }
    fclose(fp);
}

void read(Parameters *p){
    FILE* fp=fopen(p->output_filename, "rb");
    if(!fp){
        perror("Error opening the file");
        exit(1);
    }

    Record* ptr=malloc(sizeof(Record)*ARRAY_LENGTH);

    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        if(fread(&ptr[i], sizeof(Record), 1, fp)!=1){
            if(feof(fp)){
                printf("Fine dei dati\n");
            }
            else{
                fprintf(stderr, "Error reading the %u element\n", i);
            }
            break;
        }
        else{
            fprintf(stdout, "read()[%u]\t",i);
            fprintf(stdout, "[word:%s]\t[length:%d]\n", ptr[i].key, ptr[i].length);
        }
    }

    for(unsigned i=0; i<ARRAY_LENGTH; i++){
        
    }
}