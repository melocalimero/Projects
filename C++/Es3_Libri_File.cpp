/*Crea una classe Libro con titolo, autore, anno.

Leggi una lista di libri da un file di testo.

Implementa una funzione che cerca un libro per titolo.

Scrivi su un altro file i libri trovati.*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Libro{
    private:
        string titolo;
        string autore;
        int anno;
    public:
        Libro(string titolo, string autore, int anno): titolo(titolo), autore(autore), anno(anno){}

        const string& get_titolo() const {return titolo;}
        const string& get_autore() const {return autore;}
        const int& get_anno() const {return anno;}

        bool operator==(string titolo){
            return this->titolo==titolo;
        }
};
ostream& operator<<(ostream& out,Libro l){
    out<<"\""<<l.get_titolo()<<"\", "<<l.get_autore()<<", "<<l.get_anno()<<endl;
    return out;
} 
vector<Libro> read(string path);
bool ricercaLineare(vector<Libro> vec, string titolo);
void write(string path, vector<Libro>);

int main(){
    string path="input.txt";
    vector<Libro> vec= read(path);
    for(auto it=vec.begin(); it!=vec.end(); it++){
        cout<<*it<<endl;
    }
    if(ricercaLineare(vec, "L'ombra del vento"))
        cout<<"Il libro cercato è stato gia inserito"<<endl;
    else
        cout<<"Il libro non è all'interno della lista"<<endl;
    string path_output="output.txt";
    write(path_output, vec);
}

vector<Libro> read(string path){
    ifstream file;
    file.open(path);
    string s;
    vector<Libro> vec;
    if(file.is_open()){
        while(getline(file,s)){
            stringstream ss(s);
            string anno;
            string titolo;
            string autore;
            getline(ss, titolo, ',');
            getline(ss, autore, ',');
            getline(ss, anno, ',');
            int a=stoi(anno);
            vec.push_back(Libro(titolo, autore, a));
        }
    }
    file.close();
    return vec;
}

bool ricercaLineare(vector<Libro> vec, string titolo){
    for(auto it=vec.begin(); it!=vec.end(); it++){
        if((*it)==titolo){
            return true;
        }
    }
    return false;
}

void write(string path, vector<Libro> vec){
    ofstream file;
    file.open(path);
    if(file.is_open()){
        for(auto it=vec.begin(); it!=vec.end(); it++){
            file<<*it;
        }
    }
    file.close();
}