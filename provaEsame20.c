/*Si scriva un programma C che: 
A. [3 punti] Prenda in input da riga di comando un parametro stringa input_filename che 
contenga un nome di file di input (ad esempio “input.csv”). Il programma controlli che il nome 
del file abbia estensione “csv”. Se i parametri passati non rispettano i requisiti richiesti, il 
programma stampa un messaggio di errore sullo standard error e termina la sua esecuzione 
con un appropriato codice di terminazione. 
Si stampino a schermo i valori dei parametri presi in input. 

B. [10 punti] Si assuma che il file di input contenga del testo. La prima riga del file di testo 
indicato da input_filename contiene due interi n e m separati da una virgola. Ciascuna delle 
successive n righe contiene m interi separati da virgole. Il programma legga i valori n e m 
dalla prima riga del file e inserisca i valori contenuti nelle restanti righe all’interno di una 
matrice X di puntatori a short di dimensione 𝑛	× 𝑚. 
Si stampino a schermo i valori n e m e il contenuto della matrice X. 

C. [8 punti] Definisca un array Y di n short e inserisca in Y[i] il massimo dei valori indicati dai 
puntatori nella i-esima riga di X. Ordini, dunque, l’array Y in senso discendente mediante 
l’algoritmo bubblesort. 
Si stampi a schermo l’array Y prima e dopo l’ordinamento.  

D. [7 punti] Per ogni riga di X, ponga a NULL tutti puntatori nella riga i-esima che puntino a 
valori superiori al valore  
Y[i]/2+1 
Ad esempio, sia X[i][j] il j-esimo elemento della i-esima riga di X. Se X[i][j]>Y[i]/2+1, 
allora X[i][j] dovrà puntare a NULL. 
Si stampi il contenuto di X sostituendo i valori NULL con degli asterischi 

E. [2 punti] Stampi su standard error il contenuto di X sostituendo i valori NULL con dei trattini 
“-”. 

Nota: gestire opportunamente i casi in cui i file non possono essere correttamente aperti in lettura o 
scrittura stampando un errore sullo standard error e terminando l’esecuzione del programma. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sizes{
    int n,m;
}Sizes;

char* decodeParameters(int argc, char* argv[]);
Sizes* readHead(char* filename);
short***allocateMatrix(Sizes* p);
void readFile(short***X, Sizes*p, char* filename);
short MAX_A_i(short***X, Sizes*p, unsigned i);
short* getArray(short***X,Sizes*p);
void bubbleSort(short* Y, Sizes* p);
void processX(short***X, short* Y, Sizes* p);

int main(int argc, char* argv[]){
    char* input_filename=decodeParameters(argc, argv);
    printf("Input file name:%s\n", input_filename);

    Sizes* ptr=readHead(input_filename);
    printf("Dati letti dall'header:%d e %d\n", ptr->n, ptr->m);

    short***X=allocateMatrix(ptr);

    readFile(X,ptr, input_filename);

    short* Y=getArray(X,ptr);
    for(unsigned i=0; i<ptr->n; i++){
        printf("%hd ", Y[i]);
    }

    bubbleSort(Y,ptr);
    printf("\n==Sorted Array==\n");
    for(unsigned i=0; i<ptr->n; i++){
        printf("%hd ", Y[i]);
    }

    printf("\n==Matrix with *==\n");
    processX(X,Y,ptr);

    printf("\n==Matrix with - ==\n");
    for(unsigned i=0; i<ptr->n; i++){
        for(unsigned j=0; j<ptr->m; j++){
            if(X[i][j]==NULL){
                printf("- ");
            }
            else{
                printf("%hd ", *(X[i][j]));
            }
        }
        printf("\n");
    }
}

char* decodeParameters(int argc, char* argv[]){
    if(argc!=2){
        fprintf(stderr,"Usage: <%s> <string>\n", argv[0]);
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".csv")!=0){
        fprintf(stderr,"String extension must be .csv\n");
        exit(1);
    }

    return argv[1];
}

Sizes* readHead(char* filename){
    FILE* fp=fopen(filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    
    int n=0;
    int m=0;
    if(fscanf(fp,"%d,%d",&n, &m )<2){ //devo aggiungere la virgola perchè essendo csv ogni dato è separato dalla virgola
        
        fprintf(stderr,"Error reading header of file\n");
        exit(1);
        
    }
    Sizes* ptr=malloc(sizeof(Sizes));
    ptr->n=n;
    ptr->m=m;
    fclose(fp);
    return ptr;
}

short***allocateMatrix(Sizes* p){
    short***X=malloc(sizeof(short**)*p->n);
    for(unsigned i=0; i<p->n; i++){
        X[i]=malloc(sizeof(short*)*p->m);
        for(unsigned j=0; j<p->m; j++){
            X[i][j]=malloc(sizeof(short));
        }
    }

    return X;
}

void readFile(short***X, Sizes*p, char* filename){
    FILE* fp=fopen(filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    int spazzatura1, spazzatura2;
    fscanf(fp,"%d,%d", &spazzatura1, &spazzatura2);
    
    printf("\n==MATRICE==\n");
    for(unsigned i=0; i<p->n; i++){
        for(unsigned j=0; j<p->m; j++){
            fscanf(fp, "%hd,", X[i][j]);
            printf("%hd ", *(X[i][j]));
        }
        printf("\n");
    }
}

short MAX_A_i(short***X, Sizes*p, unsigned i){
    int j=0;
    int max=0;
    while(j<p->m){
        if(max<*(X[i][j])){
            max=*(X[i][j]);
        }
        j++;
    }

    return max;
}

short* getArray(short***X,Sizes*p){
    short* Y=malloc(sizeof(short)*p->n);

    printf("\n==ARRAY==\n");
    for(unsigned i=0; i<p->n; i++){
        for(unsigned j=0; j<p->m; j++){
            Y[i]=MAX_A_i(X,p,i);
            
        }
    }

    return Y;
}

void bubbleSort(short* Y, Sizes* p){
    int flag=1;
    while(flag==1){
        flag=0;
        for(unsigned i=0; i<p->n-1; i++){
            if(Y[i]<Y[i+1]){
                int hold=Y[i];
                Y[i]=Y[i+1];
                Y[i+1]=hold;
                flag=1;
            }
        }
    }
}

void processX(short***X, short* Y, Sizes* p){
    for(unsigned i=0; i<p->n; i++){
        for(unsigned j=0; j<p->m; j++){
            if(*(X[i][j])<(Y[i])/2){
                free(X[i][j]);
                X[i][j]=NULL;
                printf("* ");
            }
            else{
                printf("%hd ", *(X[i][j]));
            }

        }
        printf("\n");
    }
}