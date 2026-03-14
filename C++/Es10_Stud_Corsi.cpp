/*Si vuole progettare un sistema per la gestione di corsi universitari.

Ogni persona ha un nome e un cognome.
Gli studenti sono persone che hanno anche una matricola, e possono essere iscritti a più corsi.
Ogni corso ha un nome, un codice e un numero di CFU.

Ogni studente può:

Iscriversi ad un corso

Visualizzare i corsi a cui è iscritto

Calcolare i CFU totali acquisiti

Ogni corso può:

Avere una lista di studenti iscritti

Mostrare l’elenco degli iscritti

Creare una collezione di studenti e corsi, iscrivere alcuni studenti ai corsi, e stampare la lista degli iscritti per ciascun corso,
 e i CFU totali per ciascuno studente.

*/

#include <iostream>

using namespace std;

class Persona{
    private:
        string nome;
        string cognome;
    public:
        Persona(string nome, string cognome): nome(nome), cognome(cognome){}

        const string& get_nome() const {return nome;}
        const string& get_cognome() const {return cognome;}

        virtual void stampa(){
            cout<<this->nome<<", "<<this->cognome;
        }

        virtual ~Persona(){}
};

class Corso;
class Studente: public Persona{
    private:
        int matricola;
        int n_corsi;
        Corso** corsi;
    public:
        Studente(string nome, string cognome,int matricola):
        Persona(nome, cognome), matricola(matricola){
            n_corsi=0;
            corsi=NULL;
        }

        const int& get_matricola() const {return matricola;}

        void aggiungi_corso(Corso* c){
            n_corsi++;
            corsi[n_corsi-1]=c;
        }
        void stampa() override{
            Persona::stampa();
            cout<<", "<<this->matricola<<"Corsi"<<endl;
            if(n_corsi>0){
                for(size_t i = 0; i< n_corsi; i++){
                    corsi[i]->stampa();
                }
            }
            else{
                cout<<"Nessun corso"<<endl;
            }
        }

        ~Studente() override{}
};

class Corso{
    private:
        Studente** studenti_iscr;
        string nome;
        int codice;
        int CFU;
        int n_studenti;
    public:
        Corso(string nome, int codice, int CFU):nome(nome), codice(codice), CFU(CFU){
            studenti_iscr=NULL;
            n_studenti=0;
        }

        const string& get_nome() const {return nome;}
        const int& get_codice() const {return codice;}
        const int& get_CFU() const {return CFU;}
        const int& get_n_studenti() const {return n_studenti;}

        void aggiungi_studente(Studente* s){
            n_studenti++;
            studenti_iscr[n_studenti-1]=s;
        }

        void stampa(){
            cout<<this->nome<<", "<<this->codice<<", "<<this->CFU<<"CFU, STUDENTI:"<<endl;
            if(n_studenti>0){
                for(size_t i = 0; i < n_studenti; i++){
                    studenti_iscr[i]->stampa();
                }
            }
            else{
                cout<<"Non ce ne sono"<<endl;
            }
        }
};

int main(){
    int n_studenti=5;
    
    Persona** studenti=new Persona* [n_studenti];
    studenti[0]=new Studente("Andrea", "Riccobono", 1001);
    studenti[1]=new Studente("Vito", "Timpanaro", 1002);
    studenti[2]=new Studente("Carlo", "Ursino", 1003);
    studenti[3]=new Studente("Angelo", "Rapisarda", 1004);
    studenti[4]=new Studente("Andrea", "Gangemi", 1005);

    int n_corsi=3;
    Corso** corsi=new Corso* [n_corsi];
    corsi[0]=new Corso("Strutture discrete", 100, 6);
    corsi[1]=new Corso("Programmazione II", 101, 9);
    corsi[2]=new Corso("Elementi di analisi I", 102, 6);

    
}