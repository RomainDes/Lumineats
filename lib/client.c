#include "client.h"
#include "restaurant.h"
#include "livreur.h"

#include <stdio.h>

//Permet de mettre en place l'index, à chaque fois qu'on appelle la fonction l'id incrémente de 1
int index_client_counter(vector const* dbclient){
    client *c = (client*)value(at(dbclient, dbclient->num_elements));
    return c->id + 1;
}

                         /*----------COMPTE----------*/


//Permet a un client de se connecter a son compte, renvoie l'id du compte ou on se 
//connecte si on y parvient, 0 sinon
int connecter_compte_client(){
    //On demande au client son nom, puis on ouvre la db client, on demande ensuite
    //a l'utilisateur d'entrer son mot de passe et on verifie si il correspond au mdp
    //present dans la db a la ligne correspondant au nom entré.
    //Si le nom entré n'est pas dans la db on redemande, idem pour mdp si il ne correspond pas
    //On laisse 3 essais pour rentrer le mot de passe

    FILE * fichierlog = fopen("log.txt", "a+");

    printf("\n");
    system("clear");

    char username[TAILLE_NOM];
    char password[TAILLE_MDP];
    FILE * fichierclient;
    client const* clients;
    int res;

    fichierclient = fopen("database/clients.csv", "r");
    vector dbclient = lecture_table_clients(fichierclient);
    fclose(fichierclient);

    //On demande le nom d'utilisateur et vérifie qu'il est bien dans la db

        printf("Veuillez entrer votre nom d'utilisateur : ");
        scanf("\n%127[^\n]", username);
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
    while(valid == 0){
        printf("Veuillez entrer votre mot de passe.('q' pour quitter) : ");
        scanf("\n%127[^\n]", password);

        if (compare_char(password, "q") != 1) {

            iterator r = at(&dbclient, index);

            clients = (client*)r.element;

            if(compare_char(clients->mdp,password) == 1)  valid = 1; 

            if(valid == 0){
                printf("Mot de passe incorrect, veuillez reessayer.");
            }
        }
        else valid = -1;
    }
    //On recupere l'id du compte si on a entré le bon mdp
    if (valid == 1){
        res = clients->id;
        fprintf(fichierlog, "Le client %s s'est connecté a son compte.\n", clients->nom);
        fclose(fichierlog);
    }
    else if(valid == -1){
        res = 0;
        return res;
    }

    return res;
}



//Permet a un client de se creer un compte en entrant toutes ses informations
int creer_compte_client(){

    FILE * fichierlog = fopen("log.txt", "a+");

    printf("\n");
    system("clear");

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

    fprintf(fichierlog, "Le client %s a créé un compte.\n", c.nom);
    fclose(fichierlog);

    destroy(&dbclient);                 

    return c.id;
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

    printf("\n");
    system("clear");

    if(id > 0){

        FILE * fichierlog = fopen("log.txt", "a+");

        //Supprimer le client index
        printf("Confirmer la suppression du compte ('y' pour valider, 'r' pour refuser) : ");
        char choix;
        scanf("\n%c", &choix);
        if(choix == 'y'){

            FILE * fichierclient_read = fopen("database/clients.csv", "r");
            vector dbclient = lecture_table_clients(fichierclient_read);
            fclose(fichierclient_read);

            erase(&dbclient, at(&dbclient,id));

            if(dbclient.num_elements != 0){
                FILE * fichierclient_write = fopen("database/clients.csv", "w");
                ecriture_table_clients(fichierclient_write, &dbclient);
                fclose(fichierclient_write);
            }
            else    remove("database/clients.csv");
            id = 0;

            fprintf(fichierlog, "Le client d'index %i a été supprimé.\n", id);
            fclose(fichierlog);
        }
        else if(choix == 'r'){
            printf("Compte non supprimé.\nLoading...\n");
            sleep(4);
            return;
        }
        else{
            supprimer_compte_client(id);
        }   
    }
    else    printf("Vous n'êtes pas connecté à un compte.\n");

    return;
}

//Permet à un client de modifier son profil (Code postal et téléphone)

//Deux fonctions pour cela 

//Changer de code postal
void modifier_cp_client(int id){
    //On ouvre le fichier csv des clients, on récupere la ligne correspondant a 
    //l'index du compte ou on est connecté, on récupere le code postal
    //On remplace avec le nouveau code postal

    FILE * fichierlog = fopen("log.txt", "a+");

    printf("\n");
    system("clear");

    FILE* fichierclient_read;
    fichierclient_read = fopen("database/clients.csv","r");
    vector dbclient = lecture_table_clients(fichierclient_read);
    fclose(fichierclient_read);

    client* client_connecte;
    iterator iterateur;
    char cp_actu[6];

    iterateur = at(&dbclient, id);
    client_connecte = (struct client*) iterateur.element;

    printf("Veuillez entrer un nouveau code postal :");
    scanf("\n%127[^\n]", cp_actu);
    for(int i = 0; i < 6; i++){
        client_connecte->code_postal[i]=cp_actu[i];
    }

    set(iterateur, (void *)client_connecte);

    FILE* fichierclient_write;
    fichierclient_write = fopen("database/clients.csv","w");
    ecriture_table_clients(fichierclient_write, &dbclient);
    fclose(fichierclient_write);

    fprintf(fichierlog, "Le client %s a changé son code postal.\n", client_connecte -> nom);
    fclose(fichierlog);


    destroy(&dbclient);

    return;
}


