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
#include <ctype.h>

typedef struct parameters{
    char a,b;
    int x,y, n;
}Parameters;

typedef struct node{
    char* string;
    struct node* next;
}Node;

Parameters* readInput(int argc, char* argv[]);
char *genString(Parameters* p, int L);
void enqueue(Node** head,Node** tail, char* string);
char* dequeue(Node** head, Node** tail);
void getQueue(Node** head, Node** tail, Parameters* p);
void printList(Node** head, Node** tail);
int isVowel(char c);
unsigned int get_random() { 
    static unsigned int m_w = 123456; 
    static unsigned int m_z = 789123; 
    m_z = 36969 * (m_z & 65535) + (m_z >> 16); 
    m_w = 18000 * (m_w & 65535) + (m_w >> 16); 
    return (m_z << 16) + m_w; 
} 
int main(int aegc, char* melo[]){
    Parameters* p=readInput(aegc, melo);
    Node* head=NULL;
    Node* tail=NULL;

    printf("\n==getQueue()..==\n");
    getQueue(&head, &tail, p);

    printf("\n==printList()..==\n");
    printList(&head, &tail);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc!=6){
        fprintf(stderr,"Usage: <%s> <char> <char> <int> <int> <int>\n", argv[0]);
        exit(1);
    };

    if(*argv[1]<'A' || *argv[1]>'z'){
        fprintf(stderr,"Both char must be in range ['a', 'Z']\n");
        exit(1);
    }

    if(*argv[2]<'A' || *argv[2]>'z'){
        fprintf(stderr,"Both char must be in range ['a', 'Z']\n");
        exit(1);
    }

    if(*argv[1]>='a'&&* argv[1]<='z' && *argv[2]<'a'){
        fprintf(stderr,"The two chars must be both upper or both lower case\n");
        exit(1);
    }
    if(*argv[1]>='A'&& *argv[1]<='Z' && *argv[2]>'Z'){
        fprintf(stderr,"The two chars must be both upper or both lower case\n");
        exit(1);
    }

    if(*(argv[1])>*(argv[2])){
        fprintf(stderr,"First char must not be greater than the second\n");
        exit(1);
    }

    int x=atoi(argv[3]);
    int y=atoi(argv[4]);
    if(x<5 || x>30){
        fprintf(stderr,"First int must be in range [5,30]\n");
        exit(1);
    }
    if(y<5 || y>30){
        fprintf(stderr,"Second int must be in range [5,30]\n");
        exit(1);
    }

    if((y-x)<5){
        fprintf(stderr,"Second int minus firs int MUST be greater than 5\n");
        exit(1);
    }
    int n=atoi(argv[5]);
    if(n<15 || n>25){
        fprintf(stderr,"Last int must be in range [15,25]\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->a=*(argv[1]);
    p->b=*(argv[2]);
    p->x=x;
    p->y=y;
    p->n=n;

    return p;

}

void enqueue(Node** head, Node** tail, char* string){
    Node* nuovo=malloc(sizeof(Node));
    nuovo->string=string;
    nuovo->next=NULL;
    if(*head==NULL){
        *head=nuovo;
        *tail=nuovo;
        return;
    }

    (*tail)->next=nuovo;
    *tail=(*tail)->next;
    return;
}


char *genString(Parameters* p, int L){
    char* string=malloc(sizeof(char)*L);
    for(unsigned i=0; i<L; i++){
        string[i]=get_random()%((p->b)-(p->a)+1)+(p->a);
    }
    string[L]='\0';
    return string;
}


void getQueue(Node** head, Node** tail, Parameters* p){
    for(unsigned i=0; i<p->n; i++){
        int L=get_random()%(p->y-p->x+1)+p->x;
        char* string=genString(p,L);
        enqueue(head, tail, string);
        printf("%s\n", string);
    }
}



int isVowel(char c){
    if(strchr("aeiou", c))
        return 1;
    return 0;
}

char* dequeue(Node** head, Node** tail){
    if(*head==NULL){
        fprintf(stderr,"\nQueue is empty\n");
        exit(1);
    }

    Node* temp=(*head);
    (*head)=(*head)->next;
    return temp->string;
}

void printList(Node** head, Node** tail){
    while(*head!=NULL){
        char* string=dequeue(head, tail);
        for(unsigned i=0; string[i]!='\0'; i++){
            if(isVowel(string[i])){
                string[i]='X';
            }
        }
        printf("%s\n", string);
    }
}