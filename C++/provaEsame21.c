/*Si scriva un programma C secondo le seguenti specifiche: 
A. [3 punti] Il programma prende in input da riga di comando un parametro stringa 
input_filename che contiene il nome di un file di input ad estensione “.text” (ad esempio 
"input.text") e un parametro output_filename con estensione “.output” (ad esempio 
“output.output”). Se i parametri non rispettano i requisiti richiesti (es. estensioni), il 
programma stampa un messaggio di errore su standard error e termina la sua esecuzione con 
un appropriato codice di terminazione.  
Si stampino a schermo i valori dei parametri presi in input. 
B. [10 punti] Si assuma che il file di input contenga diverse stringhe, una per riga. Il programma 
legge queste stringhe e le inserisce in una lista concatenata X in ordine di lunghezza crescente. 
Si stampi a schermo il contenuto della lista X. 
C. [8 punti] Viene definito un array Y di short che contiene, per ogni stringa nella lista, il numero 
di vocali presenti in essa. Più precisamente, l’elemento Y[i] contiene il numero di vocali 
presenti nella lista di posizione (i+1)-esima nella lista X. Ad esempio Y[0] conterrà il numero 
di vocali contenuti nella stringa contenuta nella testa della lista X. 
Si stampi l'array Y. 
D. [6 punti] Si calcola la media dei valori di Y e si rimuovono dalla lista X tutte le stringhe che 
hanno un numero di vocali superiore alla media.  
Si stampi la media di Y e il contenuto di X dopo le cancellazioni. 
E. [3 punti] Il programma scrive sul file di output il contenuto di X.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct parameters{
    char* input_filename;
    char* output_filename;
}Parameters;

typedef struct node{
    char * string;
    struct node* next;
}Node;

Parameters* decodeParameters(int argc, char* argv[]);
int insert(Node** head, char* string);
void printList(Node* head);
int readFile(Node** head, Parameters* p);
int isVowel(char * string);
short* getArray(Node* head, int count);
void delete(Node** head, char* string);
void filterList(Node** head, short* Y, int count);
short media(short* Y, int count);
void writeToFile(Node* head, Parameters* p);

int main(int argc, char* argv[]){
    Parameters* p=decodeParameters(argc,argv);
    printf("Input filename:%s\t output filename:%s\n", p->input_filename, p->output_filename);

    Node* head=NULL;
    
    int count=readFile(&head, p);
    printf("\n==LISTA X==\n");
    printList(head);

    short* Y=getArray(head, count);
    printf("\n==Array Y==\n");
    for(unsigned i=0; i<count; i++){
        printf("%hd ",Y[i]);
    }
    printf("\n");

    filterList(&head, Y, count);
    printf("\n==Lista X==\n");
    printList(head);

    writeToFile(head, p);

}

Parameters* decodeParameters(int argc, char* argv[]){
    if(argc!=3){
        fprintf(stderr,"Usage: <%s> <string> <string>\n", argv[0]);
        exit(1);
    }

    if(strcmp(argv[1]+strlen(argv[1])-5, ".text")!=0){
        fprintf(stderr,"First string extension must be .text\n");
        exit(1);
    }

    if(strcmp(argv[2]+strlen(argv[2])-7, ".output")!=0){
        fprintf(stderr,"Second string extension must be .output\n");
        exit(1);
    }

    Parameters* p=malloc(sizeof(Parameters));
    p->output_filename=argv[2];
    p->input_filename=argv[1];
    return p;
}

int insert(Node** head, char* string){
    Node* nuovo=malloc(sizeof(Node));
    nuovo->string=string;
    
    nuovo->next=NULL;
    if(*head==NULL){
        (*head)=nuovo;
        return 1;
    }

    if(strlen(nuovo->string)<strlen((*head)->string)){
        nuovo->next=*head;
        *head=nuovo;
        return 1;
    }

    Node* curr=*head;
    while(curr->next!=NULL){
        if(strlen(nuovo->string)<strlen(curr->next->string)){
            nuovo->next=curr->next;
            curr->next=nuovo;
            return 1;
        }
        curr=curr->next;
    }

    curr->next=nuovo;
    return 1;
}

void printList(Node* head){
    Node* curr=head;
    while(curr!=NULL){
        printf("%s\n", curr->string);
        curr=curr->next;
    }
}

int readFile(Node** head, Parameters* p){
    FILE* fp=fopen(p->input_filename, "r");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }
    int count=0;
    while(!feof(fp)){
        char* string=malloc(sizeof(char)*200);
        if(fscanf(fp,"%s", string)!=1){
            if(feof(fp)){
                break;
            }
            else{
                fprintf(stderr,"Error reading from file\n");
            }
        }
        count+=insert(head, string);
    }

    fclose(fp);

    return count;
}

int isVowel(char * string){
    int count=0;
    char string1[]="aeiou";
    
    for(unsigned i=0; string[i]!='\0'; i++){
        if(strchr(string1, tolower(string[i]))){
            count++;
        }
        
        /*switch(string[i]){
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
        }*/
    }

    return count;
}

short* getArray(Node* head, int count){
    Node* curr=head;
    short* Y=malloc(sizeof(short)*count);
    int i=0;
    while(curr!=NULL && i<count){
        Y[i]=isVowel(curr->string);
        curr=curr->next;
        i++;
    }

    return Y;
}

void delete(Node** head, char* string){
    if(*head==NULL){
        fprintf(stderr,"List is already empty\n");
        exit(1);
    }

    if(strcmp(string, (*head)->string)==0){
        Node* temp=*head;
        (*head)=(*head)->next;
        free(temp);
        temp=NULL;
        return;
    }

    Node* curr=*head;
    while(curr->next!=NULL){
        if(strcmp(string, curr->next->string)==0){
            Node* temp=curr->next;
            curr->next=curr->next->next;
            free(temp);
            temp=NULL;
            return;
        }
        curr=curr->next;
    }

    Node* temp=curr;
    free(temp);
    return;
}

short media(short* Y, int count){
    int somma=0;
    for(unsigned i=0; i<count; i++){
        somma+=Y[i];
    }
    short media=somma/count; //non mi importa che non è double dato che il numero di vocali non può essere con la virgola
    return media;
}
void filterList(Node** head, short* Y, int count){
    short m=media(Y, count);
    while(*head!=NULL){
        int countHeadVowels=isVowel((*head)->string);
        if(countHeadVowels>m){
            delete(head, (*head)->string);
        }
        else{
            break;
        }
    }
    Node* curr=*head;
    printf("\nMedia:%hd\n",m);
    while(curr->next!=NULL){
        Node* temp=curr->next;
        int countVowels=isVowel(temp->string);
        if(countVowels>m){
            delete(head,temp->string);
            continue;
        }
        curr=curr->next;
    }
}

void writeToFile(Node* head, Parameters* p){
    FILE* fp=fopen(p->output_filename, "w");
    if(!fp){
        perror("Error opening file");
        exit(1);
    }

    Node* curr=head;
    while(curr!=NULL){
        fprintf(fp,"%s\n", curr->string);
        curr=curr->next;
    }

    fclose(fp);
}