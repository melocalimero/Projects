/*Scrivere un programma in C che: 
● A. Prenda un input da tastiera (argomenti della funzione main) 
costituito dal nome di un file di testo (ES: input.txt) e due 
numeri x e y, per cui sia x<y, 10 <= x <= 60 e 10 <= y <= 60 
Il file contiene una collezione di record in formato testo, 
una per riga, del tipo  
nome  
cognome  
eta’  
si supponga che la lunghezza di ogni singola stringa contenuta 
in ogni riga sia non superiore ai 30 caratteri;  
● B. Legga tutti i record contenuti nel file di input e li 
memorizzi in modo ordinato (ordine lessicografico crescente) 
in una struttura dati dinamica di tipo lista semplice; 
● C. Stampi tutti i record della lista su standard output, dal 
primo all’ultimo;  
● D. rimuova, dalla lista, tutti i record con eta’ appartenente 
all’intervallo [x, y]; 
● E. stampi sullo standard output il contenuto della lista, dal 
primo all’ultimo elemento; */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 31

typedef struct parameters{
    char *filename;
    int x, y;
}Parameters;

typedef struct node{
    char nome[STRING_LENGTH];
    char cognome[STRING_LENGTH];
    int eta;
    struct node* next;
}Node;

Parameters* readInput(int argc, char* argv[]);
void insertRecord(Node** head, char* nome, char* cognome, int eta);
void buildList(Node** head, Parameters* p);
void printList(Node* head);
void removeByAge(Node** head, Parameters* p);

int main(int argc, char*argv[]){
    Parameters* p=readInput(argc, argv);
    Node* head=NULL;
    buildList(&head, p);
    printList(head);
    removeByAge(&head, p);
    printf("\nList post eliminazione\n");
    printList(head);


    free(p);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc!=4){
        fprintf(stderr, "Usage: <./a.out> <string> <int> <int>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"String extension MUST be .txt\n");
        exit(1);
    }

    int x=atoi(argv[2]);
    int y=atoi(argv[3]);
    if(x>=y){
        fprintf(stderr,"First int MUST be less than the second\n");
        exit(1);
    }

    if(x<10 || x>60){
        fprintf(stderr,"First int MUST be betweeen 10 and 60\n");
        exit(1);
    }

    if(y<10 || y>60){
        fprintf(stderr,"Second int MUST be betweeen 10 and 60\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->filename=argv[1];
    p->x=x;
    p->y=y;

    return p;
}

void insertRecord(Node** head, char* nome, char* cognome, int eta){
    Node* nuovo=malloc(sizeof(Node));
    strcpy(nuovo->cognome, cognome);
    strcpy(nuovo->nome, nome);
    nuovo->eta=eta;
    nuovo->next=NULL;

    //testa vuota
    if(*head==NULL){
        (*head)=nuovo;
        return;
    }

    //inserimento in testa
    if(strcmp(nuovo->cognome, (*head)->cognome)<0){
        nuovo->next=(*head);
        (*head)=nuovo;
        return;
    }

    //inserimento in mezzo
    Node* curr=*head;
    while(curr->next!=NULL){
        if(strcmp(nuovo->cognome, curr->next->cognome)<0){
            nuovo->next=curr->next;
            curr->next=nuovo;
            return;
        }

        curr=curr->next;
    }

    if(curr->next==NULL){
        curr->next=nuovo;
        return;
    }
}

void printList(Node* head){
    Node* curr=head;
    while(curr!=NULL){
        printf("%s %s %d\n", curr->nome, curr->cognome, curr->eta);
        curr=curr->next;
    }
}

void buildList(Node** head, Parameters* p){
    FILE* fp=fopen(p->filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    while(!feof(fp)){
        char nome[STRING_LENGTH];
        char cognome[STRING_LENGTH];
        int eta=0;
        if(fscanf(fp, "%30s %30s %d", nome, cognome, &eta)!=3){
            if(feof(fp)){
                fprintf(stderr,"Finished data\n");
                break;
            }
            else{
                fprintf(stderr,"Error reading from file\n");
                exit(1);
            }
        }
        insertRecord(head, nome, cognome, eta);
    }
    fclose(fp);
}

void removeByAge(Node** head, Parameters* p){
    if((*head)->eta>=p->x && (*head)->eta<=p->y){
       Node* temp=(*head);
       (*head)=(*head)->next;
       free(temp);
    }
    if(*head==NULL){
        printf("List is now empty\n");
        return;
    }
    Node* curr=(*head);
    Node* prev=NULL;
    while(curr!=NULL){
        if(curr->eta>=p->x && curr->eta<=p->y){
            if(prev==NULL){
                Node* temp=curr;
                curr=curr->next;
                (*head)=curr;
                free(temp);
            } 
            else{
                Node* temp=curr;
                curr=curr->next;
                prev->next=curr;
                free(temp);
            }
        }
        else{
            prev=curr;
            curr=curr->next;
        }
    }

}