/*Comportamento:
Ogni elemento ha una “fazione” (A, B, C...)

Quando viene inserito un elemento di una fazione, vengono espulsi tutti gli elementi
più vecchi di fazioni nemiche (che puoi gestire con una mappa di conflitti).

La logica di conflitto può essere asimmetrica.

Obiettivo:
Implementare una coda politica/strategica, con elementi in mutua esclusione.

Overloading:
Overload dell’operatore % per ottenere la percentuale di elementi rimanenti per ciascuna fazione.*/

#include <iostream>
using namespace std;

template<typename T> class Nodo{
    private:
        T chiave;
        Nodo* next;
        char fazione;
        char alleato;
    public:
        Nodo(const T& chiave, const char& fazione): chiave(chiave), next(nullptr), fazione(fazione){
            if(fazione != 'A' && fazione != 'B' && fazione != 'C' && fazione != 'D'){
                cout << "Fazione deve essere A, B, C o D" << endl;
                exit(1);
            }
            switch(fazione){
                case 'A':
                    alleato = 'B';
                break;
                case 'B':
                    alleato = 'A';
                break;
                case 'C':
                    alleato = 'C';
                break;
                case 'D':
                    alleato = 'D';
                break;
            }
        }

        const T& get_chiave() const {return chiave;}
        const char& get_fazione() const {return fazione;}
        Nodo<T>* get_next() const {return next;}
        const char& get_alleato() const {return alleato;}

        void set_next(Nodo<T>* n){next = n;}
};

template<typename T> ostream& operator<<(ostream& os, Nodo<T>& n){
    os << n.get_chiave() << " Fazione: " << n.get_fazione();
    return os;
}

template<typename T> class Queue{
    private:
        Nodo<T>* head;

        Queue<T>* svuotaPila(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
            return this;
        }
    public:
        Queue(): head(nullptr){}
        Queue(const T& chiave, const char& fazione): head(new Nodo<T>(chiave, fazione)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        

        Queue<T>* enqueue(Nodo<T>* n){
            if(isempty()){
                head = n;
                return this;
            }

            Nodo<T>* tmp = head;
            if(head->get_fazione() == n->get_fazione() || head->get_alleato() == n->get_fazione()){
                while(tmp->get_next() != nullptr){
                    tmp = tmp->get_next();
                }
                tmp->set_next(n);
                return this;
            }
            svuotaPila();
            head = n;
            return this;
        }

        Queue<T>* dequeue(){
            if(isempty()){
                return this;
            }

            Nodo<T>* tmp = head;
            head = head->get_next();
            delete tmp;
            return this;
        }

        double operator%(const char& fazione){
            short somma = 0;
            size_t size = 0;

            Nodo<T>* tmp = head;
            while(tmp != nullptr){
                if(tmp->get_fazione() == fazione){
                    somma++;
                }
                size++;
                tmp = tmp->get_next();
            }

            return double(somma)/size;
        }

        ~Queue(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T> ostream& operator<<(ostream& os, Queue<T>& q){
    if(q.isempty()){
        os << "Queue is empty " << endl;
        return os;
    }

    Nodo<T>* tmp = q.get_head();
    while(tmp != nullptr){
        os << *tmp << endl;
        tmp = tmp->get_next();
    }
    return os;
}

int main(){
    Queue<int>* q = new Queue<int>();

    q->enqueue(new Nodo<int>(3, 'A'))->enqueue(new Nodo<int>(4, 'A'))->enqueue(new Nodo<int>(5, 'A'))->enqueue(new Nodo<int>(6, 'A'));
    cout << *q << endl;
    
    q->enqueue(new Nodo<int>(1, 'C'));
    cout << *q << endl;

    q->enqueue(new Nodo<int>(2, 'C'))->enqueue(new Nodo<int>(3, 'A'))->enqueue(new Nodo<int>(2, 'B'))->enqueue(new Nodo<int>(4,'B'));
    cout << *q << endl;


    cout << "Percentuale fazione B: " << (*q)%'B' << endl;
    delete q;
}