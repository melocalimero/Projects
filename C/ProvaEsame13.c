/*Scrivere un programma in C che: 
A. Prenda un input da tastiera (argomenti della funzione main) 
costituito dal nome di un file di testo di input (ES: input.txt) 
ed il nome di un file di testo di output (ES: output.txt). Il 
file di input conterra’ del testo in italiano, allegato al 
compito e riportato in questo documento; si suppone che la 
lunghezza di ogni stringa contenuta all’interno del file sia non 
superiore ai 20 caratteri.  
B. Legga tutte le stringhe contenute nel file di input e le 
memorizzi in un array di record (struct), in cui ogni record 
contiene un campo “key” che contiene la parola, un campo “vowels” 
che contiene il numero di vocali presenti nella stringa. Si 
allochi, a tale scopo, un array di 200 records; i record 
dell’array in eccesso (record vuoti) saranno inizializzati (e 
riconoscibili) mediante vowels=-1.  
C. Elabori l’array di record eliminando tutti i caratteri di 
punteggiatura ‘,’, ‘;’, ‘:’, ‘.’ (virgola, punto e virgola, due 
punti e punto) eventualmente presenti nelle parole contenute nei 
record.  
D. Crei una lista doppiamente concatenata che contenga tutti i 
record non vuoti (ovvero tali che vowels >=0 ) dell’array. La 
lista deve essere mantenuta in ordine decrescente rispetto al 
campo vowels. 
E. Elabori la lista eliminando tutti record che rappresentano un 
duplicato di un record esistente.  
F. Stampi sullo standard output tutti gli elementi della lista, a 
partire dallo elemento in testa.  
G. Memorizzi i record della lista in un file di testo con nome 
corrispondente al secondo argomento della funzione main (es: 
output.txt), un record per riga. Il singolo record sara’ 
memorizzato nel file nel seguente modo: [WORD]:[NO_OF_VOWELS]*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 21
#define ARRAY_DIM 200

typedef struct parameters{
    char* input_filename;
    char* output_filename;
}Parameters;

typedef struct record{
    char key[STRING_LENGTH];
    int vowels;
}Record;

typedef struct node{
    char string[STRING_LENGTH];
    int vowels;
    struct node* next;
    struct node* prev;
}Node;

Parameters* readInput(int argc, char* argv[]);
void buildArray(Parameters* p, Record* array);
int countVowels(char* string);
Record* filter(Record* array);
void enqueue(Node** head, char* string, int vowels);
void dequeue(Node** head);
void buildList(Node** head, Record* array);
void printList(Node* head);
void createSet(Node** head);
void save(Node* head, Parameters* p);

int main(int argc, char* argv[]){
    Parameters* p=readInput(argc, argv);
    Record* array=malloc(sizeof(Record)*ARRAY_DIM);
    Node* head=NULL;


    buildArray(p, array);
    array=filter(array);
    buildList(&head, array);
    createSet(&head);
    printList(head);
    save(head, p);
    
    free(p);
    free(array);
    
}

Parameters* readInput(int argc, char* argv[]){
    if(argc !=3){
        fprintf(stderr,"Usage: <./a.out> <string> <string>\n");
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-4, ".txt")!=0){
        fprintf(stderr, "First string extension MUST be .txt\n");
        exit(1);
    }

    if(strcmp(argv[2]+strlen(argv[2])-4, ".txt")!=0){
        fprintf(stderr, "Second string extension MUST be .txt\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->input_filename= argv[1];
    p->output_filename=argv[2];
return p;

}

void buildArray(Parameters* p, Record* array){
    
    if(!array){
        fprintf(stderr,"Error allocating memory in buildArray()\n");
        exit(1);
    }
    FILE* fp=fopen(p->input_filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    int i=0;
    while(!feof(fp)){
        fscanf(fp, "%s",array[i].key );
        array[i].vowels=countVowels(array[i].key);
        i++;
    }

    for(unsigned j=i; j<ARRAY_DIM; j++){
        array[j].vowels=-1;
    }
fclose(fp);
    
}

int countVowels(char* string){
    int length=strlen(string);
    int count=0;
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
                count++;
            break;
        }
    }

    return count;
}

Record* filter(Record* array){
    for(unsigned i=0; i<ARRAY_DIM; i++){
        
        for(unsigned j=0; j<STRING_LENGTH; j++){
            switch(array[i].key[j]){
                case '.':
                case ':':
                case ',':
                case ';':
                    array[i].key[j]='\0';
                break;
            }
        }
    }
    return array;
}

void enqueue(Node** head, char* string, int vowels){
    Node* nuovo=malloc(sizeof(Node));
    strcpy(nuovo->string, string);
    nuovo->vowels=vowels;
    nuovo->next=NULL;
    nuovo->prev=NULL;

    if(*head==NULL){
        (*head)=nuovo;
        return;
    }

    if(nuovo->vowels>(*head)->vowels){
        nuovo->next=*head;
        (*head)->prev=nuovo;
        (*head)=nuovo;
        return;
    }

    Node* curr=(*head);
    while(curr->next!=NULL){
        if(nuovo->vowels> curr->next->vowels){
            nuovo->next=curr->next;
            nuovo->prev=curr;
            curr->next=nuovo;
            return;
        }
        curr=curr->next;
    }

    curr->next=nuovo;
    nuovo->prev=curr;
    
    return;
}

void printList(Node* head){
    Node* brr=head;
    while(brr!=NULL){
        printf("%s %d\n", brr->string, brr->vowels);
        brr=brr->next;
    }
}

void buildList(Node** head, Record* array){
    for(unsigned i=0; i<ARRAY_DIM; i++){
        if(array[i].vowels>=0){
            enqueue(head,array[i].key, array[i].vowels);
        }
    }
}

void createSet(Node** head){
    Node* curr1=(*head);
    while(curr1->next!=NULL){
        Node* curr2=curr1->next;
        while(curr2->next!=NULL){
            if(strcmp(curr1->string, curr2->string)==0 && curr1->vowels==curr2->vowels){
                Node* temp=curr2;
                curr2->next->prev=curr2->prev;
                curr2->prev->next=curr2->next;
                free(temp);
            }
            curr2=curr2->next;
        }
        curr1=curr1->next;
    }
    return;
}

void save(Node* head, Parameters* p){
    Node* curr=head;
    FILE* fp=fopen(p->output_filename, "w");
    if(!fp){
        perror("Error openign the file");
        return;
    }

    while(curr!=NULL){
        fprintf(fp,"[%s]: [%d]\n", curr->string, curr->vowels);
        curr=curr->next;
    }

    fclose(fp);
}