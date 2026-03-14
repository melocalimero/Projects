#include <iostream>
using namespace std;

class Priorità{
    private:
        int valore;
    public: 
        Priorità(int valore): valore(valore){}

        virtual int get_valore() const = 0;
        virtual string get_tipologia() const = 0;
        virtual ~Priorità(){}
};

class Alta: public Priorità{
    public:
        Alta(): Priorità(1){}

        int get_valore() const override {return 1;}
        string get_tipologia() const override {return "Alta";}

        ~Alta() override{}
};

class Media: public Priorità{
    public:
        Media(): Priorità(2){}

        int get_valore() const override {return 2;}
        string get_tipologia() const override {return "Media";}

        ~Media() override{}
};

class Bassa: public Priorità{
    public:
        Bassa(): Priorità(3){}

        int get_valore() const override {return 3;}
        string get_tipologia() const override {return "Bassa";}

        ~Bassa() override{}
};

class Ordine{
    private:
        double prezzo;
        Priorità* p;
    public:
        Ordine(double prezzo, Priorità* p):prezzo(prezzo), p(p){}

        const double& get_prezzo() const {return prezzo;}
        Priorità* get_priorità() const {return p;}

        bool operator<(Ordine& o){
            return this->get_priorità()->get_valore() < o.get_priorità()->get_valore();
        }
        void stampa() const {
            cout << this->prezzo << ", priorità: " << this->p->get_valore() << ": " << this->p->get_tipologia() << endl;
        }

        ~Ordine(){
            delete p;
        }
};

template<class T> void merge(T* arr, const size_t& begin, const size_t& mid, const size_t& end){
    //dimensione dei due array (sinistra - destra)
    size_t leftSize = mid - begin + 1;
    size_t rightSize = end - mid;

    //inizializzo i due array e li riempio
    T* leftArray = new T[leftSize];
    T* rightArray = new T[rightSize];

    for(size_t i = 0; i < leftSize; i++){
        leftArray[i] = arr[begin + i];
    }

    for(size_t i = 0; i < rightSize; i++){
        rightArray[i] = arr[mid + 1 + i];
    }

    //inizializzo gli indici
    size_t leftIndex = 0;
    size_t rightIndex = 0;
    size_t arrIndex = begin;

    //Riempio l'array prendendo elementi da destra o sinistra, in base a qual è il più piccolo
    while(leftIndex < leftSize && rightIndex < rightSize){
        if(*(leftArray[leftIndex]) < *(rightArray[rightIndex])){
            arr[arrIndex++] = leftArray[leftIndex++];
        }
        else{
            arr[arrIndex++] = rightArray[rightIndex++];
        }
    }

    while(leftIndex < leftSize){
        arr[arrIndex++] = leftArray[leftIndex++];
    }

    while(rightIndex < rightSize){
        arr[arrIndex++] = rightArray[rightIndex++];
    }

    delete[] leftArray;
    delete[] rightArray;
}

template<class T> void ordina(T* arr, const size_t& begin, const size_t& end){
    if(begin >= end){
        return;
    }

    size_t mid = (begin+end)/2;
    ordina(arr, begin, mid);
    ordina(arr, mid+1, end);
    merge(arr, begin, mid, end);
}

template<class T> void ordina(T* arr, const size_t& dim){
    ordina(arr, 0, dim-1);
}
int main(){
    const size_t dim = 4;
    Ordine** o = new Ordine*[dim];
    
    o[0] = new Ordine(32, new Alta());
    o[1] = new Ordine(23, new Bassa());
    o[2] = new Ordine(43, new Media());
    o[3] = new Ordine(56, new Alta());

    ordina<Ordine*>(o, dim);

    for(size_t i = 0; i < dim; i++){
        o[i]->stampa();
    }

    
}