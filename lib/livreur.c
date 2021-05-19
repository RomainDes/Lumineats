#include <stdio.h>
#include "livreur.h"
#include "restaurant.h"

//Fonction utilitaire permettant de trouver dans la base de donnee
//les informations du compte avec l'id id
iterator trouver_livreur_avec_id(int id){
    FILE* fichierlivreur;
    fichierlivreur = fopen("./database/livreur.csv", "r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    iterator actu;
    int trouve = 0;
    livreur * livreur_dans_bd;

    actu = begin(&dblivreur);

    while(trouve == 0){
        livreur_dans_bd = (struct livreur*) actu.element;
        if(livreur_dans_bd->id == id){
            trouve = 1;
        }
        increment(&actu, 1);
    }

    fclose(fichierlivreur);
    return actu;
}

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
    int essai = 3;
    int res;

    fichierlivreur = fopen("./database/livreur.csv", "r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    //On demande le nom d'utilisateur et virifie qu'il est bien dans la db
    while (valid == 0){

        printf("Veuillez entrer votre nom d'utilisateur : ");
        scanf("%s \n", username);

        actu = begin(&dblivreur);
        fin = end(&dblivreur);

        while(actu.element != fin.element && valid == 0){
            livreur_dans_bd = (struct livreur*) actu.element;
            if (livreur_dans_bd->nom == username){
                valid = 1;
            }
            else{
                increment(&actu, 1);
            }
        }
        //A la fin de cette boucle, on aura dans livreur_dans_db la struct livreur correspondant au compte auquel on veut
        //se connecter, on aura donc plus besoin de la chercher
        if(valid == 0){
            printf("Nom d'utilisateur invalide, veuillez réessayer.\n");
        }
    }
    //On remet valid a 0 pour le test du mot de passe
    valid = 0;
    while(essai > 0 && valid == 0){
        printf("Veuillez entrer votre mot de passe. Il vous reste %i essais\n", essai);
        scanf("%s \n", password);
        if (livreur_dans_bd->mdp == password){
            valid = 1;
        }
        else{
            printf("Mot de passe incorrect, veuillez reessayer.\n");
            essai -= 1;
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
    return res;
}

//Permet a un livreur de se creer un compte en entrant toutes ses informations
void creer_compte_livreur(){
    //On commence par creer une structure livreur.
    //On fait entrer a l'utilisateur les valeurs de nom,mdp,tel et 
    //on cree un char* dans lequel l'utilisateur va rentrer
    //les differents codes postaux ou il livre
    char* cp_actu;
    char* nom_restau;
    int cpt, check, valid;
    livreur nouv_livreur;
    livreur* livreur_actuel;
    FILE * fichierlivreur;
    FILE * fichierrestau;
    iterator actu, fin;
    restaurant* restau_actuel;
    int trouve = 0;
    livreur* dernier_livreur;

    fichierlivreur = fopen("./database/livreur.csv", "r+");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    fichierrestau = fopen("./database/restaurants.csv", "r");
    vector dbrestau = lecture_table_restaurants(fichierrestau);

    valid = 0;
    while(valid == 0){
        printf("Veuillez entrer votre nom : ");
        scanf("%s \n", nouv_livreur.nom);
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
    

    printf("Veuillez entrer votre mot de passe : ");
    scanf("%s \n", nouv_livreur.mdp);

    printf("Veuillez entrer votre numero de telephone (En entrant les espaces): ");
    scanf("%s \n", nouv_livreur.tel);

    cpt = 0;
    cp_actu = "1";
    while(cp_actu != 0 && cpt<MAX_CP){
        printf("Veuillez entrer un code postal ou vous pouvez livrer (Entrez 0 pour arreter");
        scanf("%s \n", cp_actu);
        if (cp_actu != 0){
            nouv_livreur.deplacements[cpt]=cp_actu;
            cpt += 1;
        }
    }

    nom_restau = "";
    printf("Dependez vous d'un restaurant ? Si oui entrez 1 sinon entrez 0 :");
    scanf("%d \n", &check);
    if(check){
        while(trouve == 0){
            printf("Entrez le nom du restaurant dont vous dependez : ");
            scanf("%s \n", nom_restau);

            //Recherche de ce nom dans la bd et recuperation de l'id
            actu = begin(&dbrestau);
            fin = end(&dbrestau);
            cpt = 0;
            while(actu.element != fin.element && trouve == 0){
                restau_actuel = (struct restaurant*) actu.element;
                if (restau_actuel->nom == nom_restau){
                    trouve = 1;
                    nouv_livreur.restaurant = restau_actuel->id;
                }
                cpt += 1;
                increment(&actu, 1);
            }
            if (trouve == 0){
                printf("Erreur, restaurant  nom présent dans la base de donnee");
            }
        }
    }
    else{
        nouv_livreur.restaurant = 0;
    }

    nouv_livreur.solde = 0;

    //On finit par ajouter la struct livreur au fichier csv
    //(on l'ajoute a la fin de la db avec index = indexmax + 1)
    
    increment (&fin, -1);
    dernier_livreur = (struct livreur*)fin.element;

    nouv_livreur.id = dernier_livreur->id + 1;
    

    push_back(&dblivreur, &nouv_livreur);
    ecriture_table_livreurs(fichierlivreur, &dblivreur);

    fclose(fichierlivreur);
    fclose(fichierrestau);

    return;
}

//Permet a un liveur de supprimer son compte et toutes les information y etant contenues
void supprimer_compte_livreur(int id){
    //On ouvre le fichier csv des livreurs, on cherche l'index du compte ou on est 
    //connecté et on supprime la ligne dans le fichier csv
    //Ensuite, on fait "remonter" les lignes d'apres en réduisant tous leurs indexs
    //de 1
    FILE* fichierlivreur;
    fichierlivreur = fopen("./database/livreur.csv","r+");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    iterator iterateur;

    iterateur = trouver_livreur_avec_id(id);

    erase(&dblivreur, iterateur);

    return;
}

//Permet a un livreur de modifier les cp ou il lui est possible de livrer
void modifier_cp_livreur(int id){
    //On ouvre le fichier csv des livreurs, on récupere la ligne correspondant a 
    //l'index du compte ou on est connecté, on récupere la liste
    //des codes postaux.
    //On remplace avec la nouvelle liste de ses codes postaux

    FILE* fichierlivreur;
    fichierlivreur = fopen("./database/livreur.csv","r+");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    livreur* livreur_connecte;
    iterator iterateur;
    int cpt;
    char* cp_actu;

    iterateur = trouver_livreur_avec_id(id);
    livreur_connecte = (struct livreur*) iterateur.element; 

    cpt = 0;
    cp_actu = "1";
    while(cp_actu != 0 && cpt<MAX_CP){
        printf("Veuillez entrer un code postal ou vous pouvez livrer (Entrez 0 pour arreter");
        scanf("%s \n", cp_actu);
        if (cp_actu != 0){
            livreur_connecte->deplacements[cpt]=cp_actu;
            cpt += 1;
        }
    }

    set(iterateur, (void *)livreur_connecte);
    ecriture_table_livreurs(fichierlivreur, &dblivreur);

    fclose(fichierlivreur);

    return;
}

//Permet a un livreur de modifier son numero de telephone 
void modifier_tel_livreur(int id){
    //Idem qu'au dessus sauf qu'on modifie le string du telephone
    FILE* fichierlivreur;
    fichierlivreur = fopen("./database/livreur.csv","r+");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    livreur* livreur_connecte;
    iterator iterateur;

    iterateur = trouver_livreur_avec_id(id);
    livreur_connecte = (struct livreur*) iterateur.element; 

    printf("Veuillez entrer votre numero de telephone (En entrant les espaces): ");
    scanf("%s \n", livreur_connecte->tel);

    set(iterateur, (void *)livreur_connecte);
    ecriture_table_livreurs(fichierlivreur, &dblivreur);

    fclose(fichierlivreur);

    return;
}


//Permet a un livreur de modifier ou retirer son exclusivité à un restaurateur
void modifier_resto_livreur(int id){
    //On commence par faire la meme chose qu'au dessus
    //Ensuite on demande si on veut retirer ou changer l'exclu
    //Si on retire, on change la valeur a 0, et sinon on demande a l'utilisateur
    //de rentrer le nom du resto, on cherche ce nom sur la bd des restos et on
    //recup l'id de ce resto
    FILE* fichierlivreur;
    fichierlivreur = fopen("./database/livreur.csv","r+");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    FILE* fichierresto;
    fichierresto = fopen("./database/livreur.csv","r");
    vector dbrestau = lecture_table_restaurants(fichierresto);

    livreur* livreur_connecte;
    iterator iterateur, actu, fin;
    char* nom_restau;
    int check = 0, trouve = 0;
    restaurant* restau_actuel;
    int cpt;

    iterateur = trouver_livreur_avec_id(id);
    livreur_connecte = (struct livreur*) iterateur.element; 

    nom_restau = "";
    printf("Dependez vous d'un restaurant ? Si oui entrez 1 sinon entrez 0 :");
    scanf("%d \n", &check);
    if(check){
        while(trouve == 0){
            printf("Entrez le nom du restaurant dont vous dependez : ");
            scanf("%s \n", nom_restau);

            //Recherche de ce nom dans la bd et recuperation de l'id
            actu = begin(&dbrestau);
            fin = end(&dbrestau);
            cpt = 0;
            while(actu.element != fin.element && trouve == 0){
                restau_actuel = (struct restaurant*) actu.element;
                if (restau_actuel->nom == nom_restau){
                    trouve = 1;
                    livreur_connecte->restaurant = restau_actuel->id;
                }
                cpt += 1;
                increment(&actu, 1);
            }
            if (trouve == 0){
                printf("Erreur, restaurant  nom présent dans la base de donnee");
            }
        }
    }
    else{
        livreur_connecte->restaurant = 0;
    }

    set(iterateur, (void *)livreur_connecte);
    ecriture_table_livreurs(fichierlivreur, &dblivreur);

    fclose(fichierlivreur);
    fclose(fichierresto);

    return;
}

//Permet a un livreur de modifier les differents elements vu ci dessus
void modifier_compte_livreur(int id){
    //On combine les trois fonctions au dessus en demandant a l'utilisateur ce qu'il
    //souhaite  modifier
}

//Permet à un liveur de consulter la somme d'argent qu'il a sur son solde 
void consulter_solde_livreur(int id){
    //On ouvre la bd livreur et on recup la valeur de la solde a la ligne du compte
    //auquel on est connecté

    iterator iterateur;
    livreur * livreur_dans_bd;

    iterateur = trouver_livreur_avec_id(id);

    livreur_dans_bd = (struct livreur*) iterateur.element;
    
    printf("Le solde sue ce compte est de %f euros", livreur_dans_bd->solde);

    return;
}

