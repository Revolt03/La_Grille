#include "Grille.h"
#include <iostream>
#include <fstream>
#include <iomanip>

bool lireInstance(std::string & cheminFichier, Grille & grille) {
    std::ifstream fichier(cheminFichier);
    
    if (! fichier.is_open() ) 
    {
        std::cout<<"Erreur : impossible d'ouvrir le fichier "<<cheminFichier<<std::endl;
        return false;
    }

    fichier>>grille.taille>>grille.nb_jetons>>grille.penalite;

    //redimensionne dynamiquement le tableau a 2 dimension
    grille.valeurs.resize(grille.taille, std::vector<int>(grille.taille));

    for (int i = 0; i < grille.taille; ++i) {
        for (int j = 0; j < grille.taille; ++j) {
            fichier >> grille.valeurs[i][j];
        }
    }
    fichier.close();
    return true;
}

void afficherGrille(Grille & grille) {
    std::cout<<"Parametres de l'instance : "<<std::endl;
    std::cout<<"Taille (t) : "<<grille.taille << "x" << grille.taille << std::endl;
    std::cout<<"Nombre de jetons (r) : " << grille.nb_jetons << std::endl;
    std::cout<<"Penalite par conflit (p) : " << grille.penalite << std::endl;
    
    std::cout <<std::endl<<"--- Plateau de jeu ---" << std::endl;
    for (int i = 0; i < grille.taille; ++i) {
        for (int j = 0; j < grille.taille; ++j) {
            // std::setw(4) pour que chaque nombre prenne 4 de largeur
            // Et ca aligne les colonnes même s'il y a des nombres négatifs
            std::cout << std::setw(4) << grille.valeurs[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout<<"----------------------"<<std::endl;
}