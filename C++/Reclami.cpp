/*Esercizio: Sistema di Smistamento Reclami Tecnici
Contesto
Un'azienda informatica riceve reclami tecnici da parte di clienti. 
Ogni reclamo contiene un codice identificativo, il nome del cliente e una priorità (bassa, media o alta).

I reclami vengono:

Inseriti inizialmente in una coda in ordine di arrivo.

I reclami urgenti (priorità alta) vengono spostati in una pila per essere risolti più velocemente.

Tutti i reclami vengono infine ordinati per priorità (dalla più bassa alla più alta) usando quicksort prima di generare un report.

Specifiche
Implementa:

Una gerarchia di classi per rappresentare la priorità (Priorita, Bassa, Media, Alta) come già fatto.

Una classe Reclamo (invece di Cliente) con:

codice reclamo (string)

nome cliente (string)

priorità (Priorita*)

Una coda Coda<Reclamo*> che simula l’arrivo dei reclami.

Una pila Pila<Reclamo*> per gestire solo i reclami urgenti.
Una funzione riempi_pila che prende in input la coda e riempie la pila con solo i reclami urgenti.

Una funzione quickSort per ordinare i reclami secondo la priorità.

Stampa finale:

Lista dei reclami nella coda.

Lista dei reclami urgenti nella pila.

Array dei reclami ordinato per priorità.
*/

#include <iostream>
using namespace std;

class Priorità{
    private:
        int valore;
    protected:
        Priorità(int valore): valore(valore){}
    public:
        virtual int get_valore() const = 0;
        virtual string get_tipologia() const = 0;
        virtual ~Priorità(){}
};

class Alta: public Priorità{
    public:
        Alta(): Priorità(1){}
        int get_valore() const override {return 1;}
        string get_tipologia() const override {return "Alta";}

        ~Alta() override{}
};

class Media: public Priorità{
    public:
        Media(): Priorità(2){}
        int get_valore() const override {return 2;}
        string get_tipologia() const override {return "Media";}

        ~Media() override{}
};

class Bassa: public Priorità{
    public:
        Bassa(): Priorità(3){}
        int get_valore() const override {return 3;}
        string get_tipologia() const override {return "Bassa";}

        ~Bassa() override{}
};

class Reclamo{
    private:
        int codice;
        string nome;
        Priorità* p;
    public:
        Reclamo(int codice, string nome, Priorità* p): 
        codice(codice), nome(nome), p(p){}

        const int& get_codice() const {return codice;}
        const string& get_nome() const {return nome;}
        Priorità* get_priorità() const {return p;}


        bool operator<=(Reclamo& r){
            return this->p->get_valore() <= r.p->get_valore();
        }
        void stampa() const{
            cout << this->codice << ": " << this->nome << " Priorità: " << this->p->get_valore() << ", " << this->p->get_tipologia(); 
        }

        ~Reclamo(){
            delete p;
        }
};

ostream& operator<<(ostream& os, Reclamo& r){
    r.stampa();
    return os;
}

template<typename T> class Nodo{
    private:
        T chiave;
        Nodo* next;
    public:
        Nodo(const T& chiave): chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;} 
        Nodo* get_next() const {return next;}

        void set_next(Nodo* n) {this->next = n;}
};

template<typename T> class Queue{
    private:
        Nodo<T>* head;
    public:
        Queue(): head(nullptr){}
        Queue(const T& chiave): head(new Nodo<T>(chiave)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Queue<T>* enqueue(const T& chiave){
            if(isempty()){
                head = new Nodo<T>(chiave);
                return this;
            }

            Nodo<T>* tmp = head;
            while(tmp->get_next() != nullptr){
                tmp = tmp->get_next();
            }

            tmp->set_next(new Nodo<T>(chiave));
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
        os << "Coda vuota" << endl;
        return os;
    }

    Nodo<T>* tmp = q.get_head();
    while(tmp != nullptr){
        os << (*tmp->get_chiave()) << endl;
        tmp = tmp->get_next();
    }

    return os;
}

template<typename T> class Stack{
    private:
        Nodo<T>* head;
    public:
        Stack(): head(nullptr){}
        Stack(const T& chiave): head(new Nodo<T>(chiave)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Stack<T>* push(const T& chiave){
            Nodo<T>* tmp = head;
            head = new Nodo<T>(chiave);
            head->set_next(tmp);
            return this;
        }

        Stack<T>* pop(){
            if(isempty()){
                return this;
            }

            Nodo<T>* tmp = head;
            head = head->get_next();
            delete tmp;
            return this;
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
        os << "Coda vuota" << endl;
        return os;
    }

    Nodo<T>* tmp = s.get_head();
    while(tmp != nullptr){
        os << (*tmp->get_chiave()) << endl;
        tmp = tmp->get_next();
    }

    return os;
}


template<typename T>size_t partition(T* arr, const size_t& begin, const size_t end){
    size_t p = begin;

    for(size_t i = begin; i <= end; i++){
        if(*(arr[i]) <= *(arr[end])){
            swap(arr[i], arr[p]);
            p++;
        }
    }
    return p-1;
}

template<typename T> void ordina(T* arr, const size_t& begin, const size_t& end){
    if(begin >= end){
        return;
    }

    size_t p = partition(arr, begin, end);

    ordina(arr, p+1, end);
    if(p > 0){
        ordina(arr, begin, p-1);
    }
}
template<typename T> void ordina(T* arr, const size_t& dim){
    ordina(arr, 0, dim-1);
}

Stack<Reclamo*>* filter(Queue<Reclamo*>* q){
    Stack<Reclamo*>* s = new Stack<Reclamo*>();
    Nodo<Reclamo*>* tmp = q->get_head();

    while(tmp != nullptr){
        if(dynamic_cast<Alta*>(tmp->get_chiave()->get_priorità())){
            s->push(tmp->get_chiave());
        }
        tmp = tmp->get_next();
    }
    return s;
}
int main(){
    const size_t dim = 5;
    Reclamo** r = new Reclamo*[dim];

    r[0] = new Reclamo(1, "Vito", new Bassa());
    r[1] = new Reclamo(2, "Andrea", new Alta());
    r[2] = new Reclamo(3, "Ussino", new Alta());
    r[3] = new Reclamo(4, "Delfo", new Media());
    r[4] = new Reclamo(5, "Angelo", new Bassa());


    ordina(r, dim);

    Queue<Reclamo*>* q = new Queue<Reclamo*>();
    for(size_t i = 0; i < dim; i++){
        cout << *r[i] << endl;
        q->enqueue(r[i]);
    }

    cout << "Coda: "<< endl << *q << endl;
    Stack<Reclamo*>* s = filter(q);

    cout << "Pila: "<< endl << *s << endl;


    delete s;
    delete q;
    for(size_t i = 0; i < dim; i++){
        delete r[i];
    }

    delete[] r;
}