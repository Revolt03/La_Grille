#include <iostream>
#include <chrono>
#include "Grille.h"

int main() {
    Grille maGrille;
    std::string nomFichier = "../instances/probleme_4_c.txt";

    // lecture du fichier et affichage
    if (lireInstance(nomFichier, maGrille)) {
        afficherGrille(maGrille);

        //chrono et limite de temps de 3heures
        auto debut = std::chrono::steady_clock::now();
        const int limite_minutes = 180;
        const int limite_ms = limite_minutes * 60 * 1000;
        Resultat_Grille resultat = separationEtEvaluation(maGrille, limite_ms);
        auto fin = std::chrono::steady_clock::now();
        auto duree_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut).count();

        // affichage du resultat
        std::cout<<"\nMeilleure solution"<<std::endl;
        std::cout<<"Temps de calcul : "<<duree_ms<<" ms"<<std::endl;
        std::cout<<"Score final : "<<resultat.meilleur_score<<std::endl;
        std::cout<<"Nombre de conflits : "<<resultat.conflits<<std::endl;
        if (resultat.limite_atteinte) {
            std::cout<<"Limite de temps atteinte ("<<limite_minutes<<" min)"<<std::endl;
        }
        std::cout<<"Placement (1=jeton, 0=vide) :"<<std::endl;

        for (int i = 0; i < maGrille.taille; ++i) {
            for (int j = 0; j < maGrille.taille; ++j) {
                std::cout<<resultat.placement[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<"-------------------------------"<<std::endl;

        std::string baseNom = nomFichier;
        std::size_t slashPos = baseNom.find_last_of("/\\");
        if (slashPos != std::string::npos) {
            baseNom = baseNom.substr(slashPos + 1);
        }
        std::size_t dotPos = baseNom.find_last_of('.');
        if (dotPos != std::string::npos) {
            baseNom = baseNom.substr(0, dotPos);
        }

        std::string cheminSolution = "../resultat/" + baseNom + "_solution.txt";
        if (ecrireSolution(cheminSolution, maGrille, resultat)) {
            std::cout<<"Solution ecrite dans : "<<cheminSolution<<std::endl;
        } else {
            std::cerr<<"Echec de l'ecriture de la solution"<<std::endl;
        }
    } 
    else 
    {
        std::cerr<<"Echec de la lecture de l'instance"<<std::endl;
    }

    return 0;
}