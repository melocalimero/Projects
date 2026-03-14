/*Un laboratorio vuole gestire il proprio archivio di dispositivi elettronici.

Ogni dispositivo ha un codice identificativo, un consumo energetico (Watt), e una marca.
I dispositivi possono essere portatili (es. laptop), caratterizzati dall’autonomia in ore, 
oppure fissi (es. desktop), e hanno invece un valore di potenza massima supportata.

Ogni dispositivo può essere acceso o spento, e in base allo stato, può calcolare il consumo effettivo (che è zero da spento).

Creare un archivio di dispositivi, accenderne alcuni, e stampare lo stato e il consumo di ciascuno*/

#include <iostream>
using namespace std;

class Dispositivo{
    private:
        int codice;
        double consumo;
        string marca;
        bool acceso;
    public:
        Dispositivo(int codice, string marca): codice(codice), marca(marca){
            acceso=false;
            consumo=0;
        }

        const int& get_codice() const {return codice;}
        const double& get_consumo() const {return consumo;}
        const string& get_marca() const {return marca;}

        void accendi() {
            this->acceso=true;
            this->consumo=3000;
        }
        void spegni() {
            this->acceso=false;
            this->consumo=0;
        }

        virtual void stampa(){
            cout<<"Codice: ["<<this->codice<<"], Acceso:["<<this->acceso<<"], Consumo: ["<<this->get_consumo()<<"kW], Marca: ["<<this->marca<<"]";
        }

        virtual ~Dispositivo(){}
};

class Portatile: public Dispositivo{
    private:
        double autonomia;
    public:
        Portatile(int codice, string marca,double autonomia):Dispositivo(codice, marca), autonomia(autonomia){}

        const double& get_autonomia(){return this->autonomia;}

        void stampa() override{
            Dispositivo::stampa();
            cout<<", Autonomia: ["<<this->autonomia<<"h]"<<endl;
        }

        ~Portatile() override {}
};

class Fisso: public Dispositivo{
    private:
        double potenza;
    public:
        Fisso(int codice, string marca, double potenza): Dispositivo(codice, marca), potenza(potenza){}

        const double& get_potenza() const {return this->potenza;}
        void stampa() override {
            Dispositivo::stampa();
            cout<<", Potenza: ["<<this->potenza<<"W]"<<endl;
        }

        ~Fisso() override {}
};

int main(){
    size_t dim=3;
    Dispositivo** dis=new Dispositivo* [dim];
    dis[0]=new Portatile(1,"Apple", 6);
    dis[1]=new Portatile(2, "Lenovo", 6);
    dis[2]=new Fisso(3, "Asus", 600);

    for(size_t i=0; i<dim; i++){
        dis[i]->stampa();
    }

    dis[0]->accendi();
    for(size_t i=0; i<dim; i++){
        dis[i]->stampa();
    }
}