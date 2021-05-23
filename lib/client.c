#include "client.h"
#include "restaurant.h"

#include <stdio.h>

//Permet de mettre en place l'index, à chaque fois qu'on appelle la fonction l'id incrémente de 1
int index_client_counter(vector const* dbclient){
    client *c = (client*)value(at(dbclient, dbclient->num_elements - 1));
    return c->id + 1;
}

                         /*----------COMPTE----------*/


//Fonction utilitaire permettant de trouver dans la base de donnee
//les informations du compte avec l'id id
iterator trouver_client_avec_id(int id){
    FILE* fichierclient;
    fichierclient = fopen("database/clients.csv", "r");
    vector dbclient = lecture_table_clients(fichierclient);
    fclose(fichierclient);

    iterator actu;
    int trouve = 0;
    client * client_dans_bd;

    actu = begin(&dbclient);

    while(trouve == 0){
        client_dans_bd = (struct client*) actu.element;
        if(client_dans_bd->id == id){
            trouve = 1;
        }
        increment(&actu, 1);
    }

    return actu;
}


//Permet a un client de se connecter a son compte, renvoie l'id du compte ou on se 
//connecte si on y parvient, 0 sinon
int connecter_compte_client(){
    //On demande au client son nom, puis on ouvre la db client, on demande ensuite
    //a l'utilisateur d'entrer son mot de passe et on verifie si il correspond au mdp
    //present dans la db a la ligne correspondant au nom entré.
    //Si le nom entré n'est pas dans la db on redemande, idem pour mdp si il ne correspond pas
    //On laisse 3 essais pour rentrer le mot de passe
    char username[TAILLE_NOM];
    char password[TAILLE_MDP];
    FILE * fichierclient;
    client const* clients;
    int essai = 3;
    int res;

    fichierclient = fopen("database/clients.csv", "r");
    vector dbclient = lecture_table_clients(fichierclient);

    //On demande le nom d'utilisateur et vérifie qu'il est bien dans la db

        printf("Veuillez entrer votre nom d'utilisateur : ");
        scanf("\n%127[^\n]", username);
        fclose(fichierclient);
        int index = nom_client_exist(&dbclient, username);

        while( index == 0){
            //demander de nouveau le nom du client
            printf("Veuillez entrer un nom valide :");
            scanf("\n%127[^\n]", username);
            index = nom_client_exist(&dbclient, username);
        }
        //A la fin de cette boucle, on aura dans client_dans_db la struct client correspondant au compte auquel on veut
        //se connecter, on aura donc plus besoin de la chercher


    //On crée valid a 0 pour le test du mot de passe
    int valid = 0;
    while(essai > 0 && valid == 0){
        printf("Veuillez entrer votre mot de passe. Il vous reste %i essais :", essai);
        scanf("\n%127[^\n]", password);

        iterator r = at(&dbclient, index);

        clients = (client*)r.element;

        if(compare_char(clients->mdp,password) == 1)  valid = 1; 

        if(valid == 0){
            printf("Mot de passe incorrect, veuillez reessayer.");
            essai -= 1;
        }
    }
    //On recupere l'id du compte si on a entré le bon mdp
    if (valid == 1){
        res = clients->id;
    }
    else{
        res = 0;
    }

    return res;
}



//Permet a un client de se creer un compte en entrant toutes ses informations
void creer_compte_client(){
    client c;                                 //   On crée un client c

    int exist;
        
    //Permet de savoir si la base de donnée existe ou non 
    //S'il elle n'existe pas on créer le fichier et on met l'index à 1
    //Sinon on prend l'index + 1 du dernier dernier index
    if( access("database/clients.csv", F_OK ) == -1){
        FILE * fichierclient_create = fopen("database/clients.csv", "w+");
        fclose(fichierclient_create);
        exist = 0;
    }
    else{
        exist = 1;
    }
    

    FILE *fichierclient_lire = fopen("database/clients.csv", "r");
    vector dbclient = lecture_table_clients(fichierclient_lire);
    fclose(fichierclient_lire);

    if(exist == 0){
        erase(&dbclient, begin(&dbclient));
        c.id = 1;
    }
    else   c.id = index_client_counter(&dbclient); 


    printf("Veuillez entrer votre nom :");
    scanf("\n%127[^\n]", c.nom);
    while(nom_client_exist(&dbclient, c.nom) >= 1){
        //demander de nouveau le nom du client
        printf("Ce nom existe déjà, veuillez en choisir un nouveau :");
        scanf("\n%127[^\n]", c.nom);
    }

    //On demande à l'utilisateur de rentrer son code postal
    printf("Veuillez entrer votre code postal :"); 
    scanf("\n%127[^\n]", c.code_postal);     

    //On demande à l'utilisateur de rentrer son téléphone
    printf("Veuillez entrer votre numéro de téléphone (De la forme 04 XX XX XX XX) :"); 
    scanf("\n%127[^\n]", c.tel);                           

    //La valeur du solde de début du client est de 0
    c.solde = 0.;           

    //On demande à l'utilisateur de rentrer son mot de passe
    printf("Veuillez entrer votre mot de passe :");
    scanf("\n%127[^\n]", c.mdp);  

    
    //On finit par ajouter la struct client au fichier csv  

    push_back(&dbclient, &c);

    FILE *fichierclient_ecrire = fopen("database/clients.csv", "w");
    ecriture_table_clients(fichierclient_ecrire, &dbclient);

    fclose(fichierclient_ecrire);

    destroy(&dbclient);                 

    return;
}

