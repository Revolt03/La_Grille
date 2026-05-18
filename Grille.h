#pragma once
#include <vector>
#include <string>

//matrice = vecteur de vecteurs d'entiers
using matrice = std::vector<std::vector<int>>;

struct Grille {
    int taille;
    int nb_jetons;
    int penalite;
    
    matrice valeurs;
};

struct Resultat_Grille {
    int meilleur_score;
    int conflits;
    matrice placement;
    bool limite_atteinte;
};

bool lireInstance(std::string & cheminFichier, Grille & grille);
void afficherGrille(Grille & grille);
Resultat_Grille separationEtEvaluation(const Grille & grille, int max_ms);
bool ecrireSolution(const std::string & cheminFichierSolution, const Grille & grille, const Resultat_Grille & resultat);
