/*Descrizione:
Un ThinkStack è uno stack in cui ogni elemento rappresenta un "concetto", e contiene una pila di "associazioni".
Quando si add(concept, idea), se il concetto esiste già, si aggiunge idea nella sua pila.
Altrimenti viene creato un nuovo concetto in cima allo stack.
La funzione forget() rimuove l’associazione più recente dal concetto in cima.
 Se quel concetto non ha più associazioni, viene dimenticato (rimosso dal ThinkStack).

Richiesto:
get_concepts(): numero di concetti memorizzati.

get_total_ideas(): numero totale di idee.

Overloading di operator* per stampare i concetti e le loro idee:
Concept -> idea1 | idea2 | ...*/

#include <iostream>
using namespace std;

template<typename T> class Nodo{
    private:
        T chiave;
        Nodo* next;
    public:
        Nodo(const T& chiave):chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        Nodo<T>* get_next() const {return next;}

        void set_next(Nodo<T>* n){next = n;}
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
            Nodo<T>* nuovo = new Nodo<T>(chiave);
            nuovo->set_next(head);
            head = nuovo;
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
        os << "Stack is empty" << endl;
        return os;
    }

    Nodo<T>* tmp = s.get_head();
    while(tmp->get_next() != nullptr){
        os << tmp->get_chiave() << " | ";
        tmp = tmp->get_next();
    }
    os << tmp->get_chiave() << endl;
    return os;
}

template<typename T> class Nodo2{
    private:
        T concetto;
        T idea;
        Stack<T>* s;
        Nodo2<T>* next;
    public:
        Nodo2(const T& concetto, const T& idea): concetto(concetto), idea(idea), s(new Stack<T>(idea)), next(nullptr){}

        const T& get_concetto() const {return concetto;}
        const T& get_idea() const {return idea;}
        Nodo2<T>* get_next() const {return next;}
        Stack<string>* get_stack() const {return s;}

        void set_next(Nodo2<T>* n){next = n;}
};

template<typename T> class StackOfThoughts{
    private:
        Nodo2<T>* head;
        size_t dim;
    public: 
        StackOfThoughts(): head(nullptr), dim(0){}
        StackOfThoughts(const T& concetto, const T& idea): head(new Nodo2<T>(concetto, idea)), dim(1){}

        const size_t& get_dim() const {return dim;}
        Nodo2<T>* get_head() const {return head;}
        const size_t get_ideas(){
            if(isempty()){
                return 0;
            }

            size_t somma = 0;
            Nodo2<T>* tmp = head;
            while(tmp != nullptr){
                somma += tmp->get_stack()->get_size();
                tmp = tmp->get_next();
            }
            return somma;
        }

        bool isempty() const {return head == nullptr;}

        StackOfThoughts<T>* add(const T& concetto, const T& idea){
            if(isempty()){
                head = new Nodo2<T>(concetto, idea);
                dim++;
                return this;
            }

            Nodo2<T>* tmp = head;
            
            while(tmp != nullptr && tmp->get_concetto() != concetto){
                tmp = tmp->get_next();
            }

            if(tmp == nullptr){
                tmp = head;
                head = new Nodo2<T>(concetto, idea);
                head->set_next(tmp);
                dim++;
                return this;
            }

            tmp->get_stack()->push(idea);
            return this;
        }

        StackOfThoughts<T>* forget(){
            if(isempty()){
                return this;
            }

            head->get_stack()->pop();
            if(head->get_stack()->isempty()){
                Nodo2<T>* tmp = head;
                head = head->get_next();
                dim--;
                delete tmp;
            }
            return this;
        }

        ~StackOfThoughts(){
            while(head != nullptr){
                Nodo2<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<typename T> ostream& operator<<(ostream& os, StackOfThoughts<T>& s){
    if(s.isempty()){
        os << "Stack of thoughts is empty" << endl;
        return os;
    }

    Nodo2<T>* tmp = s.get_head();
    while(tmp != nullptr){
        os << tmp->get_concetto() << " -> " << *tmp->get_stack() << endl;
        tmp = tmp->get_next();
    }

    os << "Numero di concetti: " << s.get_dim() << endl;
    os << "Numero di idee: " << s.get_ideas() << endl;
    return os;
}

template<typename T> T* fill_array(StackOfThoughts<T>* s){
    size_t dim = s->get_dim();
    T* arr = new T[dim];

    Nodo2<T>* tmp = s->get_head();
    size_t i = 0;
    while(tmp != nullptr && i < dim){
        arr[i] = tmp->get_concetto();
        i++;
        tmp = tmp->get_next();
    }
    return arr;
}

template<typename T>size_t partition(T* arr, const size_t& begin, const size_t& end){
    size_t p = begin;
    for(size_t i = begin; i <= end; i++){
        if(arr[i] <= arr[end]){
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

    if(p > 0){
        ordina(arr, begin, p-1);
    }
    ordina(arr, p+1, end);
}

template<typename T> void ordina(T* arr, const size_t& dim){
    ordina(arr, 0, dim-1);
}

int main(){
    StackOfThoughts<string>* s = new StackOfThoughts<string>();
    s->add("Pranzo", "Risotto")->add("Pranzo", "Pasta col tonno");
    cout << *s << endl;

    s->add("Colazione", "Manicaretti");
    cout << *s << endl;

    s->forget()->add("Colazione", "Panbauletto")->add("Lavoro", "oind")->add("Università", "Studiare");
    cout << *s << endl;

    string* arr = fill_array<string>(s);
    size_t dim = s->get_dim();
   

    ordina(arr, dim);
    for(size_t i = 0; i < dim; i++){
        cout << arr[i] << " | ";
    }
    cout << endl;
    delete s;
    delete[] arr;
}