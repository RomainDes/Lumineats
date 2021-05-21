#include "client.h"
#include "restaurant.h"

#include <stdio.h>

size_t counter_index_client(){
    static size_t client = 0;
    ++client;
    return client;
}

                         /*----------COMPTE----------*/



//Permet a un client de se creer un compte en entrant toutes ses informations
void creer_compte_client(){
    client c;                                 //   On crée un client c
    client* client_actuel;


    size_t index = counter_index_client();    //On crée un size_t index qui prend comme valeur la variable static qui sera incrémenté à chaque appel dans la fonction appelée
    c.id = index;                             //l'id du client prend la valeur de index
    FILE * fichierclient;
    iterator actu, fin;
    

    fichierclient = fopen("./database/clients.csv", "r+");
    vector dbclient = lecture_table_clients(fichierclient);


    int valid = 0;

    while(valid == 0){
        printf("Veuillez entrer votre nom : ");
        scanf("%s \n", c.nom);
        //On verifie que le nom n'est pas deja présent dans la bd
        actu = begin(&dbclient);
        fin = end(&dbclient);
        valid = 1;
        while (actu.element != fin.element && valid == 1){
            client_actuel = (struct client*) actu.element;
            if (client_actuel->nom == c.nom){
                valid = 0;
            }
        increment(&actu, 1);
        }
    }


    //On demande à l'utilisateur de rentrer son code postal
    printf("Veuillez entrer votre code postal : \n"); 
    printf("Votre code postal doit contenir 5 caractères\n");
    //On copie ce que l'utilisateur a entré dans le code postal de client
    scanf("%s", c.code_postal);     

    //On demande à l'utilisateur de rentrer son téléphone
    printf("Veuillez entrer votre numéro de téléphone : \n"); 
    printf("Votre numéro doit être de la forme 04-XX-XX-XX-XX\n");
    //On copie ce que l'utilisateur a entré dans tel de client
    scanf("%s", c.tel);                           

    //La valeur du solde de début du client est de 0
    c.solde = 0.;           

    //On demande à l'utilisateur de rentrer son mot de passe
    printf("Veuillez entrer votre mot de passe : ");
    //On copie ce que l'utilisateur a entré dans le mot de passe de client 
    scanf("%s", c.mdp);  

    
    //On finit par ajouter la struct client au fichier csv  

    push_back(&dbclient, &c);
    ecriture_table_clients(fichierclient, &dbclient);

    fclose(fichierclient);                 

    return;
}

//Permet a un client de supprimer son compte et toutes les information y etant contenues

void supprimer_compte_client(){

}

//Permet à un client de modifier son profil (Code postal et téléphone)

void modifier_profil_client(){

}


                         /*----------SOLDE----------*/



//Permet  à un client de consulter son solde

void consulter_solde_client(){

}

//Permet à un client de créditer son solde d'un montant

void crediter_solde_client(){

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

void ajouter_item(){

}

//Permet à un client de supprimer un item

void supprimer_item(){

}