//Changer de numéro de téléphone
void modifier_tel_client(int id){

    FILE * fichierlog = fopen("log.txt", "a+");

    printf("\n");
    system("clear");

    FILE* fichierclient_read;
    fichierclient_read = fopen("database/clients.csv","r");
    vector dbclient = lecture_table_clients(fichierclient_read);
    fclose(fichierclient_read);

    client* client_connecte;
    iterator iterateur;
    char tel_actu[15];

    iterateur = at(&dbclient, id);
    client_connecte = (struct client*) iterateur.element;

    printf("Veuillez entrer un nouveau numéro de téléphone :");
    scanf("\n%127[^\n]", tel_actu);
    for(int i = 0; i < 15; i++){
        client_connecte->tel[i]=tel_actu[i];
    }

    set(iterateur, (void *)client_connecte);

    FILE* fichierclient_write;
    fichierclient_write = fopen("database/clients.csv","w");
    ecriture_table_clients(fichierclient_write, &dbclient);
    fclose(fichierclient_write);

    fprintf(fichierlog, "Le client %s a changé son numéro de téléphone.\n", client_connecte -> nom);
    fclose(fichierlog);
    

    destroy(&dbclient);

    return;
}

//Permet a un client de modifier les differents elements vu ci dessus
void modifier_compte_client(int id){
    //On combine les deux fonctions au dessus en demandant a l'utilisateur ce qu'il
    //souhaite  modifier

    printf("\n");
    system("clear");

    int value;
    printf("Veuillez taper 1 ou 2 selon ce que vous souhaitez modifier : \n1) Code postal\n2) Numero de tel\n");
    scanf("\n%i",&value);

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
    
    
    FILE* fichierclient;
    fichierclient = fopen("database/clients.csv","r+");
    vector dbclient = lecture_table_clients(fichierclient);

    iterator iterateur;
    client * client_dans_bd;

    iterateur = at(&dbclient, id);

    client_dans_bd = (struct client*) iterateur.element;
    
    printf("Le solde sur ce compte est de %.2f euros\n", client_dans_bd->solde);

    destroy(&dbclient);

    return;
}

//Permet à un client de créditer son solde d'un montant

void crediter_solde_client(int id){

    FILE * fichierlog = fopen("log.txt", "a+");

    printf("\n");
    system("clear");

    //On ouvre la bd client et on demande à l'utilisateur le montant qu'il veut crediter 
    //sur son solde a la ligne du compte auquel on est connecté

    FILE* fichierclient_lecture;
    fichierclient_lecture = fopen("database/clients.csv","r");
    vector dbclient = lecture_table_clients(fichierclient_lecture);
    fclose(fichierclient_lecture);


    iterator iterateur;
    client * client_connecte;

    float nouv_solde;
    iterateur = at(&dbclient, id);

    client_connecte = (struct client*) iterateur.element;
    
    printf("Indiquez le montant que vous voulez créditer sur votre compte (entrez 0 pour ne rien faire et revenir à la page d'avant):");
    scanf("\n%f", &nouv_solde);
    client_connecte -> solde += nouv_solde;
    
    set(iterateur, (void *)client_connecte);

    FILE* fichierclient_ecriture;
    fichierclient_ecriture = fopen("database/clients.csv","w");
    ecriture_table_clients(fichierclient_ecriture, &dbclient);
    fclose(fichierclient_ecriture);

    fprintf(fichierlog, "Le client %s a crédité son compte de %.2f euros.\n", client_connecte -> nom, nouv_solde);
    fclose(fichierlog);
    
    destroy(&dbclient);

    return;
}

//Permet de débiter le solde d'un client

void debiter_solde_client(int id, float val){
    //On ouvre la bd client et on soustrait la valeur du solde de la ligne de compte à la valeur entrée en paramètre
    //du compte auquel on est connecté

    FILE * fichierlog = fopen("log.txt", "a+");

    FILE *fichierclient_read = fopen("database/clients.csv", "r");
    vector dbclient = lecture_table_clients(fichierclient_read);
    fclose(fichierclient_read);  

    iterator iterateur = at(&dbclient, id);
    client * client_connecte;


    client_connecte = (struct client*) iterateur.element;
    
    client_connecte -> solde -= val;

    printf("Votre compte à été débité de %.2f euros \n", val);
    
    set(iterateur, (void *)client_connecte);

    FILE *fichierclient_write = fopen("database/clients.csv", "w");
    ecriture_table_clients(fichierclient_write, &dbclient);
    fclose(fichierclient_write);

    fprintf(fichierlog, "Le client %s a été débité de %.2f euros.\n", client_connecte -> nom, val);
    fclose(fichierlog);

    destroy(&dbclient); 

    return;
}

//Permet de créditer le solde du restaurant

void crediter_solde_restaurant(int id, float val){
    //On ouvre la bd restaurant et on additionne la valeur du solde de la ligne de compte à la valeur entrée en paramètre 
    //du compte auquel on est connecté

    FILE * fichierlog = fopen("log.txt", "a+");

    FILE *fichierresto_read = fopen("database/restaurants.csv", "r");
    vector dbresto = lecture_table_restaurants(fichierresto_read);
    fclose(fichierresto_read);  

    iterator iterateur = at(&dbresto, id);
    restaurant * resto_connecte;

    resto_connecte = (struct restaurant*) iterateur.element;

    resto_connecte -> solde += val;

    printf("Le compte %s a bien été crédité de %.2f euros \n", resto_connecte -> nom, val);
    
    set(iterateur, (void *)resto_connecte);

    FILE *fichierresto_write = fopen("database/restaurants.csv", "w");
    ecriture_table_restaurants(fichierresto_write, &dbresto);
    fclose(fichierresto_write);

    fprintf(fichierlog, "Le restaurant %s a été crédité de %.2f euros.\n", resto_connecte -> nom, val);
    fclose(fichierlog);

    destroy(&dbresto); 

    return;
}

