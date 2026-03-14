/*Ogni dispositivo ha:
ID
marca
stato acceso/spento

Possono essere:
TV: con diagonale in pollici
Speaker: con potenza in Watt

Funzionalità:
Accensione/spegnimento
Calcolo del consumo reale (0 se spento, altrimenti proporzionale a dimensione/potenza)
Ordinamento per consumo
Stampa stato dei dispositivi
Ricerca per marca*/

#include <iostream>
using namespace std;

class Dispositivo{
    private:
        int id;
        string marca;
        bool stato;
        double consumo;
    public:
        Dispositivo(int id, string marca, bool stato, double consumo):id(id), marca(marca), stato(stato), consumo(consumo){}

        const int& get_id() const {return id;}
        const string& get_marca() const {return marca;}
        const bool& get_stato() const {return stato;}
        const double& get_consumo() const {return consumo;}

        void accendi(){this->stato=true;}
        void spegni(){this->stato=false;}

        virtual void stampa(){
            cout<<this->id<<", "<<this->marca<<", "<<this->consumo<<", "<<this->stato;
        }
        const bool operator>(Dispositivo* d){
            return (this->get_consumo()>d->get_consumo());
        }

        const bool operator==(string marca){
            return (this->marca==marca);
        }

        const bool operator<(string marca){
            return (this->marca<marca);
        }

        const bool operator>(string marca){
            return (this->marca>marca);
        }

        virtual ~Dispositivo(){}
};

class Tv:public Dispositivo{
    private:
        float pollici;
    public:
        Tv(int id, string marca, bool stato,double consumo, float pollici):
        Dispositivo(id,marca,stato, consumo), pollici(pollici){}

        const float& get_pollici() const {return pollici;}

        
        void stampa() override{
            Dispositivo::stampa();
            cout<<", "<<this->pollici<<endl;
        }

        ~Tv() override{}
};

class Speaker:public Dispositivo{
    private:
        float potenza;
    public:
        Speaker(int id, string marca, bool stato, double consumo,float potenza):
        Dispositivo(id, marca, stato, consumo), potenza(potenza){}

        const float& get_potenza() const {return potenza;}

        void stampa() override{
            Dispositivo::stampa();
            cout<<", "<<this->potenza<<endl;
        }
        ~Speaker() override{}
};

template<typename T> void ordinaDispositivi(T** array,const size_t& dim){
    for(size_t i=1; i<dim; i++){
        int j=i-1;
        T* key= array[i];
        while(j>=0 && (*(array[j])>key)){
            array[j+1]=array[j];
            j--;
        }
        array[j+1]=key;
    }
}

template<typename T> bool ricercaDicotomica(T** array, const size_t& dim, const string& marca){
    int min=0;
    int max=dim-1;
    int flag=1;
    while(flag==1){
        int middle=(min+max)/2;
        if(*(array[middle])==marca){
            flag=0;
            return true;
        }

        if(*(array[middle])<marca){
            max=middle-1;
            flag=1;
        }

        if(*(array[middle])>marca){
            min=middle+1;
            flag=1;
        }

        if(min>max){
            flag=0;
            return false;
        }
    }

    return false;
}
int main(){
    size_t size=4;
    Dispositivo** dis=new Dispositivo* [size];
    dis[0]=new Tv(1, "Samsung", 0, 5000,43);
    dis[1]=new Tv(2, "Lg", 0, 5993,22);
    dis[2]=new Speaker(3, "Hover", 0, 4824,800);
    dis[3]=new Speaker(4, "Stones", 0, 4325,400);

   
    ordinaDispositivi<Dispositivo> (dis, size);
    for(size_t i=0; i<size; i++){
        dis[i]->stampa();
    }

    if(ricercaDicotomica<Dispositivo>(dis, size, "Asus"))
        cout<<"Dispositivo trovato"<<endl;
    else    
        cout<<"Dispositivo non trovato"<<endl;

}