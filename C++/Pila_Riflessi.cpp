/*Comportamento:
Ogni volta che viene pushato un elemento, la pila può reagire in due modi:

Se è “compatibile” con quello sopra (es. somma pari), allora viene pushato normalmente.

Se è “incompatibile”, allora entrambi vengono rimossi (push fallisce e il precedente viene anche espulso).

Obiettivo:
Pila con logica di “reazione chimica” tra elementi.

Overloading:
Overload dell’operatore == per confrontare due pile e dire se contengono gli stessi elementi nello stesso ordine.*/

#include <iostream>
using namespace std;

template<typename T> class Nodo{
    private:
        T chiave;
        Nodo* next;
        int valore;
    public:
        Nodo(const T& chiave, const int& valore): chiave(chiave), next(nullptr), valore(valore){}

        const T& get_chiave() const {return chiave;}
        const int& get_valore() const {return valore;}
        Nodo<T>* get_next() const {return next;}

        void set_next(Nodo* n){next = n;}
};

template<typename T> class Stack{
    private:
        Nodo<T>* head;
    public:
        Stack():head(nullptr){}
        Stack(const T& chiave, const int& valore): head(new Nodo<T>(chiave, valore)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Stack<T>* push(const T& chiave, const int& valore){
            if(isempty()){
                head = new Nodo<T>(chiave, valore);
                return this;
            }
            if((head->get_valore() + valore) % 2 == 0){
                Nodo<T>* tmp = head;
                head = new Nodo<T>(chiave, valore);
                head->set_next(tmp);
                return this;
            }
            pop();
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

        bool operator==(Stack<T>& s){
            if(s.isempty()){
                if(this->isempty())
                    return true;
                return false;
            }

            Nodo<T>* tmp1 = this->get_head();
            Nodo<T>* tmp2 = s.get_head();
            while(tmp1 != nullptr && tmp2 != nullptr){
                if(tmp1->get_chiave() != tmp2->get_chiave() || tmp1->get_valore() != tmp2->get_valore())
                    return false;
                tmp1 = tmp1->get_next();
                tmp2 = tmp2->get_next();
            }
            return true;
        }

        ~Stack(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T>ostream& operator<<(ostream& os, Stack<T>& s){
    if(s.isempty()){
        os << "Stack vuoto" << endl;
        return os;
    }

    Nodo<T>* tmp = s.get_head();
    while(tmp != nullptr){
        os << tmp->get_chiave() << ", " << tmp->get_valore() << endl;
        tmp = tmp->get_next();
    }
    return os;
}

int main(){
    Stack<string>* s = new Stack<string>();
    Stack<string>* s2 = new Stack<string>();

    s->push("Richi", 6)->push("Melo", 8);
    cout << *s << endl;

    s2->push("Richi", 6)->push("Melo", 1);
    cout << *s2 << endl;

    if((*s) == (*s2)){
        cout << "Sono uguali " << endl;
    }
    else{
        cout << "Non sono uguali" << endl;
    }
    delete s;
    delete s2;
}