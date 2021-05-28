#include "utility/vector.h"
#include "utility/db.h"

typedef struct client              //Structure pour la table client
{
    size_t id;
    char nom[TAILLE_NOM];
    char code_postal[6];
    char tel[15];
    float solde;
    char mdp[TAILLE_MDP];
} client;


int index_client_counter(vector const* dbclient);

// iterator trouver_client_avec_id(int id);

int creer_compte_client();

int nom_client_exist(vector const* dbclient, char nom[TAILLE_NOM]);

int connecter_compte_client();

void supprimer_compte_client(int id);

void modifier_cp_client(int id);

void modifier_tel_client(int id);

void modifier_compte_client(int id);

void consulter_solde_client(int id);

void crediter_solde_client(int id);

void debiter_solde_client(int id, float val);

void crediter_solde_restaurant(int id, float val);

void crediter_solde_livreur(int id, float val);

void voir_liste_restau();

vector voir_qui_liste_restau(int id);

void voir_type_liste_restau(vector dbresto);

void restreindre_liste_restau(int id);

void voir_liste_item();

vector voir_liste_unique_item(vector dbitem);

vector voir_liste_type_item(vector dbitem);

int type_resto_exist(vector const* dbresto, char type[TAILLE_TYPE]);

vector voir_qui_liste_item(int id, vector dbitem);

vector voir_liste_solde_item(int id, vector dbitem);

void restreindre_liste_item(int id);

void ajouter_item_commande();

void supprimer_item_commande();

////INTERFACE/////

int menu_client();

int menu_client_compte();