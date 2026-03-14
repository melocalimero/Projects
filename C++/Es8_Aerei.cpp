/*Si vuole rappresentare la flotta aerea di una compagnia.
Ogni aereo è identificato da un nome, una compagnia aerea, una autonomia e una capacità.
Alcuni aerei sono aerei passeggeri, e sono caratterizzati anche dalla classe di servizio.
Altri aerei sono cargo, e riportano la capacità di carico in tonnellate.

Ogni aereo deve fornire un’indicazione della propria efficienza operativa.

Creare una collezione contenente aerei di entrambi i tipi e mostrarne le informazioni principali.*/

#include <iostream>
using namespace std;

class Aereo{
private:
    string nome;
    string compagnia;
    int autonomia;
    int capacita;
public: 
    Aereo(string nome, string compagnia, int autonomia, int capacita):
    nome(nome), compagnia(compagnia), autonomia(autonomia), capacita(capacita){}

    const string& get_nome() const {return nome;}
    const string& get_compagnia() const {return compagnia;}
    const int& get_autonomia() const {return autonomia;}
    const int& get_capacita() const {return capacita;}

    double get_efficenza() {return (double(autonomia)/capacita);}

    virtual void stampa(){
        cout<<this->nome<<", "<<this->compagnia<<", "<<this->autonomia<<"km, "<<this->capacita<<" persone, "<<this->get_efficenza()<<"%";
    }

    virtual ~Aereo(){}
};

class Passeggero: public Aereo{
    private:
        string classe_servizio;
    public:
        Passeggero(string nome, string compagnia, int autonomia, int capacita, string classe_servizio):
        Aereo(nome, compagnia, autonomia, capacita), classe_servizio(classe_servizio){}

        const string& get_classe_servizio() const {return classe_servizio;}

        void stampa() override{
            Aereo::stampa();
            cout<<", "<<this->classe_servizio<<endl;
        }

        ~Passeggero() override{}
};

class Cargo: public Aereo{
    private:
        double capacita_carico;
    public:
        Cargo(string nome, string compagnia, int autonomia, int capacita, double capacita_carico):
        Aereo(nome, compagnia, autonomia, capacita), capacita_carico(capacita_carico){}

        const double& get_capacita_carico() const {return capacita_carico;}

        void stampa() override{
            Aereo::stampa();
            cout<<", "<<this->capacita_carico<<"t"<<endl;
        }

        //predisposizione ordinamento per capacità di carico
        bool operator<(Cargo* c){
            return (this->capacita_carico<c->capacita_carico);
        }
        ~Cargo() override {}
};

int main(){
    int dim=2;
    Aereo** aerei=new Aereo*[dim];

    aerei[0]=new Passeggero("Boeing", "Ita-Airways", 1000, 33, "Moka");
    aerei[1]=new Cargo("Cargo737", "American express", 10000, 100, 500);

    for(int i=0; i<dim; i++){
        aerei[i]->stampa();
        delete aerei[i];
    }
    delete[] aerei;
}
