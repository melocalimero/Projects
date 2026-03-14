/*Scrivere un programma in C che: 
A. Prenda un input da tastiera (argomenti della funzione main) 
costituito da due caratteri a e b, due numeri interi x e y, ed un 
numero intero n. Tali parametri devono rispettare le seguenti 
specifiche:  
o I due caratteri a e b devono appartenere al range [‘a’,’z’] 
oppure [‘A’, ‘Z’] (quindi entrambi minuscoli o entrambi 
maiuscoli); inoltre deve essere a<=b. Quindi ad esempio 
a=’C’, b=’Y’ oppure a=’d’, b=’h’; 
o numeri x e y devono appartenere al range [5,30], ed inoltre 
deve essere y-x>5;  
o il numero n deve appartenere al range [15,25].   
B. Produca n stringhe  che rispettino le seguenti specifiche:  
o Ogni stringa sia generata con caratteri pseudo-casuali 
appartenenti al range [a,b]; 
o la lunghezza di ogni stringa, sia L, dovra’ essere un 
numero subpseudo-casuale appartenente al range [x,y] (per 
ogni stringa sara’ generato un nuovo numero L in [x,y]). 
C. inserisca ogni stringa in una struttura dati dinamica CODA, e le 
stampi sullo standard output. 
D. Rimuova dalla coda tutti gli elementi e stampi sullo standard 
output le stringhe contenute all’interno di tali elementi, come 
specificato nel successivo punto E. 
E. Con riferimento al punto D, in fase di output, le vocali di ogni 
stringa andranno sostituite con il carattere ‘X’ (x maiuscolo). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

typedef struct parameters{
    char a,b;
    int x, y, n;
}Parameters;

typedef struct node{
    char *string;
    struct node* next;
}Node;

Parameters* readInput(int argc, char* argv[]);
char* genString(Parameters* p, int L);
void enqueue(Node** head, Node** tail, char* string, int L);
char* dequeue(Node** head, Node** tail);
void buildQueue(Node** head, Node** tail, Parameters* p);
void printStrings(Node** head, Node** tail);
char* elabString(char* string);

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

    buildQueue(&head,&tail, p);
    printStrings(&head, &tail);

    free(p);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc!=6){
        fprintf(stderr,"Usage: <./a.out> <char> <char> <int> <int> <int>\n");
        exit(1);
    }

    int x=atoi(argv[3]);
    int y=atoi(argv[4]);
    int n=atoi(argv[5]);

    if((*(argv[1]))<'a' && (*(argv[1]))>'Z'){
        fprintf(stderr, "The first char must be in range ['a','z'] or ['A','Z']\n");
        exit(1);
    }

    if(*(argv[2])<'a' && *(argv[2])>'Z'){
        fprintf(stderr, "The second char must be in range ['a','z'] or ['A','Z']\n");
        exit(1);
    }

    if(islower(*(argv[1]))&& isupper(*(argv[2]))){
        
        fprintf(stderr,"The two chars must be in the same case, ex: A B or a b\n");
        exit(1);
        
    }

    if(islower(*(argv[2]))&& isupper(*(argv[1]))){
        fprintf(stderr,"The two chars must be in the same case, ex: A B or a b\n");
        exit(1);
    }

    if(*(argv[1])>'A' && *(argv[1])<'Z' && *(argv[2])>'a' && *(argv[2])<'z'){
        
        fprintf(stderr,"The two chars must be in the same case, ex: A B or a b\\n");
        exit(1);
        
    }

    if((*(argv[1]))>*(argv[2])){
        fprintf(stderr,"First char must come first in the alphabet than the second\n");
        exit(1);
    }

    if(x<5 || x>30){
        fprintf(stderr,"First int must be in range [5,30]\n");
        exit(1);
    }

    if(y<5 || y>30){
        fprintf(stderr,"Second int must be in range [5,30]\n");
        exit(1);
    }

    if((y-x)<5){
        fprintf(stderr,"Y-X must be greater than 5\n");
        exit(1);
    }

    if(n<15 || n>25){
        fprintf(stderr,"Third int must be in range [15,30]\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->a=*(argv[1]);
    p->b=*(argv[2]);
    p->n=n;
    p->x=x;
    p->y=y;

    return p;
    
}

char* genString(Parameters* p, int L){
    
    char* string=malloc(sizeof(char)*L+1);
    for(unsigned i=0; i<L; i++){
        string[i]=get_random()%((p->b)-(p->a)+1)+(p->a);
    }
    string[L]='\0';

    return string;
}

void enqueue(Node** head, Node** tail, char* string, int L){
    Node* nuovo=malloc(sizeof(Node));
    nuovo->string=malloc(sizeof(char)*L);
    strcpy(nuovo->string, string);
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

char* dequeue(Node** head, Node** tail){
    if(*head==NULL){
        printf("Queue is already empty\n");
        exit(1);
    }

    if(*head==*tail){
        Node* temp=*head;
        (*head)=NULL;
        (*tail)=NULL;
        char* string=temp->string;
        free(temp);
        return string;
    }

    Node* temp=*head;
    (*head)=(*head)->next;
    char* string=temp->string;
    free(temp);
    return string;
}

void buildQueue(Node** head, Node** tail, Parameters* p){
    for(unsigned i=0; i<p->n; i++){
        int L=get_random()%(p->y-p->x+1)+p->x;
        char* string=malloc(sizeof(char)*L);
        strcpy(string, genString(p,L));
        enqueue(head, tail, string, L);
        printf(" buildQueue(): %s\n", string);
        free(string);
    }
}

void printStrings(Node** head, Node** tail){
    while(*head!=NULL){
        char* string=dequeue(head, tail);
        string=elabString(string);
        printf("printString(): %s\n", string);
        free(string);
    }
}

char* elabString(char* string){
    int length=strlen(string);
    for(unsigned i=0; i<length; i++){
        switch(string[i]){
            case 'a':
            case 'A':
            case 'e':
            case 'E':
            case 'i':
            case 'I':
            case 'o':
            case 'O':
            case 'u':
            case 'U':
                string[i]='X';
                break;
        }
    }
    return string;
}