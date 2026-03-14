/*Scrivere un programma in C che: 
A. Prenda un input da tastiera (argomenti della funzione main) 
costituito da due numeri in virgola mobile x e y, ed un numero 
intero n. Tali parametri devono rispettare le seguenti 
specifiche:  
o I numeri x e y devono entrambi appartenere al range 
[1.0,1000.0], y>x, ed infine deve essere (y-x)>500.0;  
o il numero n deve appartenere al range [20,30].   
B. Produca n numeri in virgola mobile (double) che appartengano al 
range [x, y];  
C. Inserisca ogni numero in una struttura dati dinamica PILA, e tale 
numero sia stampato sullo standard output. 
D. Rimuova dalla pila tutti gli elementi e stampi sullo standard 
output i numeri contenuti all’interno degli elementi stessi.  
E. Con riferimento al punto D, gli elementi rimossi dalla pila vanno 
contestuamente salvati all’interno di un array di n elementi 
double.  
F. Si stampi infine la media degli n valori contenuti all’interno 
dell’array ed il numero di elementi che risultino maggiori della 
media stessa.  */

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef struct parameters{
    double x,y;
    int n;
}Parameters;

typedef struct node{
    double n;
    struct node* next;
}Node;

Parameters* readInput(int argc, char* argv[]);
double genDouble(Parameters* p);
void push(Node** head, double n);
double pop(Node** head);
void buildStack(Node** head, Parameters* p);
double* buildArray(Node** head, Parameters* p);
void elabValues(double* array, Parameters* p);


unsigned int get_random() { 
    static unsigned int m_w = 123456; 
    static unsigned int m_z = 789123; 
    m_z = 36969 * (m_z & 65535) + (m_z >> 16); 
    m_w = 18000 * (m_w & 65535) + (m_w >> 16); 
    return (m_z << 16) + m_w; 
} 

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc,argv);
    Node* head=NULL;

    printf("\nBuildStack()\n");
    buildStack(&head, p);
    printf("\nbuildArray():\n");
    double* array=buildArray(&head, p); 
    printf("\nelabValues():\n");
    elabValues(array,p);
   
}

Parameters* readInput(int argc, char* argv[]){
    if(argc!=4){
        fprintf(stderr,"Usage <%s> <double> <double> <int>\n", argv[0]);
        exit(1);
    }

    double x=strtod(argv[1], &argv[1]);
    double y=strtod(argv[2], &argv[2]);

    if(x<1 || x>1000){
        fprintf(stderr,"First double MUST be in range [1.0-1000.0]\n");
        exit(1);
    }
    
    if(y<1 || y>1000){
        fprintf(stderr,"Second double MUST be in range [1.0-1000.0]\n");
        exit(1);
    }

    if(y<x){
        fprintf(stderr,"Second double MUST be greater than the first\n");
        exit(1);
    }

    if((y-x)<500){
        fprintf(stderr,"Difference between the two doubles must not be greater than 500\n");
        exit(1);
    }

    int n=atoi(argv[3]);
    if(n<20 || n>30){
        fprintf(stderr,"Int must be in range [20,30]\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->x=x;
    p->y=y;
    p->n=n;
    return p;
}

double genDouble(Parameters* p){
    double number=(double)get_random()/(double)UINT_MAX;
    number=number*(p->y-p->x)+p->x;
    return number;
}

void push(Node** head, double n){
    Node* nuovo=malloc(sizeof(Node));
    nuovo->n=n;
    nuovo->next=NULL;
    
    if(*head==NULL){
        *head=nuovo;
        return;
    }

    nuovo->next=*head;
    (*head)=nuovo;
    return;
}

double pop(Node** head){
    if(*head==NULL){
        printf("Stack is empty\n");
        exit(1);
    }

    Node* temp=*head;
    (*head)=(*head)->next;
    return temp->n;
}

void buildStack(Node** head, Parameters* p){
    for(unsigned i=0; i<p->n; i++){
        double number=genDouble(p);
        push(head, number);
        printf("DatoInserito(%u):%f\n",i, number);
    }
    
}

double* buildArray(Node** head, Parameters* p){
    double* array=malloc(sizeof(double)*p->n);
    int i=0;
    while(*head!=NULL && i<p->n){
        array[i]=pop(head);
        printf("DatoEliminato(%u):%f\n", i, array[i]);
        i++;
    }

    return array;
}

void elabValues(double* array, Parameters* p){
    double somma=0;
    for(unsigned i=0; i<p->n; i++){
        somma+=array[i];
    }

    double media=somma/p->n;
    int count=0;
    for(unsigned i=0; i<p->n; i++){
        if(array[i]>media){
            count++;
        }
    }
    printf("Media dei valori di array:%f\nNumero valori di array maggiori della media:%d\n", media, count);
}