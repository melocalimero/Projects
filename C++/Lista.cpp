#include <iostream>
using namespace std;

class Nodo{
    private:
        int i;
        Nodo* next;
    public:
        Nodo(int i): i(i), next(nullptr){}

        const int& get_i() const {return i;}
        Nodo* get_next() const {return next;}

        void set_next(Nodo* next) {this->next = next;}
};

class Lista{
    private:
        Nodo* head;
    public:
        Lista(): head(nullptr){}
        Lista(int i): head(new Nodo(i)){}

        Nodo* get_head() const {return head;}

        bool isempty() const {return head == nullptr;}

        Lista* inserimento_testa(int i){
            Nodo* nuovo = head;
            head = new Nodo(i);
            head->set_next(nuovo);
            return this;
        }

        Lista* inserimento_coda(int i){
            if(isempty()){
                head = new Nodo(i);
                return this;
            }

            Nodo* tmp = head;
            while(tmp->get_next() != nullptr){
                tmp = tmp->get_next();
            }

            tmp->set_next(new Nodo(i));
            return this;
        }

        Lista* inserimento_ordinato(int i){
            if(isempty()){
                head = new Nodo(i);
                return this;
            }

            if(i < head->get_i()){
                Nodo* tmp = head;
                head = new Nodo(i);
                head->set_next(tmp);
                return this;
            }

            Nodo* tmp = head;
            Nodo* prev = nullptr;
            while(tmp != nullptr && tmp->get_i() < i){
                prev = tmp;
                tmp = tmp->get_next();
            }

            if(tmp == nullptr){
                prev->set_next(new Nodo(i));
                return this;   
            }

            Nodo* nuovo = new Nodo(i);
            nuovo->set_next(tmp);
            prev->set_next(nuovo);
            return this;
        }

        Lista* eliminazione_testa(){
            if(isempty()){
                cout << "Lista vuota" << endl;
                return this;
            }

            Nodo* tmp = head;
            head = head->get_next();
            tmp->set_next(nullptr);
            delete tmp;
            return this;
        }

        Lista* elimina_elemento(int i){
            if(isempty()){
                cout << "Lista vuota" << endl;
                return this;
            }

            if(head->get_i() == i){
                Nodo* tmp = head;
                head = head->get_next();
                tmp->set_next(nullptr);
                delete tmp;
                return this;
            }

            Nodo* tmp = head;
            Nodo* prev = nullptr;

            while(tmp != nullptr && tmp->get_i() != i){
                prev = tmp;
                tmp = tmp->get_next();
            }

            if(tmp == nullptr){
                return this;
            }

            Nodo* daeliminare = tmp;
            prev->set_next(tmp->get_next());
            daeliminare->set_next(nullptr);
            delete daeliminare;
            return this;
        }

        Lista* eliminazione_coda(){
            if(isempty()){
                return this;
            }

            Nodo* tmp = head;
            Nodo* prev = nullptr;
            while(tmp->get_next() != nullptr){
                prev = tmp;
                tmp = tmp->get_next();
            }

            prev->set_next(nullptr);
            delete tmp;
        }

        void stampa() const  {
            if(isempty()){
                cout << "Lista vuota" << endl;
                return;
            }

            Nodo* tmp = head;
            while(tmp != nullptr){
                cout << tmp->get_i() << " ";
                tmp = tmp->get_next();
            }
        }
};

int main(){
    Lista* l = new Lista();
    l->inserimento_testa(1)->inserimento_testa(2)->inserimento_coda(8)->inserimento_ordinato(9);
    l->stampa();

    cout << endl;
    l->elimina_elemento(2);
    l->stampa();
}