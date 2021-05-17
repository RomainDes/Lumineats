#include "utility/vector.h"
#include "utility/db.h"

typedef struct client              //Structure pour la table client
{
    size_t id;
    char nom[TAILLE_NOM];
    char code_postal[5];
    char tel[14];
    float solde;
    char mdp[TAILLE_MDP];
} client;


void creer_compte_client();

void supprimer_compte_client();

void modifier_profil_client();

void consulter_solde_client();

void crediter_solde_client();

void debiter_solde_client();

void crediter_solde_resto();

void crediter_solde_livreur();

void voir_liste_resto();

void restreindre_liste_resto();

void voir_liste_item();

void restreindre_liste_item();

void ajouter_item();

void supprimer_item();