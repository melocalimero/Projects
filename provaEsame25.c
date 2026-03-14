/*Descrizione del programma 
Si scriva un programma C che: 
A. [3 punti] Prenda in input da riga di comando un parametro stringa filename che contenga un 
nome di file di input (ad esempio “input13.txt”). Si assuma una lunghezza massima del 
nome del file di 100 caratteri. Il programma controlli che il nome del file di input abbia 
estensione “txt”. Se i parametri passati non rispettano i requisiti richiesti, il programma stampa 
un messaggio di errore sullo standard error e termina la sua esecuzione con un appropriato 
codice di terminazione. 
Si stampino a schermo i valori dei parametri presi in input. 
B. [8 punti] Si assuma che il file di testo contenuto al percorso indicato da filename contenga un 
certo numero di righe, ciascuna contenente un numero intero. Il programma legga il contenuto 
del file e inserisca i valori interi in una pila P. 
Si stampi a schermo il contenuto della pila P. 
C. [5 punti] Estragga da P gli interi mediante delle operazioni di pop. Inserisca la media degli 
elementi estratti da P in una variabile m. 
Si stampi il valore contenuto in m. 
D. [8 punti] Legga nuovamente il file indicato da filename. Il primo valore letto viene inserito 
nella pila P. Dal secondo valore in poi (sia esso x), se il valore letto 𝑥 > 𝑚, esso viene inserito 
nella pila, altrimenti viene estratto l’elemento in cima a P (sia esso y) e viene inserito nella 
pila il valore: (x+y)/2
 Si stampi a schermo il contenuto della pila P. 

E. [6 punti] Si trasferisca il contenuto di P in un array A di opportuna dimensione e si ordini A 
con un algoritmo di ordinamento a scelta. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    int n;
    struct node* next;
}Node;

#define FILENAME_LENGTH 100
char* decodeParameters(int argc, char* argv[]);
int push(Node** head, int number);
void printStack(Node* head);
int readFile(Node** head, char* filename);
int pop(Node** head);
double getMean(Node** head, int count);
int fillP(Node** head, double media, char* filename);
int* getArray(Node** head, int dim);
void sortArray(int* array, int dim);

int main(int argc, char* argv[]){
    char* filename=malloc(sizeof(char)*FILENAME_LENGTH);
    filename=decodeParameters(argc,argv);
    printf("Filename:%s\n", filename);

    Node* head=NULL;
    int count=readFile(&head, filename);
    printStack(head);
    double media=getMean(&head, count);
    printf("\nMean:%f\n", media);

    count=fillP(&head, media, filename);
    printStack(head);

    int* array=getArray(&head, count);
    printf("\nArray before Sorting\n");
    for(unsigned i=0; i<count; i++){
        printf("%d ", array[i]);
    }
    printf("\nArray after Sorting\n");
    sortArray(array, count);
    for(unsigned i=0; i<count; i++){
        printf("%d ", array[i]);
    }

    free(array);
    free(filename);
}

char* decodeParameters(int argc, char* argv[]){
    if(argc!=2){
        fprintf(stderr,"Usage: <%s> <string>\n", argv[0]);
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"String extension MUST be .txt\n");
        exit(1);
    }

    return argv[1];
}

int push(Node** head, int number){
    Node* nuovo=malloc(sizeof(Node));
    nuovo->n=number;
    nuovo->next=NULL;

    if(*head==NULL){
        (*head)=nuovo;
        return 1;
    }

    nuovo->next=*head;
    (*head)=nuovo;
    return 1;
}

void printStack(Node* head){
    Node* curr=head;
    while(curr!=NULL){
        printf("%d\n", curr->n);
        curr=curr->next;
    }
}

int readFile(Node** head, char* filename){
    FILE* fp=fopen(filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    int count=0;
    while(!feof(fp)){
        int n=0;
        if(fscanf(fp,"%d", &n)<1){
            if(!feof(fp)){
                fprintf(stderr,"Error reading from file\n");
                exit(1);
            }
        }
        count+=push(head, n);
    }

    fclose(fp);
    return count;
}

int pop(Node** head){
    if(*head==NULL){
        fprintf(stderr,"Stack is empty, couldn't remove any element\n");
        exit(1);
    }

    Node* temp=*head;
    (*head)=(*head)->next;
    return temp->n;
}

double getMean(Node** head, int count){
    int somma=0;
    while(*head!=NULL){
        somma+=pop(head);
    }
    double media=(somma*1.0)/count;
    return media;
}

int fillP(Node** head, double media, char* filename){
    FILE*fp=fopen(filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    int firstNumber=0;
    int dim=0;
    if(fscanf(fp,"%d", &firstNumber)<1){
        fprintf(stderr,"Error reading first element\n");
        exit(1);
    }
    push(head, firstNumber);
    dim++;
    while(!feof(fp)){
        int number=0;
        if(fscanf(fp,"%d", &number)<1){
            if(!feof(fp)){
                fprintf(stderr,"Error reading from file in fillP\n");
                exit(1);
            }
        }
        if(number>media){
            push(head, number);
            dim++;
        }
        else{
            int y=pop(head);
            int numberToInsert=(number+y)/2;
            push(head, numberToInsert);
        }
    }
    return dim;
}

int* getArray(Node** head, int dim){
    int* array=malloc(sizeof(int)*dim);
    int i=0;
    while(*head!=NULL && i<dim){
        array[i]=pop(head);
        i++;
    }
    return array;
}

void sortArray(int* array, int dim){
    int flag=1;
    while(flag==1){
        flag=0;
        for(unsigned i=0; i<dim-1; i++){
            if(array[i]< array[i+1]){
                int hold=array[i];
                array[i]=array[i+1];
                array[i+1]=hold;
                flag=1;
            }
        }
    }
}