/*Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione main)
 costituito dal nome di un file di testo (ES: input.txt) ed un numero k
 in [7,13]; il file contiene una collezione di stringhe, una per riga;
 si suppone che la lunghezza di ogni stringa sia non superiore ai 30
 caratteri;
 ● B. Legga tutte le stringhe contenute nel file di input e le memorizzi
 in una lista doppiamente concatenata che viene mantenuta con
 ordinamento lessicografico crescente;
 ● C. Stampi la lista ottenuta sullo standard output, dal primo
 all’ultimo elemento;
 ● D. Stampi infine il numero di stringhe della lista cha ha una
 lunghezza pari o maggiore di k.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 31

typedef struct parameters{
    char* input_filename;
    int k;
}Parameters;

typedef struct node{
    char string[STRING_LENGTH];
    struct node* prev;
    struct node*next;
}Node;

Parameters* readInput(int argc, char* argv[]);
void insertWord(Node**head, char* word);
void printList(Node*head);
void buildList(Node**head, Parameters*p);
void elab(Node* head, Parameters*p);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc,argv);
    Node* head=NULL;
    
    buildList(&head, p);
    printList(head);
    elab(head, p);


    free(p);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc<3){
        fprintf(stderr,"Usage: <./a.out> <string> <int>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4,".txt")!=0){
        fprintf(stderr,"string extension MUST be .txt\n");
        exit(1);
    }

    int k=atoi(argv[2]);
    if(k<7 || k>13){
        fprintf(stderr, "int must be in range [7,13]\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->input_filename=argv[1];
    p->k=k;

    return p;
}

void insertWord(Node**head, char* word){
    Node* nuovo=malloc(sizeof(Node));


    strcpy(nuovo->string, word);
    nuovo->next=NULL;
    nuovo->prev=NULL;
    //4 casi: 1.lista vuota  2.Inserimento in testa  3.Inserimento casuale  4.Inserimento alla fine
    
    //1. lista vuota
    if(*head==NULL){
        (*head)=nuovo;
        return;
    }

    //2.Inserimento in testa
    if(strcmp(nuovo->string, (*head)->string)<0){
        nuovo->next=(*head);
        (*head)->prev=nuovo;
        (*head)=nuovo;
        return;
    }

    //3.Inserimento in mezzo
    Node* curr=(*head);
    while(curr->next!=NULL){
        if(strcmp(nuovo->string, curr->next->string)<0){
            nuovo->next=curr->next;
            nuovo->prev=curr;
            curr->next=nuovo;
            return;
        }

        curr=curr->next;
    }

    //4.inserimento in fondo
    nuovo->prev=curr;
    curr->next=nuovo;
    

free(nuovo);
}

void printList(Node*head){
    Node* curr=head;

    if(head==NULL){
        printf("List is empty\n");
        return;
    }

    while(curr!=NULL){
        printf("%s\n", curr->string);
        curr=curr->next;
    }

    if(curr==NULL){
        printf("Finished list\n");
    }
}

void buildList(Node**head, Parameters*p){
    FILE*fp=fopen(p->input_filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    while(!feof(fp)){
        char string[STRING_LENGTH];
        if((fscanf(fp,"%30s", string)<1)){
            fprintf(stderr,"Error reading from file\n");
            return;
        }
        insertWord(head, string);
    }

    if(feof(fp)){
        printf("Finished data\n");
    }
    

    fclose(fp);
}

void elab(Node* head, Parameters*p){
    int count=0;
    Node* curr=head;
    if(head==NULL){
        printf("List is empty\n");
        return;
    }

    while(curr!=NULL){
        if(strlen(curr->string)>=p->k){
            count++;
        }
        curr=curr->next;
    }
    printf("Numero di stringhe che hanno lunghezza maggiore di %d: %d\n",p->k, count);
}