typedef struct client              //Structure pour la table client
{
    int id;
    char nom[TAILLE_CHAMP_NOM];
    int code_postal;
    char telephone[10];
    float solde;
    char mot_de_passe[TAILLE_CHAMP_MDP];
} client;