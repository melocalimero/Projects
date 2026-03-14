/*Descrizione: Simulare il contenuto di un disco.
Ogni file ha:
Nome
Estensione (.txt, .exe, .jpg, ecc.)
Dimensione in MB

Funzionalità:
Calcolare lo spazio totale occupato
Cercare file per estensione
Trovare il file più grande
Ordinare per dimensione crescente
Mostrare solo file sotto i 10MB

Spunti:
Nessuna ereditarietà obbligatoria, ma utile per gestire diverse estensioni in sottoclassi se vuoi farlo
Algoritmi: ricerca, ordinamento, filtro
*/
#include <iostream>
using namespace std;
class File{
    private:
        string nome;
        string estensione;
        double dimensione;
    public:
        File(string nome, string estensione, double dimensione):
        nome(nome), estensione(estensione), dimensione(dimensione){}

        const string& get_nome() const {return nome;}
        const string& get_estensione() const {return estensione;}
        const double& get_dimensione() const {return dimensione;}

        void stampa(){
            cout<<"Nome: ["<<this->nome<<""<<this->estensione<<"], Dimensione: ["<<this->dimensione<<" MB]"<<endl;
        }

};

class Disco{
    private:
        string nome;
        File** array;
        int n_file;
        int dim_max;
    public:
        Disco(string nome):nome(nome){
            n_file=0;
            dim_max=100;
            array=new File* [dim_max];
        }

        const string& get_nome() const {return nome;}
        const int& get_n_file() const {return n_file;}

        void aggiungi_File(File* f){
            if(n_file<dim_max){
                n_file++;
                array[n_file-1]=f;
            }
            else{
                cout<<"[ERRORE]: limite massimo file raggiunto"<<endl;
                exit(1);
            }
        } 

        void selectionSort(){
            if(n_file>0){
                for(int i=0; i<n_file-1; i++){
                    int ind_max=i;
                    for(int j=i+1; j<n_file; j++){
                        if(array[j]->get_dimensione()>array[ind_max]->get_dimensione()){
                            ind_max=j;
                        }
                    }
                    if(ind_max!=i){
                        File* hold=array[i];
                        array[i]=array[ind_max];
                        array[ind_max]=hold;
                    }
                }
            }
        }

        bool ricercaEstensione(string estensione){
            if(n_file>0){
                for(int i=0; i<n_file; i++){
                    if(array[i]->get_estensione()==estensione){
                        return true;
                    }
                }
            }
            return false;
        }

        string file_piu_grande(){
            if(n_file>0){
                int max=0;
                for(int i=0; i<n_file; i++){
                    if(array[i]->get_dimensione()>array[max]->get_dimensione()){
                        max=i;
                    }
                }
                return array[max]->get_nome();
            }
            return NULL;
        }

        void stampa(){
            if(n_file>0){
                for(int i=0; i<n_file; i++){
                    if(array[i]->get_dimensione()<10){
                        array[i]->stampa();
                    }
                }
            }
        }

        double spazio_totale(){
            if(n_file>0){
                double somma=0;
                for(int i=0; i<n_file; i++){
                    somma+=array[i]->get_dimensione();
                }
                return somma;
            }
            return -1;
        }
};

int main(){
    int dim=4;
    File** f=new File*[dim];
    f[0]=new File("Figa", ".txt", 7);
    f[1]=new File("Vito", ".jpg", 4);
    f[2]=new File("Video", ".mp4", 500);
    f[3]=new File("Foto", ".jpg", 5);

    Disco d(":C");
    cout<<"===Prima ordinamento==="<<endl;
    for(int i=0; i<dim; i++){
        d.aggiungi_File(f[i]);
    }
    d.stampa();
    cout<<"======================="<<endl;
    cout<<"Peso totale: "<<d.spazio_totale()<<endl;

    cout<<"===Dopo ordinamento==="<<endl;
    d.selectionSort();
    d.stampa();
    cout<<"======================="<<endl;

    cout<<"File più grande: "<<d.file_piu_grande()<<endl;
    if(d.ricercaEstensione(".txt"))
        cout<<"File trovato "<<endl;
    else 
        cout<<"File non trovato"<<endl;
}