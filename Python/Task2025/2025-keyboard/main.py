import string

def readInput(path):
    buffer = []
    try:
        with open(path, "r", encoding='utf-8') as f_in:
            n_string = int(f_in.readline().strip())
            
            for _ in range(n_string):
                # ATTENZIONE: Se il file NON ha la lunghezza scritta 
                # su una riga separata, cancella la riga qui sotto!
                length = int(f_in.readline().strip()) 
                
                new_string = f_in.readline().strip()
                buffer.append(new_string)
                
        return buffer
    
    except FileNotFoundError:
        print(f"Errore: File '{path}' non trovato.")
        return [] # Ritorna lista vuota per evitare crash nel main
    except ValueError:
        print("Errore di lettura: controlla la struttura del file di input.")
        return []

def findMissingChr(stringX):
    alph_set = set(string.ascii_lowercase) 
    
    word_set = set(stringX.lower()) 
    
    missingChr = "".join(alph_set - word_set)
    return missingChr
        
def writeOutput(path, output):
    try:
        with open(path, "w", encoding='utf-8') as f_ou:
            for word in output:
                f_ou.write(f"{word}\n")
    except OSError as e: # Eccezione per errori sui file
        print(f"Errore durante la scrittura del file: {e}")

# --- MAIN ---
path1 = "subtask1_input1.txt"
path2 = "subtask1_output1.txt"

text = readInput(path1)

if text:
    missingChr = [findMissingChr(word) for word in text]
    
    writeOutput(path2, missingChr)
    print("Elaborazione completata! File di output generato.")