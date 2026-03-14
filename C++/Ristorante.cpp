/*Immagina di essere stato incaricato di progettare il sistema per un ristorante che gestisce gli ordini dei clienti. 
Ogni ordine può contenere diversi piatti e ogni piatto ha un prezzo e una categoria (ad esempio: antipasto, primo, secondo, dessert). 
Il ristorante deve anche gestire un sistema di priorità: gli ordini possono essere urgenti, standard, o ritardabili.*/

#include <iostream>
using namespace std;

class Piatto{
    private:
        string nome;
        double prezzo;
    protected:
        Piatto(string nome, double prezzo): nome(nome), prezzo(prezzo){}
    public: 
        virtual const string& get_nome() const {return nome;}
        virtual double get_prezzo() const = 0;
        virtual string get_categoria() const = 0;
        virtual ~Piatto(){}
};

class Antipasto: public Piatto{
    public:
        Antipasto(string nome): Piatto(nome, 12.5){}
        double get_prezzo() const override {return 12.5;}
        string get_categoria() const override {return "Antipasto";}
        ~Antipasto(){}
};

class Primo: public Piatto{
    public:
        Primo(string nome): Piatto(nome, 20){}
        double get_prezzo() const override {return 20;}
        string get_categoria() const override {return "Primo";}
        ~Primo(){}
};

class Secondo: public Piatto{
    public:
        Secondo(string nome): Piatto(nome, 15.4){}
        double get_prezzo() const override {return 15.4;}
        string get_categoria() const override {return "Secondo";}
        ~Secondo(){}
};

class Dessert: public Piatto{
    public:
        Dessert(string nome): Piatto(nome, 23){}
        double get_prezzo() const override {return 23;}
        string get_categoria() const override {return "Dessert";}
        ~Dessert(){}
};
class Ordine{
    protected:
        int priorità;
        int n_ordine;
        Piatto** p;
        size_t n_piatti;
        size_t dim;
        double prezzo_tot;
    public:
        Ordine(int priorità, int n_ordine):priorità(priorità) ,n_ordine(n_ordine), n_piatti(0), dim(10), prezzo_tot(0){
            p = new Piatto*[dim];
            for(size_t i = 0; i < dim; i++){
                p[i] = nullptr;
            }
        }
        virtual const int get_priorità() const = 0;
        virtual void aggiungi_piatto(Piatto* piatto){
            if(n_piatti < dim){
                n_piatti++;
                p[n_piatti-1] = piatto;
                prezzo_tot+=piatto->get_prezzo();
            }
            else{
                return;
            }
        }
        virtual double get_prezzo_tot() = 0;

        virtual bool operator<=(Ordine* o){
            if(this->get_priorità() < o->get_priorità()){
                return true;
            }
            if(this->get_priorità() > o->get_priorità()){
                return false;
            }
            return this->get_prezzo_tot() > o->get_prezzo_tot();
        }

        virtual void stampa(){};
        
        virtual ~Ordine(){
            for(size_t i = 0; i < dim; i++){
                delete p[i];
            }
            delete[] p;
        }
};

class Urgente: public Ordine{
    public:
        Urgente(int n_ordine): Ordine(1, n_ordine){}

        double get_prezzo_tot() override {return this->prezzo_tot + 10;}

        void aggiungi_piatto(Piatto* piatto) override{
            Ordine::aggiungi_piatto(piatto);
        }
        const int get_priorità() const override {return 1;}

        bool operator<=(Ordine* o) override {
            return Ordine::operator<=(o);
        }
        void stampa() override{
            cout << "Ordine numero: " << this->n_ordine <<endl;
            if(n_piatti > 0){
                for(size_t i = 0; i < n_piatti; i++){
                    cout << p[i]->get_categoria()  << ": " << p[i]->get_nome() << ", " << this->p[i]->get_prezzo() << "€; ";
                }
                cout << "Prezzo totale: " <<this->get_prezzo_tot() << "€ Tipologia: " << this->get_priorità() <<  ", urgente " << endl;
            }
            else{
                cout << "Ancora nessun piatto " << endl;
            }
        }
        ~Urgente() override {}
};

class Standard: public Ordine{
    public:
        Standard(int n_ordine): Ordine(2, n_ordine){}
        double get_prezzo_tot() override {return this->prezzo_tot;}
        void aggiungi_piatto(Piatto* piatto) override{
            Ordine::aggiungi_piatto(piatto);
        }
        const int get_priorità() const override {return 2;}
        bool operator<=(Ordine* o) override {
            return Ordine::operator<=(o);
        }
        void stampa() override{
            cout << "Ordine numero: " << this->n_ordine <<endl;
            if(n_piatti > 0){
                for(size_t i = 0; i < n_piatti; i++){
                    cout << p[i]->get_categoria()  << ": " << p[i]->get_nome() << ", " << this->p[i]->get_prezzo() << "€; ";
                }
                cout << "Prezzo totale: " <<this->get_prezzo_tot() << " Tipologia: Standard" << endl;
            }
            else{
                cout << "Ancora nessun piatto " << endl;
            }
        }
        ~Standard() override {}
};

