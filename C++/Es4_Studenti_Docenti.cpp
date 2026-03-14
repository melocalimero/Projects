/*Classi Persona → Studente, Docente.

Studente ha media, Docente ha materia.

Sovraccarica operator<< per stampa.

Leggi elenco misto da file.

Cerca per nome, ordina studenti per media, docenti per materia.*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

class Persona{
    private:
        string nome;
        string cognome;
        int anno;
    public:
        Persona(string nome, string cognome, int anno):
        nome(nome), cognome(cognome), anno(anno){}

        const string& get_nome() const {return nome;}
        const string& get_cognome() const {return cognome;}
        const int& get_anno() const {return anno;}

        virtual void stampa(){
            cout<<this->nome<<", "<<this->cognome<<", "<<this->anno;
        }
        
};

class Studente: public Persona{
    private:
        double media;
    public:
        Studente(string nome, string cognome, int anno, double media):
        Persona(nome, cognome, anno), media(media){}

        const double& get_media()const {return this->media;}
        void stampa() override{
            Persona::stampa();
            cout<<", "<<this->media<<endl;
        }

        bool operator<(Studente s) {
            return this->media< s.media;
        }
        bool operator>(Studente s){
            return this->media> s.media;
        }
};

class Docente: public Persona{
    private:
        string materia;
    public:
        Docente(string nome, string cognome, int anno, string materia):
        Persona(nome, cognome, anno), materia(materia){}

        const string& get_materia() const {return materia;}

        void stampa() override{
            Persona::stampa();
            cout<<", "<<this->materia<<endl;
        }

        bool operator<(Docente d) {
            return this->materia< d.materia;
        }

        bool operator>(Docente d){
            return this->materia > d.materia;
        }
};

ostream& operator<<(ostream& out, Persona* p){
    p->stampa();
    return out;
}

vector<Persona*> read(string path){
    ifstream file;
    file.open(path);
    string s;
    vector<Persona*> vec;
    if(file.is_open()){
        while(getline(file, s)){
            stringstream ss(s);
            string tipo;
            getline(ss, tipo, ',');
            if(tipo=="Docente"){
                //leggo parola per parola
                string nome, cognome, anno, materia;
                getline(ss, nome, ',');
                getline(ss, cognome, ',');
                getline(ss, anno, ',');
                getline(ss, materia, ',');

                //cancello spazi iniziali
                nome.erase(0, nome.find_first_not_of(" \t"));
                cognome.erase(0, cognome.find_first_not_of(" \t"));
                anno.erase(0, anno.find_first_not_of(" \t"));
                materia.erase(0, materia.find_first_not_of(" \t"));
                int a=stoi(anno);
                vec.push_back(new Docente(nome, cognome, a, materia));
            }
            else if(tipo=="Studente"){
                string nome, cognome, anno, m;
                getline(ss, nome, ',');
                getline(ss, cognome, ',');
                getline(ss, anno, ',');
                getline(ss, m, ',');

                nome.erase(0, nome.find_first_not_of(" \t"));
                cognome.erase(0, cognome.find_first_not_of(" \t"));
                anno.erase(0, anno.find_first_not_of(" \t"));
                m.erase(0, m.find_first_not_of(" \t"));
                int a=stoi(anno);
                double media=stod(m);
                vec.push_back(new Studente(nome, cognome, a, media));
            }
            else{
                cout<<"[ERRORE]: tipo non compatibile"<<endl;
                exit(1);
            }

        }
    }
    return vec;
}

bool ricercaLineare(vector<Persona*> vec, string nome){
    for(auto it=vec.begin(); it!= vec.end(); it++){
        if((*it)->get_nome()== nome)
            return true;
    }
    return false;
}

template<class T> T** split_vector(vector<Persona*> vec){
    T** array=new T* [vec.size()];
    int i=0;
    for(auto it=vec.begin(); it!=vec.end(); it++){
        if(dynamic_cast<T*> (*it)){
            array[i]=dynamic_cast<T*>(*it);
            i++;
        }
    }   
    return array;
}
vector<Persona*> ordina(vector<Persona*> vec){
    Docente** array_docenti= split_vector<Docente>(vec);
    Studente** array_studenti=split_vector<Studente>(vec);


    int flag1=1;
    while(flag1==1){
        flag1=0;
        for(int i=0; i<vec.size()-1; i++){
            if(array_docenti[i]<array_docenti[i+1]){
                Docente* hold=array_docenti[i];
                array_docenti[i]=array_docenti[i+1];
                array_docenti[i+1]=hold;
                flag1=1;
            }
        }    
    }

    vector<Persona*> vec2;
    for(int i=0; array_docenti[i]!=NULL; i++){
        vec2.push_back(new Docente(array_docenti[i]->get_nome(), array_docenti[i]->get_cognome(), array_docenti[i]->get_anno(), array_docenti[i]->get_materia()));
    }

    int flag2=1;
    while(flag2==1){
        flag2=0;
        for(int i=0; i<vec.size()-1; i++){
            if(array_studenti[i]<array_studenti[i+1]){
                Studente* hold=array_studenti[i];
                array_studenti[i]=array_studenti[i+1];
                array_studenti[i+1]=hold;
                flag2=1;
            }
        }    
    }

    for(int i=0; array_studenti[i]!=NULL; i++){
        vec2.push_back(new Studente(array_studenti[i]->get_nome(), array_studenti[i]->get_cognome(), array_studenti[i]->get_anno(), array_studenti[i]->get_media()));
    }

    return vec2;

}
    
int main(){
    string path="input.txt"; 
    vector<Persona*> vec=read(path);

    cout<<"=====Vettore non ordinato======"<<endl;
    for(auto it=vec.begin(); it!= vec.end(); it++){
        cout<<*it;
    }
    cout<<"==============================="<<endl;

    cout<<endl;
    if(ricercaLineare(vec, "Filippo"))
        cout<<"Nome trovato"<<endl;
    else
        cout<<"Nome non trovato"<<endl;

    cout<<endl;
    cout<<"Ordinamento..."<<endl;
    vec=ordina(vec);

    cout<<"=======Vettore ordinato========"<<endl;
    for(auto it=vec.begin(); it!= vec.end(); it++){
        cout<<*it;
    }
    cout<<"==============================="<<endl;
}