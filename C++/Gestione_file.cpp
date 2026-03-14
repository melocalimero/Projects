/*Archivio Gerarchico Ordinato
Un Archivio Gerarchico Ordinato è una struttura dati in cui ogni elemento della struttura principale è una pila, 
e ogni pila rappresenta un livello gerarchico. Ogni elemento in una pila è una coda di valori associati a una medesima chiave.

L'inserimento avviene con l’operazione:
inserisci(chiave, valore, livello)
Se esiste una pila al livello specificato, viene cercata al suo interno la coda associata alla chiave:

Se trovata, il valore viene accodato in fondo alla coda.

Se non trovata, viene creata una nuova coda con quel valore, inserita in cima alla pila.

Se il livello non esiste ancora, viene creata una nuova pila per quel livello, e la coda viene inserita come primo elemento.

L'operazione rimuovi() agisce sulla pila di livello massimo presente, rimuovendo il primo valore dalla coda in cima.
Se la coda diventa vuota, viene rimossa dalla pila.
Se la pila diventa vuota, viene rimossa dall'Archivio.

Implementare l’Archivio Gerarchico Ordinato garantendo le funzionalità sopra descritte ed implementare:

Il metodo get_livelli(), che restituisce il numero di pile (livelli gerarchici) presenti nell’Archivio.

Il metodo get_elementi_totali(), che restituisce il numero totale di valori in tutte le code dell’Archivio.

Il metodo ordina_livelli() che ordina le pile in base al numero totale di valori contenuti in ciascuna (dal più ricco al più povero).
L’ordinamento deve avvenire senza utilizzare array o vettori, ma solo mediante operazioni su pile e/o code.

L’overloading dell’operatore ^, in modo tale che stampi, dal livello più ricco a quello più povero, ogni pila nel formato:
Livello X -> [chiave0: v1, v2, ...] [chiave1: v1, v2, ...] ...*/

#include <iostream>
using namespace std;

template<typename T> class NodoQueue{
    private:
        T chiave;
        NodoQueue<T>* next;
    public:
        NodoQueue(const T& chiave): chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        
        NodoQueue<T>* get_next() const {return next;}

        void set_next(NodoQueue<T>* n){next = n;}
};

