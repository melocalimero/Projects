/*Descrizione: Una biblioteca vuole gestire i propri libri in formato digitale.
Ogni libro ha:
Titolo
Autore
Numero di pagine
Numero di download

Funzionalità richieste:
Aggiungere nuovi libri all’archivio
Stampare tutti i libri
Cercare libri per autore
Ordinare i libri per numero di download (decrescente)
Calcolare la media delle pagine dei libri disponibili

Spunti:
Cerca per autore → algoritmo di ricerca lineare
Ordinamento → bubble sort o selection sort base
Nessuna relazione tra libri*/

#include <iostream>

using namespace std;

class Libro{
    private:
        string titolo;
        string autore;
        int n_pagine;
        int n_download;
    public:
        Libro(){
            titolo="";
            autore="";
            n_pagine=0;
            n_download=0;
        }

        Libro(string titolo, string autore, int n_pagine, int n_download):
        titolo(titolo), autore(autore), n_pagine(n_pagine), n_download(n_download){}

        const string& get_titolo() const {return titolo;}
        const string& get_autore() const {return autore;}
        const int& get_n_pagine() const {return n_pagine;}
        const int& get_n_download() const {return n_download;}

        void stampa(){
            cout<<"\""<<this->titolo<<"\", "<<this->autore<<", "<<this->n_pagine<<" pagine, "<<this->n_download<<" download"<<endl;
        }

        bool operator==(string a){
            return (this->autore==a);
        }

        bool operator<(Libro* l){
            return (this-> n_download<l->n_download);
        }
};

class Biblioteca{
    private:
        string nome;
        Libro** libri;
        int n_libri;
    public:
        Biblioteca(string nome): nome(nome){
            n_libri=0;
            libri=new Libro*[100];
        }

        const string& get_nome() const {return nome;}

        void aggiungiLibro(Libro* l){
            if(n_libri<100){
                n_libri++;
                libri[n_libri-1]=l;
            }
            else{
                cout<<"Array pieno"<<endl;
            }
        }

        
        void bubbleSort(){
            int flag=1;
            while(flag==1){
                flag=0;
                for(int i=0; i<n_libri-1; i++){
                    if((libri[i]->get_n_download())<(libri[i+1]->get_n_download())){
                        Libro* hold=libri[i];
                        libri[i]=libri[i+1];
                        libri[i+1]=hold;
                        flag=1;
                    }
                }
            }
        }

        bool ricercaLineare(string autore){
            for(int i=0; i<n_libri; i++){
                if(libri[i]->get_autore()==autore){
                    return true;
                }
            }
            return false;
        }

        void stampa(){
            cout<<"====="<<this->nome<<"======"<<endl;
            if(n_libri>0){
                for(int i = 0; i < n_libri; i++){
                    libri[i]->stampa();
                }
            }
            else{
                cout<<"Non ce ne sono"<<endl;
            }
            cout<<"=========================="<<endl;
        }
};




int main(){
    int dim=3;
    Libro** libri=new Libro*[dim];
    libri[0]=new Libro("Melo", "Rowling", 400, 30000000);
    libri[1]=new Libro("Luca", "Turi", 300, 20000);
    libri[2]=new Libro("Pinocchio", "Francesco", 200, 1300091);

    Biblioteca b("Feltrinelli");

    for(int i=0; i<3; i++){
        b.aggiungiLibro(libri[i]);
    }
    b.stampa();

    b.bubbleSort();
    b.stampa();

    if(b.ricercaLineare("Meillo"))
        cout<<"Libro trovato"<<endl;
    else    
        cout<<"Libro non trovato"<<endl;
}