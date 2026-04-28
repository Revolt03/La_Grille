cmake_minimum_required(VERSION 3.16)
project(La_Grille)

# Ajout des avertissements de compilation
add_compile_options(-Wall)

# Création de l'exécutable nommé La_Grille.out à partir du fichier main.cpp
add_executable(${PROJECT_NAME}.out
    main.cpp
    Grille.cpp
)