/*Si vuole realizzare un sistema per il monitoraggio di mezzi pubblici urbani.

Ogni mezzo ha una targa, una capacità di passeggeri e un chilometraggio.
Alcuni mezzi sono autobus, caratterizzati dal numero di linee coperte.
Altri mezzi sono tram, e indicano la lunghezza totale dei binari su cui operano.

Ogni mezzo può:

Registrare un viaggio che incrementa il chilometraggio

Calcolare il rapporto tra chilometri percorsi e capacità, per stimare l’usura relativa

Creare una flotta mista di mezzi (autobus e tram), simulare alcuni viaggi e stampare 
la situazione aggiornata per ogni mezzo, incluso il valore di usura.*/

#include <iostream>
using namespace std;

class Mezzo{
    private:
        string targa;
        int passeggeri;
        double chilometraggio;
    public:
        Mezzo(string targa, int passeggeri, double chilometraggio):
        targa(targa), passeggeri(passeggeri), chilometraggio(chilometraggio){}

        const string& get_targa() const {return targa;}
        const int& get_passeggeri() const {return passeggeri;}
        const double& get_chilometraggio() const {return chilometraggio;}

        void inizia_viaggio(double chilometri) {this->chilometraggio+=chilometri;}
        double get_usura() {return (double(this->chilometraggio)/passeggeri);}
        
        virtual void stampa(){
            cout<<this->targa<<", "<<this->chilometraggio<<"km, "<<this->passeggeri<<" passeggeri, "<<this->get_usura()<<"\% usura";
        }

        virtual ~Mezzo(){}
};

class Autobus: public Mezzo{
    private:
        int linee;
    public:
        Autobus(string targa, int passeggeri, double chilometraggio, int linee):
        Mezzo(targa, passeggeri, chilometraggio), linee(linee){}

        const int& get_linee() const {return linee;}

        void stampa() override{
            Mezzo::stampa();
            cout<<", "<<this->linee<<"linee coperte"<<endl;
        }

        ~Autobus() override{}
};

class Tram: public Mezzo{
    private:
        double lunghezza_binari;
    public:
        Tram(string targa, int passeggeri, double chilometraggio, double lunghezza_binari):
        Mezzo(targa, passeggeri, chilometraggio), lunghezza_binari(lunghezza_binari){}

        const double& get_lunghezza_totale_binari() const {return lunghezza_binari;}

        void stampa() override {
            Mezzo::stampa();
            cout<<", "<<this->lunghezza_binari<<" lunghezza binari totale"<<endl;
        }

        ~Tram() override {}
};

int main(){
    int dim=4;
    Mezzo** mezzi=new Mezzo*[dim];
    mezzi[0]=new Autobus("EG230TN", 10, 40000, 20);
    mezzi[1]=new Autobus("XB270YW", 6, 100000, 8);
    mezzi[2]=new Tram("PO098LK", 100, 50000, 300);
    mezzi[3]=new Tram("ER635TN", 300, 30000, 1763);

    mezzi[0]->inizia_viaggio(500);
    for(size_t i = 0; i < dim; i++){
        mezzi[i]->stampa();
    }

    for(size_t i = 0; i < dim; i++){
        delete mezzi[i];
    }
    delete[] mezzi;
}