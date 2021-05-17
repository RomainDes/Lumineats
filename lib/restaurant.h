#include "utility/vector.h"
#include "utility/db.h"

#include <stdlib.h>

typedef struct restaurant {
    size_t id;
    char nom[TAILLE_NOM];
    char mdp[TAILLE_MDP];
    char code_postal[5];
    char tel[14];
    char type[TAILLE_TYPE];
    char menu[TAILLE_MENU];
    float solde;
} restaurant;

typedef struct item {
    size_t id;
    char nom[TAILLE_NOM];
    char* ingredients[MAX_INGRE];
    float prix;
    
} item;

//Permet a un livreur de se connecter a son compte, renvoie l'id du compte auquel on se 
//connecte si on y parvient, 0 sinon
int connecter_compte_livreur();

//Permet de créer un compte pour un restaurant avec les 
//informations suivante : le nom, le code postal, 
//le téléphone et le type de cuisine.
void creer_compte_resto();


//Permet de supprimer un compte de la base de données et 
//les références dans les items créé par celui-ci.
void supprimer_un_compte();


//Permet de modifier le menu d'un restaurant grâce aux trois 
//sous fonctions suivantes.
void modifier_menu();


//Permet d'ajouter un nouvel item qui n'existe pas dans la 
//base de donnée des menus, il faudra donc ajouter cette 
//items dans cette base de donnée.
void ajouter_nouvel_item();


//Permet d'ajouter un item existant dans la base de donnée 
//menu (celui-ci peut-être utilisé par d'autre restaurant)
void ajouter_item_existant();

//Permet de supprimer un item présent dans la db de menu
void supprimer_item();

//Permet de consulter le solde du restaurant 
void consulter_solde_restaurant();

