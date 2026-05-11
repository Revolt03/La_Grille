#include <iostream>
#include <chrono>
#include "Grille.h"

int main() {
    Grille maGrille;
    std::string nomFichier = "../instances/competition_04.txt";

    // lecture du fichier et affichage
    if (lireInstance(nomFichier, maGrille)) {
        afficherGrille(maGrille);

        // recherche pour trouver la meilleure solution
        auto debut = std::chrono::steady_clock::now();
        Resultat_Grille resultat = separationEtEvaluation(maGrille);
        auto fin = std::chrono::steady_clock::now();
        auto duree_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut).count();

        // affichage du resultat
        std::cout<<"\nMeilleure solution"<<std::endl;
        std::cout<<"Temps de calcul : "<<duree_ms<<" ms"<<std::endl;
        std::cout<<"Score final : "<<resultat.meilleur_score<<std::endl;
        std::cout<<"Nombre de conflits : "<<resultat.conflits<<std::endl;
        std::cout<<"Placement (1=jeton, 0=vide) :"<<std::endl;

        for (int i = 0; i < maGrille.taille; ++i) {
            for (int j = 0; j < maGrille.taille; ++j) {
                std::cout<<resultat.placement[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<"-------------------------------"<<std::endl;
    } 
    else 
    {
        std::cerr<<"Echec de la lecture de l'instance"<<std::endl;
    }

    return 0;
}