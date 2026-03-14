/*Immagina un sistema che gestisce dei lavori da eseguire. Ogni lavoro ha una sua priorità. 
I lavori ad alta priorità dovrebbero essere eseguiti prima di quelli a bassa priorità. 
Useremo una combinazione di code e pile per gestire questa situazione.

Componenti richiesti:

Classe Task:

Ogni task avrà un identificativo (es. una stringa).
Avrà un livello di priorità (es. un intero, dove un valore più basso indica una priorità più alta: 1 = alta, 2 = media, 3 = bassa).
Potrebbe avere una breve descrizione del lavoro da fare.
Un metodo stampa() per visualizzare le informazioni del task.
Strutture dati template:

Una classe Coda template (come quella che hai già implementato).
Una classe Pila template (come quella che hai già implementato).
Classe GestoreLavori:

Avrà diverse code, una per ogni livello di priorità (es. una coda per l'alta priorità, una per la media e una per la bassa).

Avrà un metodo per aggiungere un nuovo Task alla coda appropriata in base alla sua priorità.
Avrà un metodo per "processare" il prossimo task. Questo metodo dovrebbe prima controllare se ci sono task nella coda ad alta priorità.
Se ce ne sono, ne prende uno. Altrimenti, controlla la coda a media priorità, e infine quella a bassa priorità.
Se la coda di alta priorità fosse implementata come una pila, i task più recenti ad alta priorità verrebbero processati prima.
Un metodo per stampare lo stato di tutte le code.
Fasi dell'esercizio:

Implementa la classe Task.
Utilizza le tue classi Coda e Pila template. Potresti decidere se la coda ad alta priorità nel GestoreLavori
debba essere una vera coda (FIFO) o una pila (LIFO) per la gestione delle emergenze più recenti.
Implementa la classe GestoreLavori con le code per le diverse priorità e i metodi per aggiungere task e 
processare il prossimo task in base alla priorità.
Nel main(), crea un'istanza di GestoreLavori. 
Aggiungi diversi task con priorità differenti. 
Simula l'elaborazione di alcuni task e stampa lo stato delle code.
*/

#include <iostream>
using namespace std;
// =======================================CLASSI TASK ============================================
class Priorità{
    private:
        int valore;
    protected:
        Priorità(int valore): valore(valore){}
    public:
        virtual const int get_valore() const = 0;
        virtual ~Priorità(){}
};

class Alta: public Priorità{
    public:
        Alta(): Priorità(1){}

        const int get_valore() const {return 1;}

        ~Alta() override{}
};

class Media: public Priorità{
    public:
        Media(): Priorità(2){}

        const int get_valore() const {return 2;}

        ~Media() override{}
};
class Bassa: public Priorità{
    public:
        Bassa(): Priorità(3){}

        const int get_valore() const {return 3;}

        ~Bassa() override{}
};

class Task{
    private:
        string nome;
        Priorità* p;
    public:
        Task(string nome, Priorità* p): nome(nome), p(p){}

        const string& get_nome() const {return nome;}
        Priorità* get_priorità() const {return p;}

        void stampa() const {
            cout << this->nome << " Priorità: " << p->get_valore();
        }

        ~Task(){
            delete p;
        }
};

ostream& operator<<(ostream& os, Task& t){
    t.stampa();
    return os;
}

//==============================================CLASSI CODA================================================
template<class T> class Nodo{
    private:
        T chiave;
        Nodo* next;
    public: 
        Nodo(const T& chiave): chiave(chiave), next(nullptr){}

        const T& get_chiave() const {return chiave;}
        Nodo* get_next() const {return next;}

        void set_next(Nodo* n) {next = n;}
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

        ~Queue(){
            while(head != nullptr){
                Nodo<T>* tmp = head;
                head = head->get_next();
                delete tmp;
            }
        }
};

template<class T> ostream& operator<<(ostream& os, Queue<T>& q){
    if(q.isempty()){
        os << "Coda vuota" << endl;
        return os;
    }

    Nodo<T>* tmp = q.get_head();
    while(tmp != nullptr){
        os << (*tmp->get_chiave()) << endl;
        tmp =  tmp->get_next();
    }
    return os;
}


//=======================================CLASSI GESTIONE LAVORI===========================================
class GestioneLavori{
    private:
        Queue<Task*>* q_alti;
        Queue<Task*>* q_medi;
        Queue<Task*>* q_bassi;
    public:
        GestioneLavori(): q_alti(new Queue<Task*>()), q_medi(new Queue<Task*>()), q_bassi(new Queue<Task*>()){}

        void aggiungi_task(Task* t){
            if(dynamic_cast<Alta*>(t->get_priorità())){
                q_alti->enqueue(t);
            }
            else if(dynamic_cast<Media*>(t->get_priorità())){
                q_medi->enqueue(t);
            }
            else if(dynamic_cast<Bassa*>(t->get_priorità())){
                q_bassi->enqueue(t);
            }
        }

        void process(){
            while(!(q_alti->isempty())){
                Nodo<Task*>* tmp = q_alti->get_head();
                cout << "Processing " << (*tmp->get_chiave()) << " ..." << endl;
                q_alti->dequeue();
            }
            while(!(q_medi->isempty())){
                Nodo<Task*>* tmp = q_medi->get_head();
                cout << "Processing " << (*tmp->get_chiave()) << " ..." << endl;
                q_medi->dequeue();
            }
            while(!(q_bassi->isempty())){
                Nodo<Task*>* tmp = q_bassi->get_head();
                cout << "Processing " << (*tmp->get_chiave()) << " ..." << endl;
                q_bassi->dequeue();
            }
        }

        void stampa() const {
            cout << "Coda Priorità Alta: " << endl;
            cout << (*q_alti) << endl;
            cout << "Coda Priorità Media: " << endl;
            cout << (*q_medi) << endl;
            cout << "Coda Priorità Bassa: " << endl;
            cout << (*q_bassi) << endl;
        }

        ~GestioneLavori() {
            if(q_alti)
                delete q_alti;
            if(q_medi)
                delete q_medi;
            if(q_bassi)
                delete q_bassi;
        }
};

ostream& operator<<(ostream& os, GestioneLavori& g){
    g.stampa();
    return os;
}

int main(){
    const size_t dim = 5;
    Task** arr = new Task*[dim];

    arr[0] = new Task("Lavori", new Alta());
    arr[1] = new Task("Prog 2", new Alta());
    arr[2] = new Task("Analisi 1", new Media());
    arr[3] = new Task("Fondamenti", new Media());
    arr[4] = new Task("Architettura", new Bassa());

    GestioneLavori* g = new GestioneLavori();

    for(size_t i = 0; i < dim; i++){
        g->aggiungi_task(arr[i]);
    } 

    cout << *g << endl;

    g->process();
    cout << *g << endl;

    delete g;
    for(size_t i = 0; i < dim; i++){
        delete arr[i];
    }

    delete[] arr;
}