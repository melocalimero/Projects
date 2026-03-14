/*Comportamento:
È una pila in cui ogni push() può comportare uno spostamento interno degli elementi.
Ogni elemento ha un peso.

Se un nuovo elemento ha peso dispari, viene pushato sopra.

Se ha peso pari, viene inserito sotto il primo elemento più leggero di lui (se esiste), oppure in fondo.

Obiettivo:
Implementare questa pila con comportamento non strettamente LIFO ma influenzato dai valori degli elementi.

Overloading:
Overload dell’operatore ++ per aumentare il peso di tutti gli elementi della pila.*/
#include <iostream> 
using namespace std;

template<class T> class Nodo{
    private:
        T chiave;
        Nodo* next;
        int peso;
    public:
        Nodo(const T& chiave, const int& peso): chiave(chiave),  next(nullptr), peso(peso){}

        const T& get_chiave() const {return chiave;}
        const int& get_peso() const {return peso;}
        Nodo<T>* get_next() const {return next;}

        void set_next(Nodo<T>* n) {next = n;}
        void incrementa_peso() {peso++;}
};

template<typename T> ostream& operator<<(ostream& os, Nodo<T>& n){
    os << n.get_chiave() << ", peso: " << n.get_peso();
    return os;
}

template<typename T> class Pila{
    private:
        Nodo<T>* head;
    public:
        Pila(): head(nullptr){}
        Pila(const T& chiave, const int& peso): head(new Nodo<T>(chiave, peso)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}
        Pila<T>* push(const T& chiave, const int& peso){
            if(isempty()){
                head = new Nodo<T>(chiave, peso);
                return this;
            }
            if(peso % 2 != 0){
                Nodo<T>* tmp = head;
                head = new Nodo<T>(chiave, peso);
                head->set_next(tmp);
                return this;
            }

            Nodo<T>* nuovo = new Nodo<T>(chiave, peso);
            if(head->get_peso() < peso){
                nuovo->set_next(head->get_next());
                head->set_next(nuovo);
                return this;
            }
            Nodo<T>* tmp = head->get_next();
            Nodo<T>* prev = head;
            while(tmp != nullptr && tmp->get_peso() > peso){
                prev = tmp;
                tmp = tmp->get_next();
            }

            if(tmp == nullptr){
                prev->set_next(nuovo);
                return this;
            }
            nuovo->set_next(tmp->get_next());
            tmp->set_next(nuovo);
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

        void operator++(){
            Nodo<T>* tmp = head;
            while(tmp != nullptr){
                tmp->incrementa_peso();
                tmp = tmp->get_next();
            }
        }
        ~Pila(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T> ostream& operator<<(ostream& os, Pila<T>& p){
    if(p.isempty()){
        os << "This shit is empty " << endl;
        return os;
    }

    Nodo<T>* tmp = p.get_head();
    while(tmp != nullptr){
        os << *tmp << endl;
        tmp = tmp->get_next();
    }
    return os;
}

int main(){
    Pila<string>* p = new Pila<string>();
    p->push("Melo", 4);
    cout << *p << endl;

    p->push("Cucci", 5)->push("Tony", 7)->push("Vito", 9);
    cout << *p << endl;

    p->push("Stronzo", 2)->push("Andrea", 6);
    cout << *p << endl;

    ++(*p);
    cout << *p << endl;
}