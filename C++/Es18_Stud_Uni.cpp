/*Ogni studente ha:
nome, cognome
matricola
media voti

Possono essere:
Triennali: hanno il numero di esami sostenuti
Magistrali: hanno il numero di crediti ottenuti

Funzionalità:
Inserimento studenti
Ordinamento per media
Ricerca per matricola
Calcolo della media dei voti degli studenti iscritti
Stampa con override per distinguere triennale/magistrale*/

#include <iostream>
using namespace std;

class Studente{
    private:
        string nome, cognome;
        int matricola;
        double media;
    public:
        Studente(string nome, string cognome, int matricola, double media):
        nome(nome), cognome(cognome), matricola(matricola), media(media){}

        const string& get_nome() const {return nome;}
        const string& get_cognome() const {return cognome;}
        const int& get_matricola() const {return matricola;}
        const double& get_media() const {return media;}

        virtual void stampa(){
            cout<<this->matricola<<": "<<this->nome<<" "<<this->cognome<<", "<<this->media<<" di media";
        }

        virtual ~Studente(){}
};

class Triennale: public Studente{
    private:
        int esami;
    public:
        Triennale(string nome, string cognome, int matricola, double media, int esami):
        Studente(nome, cognome, matricola, media), esami(esami){}

        const int& get_esami_sostenuti() const {return esami;}

        void stampa() override{
            Studente::stampa();
            cout<<", Frequentante: Triennale, Esami sostenuti: "<<this->esami<<endl;
        }

        ~Triennale() override{}
};

class Magistrale: public Studente{
    private:    
        int crediti;
    public:
        Magistrale(string nome, string cognome, int matricola, double media, int crediti):
        Studente(nome, cognome, matricola, media), crediti(crediti){}

        const int& get_crediti() const {return crediti;}

        void stampa() override{
            Studente::stampa();
            cout<<", Frequentante: Magistrale, Crediti ottenuti: "<<this->crediti<<endl;
        }

        ~Magistrale() override{}
};

class Registro{
    private:
        Studente** studenti;
        int n_studenti;
        int dim;
    public:
        Registro(){
            n_studenti=0;
            dim=100;
            studenti=new Studente* [dim];
        }

        void aggiungiStudente(Studente* s){
            if(n_studenti<dim){
                n_studenti++;
                studenti[n_studenti-1]=s;
            }
            else
                cout<<"[ERRORE]: limite massimo studenti raggiunto"<<endl;
        }

        void ordinaPerMedia(){
            if(n_studenti>0){
                for(int i=0; i<n_studenti-1; i++){
                    int max=i;
                    for(int j=i+1; j<n_studenti; j++){
                        if(studenti[j]->get_media()>studenti[max]->get_media()){
                            max=j;
                        }
                    }

                    if(max!=i){
                        Studente* hold=studenti[i];
                        studenti[i]=studenti[max];
                        studenti[max]=hold;
                    }
                }
            }
        }

        bool ricercaBinaria(int matricola){
            if(n_studenti>0){
                int flag=1;
                int min=0;
                int max=n_studenti-1;
                while(flag==1){
                    int middle=(min+max)/2;
                    if(studenti[middle]->get_matricola()==matricola){
                        flag=0;
                        return true;
                    }
                    if(studenti[middle]->get_matricola()<matricola){
                        max=middle-1;
                        flag=1;
                    }
                    if(studenti[middle]->get_matricola()>matricola){
                        min=middle+1;
                        flag=1;
                    }
                    if(min>max){
                        flag=0; 
                        return false;
                    }
                }
            }
            
        }

        void stampa(){
            if(n_studenti>0){
                for(size_t i=0; i<n_studenti; i++){
                    studenti[i]->stampa();
                }
            }
        }
};

int main(){
    size_t dim=4;
    Studente** s=new Studente*[dim];
    s[0]=new Triennale("Andrea", "Coglione", 1001, 25, 3);
    s[1]=new Triennale("Vito", "Ricchione", 1002, 27, 2);
    s[2]=new Magistrale("Giovanni", "Secchione", 1003, 30, 185);
    s[3]=new Magistrale("Riccardo", "Sbrodolone", 1004, 29, 400);

    Registro r;
    for(size_t i=0; i<dim; i++){
        r.aggiungiStudente(s[i]);
    }

    r.stampa();
    r.ordinaPerMedia();
    cout<<"Ordinamento... "<<endl;
    r.stampa();

    if(r.ricercaBinaria(10001)){
        cout<<"Studente trovato"<<endl;
    }
    else    
        cout<<"Studente non trovato"<<endl;
}