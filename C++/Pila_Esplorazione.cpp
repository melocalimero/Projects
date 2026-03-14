/*Descrizione:
Una Pila di Esplorazione è uno stack dove ogni elemento rappresenta una posizione in uno spazio di ricerca,
 e contiene a sua volta una coda di tentativi fatti da quella posizione.
Quando si effettua una push(posizione, scelta) nella pila, se la testa è già la stessa posizione, si aggiunge scelta in coda.
Se la posizione è nuova, viene aggiunta in cima come nuova pila con il primo tentativo registrato.
Il pop() rimuove l’ultimo tentativo fatto dalla posizione in cima, e se i tentativi finiscono, si rimuove anche quella posizione.

Richiesto:
get_path_length(): numero di posizioni esplorate (cioè quante pile).

get_total_attempts(): totale dei tentativi registrati in tutte le posizioni.

Overloading di operator~ per stampare dalla testa la traccia nel formato:
[(posizione) => scelta1, scelta2, ...]

*/

#include <iostream>
using namespace std;


template<typename T> class Nodo_Queue{
    private:
        T chiave;
        Nodo_Queue* next;
    public:
        Nodo_Queue(const T& chiave): chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        Nodo_Queue<T>* get_next() const {return next;}

        void set_next(Nodo_Queue<T>* n){next = n;}
};

template<typename T> class Queue{
    private:
        Nodo_Queue<T>* head;
        size_t size;
    public:
        Queue(): head(nullptr), size(0){}
        Queue(const T& chiave): head(new Nodo_Queue<T>(chiave)), size(1){}

        Nodo_Queue<T>* get_head() const {return head;}
        const size_t& get_size() const {return size;}

        bool isempty() const {return head == nullptr;}

        Queue<T>* enqueue(const T& chiave){
            if(isempty()){
                head = new Nodo_Queue<T>(chiave);
                size++;
                return this;
            }

            Nodo_Queue<T>* tmp = head;
            Nodo_Queue<T>* prev = nullptr;
            while(tmp != nullptr){
                if(tmp->get_chiave() == chiave){
                    return this;
                }
                prev = tmp;
                tmp = tmp->get_next();
            }
            size++;
            prev->set_next(new Nodo_Queue<T>(chiave));
            return this;
        }

        Queue<T>* dequeue(){
            if(isempty()){
                return this;
            }

            Nodo_Queue<T>* tmp = head;
            head = head->get_next();
            size--;
            delete tmp;
            return this;
        }

        ~Queue(){
            while(head != nullptr){
                Nodo_Queue<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T> ostream& operator<<(ostream& os, Queue<T>& q){
    if(q.isempty()){
        os << "Coda vuota" << endl;
        return os;
    }

    Nodo_Queue<T>* tmp = q.get_head();
    while(tmp->get_next() != nullptr){
        os << tmp->get_chiave() << ", ";
        tmp = tmp->get_next();
    }

    os << tmp->get_chiave();
    return os;
}


template<typename T> class Nodo_Stack{
    private:
        Nodo_Stack* next;
        T posizione;
        int scelta;
        Queue<int>* q;
    public:
        Nodo_Stack( const T& posizione, const int& scelta):
        next(nullptr), posizione(posizione), scelta(scelta), q(new Queue<int>(scelta)){}

        const T& get_posizione() const {return posizione;}
        const int& get_scelta() const {return scelta;}
        Nodo_Stack<T>* get_next() const {return next;}
        Queue<int>* get_queue() const {return q;}

        void set_next(Nodo_Stack* n){next = n;}

        ~Nodo_Stack(){
            delete q;
        }
};


template<typename T> class Stack{
    private:
        Nodo_Stack<T>* head;
    public:
        Stack(): head(nullptr){}
        Stack(const T& posizione, const int& scelta):head(new Nodo_Stack(posizione, scelta)){}

        Nodo_Stack<T>* get_head() const {return head;}
        const size_t get_path_length(){
            if(isempty()){
                return 0;
            }

            size_t somma = 0;
            Nodo_Stack<T>* tmp = head;
            while(tmp != nullptr){
                somma++;
                tmp = tmp->get_next();
            }

            return somma;
        }

        const size_t get_total_attempts(){
            if(isempty()){
                return 0;
            }

            size_t somma = 0;
            Nodo_Stack<T>* tmp = head;
            while(tmp != nullptr){
                somma += tmp->get_queue()->get_size();
                tmp = tmp->get_next();
            }
            return somma;
        }

        bool isempty() const {return head == nullptr;}

        Stack<T>* push(const T& posizione, const int& scelta){
            if(isempty()){
                head = new Nodo_Stack<T>(posizione, scelta);
                return this;
            }

            Nodo_Stack<T>* tmp = head;
            while(tmp != nullptr && tmp->get_posizione() != posizione){
                tmp = tmp->get_next();
            }
            if(tmp == nullptr){
                tmp = head;
                head = new Nodo_Stack<T>(posizione, scelta);
                head->set_next(tmp);
                return this;
            }

            tmp->get_queue()->enqueue(scelta);
            return this;
        }

        Stack<T>* pop(){
            if(isempty()){
                return this;
            }

            head->get_queue()->dequeue();
            if(head->get_queue()->isempty()){
                Nodo_Stack<T>* tmp = head;
                head = head->get_next();
                tmp->set_next(nullptr);
                delete tmp;
            }
            return this;
        }

        ~Stack(){
            while(head != nullptr){
                Nodo_Stack<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T> ostream& operator<<(ostream& os, Stack<T>& s){
    if(s.isempty()){
        os << "Stack vuoto " << endl;
        return os;
    }

    Nodo_Stack<T>* tmp = s.get_head();
    while(tmp != nullptr){
        os << "[(" << tmp->get_posizione() << ")=> " << *tmp->get_queue() << "]" <<endl;
        tmp = tmp->get_next();
    }

    os << "Posizioni: " << s.get_path_length() << endl;
    os << "Tentativi totali: " << s.get_total_attempts() << endl;
    return os;
}

int main(){
    Stack<string>* s = new Stack<string>();
    s->push("Catania", 4)->push("Palermo", 4)->push("Messina", 5)->push("Catania", 6)->push("Palermo", 4);
    cout << *s << endl;

    delete s;
}