/*Comportamento:
Ogni elemento ha un valore intero.

Se il totale della pila (somma di tutti gli elementi) supera una soglia S, il push rifiuta l’elemento e lo rimanda in coda.

Se invece è sotto soglia, l’elemento viene pushato normalmente.

Obiettivo:
Mischiare comportamento di pila e coda, e una logica di gestione globale dinamica.

Overloading:
Overload dell’operatore ! per invertire l’ordine della pila (reverse interno).*/

#include <iostream>
using namespace std;
template<typename T> class Nodo{
    private:
        T chiave;
        Nodo* next;
        short valore;
    public:
        Nodo(const T& chiave, const short& valore): chiave(chiave), next(nullptr), valore(valore){}

        const T& get_chiave() const {return chiave;}
        Nodo* get_next() const {return next;}
        const short& get_valore() const {return valore;}

        void set_next(Nodo* n){next = n;}
};

template<typename T> ostream& operator<<(ostream& os, Nodo<T>& n){
    os << n.get_chiave() << " valore: " << n.get_valore();
    return os;
}

template<typename T> class Queue{
    private:
        Nodo<T>* head;
        Nodo<T>* tail;
    public:
        Queue(): head(nullptr), tail(nullptr){}
        Queue(const T& chiave, const short& valore): head(new Nodo<T>(chiave, valore)), tail(head){}

        Nodo<T>* get_head() const {return head;}
        Nodo<T>* get_tail() const {return tail;}

        bool isempty() const {return head == nullptr;}

        Queue<T>* enqueue(const T& chiave, const short& valore){
            if(isempty()){
                head = new Nodo<T>(chiave, valore);
                tail = head;
                return this;
            }

            tail->set_next(new Nodo<T>(chiave, valore));
            tail = tail->get_next();
            return this;
        }

        Nodo<T>* dequeue(){
            if(isempty()){
                return nullptr;
            }

            Nodo<T>* tmp = head;
            head =  head->get_next();
            return tmp;
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
        os << "This shit is empty " << endl;
        return os;
    }

    Nodo<T>* tmp = q.get_head();
    while(tmp != nullptr){
        os << *tmp << endl;
        tmp = tmp->get_next();
    }
    return os;
}

template<typename T> class Stack{
    private:
        Nodo<T>* head;
        short size=20;
        short somma;
        Queue<T>* q;
    public:
        Stack(): head(nullptr), somma(0), q(new Queue<T>()){}
        Stack(const T& chiave, const short& valore): head(new Nodo<T>(chiave, valore)), somma(valore), q(new Queue<T>()){}

        const short& get_somma() const {return somma;}
        const short& get_size() const {return size;}
        Nodo<T>* get_head() const {return head;}
        Queue<T>* get_queue() const {return q;}

        bool isempty() const {return head == nullptr;}
        bool isfull() const {return somma >= size;}

        Stack<T>* push(const T& chiave, const short& valore){
            if(isfull()){
                q->enqueue(chiave, valore);
                return this;
            }
            Nodo<T>* tmp = head;
            head = new Nodo<T>(chiave, valore);
            head->set_next(tmp);
            somma+=valore;
            return this;
        }

        Stack<T>* pop(){
            if(isempty()){
                return this;
            }

            Nodo<T>* tmp = head;
            head = head->get_next();
            somma-=tmp->get_valore();
            delete tmp;
            if(!q->isempty()){
                Nodo<T>* nuovo = q->dequeue();
                push(nuovo->get_chiave(), nuovo->get_valore());
            }
            return this;
        }

        ~Stack(){
           while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            } 
            delete q;
        }

};

template<typename T> ostream& operator<<(ostream& os, Stack<T>& s){
    if(s.isempty()){
        os << "This shit is empty " << endl;
        return os;
    }

    Nodo<T>* tmp = s.get_head();
    while(tmp != nullptr){
        os << *tmp << endl;
        tmp = tmp->get_next();
    }

    os << "Somma: " << s.get_somma() << " su " << s.get_size() << endl;
    if(!s.get_queue()->isempty()){
        cout << "-------" << endl;
        cout<< "Waiting queue: " << endl << *s.get_queue() << endl;
    }
    return os;
}

template<typename T> Stack<T>* operator!(Stack<T>& s){
    if(s.isempty()){
        return &s;
    }

    Stack<T>* s1 = new Stack<T>();

    Nodo<T>* tmp = s.get_head();
    while(tmp != nullptr){
        s1->push(tmp->get_chiave(), tmp->get_valore());
        tmp = tmp->get_next();
    }

    Nodo<T>* curr = s.get_queue()->get_head();
    while(curr != nullptr){
        s1->get_queue()->enqueue(curr->get_chiave(), curr->get_valore());
        curr = curr->get_next();
    }
    return s1;
}

int main(){
    Stack<string>* s = new Stack<string>();
    cout << *s << endl;

    s->push("Richi", 10)->push("Vito", 5)->push("Ussino", 7);
    cout << *s << endl;
    s->push("Melo", 5)->push("Andrea", 100)->push("Milo", 13)->push("AppenaEntroTIStupro", 4);
    cout << *s << endl;

    s->pop();
    cout << *s <<endl;

    Stack<string>* s1 = !(*s);
    cout << "s1: " << endl << *s1 << endl;
    delete s;
}