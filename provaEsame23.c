/*Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione
 main) costituito dal nome di un file di testo (ES:
 input3.txt). il file contiene una collezione di stringhe, una
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
    struct node* next;
}Node;

char* readInput(int argc, char* argv[]);
int enqueue(Node** head, Node** tail, char* string);
void printQueue(Node* head);
int getQueue(Node** head, Node** tail, char* filename);
char* dequeue(Node** head, Node** tail);
char** getArray(Node** head, Node** tail, int count);
void sortArray(char**array, int dim);


int main(int argc, char* argv[]){
    char* filename=readInput(argc, argv);
    Node* head=NULL;
    Node* tail=NULL;

    int count=getQueue(&head, &tail, filename);
    printf("\n==getQueue()==\n");
    printQueue(head);
    printf("\n==getArray()==\n");
    char** array=getArray(&head, &tail, count);

    sortArray(array, count);
}

char* readInput(int argc, char* argv[]){
    if(argc!=2){
        fprintf(stderr,"Usage: <%s> <string>\n", argv[0]);
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"String extension must be .txt\n");
        exit(1);
    }

    return argv[1];
}

int enqueue(Node** head, Node** tail, char* string){
    Node* nuovo=malloc(sizeof(Node));
    strncpy(nuovo->string,string, STRING_LENGTH );
    nuovo->next=NULL;

    if(*head==NULL){
        *head=nuovo;
        *tail=nuovo;
        return 1;
    }

    (*tail)->next=nuovo;
    (*tail)=(*tail)->next;
    return 1;
}

void printQueue(Node* head){
    Node* curr=head;
    while(curr!=NULL){
        printf("%s\n", curr->string);
        curr=curr->next;
    }
}


int getQueue(Node** head, Node** tail, char* filename){
    FILE* fp=fopen(filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    int count=0;
    while(!feof(fp)){
        char string[STRING_LENGTH];
        if(fscanf(fp,"%s",string )<1){
            if(!feof(fp)){
                fprintf(stderr,"Error reading from file\n");
                exit(1);
            }
        }
        count+=enqueue(head, tail, string);
    }

    return count;
}

char* dequeue(Node** head, Node** tail){
    if(*head==NULL){
        printf("Queue is empty\n");
        exit(1);
    }

    Node* temp=(*head);
    (*head)=(*head)->next;
    return temp->string;
}

char** getArray(Node** head, Node** tail, int count){
    char** array=malloc(sizeof(char*)*count);
    for(unsigned i=0; i<count; i++){
        array[i]=malloc(sizeof(char)*STRING_LENGTH);
    }
    int i=0;
    while(*head!=NULL){
        strncpy(array[i], dequeue(head, tail), STRING_LENGTH);
        printf("%s\n", array[i]);
        i++;
    }

    return array;
}

void sortArray(char**array, int dim){
    int count=0;
    while(count<dim-1){
        for(unsigned i=count+1; i<=dim; i++){
            printf("i:%u\n", i);
            int index=i-1;
            char key[STRING_LENGTH]={};
            strcpy(key, array[i]);
            printf("\n");
            if(strcmp(key, array[index])>0){
                continue;
            }
            printf("\n");
            while(strcmp(key, array[index])<0){
                strcpy(array[index+1], array[index]);
                index--;
            }
            printf("\n");

            strcpy(array[index+1], key);
        }
        count++;
    }
    printf("\n==sortArray()==\n");
    for(unsigned i=0; i<dim; i++){
        printf("%s\n", array[i]);
    }

}