/*Scrivere un programma in C che:
 ● A. Prenda come argomenti da riga di comando il nome IN di un
 file di testo di input (es. “input.txt”) e il nome OUT di un
 file di testo di output (es. “output.txt”). Si assuma che i
 nomi dei file non superino i 255 caratteri.
 ● B. Si assuma che il file di input ("input1.txt")contenga un numero n di
 righe di testo e che ogni riga contenga un numero m di valori
 in virgola mobile separati da spazi; il programma allochi una
 matrice di puntatori a double di dimensioni n x m, che
 punteranno ai numeri contenuti nel file di input;
 ● C. Elimini dalla matrice i valori di A che sono inferiori ai
 valori medi delle righe corrispondenti (i corrispondenti
 puntatori dovranno puntare a NULL con conseguente liberazione
 della memoria occupata dai corrispondenti double);
 ● D Inserisca i valori puntati dai puntatori non nulli in A in
 un array di double;
 ● E scriva i valori dell’array sul file di output ("output2.txt") il cui nome
 e’ stato specificato a linea di comando.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 256

typedef struct parameters{
    char IN[NAME_LENGTH];
    char OUT[NAME_LENGTH];
}Parameters;

typedef struct size{
    unsigned n;
    unsigned m;
}Size;

Parameters * readInput(int argc, char* argv[]);
Size* countRows(Parameters* p);
double***allocMatrix(Parameters*p, Size*ptr);
double ***createMatrix(double*** matrix, Parameters* p, Size* ptr);
double media(double*** matrix, Size*ptr, unsigned i);
void sparsify(double*** matrix, Size* ptr);
double* collect(double***matrix,Size*ptr);
void writeToFile(double* array, int sizeof_array, Parameters* p);

unsigned int get_random() {
    static unsigned int m_w = 123456;
    static unsigned int m_z = 789123;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}
int main(int argc, char* argv[]){
    Parameters* p=readInput(argc, argv);
    Size* ptr=countRows(p);
    double***matrix=allocMatrix(p,ptr);
    matrix=createMatrix(matrix, p, ptr);
    sparsify(matrix,ptr);
    double* array=collect(matrix,ptr);
    int array_size=0;
    for(unsigned i=0; i<ptr->n; i++){
        for(unsigned j=0; j<ptr->m; j++){
            if(matrix[i][j]!=NULL){
                array_size++;
            }
        }
    }

    writeToFile(array, array_size, p);

    for(unsigned i=0; i<ptr->n; i++){
        for(unsigned j=0; j<ptr->m; j++){
            free(matrix[i][j]);
            matrix[i][j]=NULL;
        }
    }
    for(unsigned i=0; i<ptr->n; i++){
        free(matrix[i]);
        matrix[i]=NULL;
    }

    free(matrix);
    matrix=NULL;
    free(p);
    p=NULL;
    free(ptr);
    ptr=NULL;

    free(array);
    array=NULL;

}

Parameters * readInput(int argc, char* argv[]){
    if(argc!=3){
        fprintf(stderr,"Usage: <./a.out> <string> <string>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"First string extension MUST be .txt\n");
        exit(1);
    }

    if(strcmp(argv[2]+strlen(argv[2])-4, ".txt")!=0){
        fprintf(stderr,"Second string extension MUST be .txt\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    strcpy(p->IN, argv[1]);
    strcpy(p->OUT, argv[2]);
    return p;
}

Size* countRows(Parameters* p){
    FILE* fp=fopen(p->IN, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
Size* ptr=malloc(sizeof(Size));
int countCol=0;
int countRow=0;
    while(!feof(fp)){
        char* string=malloc(sizeof(char)*NAME_LENGTH);
        if(!(fgets(string, NAME_LENGTH, fp))){
            fprintf(stderr,"Error reading from file\n");
            exit(1);
        }
        countRow++;
        countCol+=strlen(string);
    }
    ptr->n=countRow;
    ptr->m=countCol/countRow/8;
    fclose(fp);
    return ptr;
}

double***allocMatrix(Parameters*p, Size*ptr){
    double***Matrix=malloc(sizeof(double**)*ptr->n);
    for(unsigned i=0; i<ptr->n; i++){
        Matrix[i]=malloc(sizeof(double*)*ptr->m);
        for(unsigned j=0; j<ptr->m; j++){
            Matrix[i][j]=malloc(sizeof(double));
        }
    }
    return Matrix;
}

double ***createMatrix(double*** matrix,Parameters* p, Size* ptr){
    FILE* fp=fopen(p->IN, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    
    for(unsigned i=0; i<ptr->n; i++){
        for(unsigned j=0; j<ptr->m; j++){
            if(fscanf(fp, "%lf", matrix[i][j])<1){
                fprintf(stderr,"Error reading from file\n");
                exit(1);
            }
        }
    }
    fclose(fp);
    return matrix; 
}

double media(double*** matrix, Size*ptr, unsigned i){
    double somma=0; 
    double media;
    unsigned j=0;
    while(j<ptr->m){
        somma+=*(matrix[i][j]);
        j++;
    }

    media=somma/ptr->m;
    return media;
}

void sparsify(double*** matrix, Size* ptr){
    for(unsigned k=0; k<ptr->n; k++){
        double relativeMedia=media(matrix, ptr, k);
        for(unsigned l=0; l<ptr->m; l++){
            if(*(matrix[k][l])<relativeMedia){
                free(matrix[k][l]);
                matrix[k][l]=NULL;
            }
        }
    }
        
}
    
double* collect(double***matrix,Size*ptr){
    unsigned l=0;
    for(unsigned i=0; i<ptr->n; i++){
        for(unsigned j=0; j<ptr->m; j++){
            if(matrix[i][j]!=NULL){
                l++;
            }
        }
    }

    double* array=malloc(sizeof(double)*l);
    unsigned i=0;
    while(i<l){
        for(unsigned k=0; k<ptr->n; k++){
            for(unsigned p=0; p<ptr->m; p++){
                if(matrix[k][p]!=NULL){
                    array[i]=*(matrix[k][p]);
                    i++;
                }
            }  
        }
    }

    return array;
}

void writeToFile(double* array, int sizeof_array, Parameters* p){
    FILE* fp=fopen(p->OUT, "w");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    for(unsigned i=0; i<sizeof_array; i++){
        if(i==10){
            fprintf(fp,"\n");
        }
        fprintf(fp, "%f ", array[i]);
    }
    fclose(fp);
}