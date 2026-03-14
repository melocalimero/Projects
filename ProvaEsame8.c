/* Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione main)
 costituito dal nome di un file di testo (ES: input.txt) e due numeri
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
    int N;
    int M;
}Parameters;

Parameters* readInput(char* argv[]);
double*** allocateMatrix(Parameters*p);
void fillMatrix(double***A, Parameters*p);
void printMatrix(double***A, Parameters*p);
double*** normalize(double***A, Parameters*p);
double MAX_A_j(double***A, Parameters*p, int j);
double ***sortMatrixCols(double***B, Parameters*p);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argv);

    double***A=allocateMatrix(p);
    fillMatrix(A, p);
    printf("**A**\n");
    printMatrix(A,p);

    double***B=normalize(A,p);
    printf("**B**\n");
    printMatrix(B,p);

    B=sortMatrixCols(B,p);
    printf("**Sorted B**\n");
    printMatrix(B,p);

    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            free(A[i][j]);
            A[i][j]=NULL;
        }
    }

    for(unsigned i=0; i<p->N; i++){
        free(A[i]);
        A[i]=NULL;
    }

    free(A);
    A=NULL;

    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            free(B[i][j]);
            B[i][j]=NULL;
        }
    }

    for(unsigned i=0; i<p->N; i++){
        free(B[i]);
        B[i]=NULL;
    }

    free(B);
    B=NULL;

}

Parameters* readInput(char* argv[]){
    int i=0;
    int count=0;
    while(argv[i]!=NULL){
        count++;
        i++;
    }

    if(count<4){
        fprintf(stderr,"Usage: <./a.out> <string> <int> <int>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"String extension MUST be .txt\n");
        exit(1);
    }

    int n=atoi(argv[2]);
    int m=atoi(argv[3]);

    if(n<5 || n>10){
        fprintf(stderr,"First int MUST be in range [5,10]\n");
        exit(1);
    }

    if(m<5 || m>10){
        fprintf(stderr,"Second int MUST be in range [5,10]\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->filename=argv[1];
    p->N=n;
    p->M=m;

    return p;
}

double*** allocateMatrix(Parameters*p){
    double***A=malloc(sizeof(double**)*p->N);

    for(unsigned i=0; i<p->N; i++){
        A[i]=malloc(sizeof(double*)*p->M);
        for(unsigned j=0; j<p->M; j++){
            A[i][j]=malloc(sizeof(double));
        }
    }

    return A;
}

void fillMatrix(double***A,Parameters*p){

    FILE* fp=fopen(p->filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    
    while(!feof(fp)){
        for(unsigned i=0; i<p->N; i++){
            for(unsigned j=0; j<p->M; j++){
                fscanf(fp,"%lf", A[i][j]);
            }
        }

    }

}

void printMatrix(double***A, Parameters*p){
    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            fprintf(stdout,"%f ", *(A[i][j]));
        }
        puts("");
    }
}

double MAX_A_j(double***A, Parameters*p, int j){
    double max=0;
    int i=0;
    while(i<p->N){
        if(*(A[i][j])>max){
            max=*(A[i][j]);
        }
        i++;
    }

    return max;
}

double*** normalize(double***A, Parameters*p){
    double***B=allocateMatrix(p);
    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            double max=MAX_A_j(A,p,j);
            *(B[i][j])=*(A[i][j])/max;
        }
    }

    return B;
}

double ***sortMatrixCols(double***B, Parameters*p){
    for(unsigned i=0; i<p->N; i++){
        for(unsigned j=0; j<p->M; j++){
            for(unsigned l=0; l<p->N-1; l++){
                for(unsigned k=0; k<p->M; k++){
                    if(*(B[l][k])>*(B[l+1][k])){
                        double hold=*(B[l][k]);
                        *(B[l][k])=*(B[l+1][k]);
                        *(B[l+1][k])=hold;
                    }
                }
            }   
        }   
    }

    return B;
}