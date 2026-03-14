/*Si vuole sviluppare un programma per la gestione di droni utilizzati in diversi settori industriali.

Ogni drone ha un identificativo, una massa, una capacità di carico e un'autonomia.
Ogni drone può trasportare un certo carico e coprire una certa distanza.

Esistono due tipi di droni:

Droni per consegne (es. pacchi), caratterizzati da un numero massimo di consegne giornaliere.

Droni per ispezioni (es. impianti), dotati di una telecamera con risoluzione.

Ogni drone può essere caricato con un certo peso (entro il limite della capacità di carico), e può essere scaricato.

Ogni drone può calcolare la sua efficienza, data dal rapporto tra carico trasportato e autonomia residua.

Creare una collezione di droni misti (consegne e ispezioni), caricare alcuni droni, e stampare per ciascuno la situazione aggiornata, compresa l’efficienza.

*/

#include <iostream>
using namespace std;

class Drone{
    private:
        string identificativo;
        double massa;
        double capacita_carico;
        int autonomia;
        double carica;
    public:
        Drone(string identificativo, double massa, double capacita_carico, int autonomia):
        identificativo(identificativo), massa(massa), capacita_carico(capacita_carico), autonomia(autonomia){
            carica=0;
        }

        const string& get_identificativo() const {return identificativo;}
        const double& get_massa() const {return massa;}
        const double& get_capacita_carico() const {return capacita_carico;}
        const int& get_autonomia() const {return autonomia;}

        double get_efficenza() {return (double (this->carica)/this->autonomia);}

        void carica_drone(double peso){
            if(((this->carica)+peso)<=this->capacita_carico){
                this->carica+=peso;
            }
            else{
                cout<<"[ERRORE]: Superata capacità massima di trasporto"<<endl;
            }
        }

        void scarica_drone(){this->carica=0;}

        virtual void stampa(){
            cout<<this->identificativo<<", "<<this->massa<<"kg, "<<this->capacita_carico<<"kg massimo, "<<this->autonomia<<"km, "<<", Carico trasportato: "<<this->carica<<"kg, "<<this->get_efficenza()<<"\% efficenza";
        }

        virtual ~Drone(){}

};

class Consegna:public Drone{
    private:
        int n_max_cons;
    public:
        Consegna(string identificativo, double massa, double capacita_carico, int autonomia, int n_max_cons):
        Drone(identificativo, massa, capacita_carico, autonomia), n_max_cons(n_max_cons){}

        const int& get_n_max_consegne() const {return n_max_cons;}

        void stampa() override{
            Drone::stampa();
            cout<<", "<<this->n_max_cons<<" max consegne giornaliere"<<endl;
        }

        ~Consegna() override{}
};

class Ispezione: public Drone{
    private:
        int risoluzione;
    public:
        Ispezione(string identificativo, double massa, double capacita_carico, int autonomia, int risoluzione):
        Drone(identificativo, massa, capacita_carico, autonomia), risoluzione(risoluzione){}

        const int& get_risoluzione() const {return risoluzione;}

        void stampa() override{
            Drone::stampa();
            cout<<", "<<this->risoluzione<<"px"<<endl;
        }

        ~Ispezione() override{}
};

int main(){
    int dim=2;
    Drone** droni=new Drone* [dim];

    droni[0]=new Consegna("Drone101", 4, 15, 10, 100);
    droni[1]=new Ispezione("Drone102", 8, 16, 200, 1500);

    cout<<"====Stato iniziale===="<<endl;
    for(int i=0; i<dim; i++){
        droni[i]->stampa();
    }
    cout<<"======================"<<endl;

    cout<<"Carico drone 101..."<<endl;
    droni[0]->carica_drone(8);

    cout<<"=====Nuovo stato====="<<endl;
    for(int i=0; i<dim; i++){
        droni[i]->stampa();
    }
    cout<<"======================"<<endl;

    cout<<"Carico drone 101..."<<endl;
    droni[0]->carica_drone(7);

    cout<<"=====Nuovo stato====="<<endl;
    for(int i=0; i<dim; i++){
        droni[i]->stampa();
    }
    cout<<"======================"<<endl;

    cout<<"Scarico drone 101..."<<endl;
    droni[0]->scarica_drone();

    cout<<"=====Nuovo stato====="<<endl;
    for(int i=0; i<dim; i++){
        droni[i]->stampa();
    }
    cout<<"======================"<<endl;

    cout<<"Carico drone 102..."<<endl;
    droni[1]->carica_drone(8);

    cout<<"=====Nuovo stato====="<<endl;
    for(int i=0; i<dim; i++){
        droni[i]->stampa();
    }
    cout<<"======================"<<endl;

    cout<<"Dealloco..."<<endl;
    for(int i=0; i<dim; i++){
        delete droni[i];
    }
    delete[] droni;
}