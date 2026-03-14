/*Contesto:
Si implementi una classe Film con attributi privati:
titolo, regista, anno, rating.

Si costruisca una lista doppiamente concatenata per la gestione di una programmazione settimanale del cinema.

Dal file film.txt (formato: titolo regista anno rating), inserire 7 film nella lista, ordinandoli in base all’anno.

Obiettivi:
Permettere la rimozione dei film con rating inferiore a 7.
Consentire lo scorrimento in avanti e indietro della lista.
Stampare l’elenco finale dei film in entrambi i versi.*/


#include <iostream>
#include <fstream>
using namespace std;

class Film{
    private:   
        string titolo;
        string regista;
        int anno;
        double rating;
    public:
        Film(string titolo, string regista, int anno, double rating):
        titolo(titolo), regista(regista), anno(anno), rating(rating){}
        

        const string& get_titolo() const {return titolo;}
        const string& get_regista() const {return regista;}
        const int& get_anno() const {return anno;}
        const double& get_rating() const {return rating;}

        bool operator<(Film* f2){
            return this->anno < f2->anno; 
        }
        bool operator==(Film* f2){
            return this->anno == f2->anno;
        }

        bool operator!=(const Film* f){
            return this->anno != f->anno;
        }
        void stampa() const {
            cout<<"\""<<this->titolo<<"\" "<<this->regista<<" "<<this->anno<<" "<<this->rating;
        }
};



ostream& operator<<(ostream& os, Film* f){
    f->stampa();
    return os;
}

template <class T> class Nodo{
    private:
        T chiave;
        Nodo* next;
        Nodo* prev;
    public:
        Nodo(const T& chiave): chiave(chiave), next(nullptr), prev(nullptr){}

        const T& get_chiave() const {return chiave;}
        Nodo<T>* get_next() const {return next;}
        Nodo<T>* get_prev() const {return prev;}

        void set_next(Nodo<T>* n) {this->next = n;}
        void set_prev(Nodo<T>* p) {this->prev = p;}
};

template<class T> class Lista{
    private:
        Nodo<T>* head;
    public:
        Lista(): head(nullptr){}
        Lista(const T& chiave): head(new Nodo<T>(chiave)){}

        Nodo<T>* get_head() const {return head;}

        bool isempty() {return head == nullptr;}

        Lista<T>* inserimento_ordinato(const T& chiave){
            if(isempty()){
                head=new Nodo<T>(chiave);
                return this;
            }

            Nodo<T>* nuovo=new Nodo<T>(chiave);
            if(*(nuovo->get_chiave()) < (head->get_chiave())){
                Nodo<T>* tmp = head;
                head = nuovo;
                head->set_next(tmp);
                tmp->set_prev(head);
                return this;
            }

            Nodo<T>* tmp = head;
            Nodo<T>* prev = nullptr;
            while(tmp!=nullptr && *(tmp->get_chiave()) < (nuovo->get_chiave())){
                prev = tmp;
                tmp = tmp->get_next();
            }
            if(tmp == nullptr){
                nuovo->set_prev(prev);
                prev->set_next(nuovo);
                return this;
            }
            nuovo->set_next(tmp);
            nuovo->set_prev(prev);
            tmp->set_prev(nuovo);
            prev->set_next(nuovo);
            return this;
        }

        Lista<T>* rimuovi_chiave(T chiave) {
            if(isempty()){
                return this;
            }

            if(*(head->get_chiave()) == chiave){
                Nodo<T>* tmp = head;
                head = head->get_next();
                head->set_prev(nullptr);
                delete tmp;
                return this;
            }

            Nodo<T>* tmp = head;
            Nodo<T>* prev = nullptr;
            while(tmp != nullptr && *(tmp->get_chiave()) != chiave){
                prev = tmp;
                tmp = tmp->get_next();
            }

            if(tmp != nullptr){
                prev->set_next(tmp->get_next());
                if(tmp->get_next() != nullptr)
                    tmp->get_next()->set_prev(prev);
                delete tmp;
                return this;
            }
            delete tmp;
            return this;
        }
        ~Lista(){
            Nodo<T>* tmp = head;
            while(tmp != nullptr){
                Nodo<T>* todel = tmp;
                tmp = tmp->get_next();
                delete todel->get_chiave();
                delete todel;
            } 
        }
    };

template<class T> ostream& operator<<(ostream& os, Lista<T>& l){
    if(l.isempty()){
        os<<"Lista vuota"<<endl;
        return os;
    }
    os<<"========================="<<endl;
    os<<"ANDATA: "<<endl;
    Nodo<T>* tmp = l.get_head();
    while(tmp->get_next()!=nullptr){
        os<<tmp->get_chiave()<<endl;
        tmp = tmp->get_next();
    }
    os<<tmp->get_chiave()<<endl;
    os<<"RITORNO: "<<endl;
    while (tmp != nullptr){
        os << tmp->get_chiave() << endl;
        tmp=tmp->get_prev();
    }
    os<<"========================"<<endl;
    return os;  
}

Lista<Film*>* read(string path){
    ifstream file;
    file.open(path);
    Lista<Film*>* lista = new Lista<Film*>();
    if(file.is_open()){
        string titolo, regista;
        int anno;
        double rating;
        while(file >> titolo >> regista >> anno >> rating){
            lista->inserimento_ordinato(new Film(titolo, regista, anno, rating));
            
        }
    }
    else{
        cout<<"[ERROR]: can't open the file"<<endl;
        exit(1);
    }
    return lista;
}

Lista<Film*>* filter(Lista<Film*>* l, double rating){
    Nodo<Film*>* tmp = l->get_head();
    while(tmp != nullptr){
        if(tmp->get_chiave()->get_rating() < rating){
            Nodo<Film*>* todel = tmp;
            tmp = tmp->get_next();
            l->rimuovi_chiave(todel->get_chiave());
        }
        else
            tmp = tmp->get_next();
    }
    return l;
}

int main(){
    string path="film.txt";
    Lista<Film*>* lista = read(path);
    cout<<*lista<<endl;

    lista = filter(lista, 7);
    cout<<*lista<<endl;

    delete lista;
    
}