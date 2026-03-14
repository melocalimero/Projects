/*Obiettivo: Progettare e implementare una PilaFlessibile che supporti operazioni push, pop, peek e un metodo set_capacity(new_capacity).

Manipolazione Fantasiosa: 
L'aspetto interessante qui è la gestione della riduzione di capacità.
Invece di segnalare un errore o bloccare l'inserimento, la pila "si adatta" rimuovendo gli elementi meno recenti.

Overloading Operatore:
Esegui l'overloading dell'operatore + per unire due PilaFlessibile.
L'unione dovrebbe creare una nuova PilaFlessibile contenente gli elementi di entrambe, à
rispettando l'ordine di inserimento (gli elementi della seconda pila vengono aggiunti dopo quelli della prima)
e la capacità della nuova pila dovrebbe essere la somma delle capacità delle due pile originali.*/
#include <iostream>
using namespace std;

template<typename T> class Nodo{
    private:
        T chiave;
        Nodo* next;
    public:
        Nodo(const T& chiave):chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        Nodo<T>* get_next() const {return next;}

        void set_next(Nodo* n){next = n;}
};

template<typename T> class Stack{
    private:
        Nodo<T>* head;
        size_t dim;
        size_t max;
    public:
        Stack(): head(nullptr),dim(0), max(20){}

        Nodo<T>* get_head() const {return head;}
        const size_t& get_max() const {return max;}
        const size_t& get_dim() const {return dim;}

        void set_max(size_t m){
            if(dim > m){
                change(dim, m);
                max = m;

                return;
            }
            max = m;
        }

        bool isempty() const {return head == nullptr;}

        Stack<T>* push(const T& chiave){
            if(this->dim >= max){
                return this;
            }

            Nodo<T>* tmp = head;
            head = new Nodo<T>(chiave);
            head->set_next(tmp);
            dim++;
            return this;
        }

        Stack<T>* pop(){
            if(isempty())
                return this;
            
            Nodo<T>* tmp = head;
            head = head->get_next();
            dim--;
            delete tmp;
            return this;
        }

        void change(size_t& old_size, size_t new_size){
            cout << "Numero di elementi da eliminare: " << old_size-new_size << endl;
            for(size_t i = 0; i <= (old_size - new_size); i++){
                pop();
            }
        }

        ~Stack(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T> ostream& operator<<(ostream& os, Stack<T>& s){
    if(s.isempty()){
        os << "Stack is empty" << endl;
        return os;
    }

    Nodo<T>* tmp = s.get_head();
    while(tmp != nullptr){
        os << tmp->get_chiave() << endl;
        tmp = tmp->get_next();
    }
    os << "Size: " << s.get_dim() << " out of " << s.get_max() << endl;
    return os;
}

int main(){
    Stack<int>* s = new Stack<int>();
    s->push(1)->push(2)->push(3)->push(4)->push(5)->push(6)->push(7);
    cout << *s << endl;

    s->set_max(5);
    cout << *s << endl;

    s->set_max(20);
    cout << *s << endl;

    s->set_max(6);
    cout << *s << endl;

    s->set_max(3);
    cout << *s << endl;

    delete s;
}