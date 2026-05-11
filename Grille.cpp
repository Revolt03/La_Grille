#include "Grille.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>

struct Position {
    int ligne;
    int colonne;
};

bool lireInstance(std::string & cheminFichier, Grille & grille) {
    std::ifstream fichier(cheminFichier);
    
    if (! fichier.is_open() ) 
    {
        std::cout<<"Erreur : impossible d'ouvrir le fichier "<<cheminFichier<<std::endl;
        return false;
    }

    fichier>>grille.taille>>grille.nb_jetons>>grille.penalite;

    // redimensionne dynamiquement le tableau a 2 dimensions
    grille.valeurs.resize(grille.taille, std::vector<int>(grille.taille));

    for (int i = 0; i < grille.taille; ++i) {
        for (int j = 0; j < grille.taille; ++j) {
            fichier>>grille.valeurs[i][j];
        }
    }
    fichier.close();
    return true;
}

void afficherGrille(Grille & grille) {
    std::cout<<"Parametres de l'instance : "<<std::endl;
    std::cout<<"Taille (t) : "<<grille.taille<<"x"<<grille.taille<<std::endl;
    std::cout<<"Nombre de jetons (r) : "<<grille.nb_jetons<<std::endl;
    std::cout<<"Penalite par conflit (p) : "<<grille.penalite<<std::endl;

    std::cout<<std::endl<<"--- Plateau de jeu ---"<<std::endl;
    for (int i = 0; i < grille.taille; ++i) {
        for (int j = 0; j < grille.taille; ++j) {
            // std::setw(4) pour que chaque nombre prenne 4 de largeur
            // et ca aligne les colonnes meme s'il y a des nombres negatifs
            std::cout<<std::setw(4)<<grille.valeurs[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"----------------------"<<std::endl;
}

int conflitsAvec(const std::vector<Position> & positions, int ligne, int colonne) {
    // compte combien de jetons deja posés sont en conflit avec (ligne, colonne)
    int conflits = 0;
    for (unsigned int i = 0; i < positions.size(); ++i) {
        // on recupere les coordonnées du jeton déjà posé et on vérifie s'il est en conflit avec la position (ligne, colonne et diagonales)
        int l = positions[i].ligne;
        int c = positions[i].colonne;
        if (l == ligne or c == colonne or (l - c) == (ligne - colonne) or (l + c) == (ligne + colonne)) 
        {
            conflits++;
        }
    }
    return conflits;
}

matrice construirePlacement(int taille, const std::vector<Position> & positions) {
    // créer une matrice 0/1 a partir de la liste des positions
    matrice placement(taille, std::vector<int>(taille, 0)); 
    for (unsigned int i = 0; i < positions.size(); ++i) {
        placement[positions[i].ligne][positions[i].colonne] = 1;
    }
    return placement;
}

std::vector<int> convertirGrilleEnVecteur(const Grille & grille) {
    std::vector<int> valeurs(grille.taille * grille.taille, 0);
    for (int i = 0; i < grille.taille; ++i) {
        for (int j = 0; j < grille.taille; ++j) {
            valeurs[i * grille.taille + j] = grille.valeurs[i][j];
        }
    }
    return valeurs;
}

int g_taille = 0;
int g_nb_jetons = 0;
int g_penalite = 0;
std::vector<int> g_valeurs;

std::vector<Position> g_positions;
std::vector<Position> g_meilleure_positions;

int g_meilleur_score = std::numeric_limits<int>::min(); // score du meilleur placement trouvé jusqu'à présent et on commence au plus petit nombre possible pour que n'importe quelle solution soit meilleure que ce score initial.
int g_meilleurs_conflits = 0; // nombre de conflits du meilleur placement trouvé jusqu'à présent
int g_score_brut = 0; // somme des valeurs des jetons poses
int g_conflits = 0;   // nombre de conflits en cours


// A REVOIR POUR .RESERVE
int borneOptimiste(int index, int aPlacer) {
    // borne optimiste : on prend les plus grosses valeurs restantes
    // on ignore les conflits, donc c'est volontairement trop optimiste
    if (aPlacer <= 0) // si on n'a plus de jetons à placer, on arrete
    {
        return 0;
    }

    int total = static_cast<int>(g_valeurs.size()); // nombre total de cases dans la grille, on convertit g_valeurs.size en entier signé
    std::vector<int> candidates; // valeurs des cases restantes

    candidates.reserve(total - index);// on reserve de la place pour éviter les reallocations

    for (int i = index; i<total; ++i) {
        candidates.push_back(g_valeurs[i]); 
    }

    std::sort(candidates.begin(), candidates.end());

    int somme = 0;
    for (int i = 0; i < aPlacer and i< static_cast<int>(candidates.size()); ++i) 
    {
        int pos = static_cast<int>(candidates.size()) - 1 - i; // on prend les plus grandes valeurs, donc on commence à la fin du tableau trié
        somme += candidates[pos]; //on ajoute la valeur de la case à la somme
    }
    return somme;
}



// backtracking + branch and bound
void backtrackingBB(int index, int places) {
    int total = g_taille * g_taille;

    static long long noeuds = 0;
    ++noeuds;
    if (noeuds % 1000000 == 0) {
        std::cout<<"Noeuds visites : "<<noeuds<<" | index="<<index<<" | places="<<places<<std::endl; //
    }

    if (places == g_nb_jetons) 
    {
        int scoreFinal = g_score_brut - g_conflits * g_penalite;
        if (scoreFinal > g_meilleur_score) 
        {
            g_meilleur_score = scoreFinal;
            g_meilleurs_conflits = g_conflits;
            g_meilleure_positions = g_positions;
            std::cout<<"Nouveau meilleur score : "<<g_meilleur_score <<" | conflits="<<g_meilleurs_conflits<<" | noeuds="<<noeuds<<std::endl;
        }
        return;
    }

    if (index == total) // si on a parcouru toutes les cases
    {
        return;
    }

    int restantes = total - index; 
    if (places + restantes < g_nb_jetons) // pas assez de cases pour finir
    {
        return;
    }

    int scoreActuel = g_score_brut - g_conflits * g_penalite; // score actuel du placement en cours
    int aPlacer = g_nb_jetons - places;
    int borne = borneOptimiste(index, aPlacer);// score maximum qu'on pourrait atteindre à partir de la configuration actuelle
    
    if (scoreActuel + borne <= g_meilleur_score) {  // meme dans le meilleur des cas, on ne bat pas le meilleur score
        return;
    }

//Convertit l'index linéaire en coordonnées 2D de la grille :
// ligne = quotient de la division, colonne = reste de la division.
    int ligne = index / g_taille;
    int colonne = index % g_taille;

    // branche 1 : on place un jeton
    int conflitsAjoutes = conflitsAvec(g_positions,ligne,colonne); 
    g_positions.push_back({ligne,colonne}); // on ajoute la position du jeton placé à la liste des positions
    g_score_brut += g_valeurs[index]; // on ajoute la valeur de la case au score brut
    g_conflits += conflitsAjoutes;

    backtrackingBB(index+1, places+1); //on continue la recherche en placant un jeton à la position actuelle

    g_conflits -= conflitsAjoutes;
    g_score_brut -= g_valeurs[index];
    g_positions.pop_back();

    // branche 2 : on ne place rien
    backtrackingBB(index+1, places);// on continue la recherche sans placer de jeton à la position actuelle
}





Resultat_Grille separationEtEvaluation(const Grille & grille) {
    // point d'entree de la recherche
    g_taille = grille.taille;
    g_nb_jetons = grille.nb_jetons;
    g_penalite = grille.penalite;

    g_valeurs = convertirGrilleEnVecteur(grille);

    g_positions.clear();
    g_positions.reserve(g_nb_jetons);
    g_meilleure_positions.clear();

    g_meilleur_score = std::numeric_limits<int>::min();
    g_meilleurs_conflits = 0;
    g_score_brut = 0;
    g_conflits = 0;

    backtrackingBB(0, 0);

    Resultat_Grille resultat;
    resultat.meilleur_score = g_meilleur_score;
    resultat.conflits = g_meilleurs_conflits;
    resultat.placement = construirePlacement(grille.taille, g_meilleure_positions);
    return resultat;
}