//On lit toute la base de donnée pour savoir si il existe déjà le nom, renvoie 0 si il n'existe pas sinon l'index où il existe
int nom_client_exist(vector const* dbclient, char nom[TAILLE_NOM]){

    for(iterator r = begin(dbclient), e = end(dbclient); compare(r, e) != 0; increment(&r, 1)){
        client const* clients = (client*)r.element;

        if(compare_char(clients->nom,nom) == 1)   return clients -> id;
        
    }

    return 0;
}

//Permet a un client de supprimer son compte et toutes les information y etant contenues

void supprimer_compte_client(int id){
    FILE* fichierclient;
    fichierclient = fopen("database/clients.csv","r+");
    vector dbclient = lecture_table_clients(fichierclient);

    iterator iterateur;

    iterateur = trouver_client_avec_id(id);

    erase(&dbclient, iterateur);

    return;
}

//Permet à un client de modifier son profil (Code postal et téléphone)

//Deux fonctions pour cela 

//Changer de code postal
void modifier_cp_client(int id){
    //On ouvre le fichier csv des clients, on récupere la ligne correspondant a 
    //l'index du compte ou on est connecté, on récupere le code postal
    //On remplace avec le nouveau code postal

    FILE* fichierclient;
    fichierclient = fopen("database/clients.csv","r+");
    vector dbclient = lecture_table_clients(fichierclient);

    client* client_connecte;
    iterator iterateur;
    char cp_actu[5];

    iterateur = trouver_client_avec_id(id);
    client_connecte = (struct client*) iterateur.element;

    printf("Veuillez entrer un nouveau code postal\n");
    scanf("%s", cp_actu);
    for(int i = 0; i < 5; i++){
        client_connecte->code_postal[i]=cp_actu[i];
    }

    set(iterateur, (void *)client_connecte);
    ecriture_table_clients(fichierclient, &dbclient);

    fclose(fichierclient);

    return;
}


//Changer de numéro de téléphone
void modifier_tel_client(int id){

    FILE* fichierclient;
    fichierclient = fopen("database/clients.csv","r+");
    vector dbclient = lecture_table_clients(fichierclient);

    client* client_connecte;
    iterator iterateur;
    char tel_actu[14];

    iterateur = trouver_client_avec_id(id);
    client_connecte = (struct client*) iterateur.element;

    printf("Veuillez entrer un nouveau numéro de téléphone\n");
    scanf("%s", tel_actu);
    for(int i = 0; i < 14; i++){
        client_connecte->tel[i]=tel_actu[i];
    }

    set(iterateur, (void *)client_connecte);
    ecriture_table_clients(fichierclient, &dbclient);

    fclose(fichierclient);

    return;
}

//Permet a un client de modifier les differents elements vu ci dessus
void modifier_compte_client(int id){
    //On combine les deux fonctions au dessus en demandant a l'utilisateur ce qu'il
    //souhaite  modifier
    int value;
    printf("Veuillez taper 1 ou 2 selon ce que vous souhaitez modifier : \n1) Code postal\n2) Numero de tel");
    scanf("\n%i\n",&value);

    switch (value)
    {
    case 1:
        modifier_cp_client(id);
        break;
    case 2:
        modifier_tel_client(id);
        break;
    default:
        printf("Valeur incorrecte, veuillez recommencer\n");
        break;
    }

    return;
}


                         /*----------SOLDE----------*/



//Permet à un client de consulter son solde
void consulter_solde_client(int id){
    //On ouvre la bd client et on recupère la valeur du solde a la ligne du compte
    //auquel on est connecté

    iterator iterateur;
    client * client_dans_bd;

    iterateur = trouver_client_avec_id(id);

    client_dans_bd = (struct client*) iterateur.element;
    
    printf("Le solde sur ce compte est de %f euros", client_dans_bd->solde);

    return;
}

//Permet à un client de créditer son solde d'un montant

void crediter_solde_client(int id){
    //On ouvre la bd client et on demande à l'utilisateur le montant qu'il veut crediter 
    //sur son solde a la ligne du compte auquel on est connecté

    FILE* fichierclient;
    fichierclient = fopen("database/clients.csv","r+");
    vector dbclient = lecture_table_clients(fichierclient);


    iterator iterateur;
    client * client_connecte;

    float nouv_solde;
    iterateur = trouver_client_avec_id(id);

    client_connecte = (struct client*) iterateur.element;
    
    printf("Indiquez le montant que vous voulez créditer sur votre compte\n");
    scanf("%f", &nouv_solde);
    client_connecte -> solde += nouv_solde;
    
    set(iterateur, (void *)client_connecte);
    ecriture_table_clients(fichierclient, &dbclient);

    fclose(fichierclient);

    return;
}

//Permet de débiter le solde d'un client

void debiter_solde_client(){

}

//Permet de créditer le solde du restaurant

void crediter_solde_restaurant(){

}

//Permet de crediter le solde d'un livreur

void crediter_solde_livreur(){
    
}


                         /*----------LISTE DES RESTAU----------*/



//Permet à un client de voir la liste des restaurants

void voir_liste_restau(){

}

//Permet à un client de restreindre la liste de restaurants qu'il peut voir

void restreindre_liste_restau(){

}


                         /*----------COMMANDE----------*/


//Permet à un client de consulter la liste des items

void voir_liste_item(){

}

//Permet à un client de restreindre la liste des items

void restreindre_liste_item(){

}

//Permet à un client d'ajouter un item

void ajouter_item_commande(){

}

//Permet à un client de supprimer un item

void supprimer_item_commande(){

}



