/* Scrivere un programma in C che:
 ● A. Prenda un input da tastiera (argomenti della funzione
 main) costituito dal nome di un file di testo (ES: input.txt)
 ed un numero intero “k” nell’intervallo [5-15]. il file
 contiene una collezione di stringhe, una per riga; si suppone
 che la lunghezza di ogni stringa sia non superiore ai 30
 caratteri;
 ● B. Legga tutte le stringhe contenute nel file di input e le
 memorizzi in una struttura dati di tipo stack;
 ● C. Stampi tutte le stringhe mediante una sequenza di
 operazioni di rimozione dallo stack (pop()).
 ● D. con riferimento al punto C, i caratteri delle stringhe con
 lunghezza maggiore o uguale al numero k vanno stampate in
 ordine lessicografico crescente*/

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
    struct node* next;
}Node;



Parameters* readInput(int argc, char* argv[]);
void pushWord(char* string, Node** top);
void buildStack(Parameters* p, Node**top);
void popWord(Node** top);
char* sortWord(char* string);
void printStack(Node* top, Parameters*p);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc,argv);
    Node* top=malloc(sizeof(Node));
    top=NULL;

    buildStack(p, &top);
    printStack(top,p);

//dealloco p
    free(p);

    //dealloco top
    free(top);
}

Parameters* readInput(int argc, char* argv[]){
    if(argc<3){
        perror("Usage: <./a.out> <input file name> <int>");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4,".txt")!=0){
        fprintf(stderr,"Wrong input file name extensione, MUST be .txt\n");
        exit(1);
    }

    if(atoi(argv[2])<5 || atoi(argv[2])>15){
        fprintf(stderr,"Int must be between 5 and 15\n");
        exit(1);
    }

    Parameters*p=malloc(sizeof(Parameters));

    p->input_filename=argv[1];
    p->k=atoi(argv[2]);

    return p;
}


void pushWord(char* string, Node** top){
    //2 casi: stack vuoto, stack non vuoto
    Node* nuovo=malloc(sizeof(Node));
    strcpy(nuovo->string, string);
    //1. stack vuoto
    if((*top)==NULL){
        (*top)=nuovo;
        return;
    }
    //2. stack non vuoto
    if(*top!=NULL){
        nuovo->next=(*top);
        (*top)=nuovo;
        return;
    }

free(nuovo);
}

void buildStack(Parameters* p, Node**top){
    FILE* fp=fopen(p->input_filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    
    while(!feof(fp)){
        char string[STRING_LENGTH];
        fscanf(fp, "%s",string );
        pushWord(string, top);
    }

    fclose(fp);
}



void popWord(Node** top){
    //1: pila vuota
    //2: pila con un elemento
    //3: pila con piu elementi
    //pila vuota
    if((*top)==NULL){
        printf("Stack is empty\n");
        return;
    }


    //2: pila con un solo elemento
    if((*top)->next==NULL){
        Node* temp=(*top);
        (*top)=NULL;
        free(temp);
        return;
    }

    //3: più di un elemento
    if((*top)->next!=NULL){
        Node* temp=(*top);
        (*top)=(*top)->next;
        free(temp);
        return;
    }
}

char* sortWord(char* string){
    int length=strlen(string);
    for(unsigned i=0; i<length; i++){
        for(unsigned j=0; j<length-1; j++){

            if(string[j]>string[j+1]){
                char hold=string[j];
                string[j]=string[j+1];
                string[j+1]=hold;
            }
        }
    }   

    return string;
    
}

void printStack(Node* top, Parameters* p){
    Node* curr=top;

    while(curr!=NULL){
        if(strlen(curr->string)>=p->k){
            strcpy(curr->string, sortWord(curr->string));
            printf("%s\n",curr->string);
            popWord(&top);
            
        }
        else{
            printf("%s\n",curr->string);
            popWord(&top);
        }
        curr=curr->next;
    }
    if(curr==NULL){
        printf("Finished stack\n");
        free(curr);
        exit(1);
    }

    
}