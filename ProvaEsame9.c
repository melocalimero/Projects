/* Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione main)
 costituito dal nome di un file di testo (ES: input.txt) ed un numero F
 in virgola mobile in [1.0, 20.0]; il file contiene una collezione di
 stringhe, una per riga; si suppone che la lunghezza di ogni stringa
 sia non superiore ai 30 caratteri; Ogni coppia di stringhe rappresenta
 un singolo record, il quale risulta composto da un numero in virgola
 mobile (in una riga) ed una parola (nella riga successiva)-- vedi
 input in seconda pagina
 ● B. il programma legga tutte le stringhe contenute nel file di input e
 le memorizzi in una lista doppiamente concatenata in cui ogni record
 contenga il numero in virgola mobile (sia V) e la parola (sia P); i
 record vanno inseriti all’interno della lista sulla base
 dell’ordinamento lessicografico crescente del campo P;
 ● C. Stampi la lista ottenuta sullo standard output, dal primo
 all’ultimo elemento della lista stessa;
 ● D. Stampi infine il numero di record della lista tali che V >= F*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 31

typedef struct parameters{
    char* filename;
    double F;
}Parameters;

typedef struct node{
    char P[STRING_LENGTH];
    double V;
    struct node* next;
    struct node* prev;
}Node;

Parameters* readInput(char* argv[]);
void insertRecord(Node** head, char* string, double number);
void printList(Node* head);
void buildList(Parameters* p, Node**head);
void elab(Parameters*p, Node*head);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argv);
    Node* head=NULL;

    buildList(p, &head);
    elab(p,head);
    printList(head);

}

Parameters* readInput(char* argv[]){
    int count=0;
    int i=0;
    while(argv[i]!=NULL){
        count++;
        i++;
    }

    if(count!=3){
        fprintf(stderr,"Usage: <./a.out> <string> <double>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"String extension MUST be .txt\n");
        exit(1);
    }
    
    double k=strtod(argv[2], &argv[2]);
    if(k<1.0 || k>20.0){
        fprintf(stderr,"Double must be in range [1.0-20.0]\n");
        exit(1);
    }

    Parameters*p=malloc(sizeof(Parameters));
    p->filename=argv[1];
    p->F=k;

    return p;
}

void insertRecord(Node** head, char* string, double number){
    Node* nuovo=malloc(sizeof(Node));
    strcpy(nuovo->P, string);
    nuovo->V=number;
    nuovo->next=NULL;
    nuovo->prev=NULL;

    //Lisat vuota
    if(*head==NULL){
        (*head)=nuovo;
        return;
    }

    //Inserimento in testa
    
    if(strcmp(nuovo->P, (*head)->P)<0){
        nuovo->next=(*head);
        (*head)->prev=nuovo;
        (*head)=nuovo;
        return;
    }

    //inserimento in mezzo alla lista
    Node* curr=(*head);
    while(curr->next!=NULL){
        if(strcmp(nuovo->P, curr->next->P)<0){
            nuovo->next=curr->next;
            nuovo->prev=curr;
            curr->next=nuovo;
            return;
        }
        curr=curr->next;
    }

    //inserimento alla fine
    if(curr->next==NULL){
        curr->next=nuovo;
        nuovo->prev=curr;
        return;
    }
}

void printList(Node* head){
    Node* curr=head;
    while(curr!=NULL){
        printf("%f\n%s\n", curr->V, curr->P);
        curr=curr->next;
    }
}


void buildList(Parameters* p, Node**head){
    FILE* fp=fopen(p->filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    while(!feof(fp)){
        char string[STRING_LENGTH];
        char string1[STRING_LENGTH];
        if(!(fgets(string, STRING_LENGTH, fp))){
            fprintf(stderr,"Error reading from file\n");
            exit(1);
        }

        if(!(fgets(string1, STRING_LENGTH, fp))){
            fprintf(stderr,"Error reading from file");
            exit(1);
        }
        char* string_buffer[STRING_LENGTH];
        double k=strtod(string, string_buffer);
        insertRecord(head, string1, k);
        
        
    }
}

void elab(Parameters*p, Node*head){
    Node* curr=head;
    int count=0;
    while(curr!=NULL){
        if(curr->V>=p->F){
            count++;
        }
        curr=curr->next;
    }

    printf("La lista ha %d record con V maggiore uguale di F\n", count);
}