/*Scrivere un programma in c che:
1) Legga dall'input da tastiera(argomenti della funzione main) il nome di un file
input (input11.txt), il nome di un file output (lab1.bin) e controlli la validità
dei dati inseriti e dia un messaggio di errore quando necessario 
con conseguente chiusura del programma.
Il file conterrà delle stringhe si suppone che la lunghezza delle
stringhe non sia superiore a 30.
 
2)Legga dal file e Inserisca  all'interno di una lista
doppiamente concatenata le stringhe in ordine lessicografico
crescente 

3)Legga dalla lista e per ognuna elimini i caratteri di punteggiatura

4)Stampi la lista eliminando ad una ad una le stringhe in essa.
Inserisca inoltre le stringhe in un array di stringhe

5)Stampi nello stream di output il numero di stringhe palindrome all'interno dell'array

6)Salvi l'array in un file di output binario e lo stampi nello stream di output*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 31

typedef struct parameters{
    char* in_filename;
    char* out_filename;
}Parameters;

typedef struct node{
    char string[STRING_LENGTH];
    int count;
    struct node* next;
    struct node* prev;
}Node;

Parameters* readInput(int argc, char* argv[]);
int insert(Node** head, char* word);
char* delete(Node** head);
char** printList(Node**head, int count);
int buildList(Node** head, Parameters* p);
int countPalindrome(char** array, int dim);
int isPalindrome(char* word);
void save(Parameters*  p, char** array, int dim);
void read(Parameters* p, int count);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc, argv);
    Node* head=NULL;
    
    int count=buildList(&head, p);
    
    char** array=printList(&head, count);
    
    int countPal=countPalindrome(array, count);
    printf("Ci sono %d palindrome\n", countPal);

    save(p, array, count);
    printf("\nRead from file\n");
    read(p, count);

    for(unsigned i=0; i<count; i++){
        free(array[i]);
        array[i]=NULL;
    }
    free(array);
    array=NULL;

}

Parameters* readInput(int argc, char* argv[]){
    if(argc!=3){
        fprintf(stderr,"Usage: <./a.out> <string> <string>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr,"First string extension MUST be .txt\n");
        exit(1);
    }

    if(strcmp(argv[2]+strlen(argv[2])-4, ".bin")!=0){
        fprintf(stderr,"First string extension MUST be .bin\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->in_filename=argv[1];
    p->out_filename=argv[2];
    return p;
}

int insert(Node** head, char* word){
    Node* nuovo=malloc(sizeof(Node));

    strcpy(nuovo->string, word);
    
    nuovo->next=NULL;
    nuovo->prev=NULL;
    if(*head==NULL){
        (*head)=nuovo;
        
        return 1;
    }

    if(strcmp(nuovo->string, (*head)->string)<0){
        nuovo->next=(*head);
        (*head)->prev=nuovo;
        (*head)=nuovo;

        return 1;
    }

    Node* curr=(*head);
    while(curr->next!=NULL){
        if(strcmp(nuovo->string, curr->next->string)<0){
            nuovo->next=curr->next;
            nuovo->prev=curr;
            curr->next->prev=nuovo;
            curr->next=nuovo;
    
            return 1;
        }
        curr=curr->next;
    }
    
    curr->next=nuovo;
    nuovo->prev=curr;
    
    return 1;

}


char* delete(Node** head){
    if(*head==NULL){
        printf("List is empty\n");
        exit(1);
    }
    Node* temp=(*head);
    (*head)=(*head)->next;
    char* string=malloc(sizeof(char)*STRING_LENGTH);
    strcpy(string, temp->string);
    free(temp);
    return string;
    
}

int buildList(Node** head, Parameters* p){
    FILE* fp=fopen(p->in_filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    int count=0;
    while(!feof(fp)){
        char string[STRING_LENGTH];
        if((fscanf(fp,"%s",string)<1)){
            fprintf(stderr,"Error reading from file\n");
            exit(1);
        }
        count+=insert(head, string);
    }
    fclose(fp);
    return count;
}

char** printList(Node** head, int count){
    char** array=malloc(sizeof(char*)*count);
    for(unsigned i=0; i<count; i++){
        array[i]=malloc(sizeof(char)*STRING_LENGTH);
        array[i][STRING_LENGTH]='\0';
    }
    int i=0;
    while((*head)!=NULL){
        printf("%s \n", (*head)->string);
        array[i]=delete(head);
        array[i][STRING_LENGTH]='\0';
        i++;
    }
    return array;
}

int isPalindrome(char* word){
    int length=strlen(word);
    
        for(unsigned i=0; i<length/2; i++){
            if(word[i]!=word[(length-1)-i]){
                return 0;
            }
        }
    
    return 1;
}

int countPalindrome(char** array, int dim){
    int count=0;
    for(unsigned i=0; i<dim; i++){
        if(isPalindrome(array[i])==1){
            
            count++;
        }
    }
    return count;
}

void save(Parameters*  p, char** array, int dim){
    FILE* fp=fopen(p->out_filename, "wb");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    for(unsigned i=0; i<dim; i++){
        if(fwrite(array[i],sizeof(array[i]), 1, fp)!=1){
            fprintf(stderr,"Error writing the %u element\n", i);
            exit(1);
        }
        printf("Wrote on binary file:%s\n", array[i]);
    }
fclose(fp);

}

void read(Parameters* p, int count){
    FILE* fp=fopen(p->out_filename, "rb");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    
    int i=0;
    while(!feof(fp)){
        char** string=malloc(sizeof(char*)*count);
        for(unsigned j=0; j<count; j++){
        string[j]=malloc(sizeof(char)*STRING_LENGTH);
        
    }
        if(fread(string[i], sizeof(string[i]), 1, fp)!=1){
            if(feof(fp)){
                printf("Finished file\n");
                exit(1);
            }
            fprintf(stderr,"Error reading from binary file\n");
            exit(1);
        }
        printf("%s\n", string[i]);
        i++;
    }
    fclose(fp);
}