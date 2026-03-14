/*
===============================================================
|           ESERCIZIO - Gestione Robot Industriali           |
===============================================================

Si vuole progettare un sistema per la gestione dei robot
presenti in un impianto industriale.

Ogni robot ha:
- un ID (stringa),
- un peso (kg),
- un consumo energetico (kWh),
- un tempo operativo (ore).

Ogni robot può calcolare:
- la propria efficienza: 
  EFF = tempo_operativo / consumo_energia

Esistono due tipologie di robot:

1. Robot di Montaggio:
   - Numero di bracci meccanici (int)
   - Tipo di materiale lavorato (string)

2. Robot di Saldatura:
   - Temperatura di saldatura (°C)
   - Precisione (valore tra 0.0 e 1.0)

======================
|     OBIETTIVI      |
======================

1. Modellare le classi con una gerarchia adatta.
2. Implementare un metodo stampa() virtuale che stampi
   tutte le informazioni specifiche di ciascun robot.
3. Creare un array dinamico di Robot* con robot
   sia di tipo Montaggio che Saldatura.
4. Implementare un algoritmo di ordinamento generico 
   (template) per:
   - Ordinare i robot in base all’efficienza.
5. Implementare una funzione di ricerca che, data una
   soglia di efficienza, restituisca tutti i robot che
   superano tale soglia.
6. Overload dell’operatore << per Robot.
7. Salvataggio dei dati dei robot su file.

===============================================================
*/

#include <iostream>
using namespace std;

class Robot{
    private:
        string id;
        double peso;
        double consumo;
        double tempo_operativo;
    public:
        Robot(){
            id="";
            peso=0;
            consumo=0;
            tempo_operativo=0;
        }
        Robot(string id, double peso, double consumo, double tempo_operativo):
        id(id), peso(peso), consumo(consumo), tempo_operativo(tempo_operativo){}

        const string& get_id() const {return id;}
        const double& get_peso() const {return peso;}
        const double& get_consumo() const {return consumo;}
        const double& get_tempo_operativo() const {return tempo_operativo;}
        const double get_efficenza() const {return (tempo_operativo/consumo);}

        virtual void stampa(){
            cout<<this->id<<", "<<this->peso<<" kg, "<<this->consumo<<" kW/h, "<<this->tempo_operativo<<" ore lavorative, Efficenza: "<<this->get_efficenza();
        }

        bool operator>(Robot* r){
            return (this->get_efficenza()>r->get_efficenza());
        }

        bool operator<(Robot* r){
            return (this->get_efficenza()<r->get_efficenza());
        }

        bool operator>(double efficenza){
            return (this->get_efficenza()>efficenza);
        }

        virtual ~Robot() {}
};

class Montaggio: public Robot{
    private:
        int n_bracci;
        string materiale;
    public:
        Montaggio(string id, double peso, double consumo, double tempo_operativo, int n_bracci, string materiale):
        Robot(id, peso, consumo, tempo_operativo), n_bracci(n_bracci), materiale(materiale){}

        const int& get_n_bracci() const {return n_bracci;}
        const string& get_materiale() const {return materiale;}

        void stampa() override{
            Robot::stampa();
            cout<<", "<<this->n_bracci<<" bracci meccanici, "<<this->materiale<<endl;
        }

        ~Montaggio() override{}
};

class Saldatura:public Robot{
    private:
        float temperatura;
        float precisione;
    public:
        Saldatura(string id, double peso, double consumo, double tempo_operativo, float temperatura, float precisione):
        Robot(id, peso, consumo, tempo_operativo), temperatura(temperatura), precisione(precisione){}

        const float& get_temperatura() const {return temperatura;}
        const float& get_precisione() const {
            if(precisione>0.0 && precisione<1.0){
                return precisione;
            }
            cout<<"[ERRORE]: Precisione compresa tra 0.0 e 1.0"<<endl;
            exit(1);
        }

        void stampa() override{
            Robot::stampa();
            cout<<", "<<this->temperatura<<"°C, "<<this->precisione<<" di precisione"<<endl;
        }

        ~Saldatura() override{}
};

ostream& operator<<(ostream& out, Robot* r){
    r->stampa();
    return out;
}

template<typename T> void ordina(T** array, size_t dim){
    for(size_t i=1; i<dim; i++){
        int j=i-1;
        T* key=array[i];
        while(*(array[j])>key){
            array[j+1]=array[j];
            j--;
        }
        array[j+1]=key;
    }
}

Robot** ricercaEfficenza(Robot** array, size_t dim, double efficenza){
    if(efficenza>1.0 || efficenza<0.0){
        exit(1);
    }
    Robot** new_array=new Robot* [dim];
    size_t j=0;
    for(size_t i=0; i<dim; i++){
        if(*(array[i])>efficenza){
            new_array[j]=array[i];
            j++;
        }
    }
    return new_array;
}

int main(){
    size_t dim=4;
    Robot** r=new Robot* [dim];
    r[0]=new Montaggio("Robot101", 50, 400, 6, 3, "Ferro");
    r[1]=new Montaggio("Robot102", 30, 500, 100, 7, "Platino");
    r[2]=new Saldatura("Robot001", 40, 200, 50, 1000, 0.6);
    r[3]=new Saldatura("Robot002", 15, 600.34, 69, 500, 0.87);

    for(size_t i=0; i<dim; i++){
        cout<<r[i];
    }

    ordina<Robot>(r, dim);
    cout<<"Ordinamento.."<<endl;
    for(size_t i=0; i<dim; i++){
        cout<<r[i];
    }

    Robot** new_array=ricercaEfficenza(r, dim, 0.12);

    cout<<"Lista robot che superano 0.12 di eff"<<endl;
    int dim_new_array=0;
    for(size_t i=0; new_array[i]!=NULL; i++){
        cout<<new_array[i];
        dim_new_array=i;
    }

    for(size_t i=0; i<dim; i++){
        delete r[i];
    }
    delete [] r;


}