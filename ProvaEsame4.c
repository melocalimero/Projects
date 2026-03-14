/*Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione
 main) costituito da un intero positivo N in [5,10] e due
 caratteri a e b in [‘a’,’z’]. Il programma deve verificare
 che i parametri N, a e b rispettino le specifiche e
 restituire in caso contrario un messaggio di errore.
 ● B. Iteri la seguente procedura N volte, facendo uso di una
 struttura dati LIFO dinamica (pila o stack) da implementare
 mediante lista concatenata semplice (top==testa della lista):

 ○ per ognuno degli N passi, si generi innanzitutto un
 carattere x in [‘1’-’9’];
 ■ se x rappresenta un numero in [‘1’-’4’], allora si
 proceda ad x operazioni di inserimento (push) di
 caratteri pseudo-casuali che siano vocali, seguite
 dall’inserimento del carattere x;
 ■ se x rappresenta un numero in [‘5’-’9’], allora si
 proceda ad x operazioni di inserimento (push) di
 caratteri pseudo-casuali che siano consonanti,
 seguite dall’inserimento del carattere x;
 ■ in particolare, sia c il generico carattere da
 inserire sullo stack:
 ● se c==a (a rappresenta il primo carattere
 specificato come argomento della funzione
 main), si inserisca sullo stack al posto di
 esso il carattere '*';
 ● se c==b (b rappresenta il secondo carattere
 specificato come argomento della funzione
 main) si inserisca sullo stack al posto di
 esso il carattere '?';
 ■ si inserisca infine il carattere x sullo stack
 ● C. Stampi, sullo standard output, l’intero stack.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct parameters{
    int N;
    char a;
    char b;
}Parameters;

typedef struct node{
    char c;
    struct node*next;
}Node;

Parameters* readInput(int argc, char* argv[]);
char genVowel();
char genConsonant();
void push(Node** top, char c);
void fillStack(Parameters* p,Node**top);
void printStack(Node*top);

int main(int argc, char* argv[]){
    srand(time(0));
    Parameters* p=readInput(argc, argv);
    Node* top=NULL;

    fillStack(p,&top);
    printStack(top);


    free(p);
    free(top);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc<4){
        fprintf(stderr,"Usage: <./a.out> <int> <char> <char>");
        exit(1);
    }
    int N=atoi(argv[1]);
    if(N<5||N>10){
        fprintf(stderr,"int must be between 5 and 10\n");
        exit(1);
    }

    if((*argv[2])<'a'|| (*argv[2])>'z'){
        fprintf(stderr,"First character must be betweeen 'a' and 'z'\n");
        exit(1);
    }

    if((*argv[3])<'a'|| (*argv[3])>'z'){
        fprintf(stderr,"Second character must be betweeen 'a' and 'z'\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));

    p->N=N;
    p->a=(*argv[2]);
    p->b=(*argv[3]);

    return p;
}


char genVowel(){
    char c='b';
    while(c!='a' && c!='e' && c!='i' && c!='o' && c!='u'){
        c=rand()%('u'-'a'+1)+'a';
    
    }

    return c;
}

char genConsonant(){
    char c='a';

    while(c=='a' || c=='e'|| c=='i'|| c=='o'|| c=='u'){
        c=rand()%('z'-'b'+1)+'b';
    }

    return c;
}

void push(Node** top, char c){

    Node* nuovo=malloc(sizeof(Node));
    nuovo->c=c;
    nuovo->next=NULL;

    nuovo->next=(*top);
    (*top)=nuovo;
    
    return;
    free(nuovo);
    
    
}

void fillStack(Parameters* p,Node**top){
    int i=0;
    while(i<p->N){
        char x=rand()%('9'-'1'+1)+'1';
        
        if(x>='1' && x<='4'){
            
            for(unsigned j=0; j<x-'0'; j++){
                char c=genVowel();
                
                if(c==p->a){
                    push(top,'*');
                }
                if(c==p->b){
                    push(top,'?');
                }
                if(c!=p->a && c!=p->b){
                    push(top,c);
                }
            }
            push(top,x);
            
        }

        if(x>='5' && x<='9'){
            for(unsigned j=0; j<x-'0'; j++){
                char c=genConsonant();
                if(c==p->a){
                    push(top,'*');
                }
                else if(c==p->b){
                    push(top,'?');
                }
                else{
                    push(top,c);
                }
            }
            push(top,x);
        }

        i++;
    }

   
}

void printStack(Node*top){
    Node* curr=top;

    if(top==NULL){
        printf("Stack vuoto\n");
        return;
    }
    printf("**TOP->   ");
    while(curr!=NULL){
        printf("%c  ", curr->c);
        curr=curr->next;
    }
    if(curr==NULL){
        printf("  <-BOTTOM**\n");
    }
}