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
    int N, M;
    char* filename;
}Parameters;

Parameters* readInput(int argc, char* argv[]);
double*** allocateMatrix(Parameters* p);
void fillMatrix(double***A, Parameters* p);
void printMatrix(double***A, Parameters* p);
double MAX_A_j(double***A, Parameters* p, int j);
void normalize(double***A, double***B, Parameters* p);
void sortMatrixCols(double***B, Parameters* p);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc, argv);
    double***A=allocateMatrix(p);
    fillMatrix(A,p);
    printf("**Matrice A:**\n");
    printMatrix(A,p);
    double***B=allocateMatrix(p);
    normalize(A,B,p);
    printf("\n**Matrice B:**\n");
    printMatrix(B,p);
    sortMatrixCols(B,p);
    printf("\n**Matrice B sorted:**\n");
    printMatrix(B,p);

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
        fprintf(stderr,"First int MUST be between 5 and 10\n");
        exit(1);
    }

    if(M<5 || M>10){
        fprintf(stderr,"Second int MUST be between 5 and 10\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->filename=argv[1];
    p->M=M;
    p->N=N;

    return p;
}

double*** allocateMatrix(Parameters* p){
    double***A=malloc(sizeof(double**)*p->N);
    for(unsigned i=0; i<p->N; i++){
        A[i]=malloc(sizeof(double*)*p->M);
        for(unsigned j=0; j<p->M; j++){
            A[i][j]=malloc(sizeof(double));
        }
    }

    return A;
}

void fillMatrix(double***A, Parameters* p){
    FILE* fp=fopen(p->filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            if(fscanf(fp,"%lf", A[i][j])!=1){
                if(feof(fp)){
                    return;
                }
                fprintf(stderr,"Error reading from file\n");
                exit(1);
            }
        }
    }
}

void printMatrix(double***A, Parameters* p){
    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            printf("%f ", *(A[i][j]));
        }
        printf("\n");
    }
}

double MAX_A_j(double***A, Parameters* p, int j){
    int i=0;
    double max=0;
    while(i<p->N){
        if(max<*(A[i][j])){
            max=*(A[i][j]);
        }
        i++;
    }

    return max;
}

void normalize(double***A, double***B, Parameters* p){
    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            *(B[i][j])=*(A[i][j])/MAX_A_j(A,p,j);
        }
    }
}

void sortMatrixCols(double***B, Parameters* p){
    int flag=1;
    while(flag==1){
        flag=0;
        for(unsigned i=0; i<p->N-1; i++){
            for(unsigned j=0; j<p->M; j++){
                if(*(B[i][j])>*(B[i+1][j])){
                    double hold=*(B[i][j]);
                    *(B[i][j])=*(B[i+1][j]);
                    *(B[i+1][j])=hold;
                    flag=1;
                }
            }   
        }
    }
}