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
void enqueueWord(Node**head, Node**tail, char* word);
void dequeueWord(Node** head, Node** tail);
void buildqueue(Node** head, Node** tail, char* filename);
void buildStringArray(Node** head, Node** tail, int length);
void printArray(char **array);

int main(int argc, char* argv[]){
    char* input_filename=readInput(argc,argv);
    Node* head=NULL;
    Node* tail=NULL;

    buildqueue(&head, &tail, input_filename);
    

    tail->count=head->count;
    int length=head->count;
    buildStringArray(&head,&tail, length);
    //char** array=buildStringArray(&head,&tail);
    head->count=tail->count;
    printf("prova\n");

    printf("\nTail count:%d\n", tail->count);
}

char* readInput(int argc, char* argv[]){
    if(argc<2){
        fprintf(stderr,"Usage: <./a.out> <string>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4,".txt")!=0){
        fprintf(stderr, "String extension MUST be .txt\n");
        exit(1);
    }

    return argv[1];
}

void enqueueWord(Node**head, Node**tail, char* word){
    Node* nuovo=malloc(sizeof(Node));
    if(!nuovo){
        fprintf(stderr,"Error allocating memory in enqueueWord()\n");
        return;
    }

    strcpy(nuovo->string, word);

    if(*head==NULL){
        (*head)=nuovo;
        (*tail)=nuovo;
        (*head)->count=1;
        return;
    }

    (*tail)->next=nuovo;
    (*tail)=(*tail)->next;
    (*head)->count++;
    
}

void dequeueWord(Node** head, Node** tail){
    //1.Lista vuota
    if((*head)==NULL){
        printf("Queue is empty\n");
        return;
    }
    printf("dequeuing node with string:%s\n", (*head)->string);

    //2.Lista con un solo elemento
    if((*head)==(*tail)){
        Node*temp=(*head);
        (*head)=NULL;
        (*tail)=NULL;
        free(temp);
        return;
    }
    printf("dequeue completed\n");

    //lista con più elementi
    if((*head)!=(*tail)){
        Node* temp=(*head);
        (*head)=(*head)->next;
        //S(*tail)->count--;
        free(temp);
    }
    
}

void buildqueue(Node** head, Node** tail, char* filename){
    FILE* fp=fopen(filename, "r");
    if(!fp){
        perror("Error opening file");
        return;
    }

    while(!feof(fp)){
        char string[STRING_LENGTH];
        if(!(fscanf(fp,"%s",string))){
            fprintf(stderr,"Error reading from file\n");
            return;
        }
        
        enqueueWord(head,tail,string);
    }
}

void buildStringArray(Node** head, Node** tail, int length){
    printf("length:%d\n",length);
    char** array=malloc(sizeof(char*)*length);
    if(array==NULL){
        fprintf(stderr,"Error allocating memory for array\n");
        exit(1);
    }

    for(unsigned i=0; i<length; i++){
        array[i]=malloc(sizeof(char)*STRING_LENGTH);
        if(array[i]==NULL){
            fprintf(stderr,"Error allocating memory for array[%u]\n", i);
            exit(1);
        }
    }
    int i=0;
    Node* curr=(*head);
    while(curr->next!=NULL){
        strncpy(array[i], curr->string, STRING_LENGTH-1);
        array[i][STRING_LENGTH-1]='\0';
        printf("%s\n", array[i]);
        dequeueWord(head,tail);
        i++;
        curr=curr->next;
    }
    if(curr->next==NULL){
        strncpy(array[i], curr->string, STRING_LENGTH-1);
        array[i][STRING_LENGTH-1]='\0';
        printf("%s\n", array[i]);
        dequeueWord(head,tail);
    }
    

    for(unsigned i=0; i<length; i++){
        free(array[i]);
    }
    free(array);
}