template<typename T> class Queue{
    private:
        NodoQueue<T>* head;
        NodoQueue<T>* tail;
        int key;
        size_t size;
    public:
        Queue(const int& key): head(nullptr), tail(nullptr), key(key), size(0){}
        Queue(const T& chiave, const T& key): head(new NodoQueue<T>(chiave)), tail(head), key(key), size(1){}

        NodoQueue<T>* get_queue_head() const {return head;}
        NodoQueue<T>* get_queue_tail() const {return tail;}
        const size_t& get_queue_size() const {return size;}
        const int& get_key() const {return key;}

        bool isempty() const {return head == nullptr;}

        Queue<T>* enqueue(const T& chiave){
            if(isempty()){
                
                head = new NodoQueue<T>(chiave);
                tail = head;
                size++;
                return this;
            }

            tail->set_next(new NodoQueue<T>(chiave));
            tail = tail->get_next();
            size++;
            return this;
        }

        Queue<T>* dequeue(){
            if(isempty()){
                return this;
            }

            NodoQueue<T>* tmp = head;
            head = head->get_next();
            tmp->set_next(nullptr);
            size--;
            delete tmp;
            return this;
        }

        ~Queue(){
            while(head != nullptr){
                NodoQueue<T>* tmp = head;
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

    NodoQueue<T>* tmp = q.get_queue_head();
    os << q.get_key() << ": ";
    while(tmp != nullptr){
        os << tmp->get_chiave() << ", ";
        tmp = tmp->get_next();
    }
    
    return os;
}

template<typename T> class NodoStack{
    private:
        Queue<T>* q;
        NodoStack<T>* next;
    public:
        NodoStack(const T& chiave, const T& key): q(new Queue<T>(chiave, key)), next(nullptr){}

        Queue<T>* get_q() const {return q;}
        NodoStack<T>* get_next() const {return next;}

        void set_next(NodoStack<T>* n){next = n;}
        ~NodoStack(){
            delete q;
        }
};

template<typename T> class Stack{
    private:
        NodoStack<T>* head;
        size_t size;
    public:
        Stack(): head(nullptr), size(0){}
        Stack(const T& chiave, const T& key): head(new NodoStack<T>(chiave, key)), size(1){}

        const size_t& get_stack_size() const {return size;}
        NodoStack<T>* get_stack_head() const {return head;}
        const size_t get_elements() const {
            if(isempty()){
                return 0;
            }
            NodoStack<T>* tmp = head;
            size_t somma = 0;
            while(tmp != nullptr){
                somma += tmp->get_q()->get_queue_size();
                tmp = tmp->get_next();
            }
            return somma;
        }

        bool isempty() const {return head == nullptr;}

        Stack<T>* push(const T& chiave, const T& key){
            if(isempty()){
                head = new NodoStack<T>(chiave, key);
                size++;
                return this;
            }

            NodoStack<T>* tmp = head;
            while(tmp != nullptr && tmp->get_q()->get_key() != key){
                tmp = tmp->get_next();
            }
            if(tmp == nullptr){
                tmp = head;
                head = new NodoStack<T>(chiave, key);
                head->set_next(tmp);
                size++;
                return this;
            }

            tmp->get_q()->enqueue(chiave);
            return this;
        }

        Stack<T>* pop(){
            if(isempty()){
                return this;
            }

            head->get_q()->dequeue();
            if(head->get_q()->isempty()){
                NodoStack<T>* tmp = head;
                head = head->get_next();
                size--;
                delete tmp;
            }
            return this;
        }

        ~Stack(){
            while(head != nullptr){
                NodoStack<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T> ostream& operator<<(ostream& os, Stack<T>& s){
    if(s.isempty()){
        os << "Pila vuota" << endl;
        return os;
    }

    NodoStack<T>* tmp = s.get_stack_head();
    while(tmp != nullptr){
        os << "[" << *tmp->get_q() << "]";
        tmp = tmp->get_next();
    }
    return os;
}

template<typename T> class Nodo{
    private:
        Stack<T>* s;
        Nodo<T>* next;
    public:
        Nodo(const T& chiave, const T& key): s(new Stack<T>(chiave, key)), next(nullptr){}

        Stack<T>* get_stack() const {return s;}
        Nodo<T>* get_next() const {return next;}

        void set_next(Nodo<T>* n){next = n;}
};

template<typename T> class Gestione{
    private:
        Nodo<T>* head;
        size_t size;
    public:
        Gestione(): head(nullptr), size(0){}
        Gestione(const T& chiave, const T& key): head(new Nodo<T>(chiave, key)), size(1){}

        Nodo<T>* get_head() const {return head;}
        const size_t& get_livelli() const {return size;}
        const size_t get_elementi() const {
            if(isempty()){
                return 0;
            }

            Nodo<T>* tmp = head;
            size_t somma = 0;
            while(tmp != nullptr){
                somma += tmp->get_stack()->get_elements();
                tmp = tmp->get_next();
            }
            return somma;
        }

        bool isempty() const {return head == nullptr;}

        Gestione<T>* inserisci(const T& chiave, const T& key, const size_t& livello){
            if(isempty()){
                head = new Nodo<T>(chiave, key);
                size++;
                return this;
            }

            Nodo<T>* tmp = head;
            size_t counter = 1;
            while(tmp != nullptr && counter < livello){
                tmp = tmp->get_next();
                counter++;
            }

            if(tmp == nullptr){
                tmp = head;
                head = new Nodo<T>(chiave, key);
                head->set_next(tmp);
                size++;
                return this;
            }

            tmp->get_stack()->push(chiave, key);
            return this;
        }

        Gestione<T>* pop(){
            if(isempty()){
                return this;
            }

            head->get_stack()->pop();
            if(head->get_stack()->isempty()){
                Nodo<T>* tmp = head;
                head = head->get_next();
                size--;
                delete tmp;
            }
            return this;
        }

        ~Gestione(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T> ostream& operator<<(ostream& os, Gestione<T>& g){
    if(g.isempty()){
        os << "Non c'è niente" << endl;
        return os;
    }

    Nodo<T>* tmp = g.get_head();
    size_t i = 1;
    while(tmp != nullptr){
        os << "Livello " << i << ": " << *tmp->get_stack() << endl;
        tmp = tmp->get_next();
        i++;
    }

    os << "Livelli: " << g.get_livelli() << endl;
    os << "Elementi: " << g.get_elementi() << endl;
    return os;
}
    

int main(){
    Gestione<int>* g = new Gestione<int>();
    
    g->inserisci(100, 56, 1)->inserisci(200,56, 1)->inserisci(300, 56, 2);
    cout << *g << endl;
}