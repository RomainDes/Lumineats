#include "utility/vector.h"

#define TAILLE_CHAMP  50

typedef struct client              //Structure pour la table client
{
    size_t id;
    char nom[TAILLE_CHAMP];
    size_t code_postal;
    char telephone[10];
    float solde;
    char mot_de_passe[TAILLE_CHAMP];
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