/*Scrivere un programma in C che:
 ● A. Prenda come argomenti da riga di comando il nome IN di un
 file di testo di input (es. “input.txt”) e il nome OUT di un
 file di testo di output (es. “output.txt”). Si assuma che i
 nomi dei file non superino i 255 caratteri.
 ● B. Si assuma che il file di input contenga un numero n di
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
 ● E scriva i valori dell’array sul file di output il cui nome
 e’ stato specificato a linea di comando.*/

 #include <stdio.h>
 #include <stdlib.h>


#define N 6
#define M 5 

typedef struct parameters{
    char* IN;
    char *OUT;
}Parameters;


Parameters* readParameters(int argc, char* argv[]);
void createMatrix(unsigned m, double ***A, unsigned n, Parameters* p);
void sparsify(unsigned m, double***A, unsigned n);
double *collect(unsigned m, double ***A, unsigned n);
void writeToFile(double* array, Parameters* p, unsigned n);

int main(int argc, char* argv[]){
    Parameters* p=readParameters(argc, argv);

    double ***A=malloc(sizeof(double**)*N);
    for(unsigned i=0; i<N; i++){
        A[i]=malloc(sizeof(double*)*M);
        for(unsigned j=0; j<M; j++){
            A[i][j]=malloc(sizeof(double));
            
        }
    }
createMatrix(M,A,N,p);
sparsify(M,A,N);

double* V=collect(M,A,N);

writeToFile(V,p, N*M);



//deallocazione matrice double
    for(unsigned i=0; i<N; i++){
        for(unsigned j=0; j<M; j++){
            free(A[i][j]);
            A[i][j]=NULL;
        }
    }

    for(unsigned i=0; i<N; i++){
        free(A[i]);
        A[i]=NULL;
    }

    free(A);
    A=NULL;
}

Parameters* readParameters(int argc, char* argv[]){
    if(argc<3){
        perror("Usage: <a.out> <input file name> <output file name>");
        exit(1);
    }
    if(strcmp(argv[1]+strlen(argv[1])-4,".txt")!=0){
        fprintf(stderr, "Invalid extension of the input file name\n");
        exit(1);
    }

    if(strcmp(argv[2]+strlen(argv[2])-4,".txt")!=0){
        fprintf(stderr, "Invalid extension of the output file name\n");
        exit(1);
    }

    if(strlen(argv[1])>255){
        fprintf(stderr, "Input file name is too long (Max 255 characters)\n");
        exit(1);
    }

    if(strlen(argv[2])>255){
        fprintf(stderr, "Output file name is too long (Max 255 characters)\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->IN=argv[1];
    p->OUT=argv[2];

    return p;
}


void createMatrix(unsigned m, double ***A, unsigned n, Parameters* p){
    FILE* fp=fopen(p->IN, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    for(unsigned i=0; i<n; i++){
        for(unsigned j=0; j<m; j++){
            fscanf(fp, "%lf", A[i][j]);
        }
    }

    fclose(fp);
}

void sparsify(unsigned m, double***A, unsigned n){
    double* media=malloc(sizeof(double)*n);
    double *somma=calloc(n,sizeof(double));

    for(unsigned i=0; i<n; i++){
        for(unsigned j=0; j<m; j++){
            somma[i]+=*(A[i][j]);
        }
    }

    for(unsigned i=0; i<n; i++){
        for(unsigned j=0; j<m; j++){
            if(*(A[i][j])<media[i]){
                free(A[i][j]);
                (A[i][j])=NULL;
            }
        }
    }

    free(somma);
    free(media);
}

double *collect(unsigned m, double ***A, unsigned n){
    double* array=malloc(sizeof(double*)*(m*n));

    int k=0;
    for(unsigned i=0; i<n; i++){
        for(unsigned j=0; j<m; j++){
            if(A[i][j]){
                array[k]=*(A[i][j]);
                k++;
                
            }
        }
    }

    return array;
}

void writeToFile(double* array, Parameters* p, unsigned n){
    FILE* fp=fopen(p->OUT, "w");
    if(!fp){
        perror("Error opening the file");
        exit(1);
    }

    for(unsigned i=0; i<n; i++){
        if(array[i]){
            fprintf(fp, "%f ", array[i]);
        }

    }

    fclose(fp);
}