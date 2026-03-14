/*Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione
 main) costituito dal nome di un file di testo (ES: input.txt)
 e due lettere minuscole dell’alfabeto a e b, per cui sia
 a<=b. Il file contiene una collezione di stringhe, una per
 riga; si suppone che la lunghezza di ogni stringa sia non
 superiore ai 30 caratteri;
 ● B. Legga tutte le stringhe contenute nel file di input e le
 memorizzi in una struttura dati dinamica di tipo FIFO (una
 CODA);
 ● C. Stampi tutte le stringhe, estraendole dalla coda, mediante
 una sequenza di operazioni ``dequeue``;
 ● D. con riferimento al punto C, tutti i caratteri delle
 stringhe compresi nell’intervallo [a, b], siano sostituiti, a
 schermo, dal carattere ‘0’ (zero).*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 31

typedef struct parameters{
    char* input_filename;
    char a;
    char b;
}Parameters;

typedef struct node{
    char string[STRING_LENGTH];
    struct node* next;
}Node;

void printQueue(Node* head, Node* tail,Parameters*p);

Parameters* readInput(int argc, char* argv[]);
void enqueue(Node** head, Node**tail, char*string);
void dequeue(Node** head, Node** tail);
void buildQueue(Parameters*p, Node**head, Node**tail);
char* elab(char* string, char a, char b);

int main(int argc, char* argv[]){
    Parameters*p=readInput(argc,argv);
    Node* head=NULL;
    Node* tail=NULL;
    
    buildQueue(p,&head,&tail);
    printQueue(head,tail,p);

    free(p);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc<4){
        fprintf(stderr, "Usage: <./a.out> <string> <char> <char>\n");
        exit(1);
    }
    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"String extension MUST be .txt\n");
        exit(1);
    }
    if(*(argv[2])<'a' || *(argv[2])>'z'){
        fprintf(stderr,"First char must be lowercase\n");
        exit(1);
    }

    if(*(argv[3])<'a' || *(argv[3])>'z'){
            fprintf(stderr,"First char must be lowercase\n");
            exit(1);
    }

    if(*(argv[2])>*(argv[3])){
        fprintf(stderr,"First char must come first in the alphabet\n");
        exit(1);
    }

    Parameters*p=malloc(sizeof(Parameters));
    p->input_filename=argv[1];
    p->a=*(argv[2]);
    p->b=*(argv[3]);

    return p;

}

void enqueue(Node** head, Node**tail, char*string){
    Node* nuovo=malloc(sizeof(Node));
    if(nuovo==NULL){
        fprintf(stderr,"Error allocating memory\n");
        exit(1);
    }
    strcpy(nuovo->string, string);

    if(*head==NULL){
        (*head)=nuovo;
        (*tail)=nuovo;
        return;
    }

    if(*head!=NULL){
        (*tail)->next=nuovo;
        (*tail)=(*tail)->next;
        return;  
    }

    free(nuovo); 
    
}

void dequeue(Node** head, Node** tail){
    if((*head)==NULL){
        printf("Queue is empty\n");
        return;
    }
    if((*head)==(*tail)){
        Node* temp=(*head);
        (*head)=NULL;
        (*tail)=NULL;
        free(temp);
        return;
    }

    Node*temp=(*head);
    (*head)=(*head)->next;
    free(temp);
    return;
}

void buildQueue(Parameters*p, Node**head, Node**tail){
    FILE* fp=fopen(p->input_filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    while(!feof(fp)){
        char string[STRING_LENGTH];
        if(!(fgets(string,STRING_LENGTH,fp))){
            fprintf(stderr,"Error reading from file\n");
            exit(1);
        }
        
        enqueue(head,tail,string);
    }
    if(feof(fp)){
        printf("Finished data\n");
    }

    fclose(fp);

}



char* elab(char* string, char a, char b){
    int length=strlen(string);
    for(unsigned i=0; i<length; i++){
        if(string[i]>=a &&string[i]<=b){
            string[i]='0';
        }
    }

    return string;
}

void printQueue(Node* head, Node* tail, Parameters*p){
    

    while(head!=NULL){
        strcpy(head->string, elab(head->string,p->a,p->b));
        printf("%s",head->string);
        dequeue(&head,&tail);
        
    }
   
    return;
}