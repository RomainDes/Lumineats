#include <stdio.h>
#include <stdlib.h>
#include "livreur.h"
#include "restaurant.h"

//Permet a un livreur de se connecter a son compte, renvoie l'id du compte ou on se 
//connecte si on y parvient, 0 sinon
int connecter_compte_livreur(){
    //On demande au livreur son nom, puis on ouvre la db livreur, on demande ensuite
    //a l'utilisateur d'entrer son mot de passe et on verifie si il correspond au mdp
    //present dans la db a la ligne correspondant au nom entré.
    //Si nom entré pas dans la db on redemande, idem pour mdp si il correspond pas
    //On laisse 3 essais pour rentrer le mot de passe
    int valid = 0;
    char username[TAILLE_NOM];
    char password[TAILLE_MDP];
    livreur* livreur_dans_bd;
    FILE * fichierlivreur;
    iterator actu, fin;
    //int essai = 3;
    int res;

    printf("\n");
    system("clear");

    printf("* Menu Livreur *\n\n* Se connecter à un compte *\n\n");

    fichierlivreur = fopen("database/livreurs.csv", "r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    printf("Entrez votre nom d'utilisateur ('q' pour quitter): ");
    scanf("\n%127[^\n]", username);
    //On demande le nom d'utilisateur et virifie qu'il est bien dans la db
    while (valid == 0){

        

        if(strcmp(username, "q") == 0) return 0;//quitter

        actu = begin(&dblivreur);
        fin = end(&dblivreur);

        while(actu.element != fin.element && valid == 0){
            livreur_dans_bd = (struct livreur*) actu.element;
            if (compare_char(livreur_dans_bd->nom, username) == 1){
                valid = 1;
            }
            else{
                increment(&actu, 1);
            }
        }
        //A la fin de cette boucle, on aura dans livreur_dans_db la struct livreur correspondant au compte auquel on veut
        //se connecter, on aura donc plus besoin de la chercher
        if(valid == 0){
            printf("Nom invalide, veuillez réessayer ('q' pour quitter) : ");
            scanf("\n%127[^\n]", username);
        }
    }
    //On remet valid a 0 pour le test du mot de passe
    valid = 0;
    printf("Entrez votre mot de passe ('q' pour quitter) : ");
    scanf("\n%127[^\n]", password);
    while(valid == 0){
        
        if(strcmp(password, "q") == 0) return 0;//quitter

        if (compare_char(livreur_dans_bd->mdp, password) == 1){
            valid = 1;
        }
        else{
            printf("Mot de passe invalide, veuillez réessayer ('q' pour quitter) : ");
            scanf("\n%127[^\n]", password);
        }
    }
    //On recupere l'id du compte si on a entré le bon mdp
    if (valid == 1){
        res = livreur_dans_bd->id;
    }
    else{
        res = 0;
    }

    fclose(fichierlivreur); 

    FILE* flog = fopen("log.txt","a+");
    fprintf(flog,"Le livreur %s s'est connecte a son compte (id %i)",username,res);
    fclose(flog);  
    return res;
}

//Permet a un livreur de se creer un compte en entrant toutes ses informations
int creer_compte_livreur(){
    //On commence par creer une structure livreur.
    //On fait entrer a l'utilisateur les valeurs de nom,mdp,tel et 
    //on cree un char* dans lequel l'utilisateur va rentrer
    //les differents codes postaux ou il livre
    char cp_actu[6];
    char nom_restau[TAILLE_NOM];
    int cpt, check, valid;
    livreur nouv_livreur;
    livreur* livreur_actuel;
    FILE * fichierrestau;
    iterator actu, fin;
    restaurant* restau_actuel;
    int trouve = 0, nouvfichier;
    livreur* dernier_livreur;
    printf("\n");
    system("clear");

    printf("* Menu Livreur *\n\n* Créer un compte *\n\n");

    if(access("database/restaurants.csv", F_OK) == -1 ){
        FILE *creer_fichier_restaurant = fopen("database/restaurants.csv","w+");
        fclose(creer_fichier_restaurant);
    }
    

    fichierrestau = fopen("database/restaurants.csv", "r");
    vector dbrestau = lecture_table_restaurants(fichierrestau);


    if(access("database/livreurs.csv", F_OK) == -1 ){
        FILE *creer_fichier_livreur = fopen("database/livreurs.csv","w+");
        fclose(creer_fichier_livreur);
        nouvfichier = 1;
    }
    else{
        nouvfichier = 0;
    }
    
    FILE *fichierlivreur = fopen("database/livreurs.csv", "r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);
    fclose(fichierlivreur);

    

    valid = 0;
    while(valid == 0){
        printf("Entrez votre nom : ");
        scanf("\n%127[^\n]", nouv_livreur.nom);
        //On verifie que le nom n'est pas deja présent dans la bd
        actu = begin(&dblivreur);
        fin = end(&dblivreur);
        valid = 1;
        while (actu.element != fin.element && valid == 1){
            livreur_actuel = (struct livreur*) actu.element;
            if (livreur_actuel->nom == nouv_livreur.nom){
                valid = 0;
            }
        increment(&actu, 1);
        }
    }
    
    printf("Entrez votre mot de passe : ");
    scanf("\n%127[^\n]", nouv_livreur.mdp);

    printf("Entrez votre numéro de téléphone (XX XX XX XX XX): ");
    scanf("\n%127[^\n]", nouv_livreur.tel);

    cpt = 0;
    do{
        printf("Entrez un code postal où vous pouvez livrer ('0' pour arreter) : ");
        scanf("\n%127[^\n]", cp_actu);
        if (compare_char(cp_actu, "0") != 1){
            nouv_livreur.deplacements[cpt]=malloc(sizeof(cp_actu));
            strcpy(nouv_livreur.deplacements[cpt], cp_actu);
            cpt += 1;
        }
    }while(compare_char(cp_actu, "0") != 1 && cpt<MAX_CP);
    nouv_livreur.nb_deplacements = cpt;

    printf("Dépendez-vous d'un restaurant ('1' si oui, '0' sinon) : ");
    scanf("%d[^\n] \n", &check);
    if(check != 0){
        printf("Entrez le nom du restaurant dont vous dépendez : ");
        scanf("\n%127[^\n]", nom_restau);
        while (trouve == 0){
            //Recherche de ce nom dans la bd et recuperation de l'id
            actu = begin(&dbrestau);
            fin = end(&dbrestau);
            while(actu.element != fin.element && trouve == 0){
                restau_actuel = (struct restaurant*) actu.element;
                if (compare_char(restau_actuel->nom,nom_restau) == 1){
                    trouve = 1;
                    nouv_livreur.restaurant = restau_actuel->id;
                }
                increment(&actu, 1);
            }
            if (trouve == 0){
                printf("Erreur, restaurant introuvable. Veuillez réessayer ('s' si vous ne trouvez pas le restaurant) : ");
                scanf("\n%127[^\n]", nom_restau);
            }
            if(compare_char(nom_restau, "s") == 1){
                printf("Vous ne dépendez d'aucun restaurant.\n\n");
                nouv_livreur.restaurant = 0;
                trouve=1;
            }
        }
    }
    else{
        nouv_livreur.restaurant = 0;
    }

    nouv_livreur.solde = 0.00;

    if(nouvfichier == 1){
        nouv_livreur.id = 1;
        clear(&dblivreur);
    }
    else{
        increment (&fin, -1);
        dernier_livreur = (struct livreur*)fin.element;
        nouv_livreur.id = dernier_livreur->id + 1;
    }
    int index_livreur;
    printf("\nConfirmer la création de votre compte ('y' pour valider, 'r' pour recommencer, 'q' pour quitter) : ");
    char operation;

    operation = getchar();
    operation = getchar();
    
    switch(operation)
    {   
        case 'y':
            push_back(&dblivreur, &nouv_livreur);

            FILE* ecriturelivreur = fopen("database/livreurs.csv", "w");
            ecriture_table_livreurs(ecriturelivreur, &dblivreur);
            fclose(ecriturelivreur);
            

            FILE* flog = fopen("log.txt","a+");
            fprintf(flog,"Le livreur %s a créé un nouveau compte (id %i)",nouv_livreur.nom, (int)nouv_livreur.id);
            fclose(flog);
                
            index_livreur = nouv_livreur.id;
            break;
        case 'r':
            if(nouvfichier == 1)  remove("database/livreurs.csv");
            index_livreur = creer_compte_livreur();
            break;
        case 'q':
            if(nouvfichier == 1)  remove("database/livreurs.csv");
            index_livreur = 0;
            break;
    } 

    destroy(&dblivreur);
    destroy(&dbrestau);
    fclose(fichierrestau);
    


    return index_livreur;
}

//Permet a un liveur de supprimer son compte et toutes les information y etant contenues
int supprimer_compte_livreur(int id){
    //On ouvre le fichier csv des livreurs, on cherche l'index du compte ou on est 
    //connecté et on supprime la ligne dans le fichier csv
    //Ensuite, on fait "remonter" les lignes d'apres en réduisant tous leurs indexs
    //de 1
    printf("\n");
    system("clear");

    printf("* Menu Livreur *\n\n* Supprimer votre compte *\n\n");

    printf("Confirmer la suppression du compte ('y' pour valider, 'r' pour refuser) : ");
    char choix;
    scanf("\n%c", &choix);
    if(choix == 'y'){

        FILE* fichierlivreur;
        fichierlivreur = fopen("database/livreurs.csv","r");
        vector dblivreur = lecture_table_livreurs(fichierlivreur);
        fclose(fichierlivreur);


        iterator iterateur;
        
        int trouve = 0;
        livreur * livreur_dans_bd;

        iterateur = begin(&dblivreur);

        while(trouve == 0){
            livreur_dans_bd = (struct livreur*) iterateur.element;
            if(livreur_dans_bd->id == id){
                trouve = 1;
            }
            else{
                increment(&iterateur, 1);
            }
        }

        if(dblivreur.num_elements == 1){
            remove("database/livreurs.csv");
        }
        else{
            erase(&dblivreur, iterateur);
            FILE* ecriturelivreur = fopen("database/livreurs.csv","w");
            ecriture_table_livreurs(ecriturelivreur, &dblivreur);
            fclose(ecriturelivreur);
        }
        

       

        FILE* flog = fopen("log.txt","a+");
        fprintf(flog,"Le compte livreur d'id %i a supprimé son compte", id);
        fclose(flog); 

        
        destroy(&dblivreur);

        printf("Le compte a été supprimé.\nLoading...\n");
        sleep(4);
    }
    else if(choix == 'r'){
        printf("Compte non supprimé.\nLoading...\n");
        sleep(4);
        return 0;//retourne 0 pour dire que le compte n'a pas été supprimé
    }
    else{
        return supprimer_compte_livreur(id);
    }
    return 1;
}

//Permet a un livreur de modifier les cp ou il lui est possible de livrer
void modifier_cp_livreur(int id){
    //On ouvre le fichier csv des livreurs, on récupere la ligne correspondant a 
    //l'index du compte ou on est connecté, on récupere la liste
    //des codes postaux.
    //On remplace avec la nouvelle liste de ses codes postaux

    printf("\n");
    system("clear");

    printf("* Menu Livreur *\n\n* Modifier les codes postaux *\n\n");

    FILE* fichierlivreur;
    fichierlivreur = fopen("database/livreurs.csv","r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);
    fclose(fichierlivreur);

    int cpt;
    char* cp_actu;
    livreur* livreur_connecte;
    iterator actu;
    int trouve = 0;

    actu = begin(&dblivreur);

    while(trouve == 0){
        livreur_connecte = (struct livreur*) actu.element;
        if(livreur_connecte->id == id){
            trouve = 1;
        }
        else{
            increment(&actu, 1);
        }
    }

    cp_actu = malloc(6*sizeof(char));
    cpt = 0;
    printf("Entrez les codes postaux où vous pouvez livrer ('0' pour arreter) : \n\n");
    do{
        printf("- Code postal n°%i (XXXXX) : ", cpt+1);
        scanf("\n%127[^\n]", cp_actu);
        if (compare_char(cp_actu, "0") != 1){
            livreur_connecte->deplacements[cpt]=malloc(sizeof(cp_actu));
            strcpy(livreur_connecte->deplacements[cpt], cp_actu);
            cpt += 1;
        }
    }while(compare_char(cp_actu, "0") != 1 && cpt<MAX_CP);
    livreur_connecte->nb_deplacements = cpt;

    set(actu, (void *)livreur_connecte);
    FILE*ecriturelivreur = fopen("database/livreurs.csv","w");
    ecriture_table_livreurs(ecriturelivreur, &dblivreur);

    FILE* flog = fopen("log.txt","a+");
    fprintf(flog,"Le livreur d'id %i a modifié la liste de ses codes postaux", id);
    fclose(flog); 

    fclose(ecriturelivreur);
    destroy(&dblivreur);

    printf("\nVotre compte a été modifié.\n\nLoading...\n");
    sleep(4);
    return;
}

//Permet a un livreur de modifier son numero de telephone 
void modifier_tel_livreur(int id){

    printf("\n");
    system("clear");

    printf("* Menu Livreur *\n\n* Modifier le numéro de téléphone *\n\n");


    //Idem qu'au dessus sauf qu'on modifie le string du telephone
    FILE* fichierlivreur;
    fichierlivreur = fopen("database/livreurs.csv","r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);
    fclose(fichierlivreur);

    livreur* livreur_connecte;
    iterator actu;
    int trouve = 0;

    actu = begin(&dblivreur);

    while(trouve == 0){
        livreur_connecte = (struct livreur*) actu.element;
        if(livreur_connecte->id == id){
            trouve = 1;
        }
        else{
            increment(&actu, 1);
        }
    }

    printf("Entrez votre nouveau numéro de téléphone (XX XX XX XX XX): ");
    scanf("\n%127[^\n]", livreur_connecte->tel);

    set(actu, (void *)livreur_connecte);
    FILE* ecriturelivreur = fopen("database/livreurs.csv","w");
    ecriture_table_livreurs(ecriturelivreur, &dblivreur);

    FILE* flog = fopen("log.txt","a+");
    fprintf(flog,"Le livreur d'id %i a modifié son numéro de téléphone", id);
    fclose(flog); 

    fclose(ecriturelivreur);
    destroy(&dblivreur);

    printf("\nVotre compte a été modifié.\n\nLoading...\n");
    sleep(4);

    return;
}


//Permet a un livreur de modifier ou retirer son exclusivité à un restaurateur
void modifier_resto_livreur(int id){
    printf("\n");
    system("clear");

    printf("* Menu Livreur *\n\n* Modifier le restaurant affilié *\n\n");

    //On commence par faire la meme chose qu'au dessus
    //Ensuite on demande si on veut retirer ou changer l'exclu
    //Si on retire, on change la valeur a 0, et sinon on demande a l'utilisateur
    //de rentrer le nom du resto, on cherche ce nom sur la bd des restos et on
    //recup l'id de ce resto
    FILE* fichierlivreur;
    fichierlivreur = fopen("database/livreurs.csv","r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);
    fclose(fichierlivreur);

    FILE* fichierresto;
    fichierresto = fopen("database/restaurants.csv","r");
    vector dbrestau = lecture_table_restaurants(fichierresto);
    fclose(fichierresto);

    iterator actu, fin;
    char nom_restau[TAILLE_NOM];
    int check = 0, trouve = 0;
    restaurant* restau_actuel;
    livreur* livreur_connecte;
    iterator iterateur;

    iterateur = begin(&dblivreur);

    while(trouve == 0){
        livreur_connecte = (struct livreur*) iterateur.element;
        if(livreur_connecte->id == id){
            trouve = 1;
        }
        else{
            increment(&iterateur, 1);
        }
    }

    //On remet trouve a 0 pour la recherche du resto
    trouve = 0;
    printf("Dépendez-vous d'un restaurant ('1' si oui, '0' sinon) : ");
    scanf("%d[^\n] \n", &check);
    if(check == 1){
        printf("Entrez le nom du restaurant dont vous dépendez : ");
        scanf("\n%127[^\n]", nom_restau);
        while(trouve == 0){
            //Recherche de ce nom dans la bd et recuperation de l'id
            actu = begin(&dbrestau);
            fin = end(&dbrestau);
            while(actu.element != fin.element && trouve == 0){
                restau_actuel = (struct restaurant*) actu.element;
                if (compare_char(restau_actuel->nom, nom_restau) == 1){
                    trouve = 1;
                    livreur_connecte->restaurant = restau_actuel->id;
                }
                increment(&actu, 1);
            }
            if (trouve == 0){
                printf("Erreur, restaurant introuvable. Veuillez réessayer ('s' si vous ne trouvez pas le restaurant) : ");
                scanf("\n%127[^\n]", nom_restau); 
            }
            if(compare_char(nom_restau, "s") == 1){
                printf("Vous ne dépendez d'aucun restaurant.\n\n");
                livreur_connecte->restaurant = 0;
                trouve=1;
            }
        }    
    }
    else{
            livreur_connecte->restaurant = 0;
    }
    set(iterateur, (void *)livreur_connecte);
    FILE* ecriturelivreur = fopen("database/livreurs.csv","w");
    ecriture_table_livreurs(ecriturelivreur, &dblivreur);

    FILE* flog = fopen("log.txt","a+");
    fprintf(flog,"Le livreur d'id %i a modifié sa dépendance à un restaurant",id);
    fclose(flog); 

    fclose(ecriturelivreur);
    destroy(&dblivreur);
    destroy(&dbrestau);

    printf("\nVotre compte a été modifié.\n\nLoading...\n");
    sleep(4);

    return;
}

//Permet a un livreur de modifier les differents elements vu ci dessus
void modifier_compte_livreur(int id){
    //On combine les trois fonctions au dessus en demandant a l'utilisateur ce qu'il
    //souhaite  modifier
    printf("\n");
    system("clear");

    printf("* Menu Livreur *\n\n* Modifier compte *\n\n");
    char value;
    printf("Vous voulez :\n1. Modifier les codes postaux de livraison\n2. Modifier votre numéro de téléphone\n3. Modifier le restaurant affilié\n\nVotre choix ('q' pour quitter) : ");
    scanf("%c",&value);

    switch (value)
    {
    case '1':
        modifier_cp_livreur(id);
        break;
    case '2':
        modifier_tel_livreur(id);
        break;
    case '3':
        modifier_resto_livreur(id);
        break;
    case 'q':
        break;
    default:
        modifier_compte_livreur(id);
        break;
    }

    return;
}


//Permet à un liveur de consulter la somme d'argent qu'il a sur son solde 
void consulter_solde_livreur(int id){
    //On ouvre la bd livreur et on recup la valeur de la solde a la ligne du compte
    //auquel on est connecté

    printf("\n");
    system("clear");

    printf("* Menu Livreur *\n\n* Consulter Solde *\n\n");


    FILE* fichierlivreurs = fopen("database/livreurs.csv","r");
    vector dblivreur = lecture_table_livreurs(fichierlivreurs);
    fclose(fichierlivreurs);
    
    iterator iterateur;
    livreur * livreur_dans_bd;
    int trouve = 0;

    iterateur = begin(&dblivreur);

    while(trouve == 0){
        livreur_dans_bd = (struct livreur*) iterateur.element;
        if(livreur_dans_bd->id == id){
            trouve = 1;
        }
        else{
            increment(&iterateur, 1);
        }
    }

    FILE* flog = fopen("log.txt","a+");
    fprintf(flog,"Le livreur d'id %i a consulté son solde", id);
    fclose(flog); 
    
    destroy(&dblivreur);
    printf("Solde : %.2f ('q' pour quitter) : ", livreur_dans_bd->solde);

    char quite =' ';
    while( quite != 'q'){
        scanf("%c", &quite);
    }    

    return;
}

//----Fonctions de l'interface----//

int gestion_compte_livreur(int idlivreur){
    printf("\n");
    system("clear");
    
    //La valeur de quite sera à 1 si on souhaite quitter l'application
    int quite = 0;
    char choice;

    printf("* Menu Livreur *\n\nVous voulez :\n1. Supprimer votre compte\n2. Modifier votre compte\n3. Consulter votre solde\n\nVotre choix ('q' pour quitter, 'd' pour se déconnecter) : ");
    choice = getchar();

    switch(choice){
        //On recupere l'id du livreur dans les deux cas pour appeler la fonction de gestion de compte
        case '1':
            if(supprimer_compte_livreur(idlivreur) == 0){
                quite = gestion_compte_livreur(idlivreur);
            }
            break;
        case '2':
            modifier_compte_livreur(idlivreur);
            quite = gestion_compte_livreur(idlivreur);
            break;
        case'3':
            consulter_solde_livreur(idlivreur);
            quite = gestion_compte_livreur(idlivreur);
            break;
        case 'q':
            quite = 1;
            break;
        case 'd':
            break;
        default :
            //Si une valeur differente est entree, on renvoie sur le meme menu
            quite = gestion_compte_livreur(idlivreur);
    }
    return quite;
}

int menu_livreur(){
    //On fait le vide dans la fenetre d'interface
    printf("\n");
    system("clear");

    int idlivreur,quite;
    char choice;

    printf("* Menu Livreur *\n\nVous voulez :\n1. Vous connecter à un compte\n2. Créer un nouveau compte\n\nVotre choix ('q' pour quitter) : ");
    choice = getchar();

    switch(choice){
        //On recupere l'id du livreur dans les deux cas pour appeler la fonction de gestion de compte
        case '1':
            //On verifie qu'il existe des livreurs avant de tenter de se connecter
            if (access("database/livreurs.csv", F_OK) == 0){
                idlivreur = connecter_compte_livreur();
                if(idlivreur == 0){
                    menu_livreur();
                }
                else{
                    quite = gestion_compte_livreur(idlivreur);
                    if(quite == 0){
                        menu_livreur();
                    }
                }
            }else{
                printf("\n");
                system("clear");
                printf("* Menu Livreur *\n\n* Se connecter à un compte *\n\n");
                printf("Félicitation, vous êtes la premiere personne sur cette application, veuillez créer un compte !\n\nLoading...\n");
                sleep(4);
                menu_livreur();
            }
            break;
        case '2':
            idlivreur = creer_compte_livreur();
            if(idlivreur == 0){
                    menu_livreur();
            }
            else{
                quite = gestion_compte_livreur(idlivreur);
                //Selon les choix de l'utilisateur dans gestion_compte_livreur, on le fait revenir a ce menu ou non
                if(quite == 0){
                    menu_livreur();
                }
            }
            break;
        case 'q':
            break;
        default :
            //Si une valeur differente est entree, on renvoie sur le meme menu
            menu_livreur();
    }


    return 0;
}


