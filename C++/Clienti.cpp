#include <iostream>
using namespace std;

class Priorità{
    private:
        int valore;
    public:
        Priorità(int valore): valore(valore){}
        const int& get_valore() const {return valore;}
        virtual string get_tipologia() const = 0;
        virtual ~Priorità(){}
};
class Bassa: public Priorità{
    public:
        Bassa(): Priorità(1){}
        string get_tipologia() const {return "Bassa";}
        ~Bassa() override{}
};
class Media: public Priorità{
    public:
        Media(): Priorità(2){}
        string get_tipologia() const {return "Media";}
        ~Media() override {}
};
class Alta: public Priorità{
    public:
        Alta(): Priorità(3){}
        string get_tipologia() const {return "Alta";}
        ~Alta() override {}
};
class Cliente{
    private:
        int id;
        string nome;
        Priorità* p;
    public:
        Cliente(int id, string nome, Priorità* p):
        id(id), nome(nome), p(p){}

        const int& get_id() const {return id;}
        const string& get_nome() const {return nome;}
        Priorità* get_priorità() const {return p;}

        bool operator<=(Cliente* c){
            return this->p->get_valore() <= c->get_priorità()->get_valore();
        }

        bool operator==(Priorità* pr){
            return this->p->get_valore() == pr->get_valore();
        }

        void stampa() const {
            cout << this->id << ": " << this->nome << " priorità:" << this->p->get_valore() << ", " << this->p->get_tipologia();
        }

        ~Cliente(){
            delete p;
        }
};

ostream& operator<<(ostream& os, Cliente* c){
    c->stampa();
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
            Nodo<T>* tmp = head;
            while(tmp != nullptr){
                Nodo<T>* todel = tmp;
                tmp = tmp->get_next();
                delete todel;
            }
            
        }
};

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
            if(isempty())
                return this;
            
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
 
template<class T> ostream& operator<<(ostream& os, Coda<T>* c){
    if(c->isempty()){
        os << "Coda è vuota" << endl;
        return os;
    }
    Nodo<T>* tmp = c->get_head();
    while(tmp != nullptr){
        os << tmp->get_chiave() << endl;
        tmp = tmp->get_next();
    }
    return os;
}

template<class T> ostream& operator<<(ostream& os, Pila<T>* p){
     if(p->isempty()){
        os << "Pila è vuota" << endl;
        return os;
    }
    Nodo<T>* tmp = p->get_head();
    while(tmp != nullptr){
        os << tmp->get_chiave() << endl;
        tmp = tmp->get_next();
    }
    return os;
}

template<class T> size_t partizione(T* arr, const size_t begin, const size_t end){
    size_t p = begin;
    for(size_t i = begin; i <= end; i++){
        if(*(arr[i]) <= arr[end]){
            swap(arr[i], arr[p]);
            p++;
        }
    }
    return p-1;
}

template<class T> void ordina (T* arr, const size_t begin, const size_t end){
    //caso base
    if(begin >= end){
        return;
    }

    //caso induttivo
    size_t p = partizione<T>(arr, begin, end);
    if(p > 0){
        ordina(arr, begin, p-1);
    }
    ordina(arr, p+1, end);
}

template<class T> void ordina(T* arr, const size_t& dim){
    ordina(arr, 0, dim-1);
}

Pila<Cliente*>* riempiPila(Coda<Cliente*>* c){
    Pila<Cliente*>* pila = new Pila<Cliente*>();
    Nodo<Cliente*>* tmp = c->get_head();
    while(tmp != nullptr){
        if(typeid(*(tmp->get_chiave()->get_priorità())) == typeid(Alta)){
            pila->push(tmp->get_chiave());
        }
        tmp = tmp->get_next();
    }
    return pila;
}
int main(){
    const size_t dim = 5;
    Cliente** clienti = new Cliente*[dim];
    clienti[0] = new Cliente(1, "Mario", new Bassa());
    clienti[1] = new Cliente(2, "Vito", new Media());
    clienti[2] = new Cliente(3, "Andrea", new Alta());
    clienti[3] = new Cliente(4, "Carlo", new Alta());
    clienti[4] = new Cliente(5, "Jason", new Bassa());

    Coda<Cliente*>* coda = new Coda<Cliente*>();

    for(size_t i=0; i<dim; i++){
        coda->enqueue(clienti[i]);
    }

    cout << coda << endl;

    ordina<Cliente*>(clienti, dim);
    
    for(size_t i=0; i<dim; i++){
        cout << clienti[i] <<endl;
    }

    Pila<Cliente*>* pila = riempiPila(coda);
    cout << "Pila: " << endl;
    cout << pila << endl;
    

    delete pila;
    delete coda;
    for(size_t i = 0; i < dim; i++){
        delete clienti[i];
    }
    delete[] clienti;
}