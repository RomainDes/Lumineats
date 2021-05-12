typedef struct client              //Structure pour la table client
{
    size_t id;
    char nom[TAILLE_CHAMP_NOM];
    size_t code_postal;
    char telephone[10];
    float solde;
    char mot_de_passe[TAILLE_CHAMP_MDP];
} client;


void creer_compte_client;

void supprimer_compte_client;

void modifier_profil_client;

void consulter_solde_client;

void crediter_solde_client;

void debiter_solde_client;

void crediter_solde_restaurant;

void crediter_solde_livreur;

void voir_liste_restau;

void restreindre_liste_restau;

void voir_liste_item;

void restreindre_liste_item;

void ajouter_item;

void supprimer_item;