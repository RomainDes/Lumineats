#include "vector.h"

typedef struct restaurant {
    size_t id;
    char nom[TAILLE_CHAMP];
    vector cp;
    char tel[10];
    char type[TAILLE_CHAMP];
    vector menu;
    float solde;
    
} restaurant;

typedef struct menu {
    size_t id;
    char nom[TAILLE_CHAMP];
    vector ingredients;
    float prix;
    
} menu;

void creer_compte_resto();
void supprimer_un_compte();
void modifier_menu();
void ajouter_nouvel_item();
void ajouter_item_existant();
void supprimer_item();
void consulter_solde_restaurant();

