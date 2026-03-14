/*A differenza di una coda a priorità standard che serve gli elementi con la priorità più alta per primi,
 una *Coda Anti-Priorità* serve gli elementi con la *priorità più bassa* per primi. 
 Se due elementi hanno la stessa priorità, vengono serviti in ordine di arrivo (FIFO).

*Obiettivo:* 
Implementare una CodaAntiPriorita con metodi enqueue(item, priority) e dequeue().

*Manipolazione Fantasiosa:*
 La "fantasia" sta nel rovesciare la logica della priorità standard,
 che richiede un'attenzione particolare nell'ordinamento interno o nella selezione dell'elemento da servire.

*Overloading Operatore:* 
Esegui l'overloading dell'operatore -- (decremento prefisso e/o postfisso) 
per *ridurre la priorità di tutti gli elementi nella coda di una unità*. Se la priorità scende sotto zero,
 dovrebbe essere impostata a zero (priorità minima).
*/

#include <iostream>
using namespace std;


class Priorità{
    private:
        int valore;
        string tipologia;
    protected:
        Priorità( int valore, string tipologia): valore(valore), tipologia(tipologia){}
    public:
        virtual int get_valore() const = 0;
        virtual string get_tipologia() const = 0;

        virtual ~Priorità(){}
};
class Alta: public Priorità{
    public:
        Alta(): Priorità(1, "Alta"){}

        int get_valore() const override{return 1;}
        string get_tipologia() const override{return "Alta";}

        ~Alta() override {}
};
class Media: public Priorità{
    public:
        Media(): Priorità(2, "Media"){}

        int get_valore() const override{return 2;}
        string get_tipologia() const override{return "Media";}

        ~Media() override {}
};
class Bassa: public Priorità{
    public:
        Bassa(): Priorità(3, "Bassa"){}

        int get_valore() const override{return 3;}
        string get_tipologia() const override{return "Bassa";}

        ~Bassa() override {}
};
template<typename T> class Nodo{
    private:
        T chiave;
        Priorità* priorità;
        Nodo* next;
    public:
        Nodo(const T& chiave, Priorità* priorità): chiave(chiave), priorità(priorità), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        Priorità* get_priorità() const {return priorità;}
        Nodo<T>* get_next() const {return next;}

        void set_next();

        ~Nodo(){
            delete priorità;
        }
};

template<typename T> ostream& operator<<(ostream& os, Nodo<T>& n){
    os << n.get_chiave() << " priorità: " << n.get_priorità()->get_valore() << ", " << n.get_priorità()->get_tipologia();
    return os;
}

template<typename T> class Queue{
    private:
        Nodo<T>* head;
    public:
        Queue(): head(nullptr){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Queue<T>* enqueue(const T& chiave, Priorità* p){
            if(isempty()){
                head = new Nodo<T>(chiave, p);
                return this;
            }

            
        }
}