//Permet de crediter le solde d'un livreur

void crediter_solde_livreur(int id, float val){
    //On ouvre la bd livreur et on additionne la valeur du solde de la ligne de compte à la valeur entrée en paramètre 
    //du compte auquel on est connecté

    FILE * fichierlog = fopen("log.txt", "a+");

    FILE *fichierlivreur_read = fopen("database/livreurs.csv", "r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur_read);
    fclose(fichierlivreur_read);  

    iterator iterateur = at(&dblivreur, id);
    livreur * livreur_connecte;

    livreur_connecte = (struct livreur*) iterateur.element;

    livreur_connecte -> solde += val;

    printf("Le compte %s a bien été crédité de %.2f euros \n", livreur_connecte -> nom, val);
    
    set(iterateur, (void *)livreur_connecte);

    FILE *fichierlivreur_write = fopen("database/livreurs.csv", "w");
    ecriture_table_livreurs(fichierlivreur_write, &dblivreur);
    fclose(fichierlivreur_write);

    fprintf(fichierlog, "Le livreur %s a été crédité de %.2f euros.\n", livreur_connecte -> nom, val);
    fclose(fichierlog);

    destroy(&dblivreur); 

    return;
}

                         /*----------LISTE DES RESTAU----------*/



//Permet à un client de voir la liste des restaurants

void voir_liste_restau(){

    printf("\n");
    system("clear");

    FILE *fichierresto_read = fopen("database/restaurants.csv", "r");
    vector dbresto = lecture_table_restaurants(fichierresto_read);
    fclose(fichierresto_read);  

    iterator iterateur = begin(&dbresto);
    restaurant * resto_connecte;

    printf("Liste des restaurants sur Lumineats ainsi que leur type de cuisine :\n\n");

    while(compare(iterateur, end(&dbresto)) != 0){
        resto_connecte = (struct restaurant*) iterateur.element;
        printf("%s (Type : %s)\n", resto_connecte -> nom, resto_connecte -> type);
        increment(&iterateur, 1);
    }

    destroy(&dbresto);

    printf("\n");

    return;
}

vector voir_qui_liste_restau(int id){

    printf("\n");
    system("clear");

    restaurant resto;
    vector liste = make_vector(sizeof(resto),0, 2.);

    FILE *fichierresto_read = fopen("database/restaurants.csv", "r");
    vector dbresto = lecture_table_restaurants(fichierresto_read);
    fclose(fichierresto_read);  

    FILE *fichierlivreur_read = fopen("database/livreurs.csv", "r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur_read);
    fclose(fichierlivreur_read); 

    FILE *fichierclient_read = fopen("database/clients.csv", "r");
    vector dbclient = lecture_table_clients(fichierclient_read);
    fclose(fichierclient_read);

    iterator iterateur_resto = begin(&dbresto);
    restaurant * resto_connecte;
    

    iterator iterateur_livreur = begin(&dblivreur);
    livreur * livreur_connecte;

    iterator iterateur_client = at(&dbclient, id);
    client * client_connecte; 
    client_connecte = (struct client*) iterateur_client.element;

    printf("\nListe des restaurants sur Lumineats qui peuvent vous livrer :\n\n");
    int res = 0;
    while(compare(iterateur_livreur, end(&dblivreur)) != 0){
        livreur_connecte = (struct livreur*) iterateur_livreur.element;

        for(int i = 0; i < livreur_connecte -> nb_deplacements && res == 0; i++){
            if(compare_char(client_connecte -> code_postal, livreur_connecte ->deplacements[i]) == 1 ){
                res = 1;
            }
        }
        if (res == 1){
            while(compare(iterateur_resto, end(&dbresto)) != 0){
                for(int i = 0; i < livreur_connecte -> nb_deplacements; i++){
                    resto_connecte = (struct restaurant*) iterateur_resto.element;
                    if(compare_char(resto_connecte -> code_postal, livreur_connecte ->deplacements[i]) == 1 ){
                        printf("%s (Type : %s)\n", resto_connecte -> nom, resto_connecte -> type);
                        push_back(&liste, iterateur_resto.element);
                    }
                } 
                increment(&iterateur_resto, 1);
            }
        }
                
        iterateur_resto = begin(&dbresto);
        increment(&iterateur_livreur, 1);
    }

    destroy(&dbresto);
    destroy(&dblivreur);
    destroy(&dbclient);

    printf("\n");

    return liste;
}

void voir_type_liste_restau(vector dbresto){

    printf("\n");
    system("clear");

    iterator iterateur_resto = begin(&dbresto);
    restaurant * resto_connecte; 

    int index = 0;
    char type_resto[TAILLE_TYPE];

    while(index == 0){

        printf("\nEntrez un type de cuisine pour afficher la liste des restaurants compatibles ('q' pour quitter) :");
        scanf("\n%127[^\n]", type_resto);
        if (compare_char(type_resto, "q") != 1) {
            index = type_resto_exist(&dbresto, type_resto);
            if(index == 0){
                printf("\nLe type que vous avez entré n'existe pas ou n'est pas compatible avec votre recherche. Veuillez recommencer\n");
            }
        }
        else index = -1;
    }
    if(index != -1){

        printf("\n");

        while(compare(iterateur_resto, end(&dbresto)) != 0){
            resto_connecte = (struct restaurant*) iterateur_resto.element;
            if(compare_char(resto_connecte -> type, type_resto) == 1 ){
                printf("%s (Type : %s)\n", resto_connecte -> nom, resto_connecte -> type);
            }
            increment(&iterateur_resto, 1);
        }

        printf("\n");
        return;
    }
    return;
}


//Permet à un client de restreindre la liste de restaurants qu'il peut voir

void restreindre_liste_restau(int id){
    //On combine les deux fonctions au dessus en demandant a l'utilisateur ce qu'il
    //souhaite  pouvoir voir

    printf("\n");
    system("clear");


    int value;
    printf("Veuillez taper 1, 2 ou 3 selon ce que vous souhaitez pouvoir voir : \n1) Qui peut me livrer\n2) Entrer un type de cuisine\n3) Faire les deux\n");
    scanf("\n%i",&value);

    FILE *fichierresto_read = fopen("database/restaurants.csv", "r");
    vector dbresto = lecture_table_restaurants(fichierresto_read);
    fclose(fichierresto_read);

    switch (value)
    {
    case 1:
        voir_qui_liste_restau(id);
        break;
    case 2: 
        voir_type_liste_restau(dbresto);
        break;
    case 3:
        voir_type_liste_restau(voir_qui_liste_restau(id));
        break;
    default:
        printf("Valeur incorrecte, veuillez recommencer\n");
        break;
    }

    destroy(&dbresto);

    return; 
}


                         /*----------COMMANDE----------*/


//Permet à un client de consulter la liste des items

void voir_liste_item(){

    printf("\n");
    system("clear");


    //On ouvre la db item et on affiche tous les items ainsi que leur prix correspondant 

    FILE *fichieritem_read = fopen("database/items.csv", "r");
    vector dbitem = lecture_table_items(fichieritem_read);
    fclose(fichieritem_read);  

    iterator iterateur = begin(&dbitem);
    item * item_connecte;

    printf("Liste des items proposés sur Lumineats :\n\n");

    while(compare(iterateur, end(&dbitem)) != 0){
        item_connecte = (struct item*) iterateur.element;
            printf("%s (Prix : %.2f euros)\n", item_connecte -> nom, item_connecte -> prix);
        increment(&iterateur, 1);
    }

    destroy(&dbitem);

    printf("\n");

    return;
}

//Permet à un client de consulter les items d'un restaurant qu'il aura choisit

vector voir_liste_unique_item(vector dbitem){

    printf("\n");
    system("clear");

    item items;
    vector liste = make_vector(sizeof(items),0, 2.);

    if(dbitem.data != NULL){

        FILE *fichierresto_read = fopen("database/restaurants.csv", "r");
        vector dbresto = lecture_table_restaurants(fichierresto_read);
        fclose(fichierresto_read);  
        
        int index = 0;
        char nom_resto[TAILLE_NOM];

        while(index == 0){
            printf("Entrez le nom du restaurant dont vous voulez voir les items :");
            scanf("\n%127[^\n]", nom_resto);
            index = nom_resto_exist(&dbresto, nom_resto);
            if(index == 0){
                printf("Le nom que vous avez entré n'existe pas. Veuillez recommencer\n");
            }
        }

        iterator iterateur_resto = at(&dbresto, index);
        restaurant * resto_connecte;

        resto_connecte = (struct restaurant*) iterateur_resto.element;

        printf("Liste des items proposés par ce restaurant :\n\n");

        for(int i = 0; i < resto_connecte -> nb_menu; i++){

            iterator iterateur_item = at(&dbitem, resto_connecte -> menu[i]);
            item *item_connecte = (struct item*) iterateur_item.element;
            printf("Menu %i: %s (Prix : %.2f euros)\n", i+1, item_connecte -> nom, item_connecte -> prix);
            push_back(&liste, iterateur_item.element);
        }

        destroy(&dbitem);
        destroy(&dbresto);

        printf("\n");

    }

    if(liste.data != NULL){
        return liste;
    }
    else{
        printf("Il n'y a pas d'item disponible\n");
        return liste;
    }
}

//Permet à un client de consulter les items des restaurants liés à un type de cuisine qu'il aura choisit

vector voir_liste_type_item(vector dbitem){

    printf("\n");
    system("clear");

    item items;
    vector liste = make_vector(sizeof(items),0, 2.);

    if(dbitem.data != NULL){

        FILE *fichierresto_read = fopen("database/restaurants.csv", "r");
        vector dbresto = lecture_table_restaurants(fichierresto_read);
        fclose(fichierresto_read);  
        
        int index = 0;
        char type_resto[TAILLE_TYPE];

        while(index == 0){
            printf("Entrez le type de cuisine dont vous voulez voir les items :");
            scanf("\n%127[^\n]", type_resto);
            index = type_resto_exist(&dbresto, type_resto);
            if(index == 0){
                printf("Le type que vous avez entré n'existe pas. Veuillez recommencer\n");
            }
        }

        iterator iterateur_resto = begin(&dbresto);
        restaurant * resto_connecte;


        while(compare(iterateur_resto, end(&dbresto)) != 0){
            resto_connecte = (struct restaurant*) iterateur_resto.element;
            iterator test = at(&dbitem, resto_connecte -> menu[0]);
            if(compare_char(resto_connecte -> type, type_resto) == 1 && test.element != NULL){
                printf("Le restaurant %s propose ces items pour le type de cuisine que vous avez sélectionné :\n\n", resto_connecte -> nom);
                for(int i = 0; i < resto_connecte -> nb_menu; i++){
                    iterator iterateur_item = at(&dbitem, resto_connecte -> menu[i]);
                    if(iterateur_item.element != NULL){
                        item *item_connecte = (struct item*) iterateur_item.element;
                        printf("Menu %i: %s (Prix : %.2f euros)\n", i+1, item_connecte -> nom, item_connecte -> prix);
                        push_back(&liste, iterateur_item.element);
                    }
                }
            }
            increment(&iterateur_resto, 1);
            printf("\n");
        }

        destroy(&dbitem);
        destroy(&dbresto);
    }

    if(liste.data != NULL){
        return liste;
    }
    else{
        printf("Il n'y a pas d'item disponible\n");
        return liste;
    }
}

//Fonction permettant de savoir si un type de cuisine existe dans la db 
//On lit toute la base de donnée pour savoir si il existe déjà le typ, renvoie 0 si il n'existe pas sinon l'index où il existe
int type_resto_exist(vector const* dbresto, char type[TAILLE_TYPE]){

    int index = 1;
    for(iterator r = begin(dbresto), e = end(dbresto); compare(r, e) != 0; increment(&r, 1)){
        restaurant const* resto = (restaurant*)r.element;

        if(compare_char(resto->type,type) == 1)   return index;
        else {
            index++;
        }
    }

    return 0;
}

//Permet à un client de consulter les items qui peuvent lui être livrés

vector voir_qui_liste_item(int id, vector dbitem){

    printf("\n");
    system("clear");

    item items;
    vector liste = make_vector(sizeof(items),0, 2.);

    if(dbitem.data != NULL){

        FILE *fichierresto_read = fopen("database/restaurants.csv", "r");
        vector dbresto = lecture_table_restaurants(fichierresto_read);
        fclose(fichierresto_read);  

        FILE *fichierlivreur_read = fopen("database/livreurs.csv", "r");
        vector dblivreur = lecture_table_livreurs(fichierlivreur_read);
        fclose(fichierlivreur_read); 

        FILE *fichierclient_read = fopen("database/clients.csv", "r");
        vector dbclient = lecture_table_clients(fichierclient_read);
        fclose(fichierclient_read);

        iterator iterateur_resto = begin(&dbresto);
        restaurant * resto_connecte;
        
        iterator iterateur_livreur = begin(&dblivreur);
        livreur * livreur_connecte;

        iterator iterateur_client = at(&dbclient, id);
        client * client_connecte; 
        client_connecte = (struct client*) iterateur_client.element;

        printf("Liste des items sur Lumineats qui peuvent vous être livrés :\n\n");
        int res = 0;
        while(compare(iterateur_livreur, end(&dblivreur)) != 0){
            livreur_connecte = (struct livreur*) iterateur_livreur.element;

            for(int i = 0; i < livreur_connecte -> nb_deplacements && res == 0; i++){
                if(compare_char(client_connecte -> code_postal, livreur_connecte ->deplacements[i]) == 1 ){
                    res = 1;
                }
            }
            if (res == 1){
                while(compare(iterateur_resto, end(&dbresto)) != 0){
                    for(int i = 0; i < livreur_connecte -> nb_deplacements; i++){

                        resto_connecte = (struct restaurant*) iterateur_resto.element;
                        iterator test = at(&dbitem, resto_connecte -> menu[0]);

                        if(compare_char(resto_connecte -> code_postal, livreur_connecte ->deplacements[i]) == 1 && test.element != NULL){
                            printf("Le restaurant %s propose ces items et peut vous livrer :\n\n", resto_connecte -> nom);
                            for(int j = 0; j < resto_connecte -> nb_menu; j++){
                                iterator iterateur_item = at(&dbitem, resto_connecte -> menu[j]);
                                if(iterateur_item.element != NULL){
                                    item *item_connecte = (struct item*) iterateur_item.element;
                                    printf("Menu %i: %s (Prix : %.2f euros)\n", j+1, item_connecte -> nom, item_connecte -> prix);
                                    push_back(&liste, iterateur_item.element);
                                    if(j == resto_connecte -> nb_menu - 1) printf("\n");
                                }
                            }
                        }
                    } 
                    increment(&iterateur_resto, 1);
                }
            }
                    
            iterateur_resto = begin(&dbresto);
            increment(&iterateur_livreur, 1);
        }

        destroy(&dbresto);
        destroy(&dblivreur);
        destroy(&dbclient);
        destroy(&dbitem);
    }

    if(liste.data != NULL){
        return liste;
    }
    else{
        printf("Il n'y a pas d'item correspondant qui peut vous être livré\n");
        return liste;
    }
}

//Permet à un client de consulter les items qui sont moins cher que son solde actuel

vector voir_liste_solde_item(int id, vector dbitem){
    //On ouvre la db item et on affiche tous les items moins cher que le solde du client ainsi que leur prix correspondant 

    printf("\n");
    system("clear");

    item items;
    vector liste = make_vector(sizeof(items),0, 2.);

    if(dbitem.data != NULL){
    
        FILE *fichierclient_read = fopen("database/clients.csv", "r");
        vector dbclient = lecture_table_clients(fichierclient_read);
        fclose(fichierclient_read); 

        iterator iterateur_item = begin(&dbitem);
        item * item_connecte;

        iterator iterateur_client = at(&dbclient, id);
        client * client_connecte = (struct client*) iterateur_client.element;

        printf("Votre solde actuel : %.2f\n\n", client_connecte -> solde);

        printf("Liste des items proposés moins chers que votre solde disponible:\n\n");

        while(compare(iterateur_item, end(&dbitem)) != 0){
            item_connecte = (struct item*) iterateur_item.element;
            if(client_connecte -> solde >= item_connecte -> prix){
                printf("%s (Prix : %.2f euros)\n", item_connecte -> nom, item_connecte -> prix);
                push_back(&liste, iterateur_item.element);
            }
            increment(&iterateur_item, 1);
        }

        destroy(&dbitem);
        destroy(&dbclient);

        printf("\n");
    }
    if(liste.data != NULL){
        return liste;
    }
    else{
        printf("Il n'y a pas d'item moins cher que votre solde disponible\n");
        return liste;
    }
}

//Permet à un client de restreindre la liste des items

vector restreindre_liste_item(int id){
    //On combine les quatre fonctions au dessus en demandant a l'utilisateur ce qu'il
    //souhaite  pouvoir voir

    printf("\n");
    system("clear");

    int value;
    printf("Veuillez taper 1, 2, 3, 4 ou 5 selon ce que vous souhaitez pouvoir voir : \n1) Qui peut me livrer\n2) Entrer un type de cuisine\n3) Entrer un restaurant\n");
    printf("4) Items moins chers que mon solde disponible\n5) Une combinaison de 1), 2) et 4)\n6) Une combinaison de 1), 3) et 4)\n");
    scanf("\n%i",&value);

    FILE *fichieritem_read = fopen("database/items.csv", "r");
    vector dbitem = lecture_table_items(fichieritem_read);
    fclose(fichieritem_read); 

    switch (value)
    {
    case 1:
        return voir_qui_liste_item(id, dbitem);
        break;
    case 2: 
        return voir_liste_type_item(dbitem);
        break;
    case 3:
        return voir_liste_unique_item(dbitem);
        break;
    case 4:
        return voir_liste_solde_item(id, dbitem);
        break;
    case 5:
        return voir_qui_liste_item(id,voir_liste_type_item(voir_liste_solde_item(id, dbitem)));
        break;
    case 6:
        return voir_qui_liste_item(id,voir_liste_unique_item(voir_liste_solde_item(id, dbitem)));
        break;
    default:
        printf("Valeur incorrecte, veuillez recommencer\n");
        break;
    }

    return dbitem; 
}

//Permet à un client d'ajouter un item

void ajouter_item_commande(int id, vector *liste_commande){

    FILE * fichierlog = fopen("log.txt", "a+");

    printf("\n");
    system("clear");

    char nom_item[TAILLE_NOM];

    float res = 0.;

    FILE *fichierclient_read = fopen("database/clients.csv", "r");
    vector dbclient = lecture_table_clients(fichierclient_read);
    fclose(fichierclient_read);

    iterator iterateur_client = at(&dbclient, id);
    client * client_connecte = (struct client*) iterateur_client.element;

    printf("Votre solde actuel : %.2f\n\n", client_connecte -> solde);

    vector dbitem = restreindre_liste_item(id);
    vector dbitem_livrable = voir_qui_liste_item(id, dbitem);

    iterator iterateur_item = begin(&dbitem_livrable);
    item * item_connecte;

    if(dbitem_livrable.data != NULL){

        printf("Votre solde actuel : %.2f\n\n", client_connecte -> solde);

        printf("Entrez un item que vous voulez ajouter à votre commande: ");
        scanf("\n%127[^\n]", nom_item);

        while(compare(iterateur_item, end(&dbitem_livrable)) != 0 && res == 0){
            item_connecte = (struct item*) iterateur_item.element;
            if(compare_char(item_connecte -> nom, nom_item) == 1){
                res = item_connecte -> prix;
                push_back(liste_commande, iterateur_item.element);

                fprintf(fichierlog, "Le client %s a ajouté l'item %s à son panier.\n", client_connecte -> nom, item_connecte -> nom);
                fclose(fichierlog);
            }
            increment(&iterateur_item, 1);
        }
        if(res == 0.){

            printf("\n");
            system("clear");

            
            int choix;
            printf("L'item sélectionné n'est pas livrable ou n'existe pas.\nSi vous voulez en sélectionner un autre entrez 1 et si vous voulez arrêter entrez 0\n\n");
            scanf("%i", &choix);
            if(choix == 1){
                ajouter_item_commande(id, liste_commande);
            }
            else {
                destroy(&dbclient);
                destroy(&dbitem);
                destroy(&dbitem_livrable);
                return;
            }
        }

    }
    else{

        printf("\n");
        system("clear");

        int choix;
        printf("Aucun item ne peut vous être livré avec vos critères\n");
        printf("Si vous voulez changer votre recherche, entrez 1 et si vous voulez arrêter entrez 0\n\n");
        scanf("%i", &choix);
            if(choix == 1){
                ajouter_item_commande(id, liste_commande);
            }
            else{
                destroy(&dbclient);
                destroy(&dbitem);
                destroy(&dbitem_livrable);
                return;
            } 
    }

    destroy(&dbclient);
    destroy(&dbitem);
    destroy(&dbitem_livrable);

    return;
}

//Permet à un client de consulter son panier

void voir_panier(int id, vector liste_commande){

    printf("\n");
    system("clear");

    if(liste_commande.data != NULL && liste_commande.num_elements != 0){

        FILE *fichierclient_read = fopen("database/clients.csv", "r");
        vector dbclient = lecture_table_clients(fichierclient_read);
        fclose(fichierclient_read);

        iterator iterateur_client = at(&dbclient, id);
        client * client_connecte = (struct client*) iterateur_client.element;

        float total = 0.;
        float livraison = 0.;

        printf("Votre panier : \n\n");

        iterator iterateur_liste = begin(&liste_commande);
        item * item_connecte;

        while(compare(iterateur_liste, end(&liste_commande)) != 0){
            item_connecte = (struct item*) iterateur_liste.element;
            printf("%s (Prix : %.2f euros)\n", item_connecte -> nom, item_connecte -> prix);
            total += item_connecte -> prix;
            increment(&iterateur_liste, 1);
        }

        livraison = total * 0.1;

        printf("\nTotal du panier : %.2f\n", total);
        printf("Frais de livraison : %.2f\n", livraison);
        printf("Votre solde actuel : %.2f\n\n", client_connecte -> solde);

        destroy(&dbclient);
    }
    else{
        printf("Votre panier est vide...\n");
    }

    return;
}

//Permet à un client de supprimer un item

void supprimer_item_commande(int id, vector * liste_commande){

    FILE * fichierlog = fopen("log.txt", "a+");

    printf("\n");
    system("clear");

    voir_panier(id, *liste_commande);

    iterator iterateur_item = begin(liste_commande);
    item * item_connecte;

    if(liste_commande -> data != NULL && liste_commande -> num_elements != 0){

        char nom_item[TAILLE_NOM];

        printf("Entrez un item que vous voulez supprimer de votre commande: ");
        scanf("\n%127[^\n]", nom_item);

        int val = 1;

        while(compare(iterateur_item, end(liste_commande)) != 0 && (val == 1)){
            item_connecte = (struct item*) iterateur_item.element;
            if(compare_char(item_connecte -> nom, nom_item) == 1){

                fprintf(fichierlog, "Le client d'index %i a supprimé l'item %s de son panier.\n", id, item_connecte -> nom);
                fclose(fichierlog);
                
                erase(liste_commande, at(liste_commande, item_connecte -> id));
                val = 0;
            }
            increment(&iterateur_item, 1);
        }
        if(val != 0){

            printf("\n");
            system("clear");

            int choix;
            printf("L'item que vous avez entré n'existe pas.\nSi vous voulez supprimer un item à votre commande entrez 1 et si vous voulez arrêter entrez 0\n\n");
            scanf("%i", &choix);
            if(choix == 1){
                supprimer_item_commande(id, liste_commande);
            }
            else return;
        }
    }

    return;
}

float total_commande(vector liste_commande){

    iterator iterateur_item = begin(&liste_commande);
    item * item_connecte;

    float total = 0.;

    if(liste_commande.data != NULL && liste_commande.num_elements != 0){

        while(compare(iterateur_item, end(&liste_commande)) != 0){
            item_connecte = (struct item*) iterateur_item.element;
            total += item_connecte -> prix;
            increment(&iterateur_item, 1);
        }
    }
    return total;
}

void passer_commande(int id, vector *liste_commande){

    FILE * fichierlog = fopen("log.txt", "a+");

    vector liste_resto = voir_qui_liste_restau(id);
    iterator iterateur_resto = begin(&liste_resto);

    printf("\n");
    system("clear");

    voir_panier(id, *liste_commande);

    iterator iterateur_item = begin(liste_commande);

    FILE *fichierclient_read = fopen("database/clients.csv", "r");
    vector dbclient = lecture_table_clients(fichierclient_read);
    fclose(fichierclient_read);

    FILE *fichierlivreur_read = fopen("database/livreurs.csv", "r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur_read);
    fclose(fichierlivreur_read);

    FILE *fichieritem_read = fopen("database/items.csv", "r");
    vector dbitem = lecture_table_items(fichieritem_read);
    fclose(fichieritem_read);

    iterator iterateur_client = at(&dbclient, id);
    client * client_connecte = (struct client*) iterateur_client.element;

    iterator iterateur_livreur = begin(&dblivreur);
    livreur * livreur_connecte;

    float prix_total = total_commande(*liste_commande) + (total_commande(*liste_commande) * 0.1);

    if(liste_commande -> data != NULL && liste_commande -> data != 0){
        if(client_connecte -> solde >= prix_total){
            while(compare(iterateur_item, end(liste_commande)) != 0){
                item * item_connecte = (struct item*) iterateur_item.element;
                int val = 1;
                while(compare(iterateur_livreur, end(&dblivreur)) != 0){
                    livreur_connecte = (struct livreur*) iterateur_livreur.element;
                    int res = 1;
                    for(int i = 0; i < livreur_connecte -> nb_deplacements && res == 1; i++){
                        if(compare_char(client_connecte -> code_postal, livreur_connecte ->deplacements[i]) == 1 ){
                            res = 0;
                        }
                    }
                    if (res == 0){
                        while(compare(iterateur_resto, end(&liste_resto)) != 0 && val == 1){
                            for(int i = 0; i < livreur_connecte -> nb_deplacements; i++){
                                restaurant * resto_connecte = (struct restaurant*) iterateur_resto.element;
                                if(compare_char(resto_connecte -> code_postal, livreur_connecte ->deplacements[i]) == 1){
                                    for(int j = 0; j < resto_connecte -> nb_menu; j++){
                                        iterator iterateur_tous_item = at(&dbitem, resto_connecte -> menu[j]);
                                        if(iterateur_tous_item.element != NULL){
                                            item *item_tous_connecte = (struct item*) iterateur_tous_item.element;
                                            if(compare_char(item_tous_connecte -> nom, item_connecte -> nom) == 1){
                                                debiter_solde_client(id, item_connecte -> prix);
                                                debiter_solde_client(id, (item_connecte -> prix) * 0.1);
                                                crediter_solde_restaurant(resto_connecte -> id, item_connecte -> prix);
                                                crediter_solde_livreur(livreur_connecte -> id, (item_connecte -> prix) * 0.1);
                                            }
                                        }
                                    }
                                }
                            }
                            increment(&iterateur_resto, 1); 
                        }
                        iterateur_resto = begin(&liste_resto);
                    }
                    increment(&iterateur_livreur, 1);
                }
                iterateur_livreur = begin(&dblivreur);
                increment(&iterateur_item, 1);
            }
            printf("Votre solde a été débité de %.2f euros.\n", prix_total);

            fprintf(fichierlog, "Le client %s a passé une commande de %.2f euros.\n", client_connecte -> nom, prix_total);
            fclose(fichierlog);

            destroy(&dbclient);
            destroy(&dblivreur); 
            destroy(&dbitem);
            destroy(liste_commande);   
        }
        else{
            printf("Votre solde n'est pas suffisant pour passer la commande.");
            return;
        }
    }
}


////INTERFACE/////

int menu_client(vector *liste_commande){

    int again = 1;
    while(again == 1){

        printf("\n");
        system("clear");
        
        int compare;



        printf("* Menu Client *\n\nVous voulez :\n1. Vous connecter à un compte\n2. Créer un nouveau compte\n\nVotre choix ('p' pour retourner au menu principal) : ");

        char operation;
        

        operation = getchar();
        operation = getchar();

    

        switch(operation)
        {
            case '1':
                if (access("database/clients.csv", F_OK) == 0){
                    index_client = connecter_compte_client();
                    if(index_client != 0)    compare = menu_client_compte(liste_commande);
                    else    compare = 1;
                    if(compare == 1)   menu_client(liste_commande);
                }
                else{
                    printf("\n");
                    system("clear");

                    printf("Aucun compte enregistré, impossible de se connecter.\nVous allez être redirigé au menu principal.\n\nLoading...\n");
                    sleep(4);
                    menu_client(liste_commande);
                }
                again = 0;
                break;
            case '2':
                index_client = creer_compte_client();
                if(index_client != 0)    compare = menu_client_compte(liste_commande);
                else    compare = 1;
                if(compare == 1)   menu_client(liste_commande);
                again = 0;
                break;
            case 'p':
                again = 0;
                break;
            default :
                again = 1;
        }
    }
    return 0;
}

int menu_client_compte(vector *liste_commande){
    printf("\n");
    system("clear");
    
    printf("* Menu Client *\n\nVous voulez :\n1. Supprimer votre compte\n2. Modifier votre compte\n3. Consulter votre solde\n4. Voir des restaurants ou des items\n");
    printf("5. Gérer vos commandes\n\nVotre choix ('q' pour quitter, 'd' pour se déconnecter) : ");
    int where;
    int again = 1;

    char operation = getchar();
    switch(operation)
    {  
        case '1':
            supprimer_compte_client(index_client);
            where = 1;
            break;
        case '2':
            modifier_compte_client(index_client);
            where = menu_client_compte(liste_commande);
            break;
        case '3': 
            while(again == 1){
                printf("\n");
                system("clear");

                printf("* Menu Client *\n\n* Solde *\n\n");

                consulter_solde_client(index_client);

                printf("\nCréditer votre compte ('c' pour créditer, 'q' pour quitter) : ");

                char operation2;
                operation2 = getchar();
                operation2 = getchar();
                switch(operation2)
                {
                    case 'c':
                        crediter_solde_client(index_client);
                        again = 1;
                        break;
                    case 'q':
                        again = 0;
                        break;
                    default:
                        again = 1;
                }
            }
            where = menu_client_compte(liste_commande);
            break;
            

        case '4':
            while(again == 1){
                printf("\n");
                system("clear");

                printf("* Menu Client *\n\n* Liste *\n\nVoulez-vous :\n1. Voir des restaurants\n2. Voir des items\n\nVotre choix ('q' pour quitter) : ");

                char operation3 = getchar();
                switch(operation3)
                {
                    case '1':
                        restreindre_liste_restau(index_client);
                        printf("Appuyez sur une entrée pour revenir à la page d'accueil de votre compte.\n");
                        getchar();
                        getchar();
                        again = 0;
                        break;
                    case '2':
                        restreindre_liste_item(index_client);
                        printf("Appuyez sur une entrée pour revenir à la page d'accueil de votre compte.\n");
                        getchar();
                        getchar();
                        again = 0;
                        break;
                    case 'q':
                        again = 0;
                        break;
                    default:
                        again = 1;
                }
            }
            where = menu_client_compte(liste_commande);
            break;
        case '5':
            while(again == 1){
                printf("\n");
                system("clear");

                printf("* Menu Client *\n\n* Commande *\n\n");
                printf("Voulez-vous :\n1. Voir votre panier\n2. Ajouter un item\n3.Supprimer un item\n4.Passer commande\n\nVotre choix ('q' pour quitter) : ");

                char operation4 = getchar();
                switch(operation4)
                {
                    case '1':
                        voir_panier(index_client, *liste_commande);
                        printf("\nAppuyez sur une entrée pour revenir à la page d'accueil de votre compte.\n");
                        getchar();
                        getchar();
                        again = 0;
                        break;
                    case '2':
                        ajouter_item_commande(index_client, liste_commande);
                        again = 0;
                        break;
                    case '3':
                        supprimer_item_commande(index_client, liste_commande);
                        again = 0;
                        break;
                    case '4':
                        passer_commande(index_client, liste_commande);
                        printf("Appuyez sur entrée pour revenir à la page d'accueil de votre compte.\n");
                        getchar();
                        getchar();
                        again = 0;
                        break;
                    case 'q':
                        again = 0;
                        break;
                    default:
                        again = 1;
                }
            }
            where = menu_client_compte(liste_commande);
            break;
        case 'd':
            index_client = 0;
            where = 1;
            break;
        case 'q':
            index_client = 0;
            where = 0;
            break;
        default :
            where = menu_client_compte(liste_commande);
        }
    return where;
}


