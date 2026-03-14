/*Classe Videogioco con nome, genere, prezzo.

Leggi da file e ordina per prezzo.

Filtra tutti i giochi di un certo genere.

Salva i risultati in un altro file.*/

#include <iostream>
#include <string>

using namespace std;

class Videogioco{
    private:
        string nome;
        string genere;
        double prezzo;
    public:
        Videogioco(string nome, string genere, double prezzo): nome(nome), genere(genere), prezzo(prezzo){}
        const string& get_nome() const {return nome;}
        const string& get_genere() const {return genere;}
        const double& get_prezzo() const {return prezzo;}


};

ostream& operator<<(ostream& out, Videogioco v){
    out<<"\""<<v.get_nome()<<"\", "<<v.get_genere()<<", "<<v.get_prezzo()<<endl;
    return out;
}

