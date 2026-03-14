/*Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione
 main) costituito dal nome di un file di testo (ES:
 input.txt). il file contiene una collezione di stringhe, una
 per riga; si suppone che la lunghezza di ogni stringa sia non
 superiore ai 30 caratteri;
 ● B. Legga tutte le stringhe contenute nel file di input e le
 memorizzi mediante operazioni ``enqueue’’ in una struttura
 dati CODA dinamica;
 ● C. Stampi tutte le stringhe mediante una sequenza di
 operazioni di rimozione dalla coda e le inserisca in un array
 di stringhe.
 ● D. Ordini lo array di stringhe in ordine lessicografico
 crescente, ed infine lo stampi.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 31

typedef struct node{
    char string[STRING_LENGTH];
    int count;
    struct node* next;
}Node;

char* readInput(int argc, char* argv[]);
void enqueue(Node** head, Node** tail, char* word);
void dequeue(Node** head, Node** tail);
void printArray(char** array, int length);
void buildqueue(Node** head, Node** tail, char* filename);
char** buildStringArray(Node** head, Node** tail, int length);
char** sortArray(char**array, int length);

int main(int argc, char* argv[]){
    char* filename=readInput(argc,argv);
    Node* head=NULL;
    Node* tail=NULL;

    buildqueue(&head,&tail, filename);
    int length=head->count;
    char** array=buildStringArray(&head,&tail,length);
    
    array=sortArray(array, length);

    printf("\nSorted array:\n");
    printArray(array, length);

    for(unsigned i=0; i<length; i++){
        free(array[i]);
    }
    free(array);
    
}

char* readInput(int argc, char* argv[]){
    if(argc<2){
        fprintf(stderr,"Usage <./a.out> <string>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"String extension MUST be .txt\n");
        exit(1);
    }

    return argv[1];
}

void enqueue(Node** head, Node** tail, char* word){
    Node * nuovo=malloc(sizeof(Node));
    if(!nuovo){
        fprintf(stderr,"Error allocating memory\n");
        return;
    }
    strcpy(nuovo->string, word);

    //1. lista vuota
    if(*head==NULL){
        (*head)=nuovo;
        (*tail)=nuovo;
        (*head)->count=1;
        return;
    }

    //2.Lista con almeno un elemento
    (*tail)->next=nuovo;
    (*tail)=(*tail)->next;
    (*head)->count++;
    return;
}

void dequeue(Node** head, Node** tail){
    //1.Lista vuota
    if(*head==NULL){
        printf("List is empty\n");
        return;
    }

    //2.Lista con un solo elemento
    if(*head==*tail){
        Node* temp=(*head);
        (*head)=(*head)->next;
        (*tail)=NULL;
        free(temp);
        return;
    }

    //3.Lista con più di un elemento
    Node* temp=(*head);
    (*head)=(*head)->next;
    free(temp);
    return;
}



void buildqueue(Node** head, Node** tail, char* filename){
    FILE* fp=fopen(filename, "r");
    if(!fp){
        fprintf(stderr,"Error opening file\n");
        return;
    }

    while(!feof(fp)){
        char string[STRING_LENGTH];
        if(fscanf(fp,"%s", string)<1){
            fprintf(stderr,"Error reading from file\n");
            return;
        }
        enqueue(head, tail, string);
    }

    if(feof(fp)){
        printf("Finished data\n");
    }
}

char** buildStringArray(Node** head, Node** tail, int length){
    char** array=malloc(sizeof(char*)*length);

    for(unsigned i=0; i<length; i++){
        array[i]=malloc(sizeof(char)*STRING_LENGTH);
    }

    int i=0;
    while((*head)!=NULL){
        strncpy(array[i], (*head)->string, STRING_LENGTH);
        array[i][STRING_LENGTH]='\0';
        printf("%s\n", array[i]);
        dequeue(head,tail);
        i++;
    }

    return array;
}

char** sortArray(char**array, int length){
    for(unsigned i=0; i<length; i++){
        for(unsigned j=0; j<length-1; j++){
            if(strcmp(array[j], array[j+1])>0){
                char hold[STRING_LENGTH];
                strcpy(hold, array[j]);
                strcpy(array[j], array[j+1]);
                strcpy(array[j+1], hold);
            }
        }
    }

    return array;
}

void printArray(char** array, int length){
    for(unsigned i=0; i<length; i++){
        printf("%s\n", array[i]);
    }
}