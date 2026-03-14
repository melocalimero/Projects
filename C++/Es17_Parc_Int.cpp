/*Ogni veicolo ha:
targa
marca
chilometraggio

I veicoli possono essere:
Auto: hanno il numero di porte
Moto: hanno la cilindrata

Ogni veicolo può essere parcheggiato o ritirato. Se è parcheggiato, il sistema può:
Stampare tutti i veicoli attualmente parcheggiati
Calcolare il chilometraggio medio
Cercare un veicolo in base alla targa
Ordinare i veicoli in base al chilometraggio*/


#include <iostream>
using namespace std;

class Veicolo{
    private:
        string targa;
        string marca;
        double chilometraggio;
    public:
        Veicolo(string targa, string marca, double chilometraggio): targa(targa), marca(marca), chilometraggio(chilometraggio){}

        const string& get_targa() const {return targa;}
        const string& get_marca() const {return marca;}
        const double& get_chilometraggio() const {return chilometraggio;}

        virtual void stampa(){
            cout<<this->targa<<", "<<this->marca<<", "<<this->chilometraggio<<"km";
        }

        virtual ~Veicolo(){}
};

class Auto: public Veicolo{
    private:
        int n_porte;
    public:
        Auto(string targa, string marca, double chilometraggio, int n_porte):
        Veicolo(targa, marca, chilometraggio), n_porte(n_porte){}

        const int& get_n_porte() const {return n_porte;}

        void stampa() override{
            Veicolo::stampa();
            cout<<", "<<this->n_porte<<" porte"<<endl;
        }

        ~Auto() override{}
};

class Moto: public Veicolo{
    private:
        int cilindrata;
    public:
        Moto(string targa, string marca, double chilometraggio, int cilindrata):
        Veicolo(targa, marca, chilometraggio), cilindrata(cilindrata){}

        const int& get_cilindrata() const {return cilindrata;}

        void stampa() override{
            Veicolo::stampa();
            cout<<", "<<this->cilindrata<<" cm^3"<<endl;
        }

        ~Moto() override{}
};

class Parcheggio{
    private:
        Veicolo** veicolo;
        int n_veicoli;
        int max;
    public:
        Parcheggio(){
            n_veicoli=0;
            max=100;
            veicolo=new Veicolo* [max];
        }

        void parcheggia(Veicolo* v){
            if(n_veicoli<max){
                n_veicoli++;
                veicolo[n_veicoli-1]=v;
            }
        }

        void stampa_veicoli_parcheggiati(){
            if(n_veicoli>0){
                for(int i=0; i<n_veicoli; i++){
                    veicolo[i]->stampa();
                }
            }
        }

        double chilometraggio_medio(){
            if(n_veicoli>0){
                double somma=0;
                for(int i=0; i<n_veicoli; i++){
                    somma+=veicolo[i]->get_chilometraggio();
                }
                return (somma/n_veicoli);
            }

            return -1;
        }

        bool ricercaTarga(string targa){
            if(n_veicoli>0){
                for(int i=0; i<n_veicoli; i++){
                    if(veicolo[i]->get_targa()==targa){
                        return true;
                    }
                }
                return false;
            }
            return false;
        }

        void ordinamento(){
            if(n_veicoli>0){
                for(int i=0; i<n_veicoli-1; i++){
                    int max=i;
                    for(int j=i+1; j<n_veicoli; j++){
                        if(veicolo[j]->get_chilometraggio()>veicolo[max]->get_chilometraggio()){
                            max=j;
                        }
                    }
                    if(max!=i){
                        Veicolo* hold=veicolo[i];
                        veicolo[i]=veicolo[max];
                        veicolo[max]=hold;
                    }
                }
            }
        }
};

int main(){
    size_t dim=4;
    Veicolo** v=new Veicolo* [dim];

    v[0]=new Auto("EG230TN", "Citroèn", 370000, 5);
    v[1]=new Auto("XB578NB", "Opel", 500000, 5);
    v[2]=new Moto("GL987TN", "Audi", 900000, 150);
    v[3]=new Moto("ER328MG", "Mercedes", 736439, 125);

    Parcheggio p;
    for(size_t i=0; i<dim; i++){
        p.parcheggia(v[i]);
    }

    p.stampa_veicoli_parcheggiati();
    cout<<"Chilometraggio medio: "<<p.chilometraggio_medio()<<endl;

    cout<<endl<<"Ordinamento..."<<endl;
    p.ordinamento();
    p.stampa_veicoli_parcheggiati();
    if(p.ricercaTarga("EG230TN")){
        cout<<"Veicolo trovato"<<endl;
    }
    else{
        cout<<"Veicolo non trovato"<<endl;
    }
}