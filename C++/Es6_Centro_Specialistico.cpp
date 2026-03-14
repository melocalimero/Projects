/*In un centro medico sono presenti medici di diverse specializzazioni e pazienti che possono essere presi in carico da uno o più medici.
Ogni paziente ha una cartella clinica per ogni medico che lo segue, contenente diagnosi e note.
Si vuole rappresentare questa situazione e poter:

stampare la lista dei pazienti per ogni medico,

accedere rapidamente alle cartelle cliniche di ogni paziente,aggiornare una diagnosi.*/

#include <iostream>
#include <string>

using namespace std;



class Persona{
    private:
        string nome;
        string cognome;
        int eta;
    public:
        Persona(){
            nome="";
            cognome="";
            eta=0;
        }
        Persona(string nome, string cognome, int eta): nome(nome), cognome(cognome), eta(eta){}

        const string& get_nome() const {return nome;}
        const string& get_cognome()const {return cognome;}
        const int& get_eta() const {return eta;}

        virtual void stampa(){
            cout<<this->nome<<", "<<this->cognome<<", "<<this->eta;
        }

        virtual ~Persona(){}
};


class Paziente: public Persona{
    private:
        int id_paziente;

    public:
        Paziente(){
            Persona();
            id_paziente=0;
        }
        Paziente(string nome, string cognome, int eta, int id_paziente):
        Persona(nome, cognome, eta), id_paziente(id_paziente){}

        const int& get_id_paziente() const {return id_paziente;}

        void stampa() override{
            Persona::stampa();
            cout<<", "<<this->id_paziente<<endl;
            
        }

        ~Paziente() override {}
};

class Dottore: public Persona{
    private:
        int id_medico;
        Paziente** array_pazienti;
        int n_pazienti;
    public:
        Dottore(){
            Persona();
            id_medico=0;
            array_pazienti=NULL;
            n_pazienti=-1;
        }
        Dottore(string nome, string cognome, int eta, int id_medico, Paziente** array_pazienti, int n_pazienti):
        Persona(nome, cognome, eta), id_medico(id_medico), array_pazienti(array_pazienti),n_pazienti(n_pazienti) {}

        const int& get_id_medico() const {return id_medico;}
        const int& get_n_pazienti() const {return n_pazienti;}

        void aggiungiPaziente(Paziente* p){
            if(n_pazienti>=0){
                this->n_pazienti++;
                array_pazienti[n_pazienti-1]=new Paziente(p->get_nome(), p->get_cognome(), p->get_eta(), p->get_id_paziente());
            }
            if(n_pazienti<0){
                this->n_pazienti=0;
                array_pazienti[n_pazienti]=new Paziente(p->get_nome(), p->get_cognome(), p->get_eta(), p->get_id_paziente());
            }
        
    }

        //TODO funzione per rimuovere un paziente
        
        void stampa_pazienti(){
            if(n_pazienti>=0){
                for(int i=0; i<n_pazienti; i++){
                    array_pazienti[i]->stampa();
                }
            }
            else{
                cout<<"Non ci sono pazienti"<<endl;
            }
        }

        
        void stampa() override{
            Persona::stampa();
            cout<<", "<<this->id_medico<<endl;
            
            
        }

        friend class CartellaClinica;
        ~Dottore() override {}

};

class CartellaClinica{
    private:
        Paziente p;
        Dottore** array_dottori;
        int n_dottori;
        string diagnosi;
        string note;
    public:
        CartellaClinica(){
            p=Paziente();
            array_dottori=NULL;
            n_dottori=0;
            diagnosi="";
            note="";
        }
        CartellaClinica(string diagnosi, string note, Paziente p, Dottore** array_dottori, int n_dottori): 
        diagnosi(diagnosi), note(note), p(p), array_dottori(array_dottori), n_dottori(n_dottori){}

        const string& get_diagnosi() const {return diagnosi;}
        const string& get_note() const {return note;}
        const int& get_id_paziente() const {return p.get_id_paziente();}
        const int& get_n_medici() const {return n_dottori;}

        void aggiungiMedico(Dottore* d){
            if(n_dottori>=0){
                n_dottori++;
                array_dottori[n_dottori-1]=new Dottore(d->get_nome(), d->get_cognome(), d->get_eta(), d->get_id_medico(), d->array_pazienti,d->get_n_pazienti());
            }
        }

        void set_note(string note) {this->note=note;}

        void stampa(){
            cout<<"=======Cartella di ";
            this->p.stampa();
            cout<<"Diagnosi:["<<this->diagnosi<<"], Note:["<<this->note<<"],\n\t\t Dottori:"<<endl;
            if(n_dottori>0){
                int index=0;
                for(int i=0; i<n_dottori; i++){
                    if(array_dottori[i]->get_n_pazienti()>0){
                        for(int j=0; j<array_dottori[i]->get_n_pazienti(); j++){
                            if(array_dottori[i]->array_pazienti[j]->get_id_paziente()==p.get_id_paziente()){
                                array_dottori[i]->stampa();
                                break;
                            }
                        }
                    }
                }
                
            }
            else{
                cout<<"Non ci sono dottori"<<endl;
            }
            cout<<"\t\tPaziente: "<<endl;
            this->p.stampa();
            cout<<"==========================================="<<endl;
        }
};



int main(){
    
    Paziente** array1_pazienti=new Paziente*[2];
    array1_pazienti[0]=new Paziente("Melo", "Fazio",  57, 445);
    array1_pazienti[1]=new Paziente("Angelina", "Dimitriv", 28, 446);

    Paziente** array2_pazienti=new Paziente*[1];
    array2_pazienti[0]=new Paziente("Francesco", "Bevilacqua", 44, 447);

    Paziente** array3_pazienti=new Paziente*[3];
    array3_pazienti[0]=new Paziente("Carlo", "Ursino", 19, 448);
    array3_pazienti[1]=new Paziente("Filippo", "Mangiacalippo", 35, 449);
    array3_pazienti[2]=new Paziente("Angelo", "Timpanaro", 19, 450);

    
    Dottore** array_dottori=new Dottore*[3];
    array_dottori[0]=new Dottore("Lollo", "Lacustre", 55, 1001, array3_pazienti, 3);
    array_dottori[1]=new Dottore("Calimero", "Suchitinpero", 53, 1002, array2_pazienti, 1);
    array_dottori[2]=new Dottore("Lino", "Porco", 33, 1003, array3_pazienti, 3);

    
    CartellaClinica** cartelle=new CartellaClinica* [3];
    cartelle[0]=new CartellaClinica("Malato", ".", *(array3_pazienti[0]), array_dottori, 3);
    cartelle[1]=new CartellaClinica("Cancro", ".", *(array2_pazienti[0]), array_dottori, 3);
    cartelle[2]=new CartellaClinica("Tumore", ".", *(array3_pazienti[2]), array_dottori, 3);

    for(int i=0; i<3; i++)
        cartelle[i]->stampa();
}

