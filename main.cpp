#include <iostream>
#include "Grille.h"

int main() {
    Grille maGrille;
    std::string nomFichier = "../instances/probleme_4_a.txt";

    // Lecture du fichier et affichage
    if (lireInstance(nomFichier, maGrille)) {
        
        afficherGrille(maGrille);
        
    } 
    else {
        std::cerr << "Echec de la lecture de l'instance." << std::endl;
    }

    return 0;
}