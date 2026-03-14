/*Comportamento:
Ogni elemento ha due timer: tempo_entrata e tempo_sopportato.

A ogni enqueue, il tempo_entrata degli altri elementi aumenta di 1.

Se per un elemento tempo_entrata >= tempo_sopportato, esce prima del suo turno.

L’enqueue() può causare multiple uscite.

Obiettivo:
Gestione avanzata degli elementi che si autodisconnettono dalla coda per “stress”.

Overloading:
Overload dell’operatore - tra due code per ottenere una nuova coda con solo gli elementi che sono presenti 
in entrambe (in base alla chiave).*/



#include <iostream>
using namespace std;

template<typename T> class Nodo{
    private:
        T chiave;
        Nodo<T>* next;
        int tempo_sopportato, tempo_max;
    public:
        Nodo(const T& chiave, const int& tempo_max): 
        chiave(chiave), next(nullptr), tempo_max(tempo_max), tempo_sopportato(0){}

        const T& get_chiave() const {return chiave;}
        const int& get_tempo_max() const {return tempo_max;}
        const int& get_tempo_sopportato() const {return tempo_sopportato;}
        Nodo<T>* get_next() const {return next;}

        bool operator==(Nodo<T>& n) const {
            return this->chiave == n.chiave && this->tempo_max == n.tempo_max;
        }

        void incrementa_tempo_sopportato(){tempo_sopportato++;}
        void set_next(Nodo<T>* n){next = n;}
};

template<typename T> ostream& operator<<(ostream& os, Nodo<T>& n){
    os << n.get_chiave() << ", Tempo: " << n.get_tempo_sopportato() << " min su " << n.get_tempo_max();
    return os;
}

template<typename T> class Queue{
    private:
        Nodo<T>* head;
    public:
        Queue(): head(nullptr){}
        Queue(const T& chiave, const int& tempo_max): head(new Nodo<T>(chiave, tempo_max)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Queue<T>* enqueue(const T& chiave, const int&  tempo_max){
            Nodo<T>* nuovo = new Nodo<T>(chiave, tempo_max);
            nuovo->incrementa_tempo_sopportato();
            if(isempty()){
                head = nuovo;

                return this;
            }

            Nodo<T>* tmp = head;
            while(tmp->get_next() != nullptr){
                tmp->incrementa_tempo_sopportato();
                tmp = tmp->get_next();
            }
            tmp->incrementa_tempo_sopportato();
            tmp->set_next(nuovo);
            check_stress();
            return this;
        }

        void check_stress(){
            while(head->get_tempo_sopportato() >= head->get_tempo_max()){
                dequeue();
            }
        
            Nodo<T>* tmp = head->get_next();
            Nodo<T>* prev = head;
            while(tmp != nullptr){
                if(tmp->get_tempo_sopportato() >= tmp->get_tempo_max()){
                    Nodo<T>* todel = tmp;
                    tmp = tmp->get_next();
                    prev->set_next(tmp);
                    todel->set_next(nullptr);
                    delete todel;
                }
                else{
                    prev = tmp;
                    tmp = tmp->get_next();
                }
            }
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
        os << "This shit is empty" << endl;
        return os;
    }

    Nodo<T>* tmp = q.get_head();
    while(tmp != nullptr){
        os << *tmp << endl;
        tmp = tmp->get_next();
    }
    return os;
}

template<typename T> Queue<T>* operator-(Queue<T>& q1, Queue<T>& q2){
    Queue<T>* q = new Queue<T>();
    Nodo<T>* tmp = q1.get_head();
    
    while(tmp != nullptr){
        Nodo<T>* tmp2 = q2.get_head();
        while(tmp2 != nullptr){
            if((*tmp) == (*tmp2)){
                q->enqueue(tmp->get_chiave(), tmp->get_tempo_max());
                break;
            }
            tmp2 = tmp2->get_next();
        }
        tmp = tmp->get_next();
    }
    return q;
}
int main(){
    Queue<string>* q = new Queue<string>();
    q->enqueue("Melo", 4)->enqueue("Luca", 5)->enqueue("Marco", 3);

    cout << "Q1: " << endl << *q << endl;

    Queue<string>* q2 = new Queue<string>();
    q2->enqueue("Melo", 4)->enqueue("Mario", 6)->enqueue("Luca", 5);

    cout << "Q2: " << endl << *q2 << endl;

    Queue<string>* q3 = (*q)-(*q2);

    cout << "Q3: " << endl << *q3 << endl;
    delete q;
    delete q2;
}