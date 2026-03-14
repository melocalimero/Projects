/* Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione main)
 costituito dal nome di un file di testo (ES: input1.txt) e due numeri
 interi N ed M; il file contiene una matrice di double di dimensione N
 x M, dove ogni riga del file di testo contiene una riga della matrice
 stessa. I numeri N ed M dovranno essere contenuti nell’intervallo
 [5,10];
 ● B. il programma crei in memoria una matrice A di dimensioni N x M di
 puntatori a double. La matrice sia popolata da puntatori a celle
 double che contengono i corrispondenti numeri presenti nel file di
 input.
 ● C. Si crei una nuova matrice B di dimensioni N x M di puntatori a
 double, in cui ogni singolo valore della matrice corrispondera’ al
 valore corrispondente della matrice A, normalizzato rispetto al
 massimo valore della colonna stessa di A, quindi:
 ○ B[i][j] = A[i][j]/MAX_A_j, dove MAX_A_j rappresenta il valore
 massimo della colonna j-esima della matrice A, (i in [0,N-1] e j
 in [0,M-1]).
 ● D. Si stampi, sullo standard output, la matrice A e la matrice B;
 ● E. Mediante un qualunque algoritmo di ordinamento (insertion sort /
 bubble sort / selection sort), si ordini in modo crescente ogni
 colonna della matrice B, e la si stampi sullo standard output;*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct parameters{
    char* filename;
    int N, M;
}Parameters;

Parameters* readInput(int argc, char* argv[]);
double***allocateMatrix(Parameters* p);
void readFromFile(Parameters* p, double***matrix);
double sortMatrixCols(double***matrix, Parameters* p);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc, argv);
    double***matrix=allocateMatrix(p);
    readFromFile(p, matrix);

    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            printf("%f ", *(matrix[i][j]));
        }
        printf("\n");
    }

    sortMatrixCols(matrix,p);

    printf("\nSorted matrix\n");
    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            printf("%f ", *(matrix[i][j]));
        }
        printf("\n");
    }
}

Parameters* readInput(int argc, char* argv[]){
    if(argc!=4){
        fprintf(stderr,"Usage: <%s> <string> <int> <int>\n", argv[0]);
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"String extension MUST be .txt\n");
        exit(1);
    }

    int N=atoi(argv[2]);
    int M=atoi(argv[3]);

    if(N<5 || N>10){
        fprintf(stderr,"First int MUST be in range [5,10]\n");
        exit(1);
    }

    if(M<5 || M>10){
        fprintf(stderr,"Second int MUST be in range [5,10]\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->filename=argv[1];
    p->N=N;
    p->M=M;

    return p;
}

double***allocateMatrix(Parameters* p){
    double***m=malloc(sizeof(double**)*p->N);
    for(unsigned i=0; i<p->N; i++){
        m[i]=malloc(sizeof(double*)*p->M);
        for(unsigned j=0; j<p->M; j++){
            m[i][j]=malloc(sizeof(double));
        }
    }

    return m;
}

void readFromFile(Parameters* p, double***matrix){
    FILE* fp=fopen(p->filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            if(fscanf(fp,"%lf" ,matrix[i][j])<1){
                if(!feof(fp)){
                    fprintf(stderr,"Error reading from file\n");
                    exit(1);
                }
            }
        }
    }
}

//BUBBLE SORT
double sortMatrixCols(double***matrix, Parameters* p){
    int flag=1;
    while (flag==1){
        flag=0;
        for(unsigned i=0; i<p->N-1; i++){
            for(unsigned j=0; j<p->M; j++){
                if(*(matrix[i][j])>*(matrix[i+1][j])){
                    double hold=*(matrix[i][j]);
                    *(matrix[i][j])=*(matrix[i+1][j]);
                    *(matrix[i+1][j])=hold;
                    flag=1;
                }
            }
        }
    }
}


double sortMatrixCols(double***matrix, Parameters* p){
    
}