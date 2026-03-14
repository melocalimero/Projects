/*Un istituto scientifico vuole catalogare gli strumenti in laboratorio.

Ogni strumento ha un nome, una sigla identificativa, un peso e una categoria.

Gli strumenti possono essere di due tipi:

Analitici, che hanno un valore di precisione (es. 0.001 g)

Ottici, che hanno un valore di ingrandimento massimo (es. 100x)

Ogni strumento può essere calibrato, registrando una data di calibrazione.
Si vuole poter verificare se uno strumento è da ricalibrare, considerando che la calibrazione scade dopo 365 giorni.

Creare un insieme di strumenti misti, calibrare alcuni di essi, e mostrare quali sono scaduti e necessitano nuova calibrazione.*/

#include <iostream>
using namespace std;

class Strumento{
    private:
        string nome;
        string sigla;
        double peso;
        string categoria;
        double giorno_calibrazione;

    public:
        Strumento(string nome, string sigla, double peso, string categoria):
        nome(nome), sigla(sigla), peso(peso), categoria(categoria){
            giorno_calibrazione=0;
        }

        const string& get_nome() const {return nome;}
        const string& get_sigla() const {return sigla;}
        const double& get_peso() const {return peso;}
        const string& get_categoria() const {return categoria;}

        void calibra(double giorno){
            this->giorno_calibrazione=giorno;
        }
};