class Ritardabile: public Ordine{
    public:
        Ritardabile(int n_ordine): Ordine(3, n_ordine){}
        double get_prezzo_tot() override {return this->prezzo_tot - 5;}
        void aggiungi_piatto(Piatto* piatto) override{
            Ordine::aggiungi_piatto(piatto);
        }

        const int get_priorità() const override {return 3;}
        bool operator<=(Ordine* o) override {
            return Ordine::operator<=(o);
        }

        void stampa() override{
            cout << "Ordine numero: " << this->n_ordine <<endl;
            if(n_piatti > 0){
                for(size_t i = 0; i < n_piatti; i++){
                    cout << p[i]->get_categoria()  << ": " << p[i]->get_nome() << ", " << this->p[i]->get_prezzo() << "€; ";
                }
                cout << "Prezzo totale: " <<this->get_prezzo_tot() << " Tipologia: Ritardabile" << endl;
            }
            else{
                cout << "Ancora nessun piatto " << endl;
            }
        }
        ~Ritardabile() override {}
};

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

template<class T> class Queue{
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

            Nodo<T>* tmp = tail;
            tail = new Nodo<T>(chiave);
            tmp->set_next(tail);
            return this;
        }

        Queue<T>* dequeue(){
            if(isempty())
                return this;
            
            Nodo<T>* tmp = head;
            head = head->get_next();
            delete tmp;
            return this;
        }

        friend class Ristorante;
        ~Queue(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};
ostream& operator<<(ostream& os, Ordine& o){
    o.stampa();
    return os;
}

template<class T> ostream& operator<<(ostream& os, Queue<T>& q){
    if(q.isempty()){
        os << "This shit is empty" << endl;
        return os;
    }

    Nodo<T>* tmp = q.get_head();
    while(tmp != nullptr){
        os << (*tmp->get_chiave()) << endl;
        tmp = tmp->get_next();
    }
    return os;
}

class Ristorante{
    private:
        Queue<Ordine*>* q;
    public:
        Ristorante(): q(new Queue<Ordine*>()){}

        Queue<Ordine*>* get_queue() const {return q;}

        void aggiungi_Ordine(Ordine* c){
            q->enqueue(c);
        }

        void elimina_ordine(){
            q->dequeue();
        }

        void stampa() const {
            cout << *q;
        }

        ~Ristorante(){
            delete q;
        }
};

ostream& operator<<(ostream& os, Ristorante& r){
    r.stampa();
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
    if(begin >= end)
        return;
    
    size_t p = partition(arr, begin, end);
    if(p > 0){
        ordina(arr, begin, p-1);
    }
    ordina(arr, p+1, end);
}

template<class T>  void ordina(T* arr, const size_t& dim){
    ordina(arr, 0, dim-1);
}
int main(){
    size_t dim_ordine = 5;
    Ordine** o = new Ordine*[dim_ordine];

    o[0] = new Urgente(1);
    o[1] = new Urgente(2);
    o[2] = new Standard(3);
    o[3] = new Standard(4);
    o[4] = new Ritardabile(5);

    o[0]->aggiungi_piatto(new Antipasto("Cazzi"));
    o[0]->aggiungi_piatto(new Primo("Pasta"));

    o[1]->aggiungi_piatto(new Secondo("Carne"));
    o[1]->aggiungi_piatto(new Antipasto("Tortini"));
    o[1]->aggiungi_piatto(new Dessert("Torta"));

    o[2]->aggiungi_piatto(new Primo("Pastina in brodo"));

    o[3]->aggiungi_piatto(new Secondo("Vegetali"));

    o[4]->aggiungi_piatto(new Antipasto("Porchetta"));

    o[4]->aggiungi_piatto(new Primo("Ravioli"));

    Ristorante* r = new Ristorante();
    ordina<Ordine*>(o, dim_ordine);

    for(size_t i = 0; i < dim_ordine; i++){
        r->aggiungi_Ordine(o[i]);
    }

    cout << "====Coda====" << endl;
    cout << *r ;
    cout << "===========================================" << endl;

    for(size_t i = 0; i < dim_ordine; i++){
        delete o[i];
    }
    delete[] o;
    delete r;

}