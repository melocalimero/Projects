#include <iostream>
using namespace std;

class Priorità{
    private:
        int valore;
    protected:
        Priorità(const short& valore): valore(valore){}
    public:
        virtual int get_valore() const = 0;
        virtual ~Priorità(){}
};

class Urgente: public Priorità{
    public:
        Urgente(): Priorità(1){}
        int get_valore() const override {return 1;}
        ~Urgente() override{}
};

class Ordinaria: public Priorità{
    public:
        Ordinaria(): Priorità(2){}
        int get_valore() const override {return 2;}
        ~Ordinaria() override{}
};

class Controllo: public Priorità{
    public:
        Controllo(): Priorità(3){}
        int get_valore() const override {return 3;}
        ~Controllo() override{}
};

class Prenotazione{
    private:
        string nome, cf, esame;
        Priorità* p;
    public:
        Prenotazione(string nome, string cf, string esame, Priorità* p):
        nome(nome), cf(cf), esame(esame), p(p){}

        const string& get_nome() const {return nome;}
        const string& get_cf() const {return cf;}
        const string& get_esame() const {return esame;}
        Priorità* get_priorità() const {return p;}

        bool operator<=(Prenotazione* pr){
            return this->p->get_valore() <= pr->p->get_valore();
        }

        void stampa() const {
            cout << this->nome << " " << this->cf << ": " << this->esame << " priorità: " << this->p->get_valore();
        }

        ~Prenotazione(){
            delete p;
        }
};

ostream& operator<<(ostream& os, Prenotazione& p){
    p.stampa();
    return os;
}

template<class T> class Nodo{
    private:    
        T chiave;
        Nodo* next;
    public:
        Nodo(const T& chiave): chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        Nodo* get_next() const {return next;}

        void set_next(Nodo* n) {this->next = n;}
};

template<class T> class Coda{
    private:
        Nodo<T>* head;
        Nodo<T>* tail;
    public:
        Coda(): head(nullptr), tail(nullptr){}
        Coda(const T& chiave): head(new Nodo<T>(chiave)), tail(head){}

        Nodo<T>* get_head() const {return head;}
        Nodo<T>* get_tail() const {return tail;}

        bool isempty() const {return head == nullptr;}

        Coda<T>* enqueue(const T& chiave){
            if(isempty()){
                head = new Nodo<T>(chiave);
                tail = head;
                return this;
            }

            Nodo<T>* tmp = tail;
            tail = new Nodo<T>(chiave);
            tmp->set_next(tail);
            return this;
        }

        Coda<T>* dequeue(){
            if(isempty()){
                return this;
            }

            Nodo<T>* tmp = head;
            head = head->get_next();
            delete tmp;
            return this;
        }

        ~Coda(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
            delete head; 
            
        }
};


template<class T> ostream& operator<<(ostream& os, Coda<T>& c){
    if(c.isempty()){
        os << "La coda è vuota" << endl;
        return os;
    }

    Nodo<T>* tmp = c.get_head();
    while(tmp != nullptr){
        os << (*tmp->get_chiave()) << endl;
        tmp = tmp->get_next();
    }
    return os;
}

template<class T> class Pila{
    private:
        Nodo<T>* head;
    public:
        Pila(): head(nullptr){}
        Pila(const T&  chiave): head(new Nodo<T>(chiave)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Pila<T>* push(const T& chiave){
            Nodo<T>* tmp = head;
            head = new Nodo<T>(chiave);
            head->set_next(tmp);
            return this;
        }

        Pila<T>* pop(){
            if(isempty()){
                return this;
            }

            Nodo<T>* tmp = head;
            head = head->get_next();
            delete tmp;
            return this;
        }

        ~Pila(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
            delete head;
        }
};


template<class T> ostream& operator<<(ostream& os, Pila<T>& p){
    if(p.isempty()){
        os << "La pila è vuota" << endl;
        return os;
    }
    Nodo<T>* tmp = p.get_head();
    while(tmp != nullptr){
        os << (*tmp->get_chiave()) << endl;
        tmp = tmp->get_next();
    }
    return os;
    
}

template<class T>size_t partition(T* arr, const size_t& begin, const size_t& end){
    size_t p = begin;
    for(size_t i = begin; i <= end; i++){
        if(*(arr[i]) <= arr[end]){
            swap(arr[i], arr[p]);
            p++;
        }
    }
    return p-1;
}


template<class T> void ordina(T* arr, const size_t& begin, const size_t& end){
    if(begin >= end){
        return;
    }

    size_t p = partition(arr, begin, end);
    if(p > 0)
        ordina(arr, begin, p-1);
    ordina(arr, p+1, end);
}

template<class T> void ordina(T* arr, const size_t& dim){
    ordina(arr, 0, dim-1);
}

Pila<Prenotazione*>* riempi(Coda<Prenotazione*>* c){
    Pila<Prenotazione*>* p = new Pila<Prenotazione*>();

    Nodo<Prenotazione*>* tmp = c->get_head();
    while(tmp != nullptr){
        if(dynamic_cast<Urgente*>(tmp->get_chiave()->get_priorità())){
            p->push(tmp->get_chiave());
        }
        tmp = tmp->get_next();
    }

    return p;
}

int main(){
    const size_t dim = 5;
    Prenotazione** p = new Prenotazione*[dim];

    p[0] = new Prenotazione("Andrea", "BADAUI726E82EGI", "Prostata", new Controllo());
    p[1] = new Prenotazione("Carlo", "DAUI27836YUDAB", "Culo", new Urgente());
    p[2] = new Prenotazione("Vito", "VDTAFYOIHA98Y3DIUA", "Cervello", new Ordinaria());
    p[3] = new Prenotazione("Angelo", "AIBDIA83BADA", "Cuore", new Urgente());
    p[4] = new Prenotazione("Nunzio", "NADIBUDA2YEDIBUAB", "Mani", new Controllo());

    Coda<Prenotazione*>* c = new Coda<Prenotazione*>();

    for(size_t i = 0; i < dim; i++){
        c->enqueue(p[i]);
    }

    cout << *c << endl;

    ordina(p, dim);
    cout << "Array ordinato: " << endl;
    for(size_t i = 0; i < dim; i++){
        cout << *p[i] << endl;
    }
    
    Pila<Prenotazione*>* pila = riempi(c);

    cout << endl << "Pila: "<<endl;
    cout << *pila << endl;

    delete c;
    delete pila;

    for(size_t i = 0; i < dim; i++){
        delete p[i];
    }
    delete [] p;

}