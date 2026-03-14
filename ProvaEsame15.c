/*Scrivere un programma in C che:
1.	A. Prenda in input da tastiera (argomenti della funzione main) un intero positivo M in [6,12] e due caratteri c1 e c2 in [′A′,′Z′]
Il programma deve verificare che i parametri M,c1,c2 rispettino le specifiche e restituire, in caso contrario, un messaggio di errore.
2.	B. Iteri la seguente procedura M volte, facendo uso di una struttura dati FIFO dinamica (coda o queue) 
da implementare mediante lista concatenata semplice (front == testa della lista, rear == coda della lista):
    o	Per ognuno degli M passi, si generi un numero intero pseudo-casuale y in [1,7]
        o	Se y è pari, si effettuino y operazioni di inserimento (enqueue) di numeri pseudo-casuali in [10,99].
        o	Se y è dispari, si effettuino y operazioni di inserimento (enqueue) di caratteri pseudo-casuali in [′a′,′z′].

    o	Sia v il generico elemento da inserire nella coda:
        o	Se v è uguale a c1, inserire al posto di esso il carattere #.
        o	Se v è uguale a c2, inserire al posto di esso il carattere !.

3.	C. Stampi, sullo standard output, l’intera coda dal front al rear al termine delle operazioni.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct parameters{
    int M;
    char c1, c2;
}Parameters;

typedef struct node{
    char c;
    struct node* next;
}Node;

Parameters* readInput(int argc, char* argv[]);
void enqueue(Node** head,Node** tail, char character);
void buildQueue(Node** head, Node** tail, Parameters* p);
void printQueue(Node* head);

unsigned int get_random() {
    static unsigned int m_w = 123456;
    static unsigned int m_z = 789123;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
 }

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc, argv);
    Node* head=NULL;
    Node* tail=NULL;

    buildQueue(&head,&tail,p);
    printQueue(head);
    free(p);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc!=4){
        fprintf(stderr,"Usage: <./a.out> <int> <char> <char>\n");
        exit(1);
    }

    int M=atoi(argv[1]);
    if(M<6 || M>12){
        fprintf(stderr,"Int MUST be in range [6,12]\n");
        exit(1);
    }

    if(*(argv[2])<'A' || *(argv[2])>'Z'){
        fprintf(stderr,"First char MUST be in range ['A'-Z']\n");
        exit(1);
    }

    if(*(argv[3])<'A' || *(argv[3])>'Z'){
        fprintf(stderr,"Second char MUST be in range ['A'-'Z']\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->M=M; 
    p->c1=*(argv[2]);
    p->c2=*(argv[3]);
    return p;
}

void enqueue(Node** head,Node** tail, char character){
    Node* nuovo=malloc(sizeof(Node));
    nuovo->c=character;
    nuovo->next=NULL;

    if(*head==NULL){
        (*head)=nuovo;
        (*tail)=nuovo;
        return;
    }

    (*tail)->next=nuovo;
    (*tail)=(*tail)->next;
    return;
}


void buildQueue(Node** head, Node** tail, Parameters* p){
    for(unsigned i=0; i<p->M; i++){
        int y=get_random()%(7)+1;
        if(y%2==0){
            for(unsigned j=0; j<y; j++){
                char random=get_random()%('M'-'A'+1)+'A';
                if(random==p->c1){
                    enqueue(head, tail, '#');
                }
                else{
                    enqueue(head, tail, random);
                }
            }
        }
        else{
            for(unsigned j=0; j<y; j++){
                char random=get_random()%('Z'-'O'+1)+'O';
                if(random==p->c2){
                    enqueue(head, tail, '!');
                }
                else{
                    enqueue(head, tail, random);
                }
            }   
        }
    }
}

void printQueue(Node* head){
    Node* curr=head;
    while(curr!=NULL){
        printf("%c\n", curr->c);
        curr=curr->next;
    }
}