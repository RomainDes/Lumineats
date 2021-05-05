#include "vector.h"

typedef struct livreur {
    size_t id;
    char nom[TAILLE_CHAMP_NOM];
    vector cp;
    char tel[10];
    float solde;
    char mdp[TAILLE_CHAMP_MDP];
} livreur;

//Permet a un livreur de se creer un compte en entrant toutes ses informations
void creer_compte_livreur();

//Permet a un liveur de supprimer son compte et toutes les information y etant contenues
void supprimer_compte_livreur();

//Permet a un livreur de modifier les cp ou il lui est possible de livrer
void modifier_cp_livreur();

//Permet a un livreur de modifier son numero de telephone 
void modifier_tel_livreur();

//Permet a un livreur de modifier ou retirer son exclusivité à un restaurateur
void modifier_resto_livreur();

//Permet a un livreur de modifier les differents elements vu ci dessus
void modifier_compte_livreur();

//Permet à un liveur de consulter la somme d'argent qu'il a sur son compte 
void consulter_solde_livreur();