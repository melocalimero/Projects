/*Uno StackFrequenzaLimitata è uno stack dove ogni elemento in posizione i è a sua volta uno stack contenente chiavi duplicate.
Una chiave key può essere inserita fino a un massimo di K volte in tutta la struttura.
Quando si effettua un’operazione di tipo push(key), la chiave key viene inserita nel sotto-stack corrispondente,
se non ha già raggiunto il limite di frequenza globale.
Quando si effettua un’operazione di tipo pop(), viene rimossa la chiave più recente dallo stack in testa. 
Se un sotto-stack diventa vuoto, esso viene rimosso.

Implementare lo StackFrequenzaLimitata garantendo le funzionalità sopra descritte ed implementare:

il metodo get_stack_count(), che restituisce il numero di sotto-stack presenti;

il metodo get_total_keys(), che restituisce il numero totale di chiavi nella struttura;

l’overloading dell’operatore * in modo tale che stampi, partendo dalla testa, tutti i sotto-stack, 
ciascuno nella forma *key -> { key, key, ... }.
*/

#include <iostream>
using namespace std;

template<typename T> class Nodo{
    private:
        T chiave;
        Nodo* next;
    public:
        Nodo(const T& chiave): chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        Nodo* get_next() const {return next;}

        void set_next(Nodo* n){next = n;}
};

template<typename T> class Stack{
    private:
        Nodo<T>* head;
        size_t size;
    public:
        Stack(): head(nullptr), size(0){}
        Stack(const T& chiave): head(new Nodo<T>(chiave)), size(1){}

        const size_t& get_size() const {return size;}
        Nodo<T>* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Stack<T>* push(const T& chiave){
            Nodo<T>* tmp = head;
            head = new Nodo<T>(chiave);
            head->set_next(tmp);
            size++;
            return this;
        }

        Stack<T>* pop(){
            if(isempty()){
                return this;
            }

            Nodo<T>* tmp = head;
            head = head->get_next();
            size--;
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
        os << "Stack vuoto" << endl;
        return os;
    }

    Nodo<T>* tmp = s.get_head();
    os << "{" ;
    while(tmp->get_next() != nullptr){
        os << tmp->get_chiave() << ", ";
        tmp = tmp->get_next();
    }
    os << tmp->get_chiave() << "}";
    return os;
}

template<typename T>class Nodo2{
    private:
        Stack<T>* key;
        Nodo2<T>* next;
    public:
        Nodo2(const T& chiave): key(new Stack<T>(chiave)), next(nullptr){}

        Stack<T>* get_key() const {return key;}
        Nodo2<T>* get_next() const {return next;}

        void set_next(Nodo2<T>* n){next = n;}

        ~Nodo2(){
            delete key;
        }
};

template<typename T>class StackFrequenza{
    private:
        Nodo2<T>* head;
        size_t freq;
        size_t dim;
    public:
        StackFrequenza(const size_t& freq): head(nullptr), freq(freq), dim(0){}
        StackFrequenza(const T& chiave, const size_t& freq): head(new Nodo2<T>(chiave)), freq(freq), dim(1){}

        const size_t& get_frequenza() const {return freq;}
        const size_t& get_dim() const {return dim;}
        Nodo2<T>* get_head() const {return head;}

        const size_t get_size(){
            if(isempty()){
                return 0;
            }

            size_t somma = 0;
            Nodo2<T>* tmp = head;
            while(tmp != nullptr){
                somma += tmp->get_key()->get_size();
                tmp = tmp->get_next();
            }
            return somma;
        }

        bool isempty() const {return head == nullptr;}

        StackFrequenza<T>* push(const T& chiave){
            if(isempty()){
                head = new Nodo2<T>(chiave);
                dim++;
                return this;
            }

            Nodo2<T>* tmp = head;
            while(tmp != nullptr && tmp->get_key()->get_head()->get_chiave() != chiave){
                tmp = tmp->get_next();
            }

            if(tmp == nullptr){
                tmp = head;
                head = new Nodo2<T>(chiave);
                head->set_next(tmp);
                dim++;
                return this;
            }
            if(tmp->get_key()->get_size() >= freq){
                return this;
            }
            tmp->get_key()->push(chiave);
            return this;
        }

        StackFrequenza<T>* pop(){
            if(isempty()){
                return this;
            }

            head->get_key()->pop();
            if(head->get_key()->isempty()){
                Nodo2<T>* tmp = head;
                head = head->get_next();
                dim--;
                delete tmp;
            }
            return this;
        }

        void operator*(){
            if(isempty()){
                cout << "Stack vuoto" << endl;
                return;
            }

            Nodo2<T>* tmp = head;
            while(tmp != nullptr){
                cout << endl << tmp->get_key()->get_head()->get_chiave() << "->" << *tmp->get_key();
                if(tmp->get_key()->get_size() >= freq){
                    cout << " Frequenza Raggiunta";
                }
                cout << endl;
                tmp = tmp->get_next();
            }
            cout << endl;
            cout << "Frequenza globale: " << this->freq << endl;
            cout << "Numeri sottoStack: " << this->dim << endl;
            cout << "Numero chiavi: " << this->get_size() << endl;
        }

        ~StackFrequenza(){
            while(head != nullptr){
                Nodo2<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

int main(){
    StackFrequenza<string> s(7);
    s.push("Melo")->push("Melo")->push("Giovanni");
    s.push("Andrea")->push("Andrea")->push("Andrea");
    s.push("Andrea")->push("Giovanni")->push("Andrea")->push("Andrea")->push("Andrea")->push("Andrea");
    *(s);

    s.pop()->pop()->pop();
    *s;

    ;
}