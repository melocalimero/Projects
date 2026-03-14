/*Si vuole sviluppare un programma per la gestione di veicoli terrestri.
Ogni veicolo ha un modello, una marca, una massa e una velocità massima.
Alcuni veicoli sono automobili e sono caratterizzate anche dal numero di porte.
Altri veicoli sono motociclette e includono il valore della cilindrata.

Si vuole calcolare, per ogni veicolo, un valore indicativo della performance, basato sulla massa e la velocità massima.

Creare una collezione mista di veicoli, sia automobili che motociclette, e stampare le informazioni principali di ciascuno.*/

#include <iostream>

using namespace std;

class Veicolo{
    private:
        string modello;
        string marca;
        double massa;
        double max_velocita;
    public:
        Veicolo(string modello, string marca, double massa, double max_velocita):
        modello(modello), marca(marca), massa(massa), max_velocita(max_velocita){}

        const string& get_modello() const {return modello;}
        const string& get_marca() const {return marca;}
        const double& get_massa() const {return massa;}
        const double& get_max_velocita() const {return max_velocita;}

        double get_performance(){return (this->massa/this->max_velocita);}

        virtual void stampa(){
            cout<<this->modello<<", "<<this->marca<<", "<<this->massa<<"kg, "<<this->max_velocita<<"km/h";
        }


        //Predisposizione per ordinamento per velocita
        bool operator<(Veicolo* v){
            return (this->get_max_velocita()<v->get_max_velocita());
        }

        //predisposizione ricerca per modello
        bool operator==(Veicolo* v){
            return (this->modello==v->get_modello());
        }
        virtual ~Veicolo(){
            cout<<"Veicolo distrutto"<<endl;
        }
};

class Auto: public Veicolo{
    private:
        int n_porte;
    public:
        Auto(string modello, string marca, double massa, double max_velocita, int n_porte):
        Veicolo(modello, marca, massa, max_velocita), n_porte(n_porte){}

        const int& get_n_porte() const {return n_porte;}

        void stampa() override{
            Veicolo::stampa();
            cout<<", "<<this->n_porte<<" porte"<<endl;
        }

        ~Auto() override{
            cout<<"Auto distrutta";
        }
};

class Moto: public Veicolo{
    private:
        int cilindrata;
    public:
        Moto(string modello, string marca, double massa, double max_velocita, int cilindrata):
        Veicolo(modello, marca, massa, max_velocita), cilindrata(cilindrata){}

        const int& get_cilindrata() const {return cilindrata;}

        void stampa() override{
            Veicolo::stampa();
            cout<<", "<<this->cilindrata<<" cm^3"<<endl;
        }

        ~Moto() override{
            cout<<"Moto distrutta";
        }
};

int main(){
    int dim=4;
    Veicolo** veicoli=new Veicolo*[dim];
    veicoli[0]=new Auto("Audi r4", "Audi", 500, 225, 5);
    veicoli[1]=new Auto("Yaris", "Toyota", 300, 180, 4);
    veicoli[2]=new Moto("Davidson", "Harley", 300, 290, 500);
    veicoli[3]=new Moto("Sh 125", "Sh", 150, 170, 125);

    for(int i=0; i<dim; i++){
        veicoli[i]->stampa();
    }

    if(veicoli[3]==veicoli[2]){
        cout<<"3 e 2 sono uguali"<<endl;
    }
    if(veicoli[3]<veicoli[2]){
        cout<<"3 è più lento di 2"<<endl;
    }
    else{
        cout<<"3 è più veloce di 2"<<endl;
    }
    
    for(int i=0; i<dim; i++){
        delete veicoli[i];
    }
    delete[] veicoli;
}