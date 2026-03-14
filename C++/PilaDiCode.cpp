/*Pila di code
Implementa Stack<Queue<char>>.

Funzione: enqueue_top_queue(char) che aggiunge un elemento alla coda in cima alla pila.*/

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

        void set_next(Nodo* n) {next = n;}
};

template<typename T> class Queue{
    private:
        Nodo<T>* head;
        Nodo<T>* tail;
    public:
        Queue(): head(nullptr), tail(nullptr){}
        Queue(const T& chiave): head(new Nodo<T>(chiave)), tail(head){}

        Nodo<T>* get_head() const {return head;}
        Nodo<T>* get_tail() const {return tail;}

        bool isempty() const {return head == nullptr;}

        Queue<T>* enqueue(const T& chiave){
            if(isempty()){
                head = new Nodo<T>(chiave);
                tail = head;
                return this;
            }

            tail->set_next(new Nodo<T>(chiave));
            tail = tail->get_next();
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
        os << "Coda vuota " << endl;
        return os;
    }

    Nodo<T>* tmp = q.get_head();
    while(tmp != nullptr){
        os << tmp->get_chiave() << ", ";
        tmp = tmp->get_next();
    }
    return os;
}

template<class T>class StackNode{
    private:
        T chiave;
        StackNode* next;
    public:
        StackNode(const T& chiave): chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        StackNode<T>* get_next() const {return next;}

        void set_next(StackNode* n){next = n;}
};

class Stack{
    private:
        StackNode<Queue<int>*>* head;
        size_t size;
    public:
        Stack(): head(nullptr), size(0){}
        Stack(Queue<int>* chiave): head(new StackNode<Queue<int>*>(chiave)), size(1){}

        StackNode<Queue<int>*>* get_head() const {return head;}
        const size_t& get_size() const {return size;}

        bool isempty() const {return head == nullptr;}

        Stack* push(Queue<int>* chiave){
            StackNode<Queue<int>*>* tmp = head;
            head = new StackNode<Queue<int>*>(chiave);
            head->set_next(tmp);
            size++;
            return this;
        }

        Stack* pop(){
            if(isempty()){
                return this;
            }

            StackNode<Queue<int>*>* tmp = head;
            head = head->get_next();
            size--;
            delete tmp;
            return this;
        }

        Stack* enqueue_on_top_queue(int chiave){
            if(isempty()){
                return this;
            }
            head->get_chiave()->enqueue(chiave);
            return this;
        }

        Stack* enqueue_on_specific_queue(int chiave, size_t q){
            if(q < 1 || q > size){
                cout << "Q is not valid" << endl;
                return this;
            }

            if(q == 1){
                enqueue_on_top_queue(chiave);
                return this;
            }
            StackNode<Queue<int>*>* tmp = head->get_next();
            size_t i = 2;
            while(tmp != nullptr && i < q){
                tmp = tmp->get_next(); 
                i++;
            }

            tmp->get_chiave()->enqueue(chiave);
            return this;
        }

        Stack* dequeue_on_top_queue(){
            if(isempty()){
                return this;
            }
            head->get_chiave()->dequeue();
            return this;
        }

        ~Stack(){
            while(head != nullptr){
                StackNode<Queue<int>*>* tmp = head;
                head = head->get_next();
                delete tmp->get_chiave();
                delete tmp;
            }
        }
};


ostream& operator<<(ostream& os, Stack& s){
    if(s.isempty()){
        os << "Coda vuota " << endl;
        return os;
    }

    StackNode<Queue<int>*>* tmp = s.get_head();
    size_t i = 1;
    while(tmp != nullptr){
        cout << i << "° coda: ";
        os << (*tmp->get_chiave()) << endl;
        tmp = tmp->get_next();
        i++;
    }

    os << "Size: " << s.get_size() << endl;
    return os;
}

int main(){
    Stack* s = new Stack();
    s->push(new Queue<int>(3))->enqueue_on_top_queue(3);
    s->push(new Queue<int>(5))->push(new Queue<int>(8))->push(new Queue<int>(10));
    cout << *s << endl;

    s->enqueue_on_specific_queue(100, 5);
    cout << *s << endl;

    delete s;
}