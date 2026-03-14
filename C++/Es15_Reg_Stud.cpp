/*Descrizione: Gestire un registro dove ogni studente ha:
Nome
Cognome
Voto in Matematica
Voto in Informatica
Voto in Inglese

Funzionalità:
Calcolare la media voti per ogni studente
Trovare lo studente con la media più alta
Ordinare gli studenti in base alla media (decrescente)
Stampare gli studenti con almeno una insufficienza (<6)

Spunti:
Algoritmi: ordinamento su struct/vettori
Nessuna collezione interna o relazioni
*/

#include <iostream>
using namespace std;

class Studente{
    private:
        string nome;
        string cognome;
        float voto_mate;
        float voto_inf;
        float voto_ingl;
    public:
        Studente(){
            nome="";
            cognome="";
            voto_mate=0;
            voto_inf=0;
            voto_ingl=0;
        }

        Studente(string nome, string cognome, float voto_mate, float voto_inf, float voto_ingl):
        nome(nome), cognome(cognome), voto_mate(voto_mate), voto_inf(voto_inf), voto_ingl(voto_ingl){}

        const string& get_nome() const {return nome;}
        const string& get_cognome() const {return cognome;}
        const float& get_voto_mate() const {return voto_mate;}
        const float& get_voto_inf() const {return voto_inf;}
        const float& get_voto_ingl() const {return voto_ingl;}

        float media(){
            float somma=0;
            somma=voto_mate+voto_inf+voto_ingl;
            return (somma/3);
        }

        void stampa(){
            cout<<"Nome: ["<<this->nome<<"], Cognome: ["<<this->cognome<<"], Media :["<<this->media()<<"], Voti:"<<endl; 
            cout<<"Matematica: "<<this->voto_mate<<endl<<"Informatica: "<<this->voto_inf<<endl<<"Inglese: "<<this->voto_ingl<<endl;
        }
};

class Scuola{
    private:
        string nome;
        Studente** array;
        int n_studenti;
        int dim_max;
    public:
        Scuola(string nome): nome(nome){
            n_studenti=0;
            dim_max=100;
            array=new Studente* [dim_max];
        }

        void aggiungi_Studente(Studente* s){
            if(n_studenti<dim_max){
                n_studenti++;
                array[n_studenti-1]=s;
            }
            else{
                cout<<"[ERRORE]: Registro pieno"<<endl;
                exit(1);
            }
        }

        void calcolaMedia(){
            if(n_studenti>0){
                for(int i=0; i<n_studenti; i++){
                    cout<<"Media di "<<array[i]->get_nome()<<" "<<array[i]->get_cognome()<<": "<<array[i]->media()<<endl;
                }
            }
        }

        void selectionSort(){
            for(int j=0; j<n_studenti-1; j++){
                int indice_max=j;
                for(int i=j+1; i<n_studenti; i++){
                    if(array[i]->media()>array[indice_max]->media()){
                        indice_max=i;
                    }
                }
                if(indice_max!=j){
                    Studente* hold=array[indice_max];
                    array[indice_max]=array[j];
                    array[j]=hold;
                }
            }
        }

        void stampa(){
            if(n_studenti>0){
                for(int i=0; i<n_studenti; i++){
                    array[i]->stampa();
                }
            }
        }      
};

int main(){
    int dim=5;
    Studente** s=new Studente* [dim];
    s[0]=new Studente("Melo", "Fazio", 5, 6, 3);
    s[1]=new Studente("Andrea", "Richbons", 10, 10, 5);
    s[2]=new Studente("Rici", "Rich", 7, 8, 9);
    s[3]=new Studente("Angelo", "Fazio", 9, 9, 4);
    s[4]=new Studente("Lillo", "Alliccamillo", 6, 7, 8);

    Scuola scuola("Majorana");
    for(int i=0; i<dim; i++){
        scuola.aggiungi_Studente(s[i]);
    }

    cout<<"=====Prima Ordinamento====="<<endl;
    scuola.stampa();
    cout<<"==========================="<<endl;

    scuola.selectionSort();

    cout<<"=====Dopo Ordinamento====="<<endl;
    scuola.stampa();
    cout<<"=========================="<<endl;
}