#include <iostream>
using namespace std;

class Priorità{
    private:
        int valore;
    protected:
        Priorità(int valore): valore(valore){}
    public:
        
        const int& get_valore() const {return valore;}
        virtual string terminologia() const = 0;
        virtual ~Priorità(){}
};

class Emergenza: public Priorità{
    public:
        Emergenza(): Priorità(1){}
        string terminologia() const override {return "Emergenza";}
        ~Emergenza() override {}
};

class Standard: public Priorità{
    public:
        Standard(): Priorità(2){}
        string terminologia() const override {return "Standard";}
        ~Standard() override {}
};

class Ritardabile: public Priorità{
    public: 
        Ritardabile(): Priorità(3){}
        string terminologia()const override {return "Ritardabile";}
        ~Ritardabile() override {}
};

class InVolo: public Priorità{
    public:
        InVolo(): Priorità(4){}
        string terminologia()const  override {return "In volo";}
        ~InVolo(){}
};

class Aereo{
    private:
        int id;
        string compagnia;
        bool richiesta;
        Priorità* p;

    public:
        Aereo(int id, string compagnia):
        id(id), compagnia(compagnia), p(new InVolo()), richiesta(false){}

        const int& get_id() const {return id;}
        const string& get_compagnia() const {return compagnia;}
        Priorità* get_priorità() const {return p;}
        const bool& get_richiesta() const {return richiesta;}

        void change_richiesta(Priorità* pr) {
            if(typeid(*pr) == typeid(*p)){
                delete pr;
                return;
            }

            if(richiesta){
                richiesta = false;
                p = new InVolo();
            }
            else{
                richiesta = true;
                Priorità* todel = p;
                p = pr;
                delete todel;
            }
        }

        bool operator<=(Aereo* a){
            return this->p->get_valore() <= a->get_priorità()->get_valore();
        }

        void stampa() const {
            cout << this->id << " " << this->compagnia;
            if(this->richiesta){
                cout << ", deve atterrare, Priorità: " << this->p->terminologia();
            }
            else{
                cout << ", non deve atterrare";
            }
        }

        ~Aereo() {
            delete p;
        }
};

ostream& operator<<(ostream& os, Aereo* a){
    a->stampa();
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
    public:
        Coda(): head(nullptr){}
        Coda(const T& chiave): head(new Nodo<T>(chiave)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Coda<T>* enqueue(const T& chiave){
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
            while(!isempty()){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
            delete head;
        }
};

template<class T> ostream& operator<<(ostream& os, Coda<T>* c){
    if(c->isempty()){
        os << "La coda è vuota " << endl;
        return os;
    }

    Nodo<T>* tmp = c->get_head();
    while(tmp != nullptr){
        os << tmp->get_chiave() << endl;
        tmp = tmp->get_next();
    }
    return os;
}

template<class T> class Pila{
    private:
        Nodo<T>* head;
    public:
        Pila(): head(nullptr){}
        Pila(const T& chiave): head(new Nodo<T>(chiave)){}

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

template<class T> ostream& operator<<(ostream& os, Pila<T>* p){
     if(p->isempty()){
        os << "La pila è vuota " << endl;
        return os;
    }

    Nodo<T>* tmp = p->get_head();
    while(tmp != nullptr){
        os << tmp->get_chiave() << endl;
        tmp = tmp->get_next();
    }
    return os;
}

template<class T> size_t partition(T* arr, const size_t& begin, const size_t& end){
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

    size_t p=partition(arr, begin, end);

    if(p>0)
        ordina(arr, begin, p-1);
    ordina(arr, p+1, end);
}

template<class T> void ordina(T* arr, const size_t& dim){
    ordina(arr, 0, dim-1);
}

Pila<Aereo*>* riempiPila(Coda<Aereo*>* c){
    Pila<Aereo*>* p= new Pila<Aereo*>();

    Nodo<Aereo*>* tmp = c->get_head();
    while(tmp != nullptr){
        if(typeid(*tmp->get_chiave()->get_priorità()) == typeid(Emergenza)){
            p->push(tmp->get_chiave());
        }
        tmp = tmp->get_next();
    }
    return p;
}

int main(){
    const size_t dim = 5;
    Aereo** a = new Aereo*[dim];

    a[0] = new Aereo(1, "Airways");
    a[1] = new Aereo(2, "Ryanair");
    a[2] = new Aereo(3, "Emirates");
    a[3] = new Aereo(4, "Vueling");
    a[4] = new Aereo(5, "France Airlane");

    a[0]->change_richiesta(new Emergenza());
    a[1]->change_richiesta(new Emergenza());
    a[2]->change_richiesta(new Ritardabile());
    a[3]->change_richiesta(new InVolo());
    Coda<Aereo*>* c = new Coda<Aereo*>();
    
    for(size_t i = 0; i < dim; i++){
        if(a[i]->get_richiesta()){
            c->enqueue(a[i]);
        }
    }

    cout << c << endl;
    ordina<Aereo*>(a, dim);

    for(size_t i = 0; i < dim; i++){
        cout << a[i] << endl;
    }

    Pila<Aereo*>* p = riempiPila(c);
    
    cout <<endl<<"Pila: " << endl << p <<endl;


    delete c;
    delete p;
    for(size_t i = 0; i < dim; i++){
        delete a[i];
    }
    delete[] a;
}