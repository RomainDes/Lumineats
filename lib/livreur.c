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
    int essai = 3;
    int res;


    fichierlivreur = fopen("database/livreurs.csv", "r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);

    //On demande le nom d'utilisateur et virifie qu'il est bien dans la db
    while (valid == 0){

        printf("Veuillez entrer votre nom d'utilisateur : ");
        scanf("\n%127[^\n]", username);

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
            printf("Nom d'utilisateur invalide, veuillez réessayer.\n");
        }
    }
    //On remet valid a 0 pour le test du mot de passe
    valid = 0;
    while(essai > 0 && valid == 0){
        printf("Veuillez entrer votre mot de passe. Il vous reste %i essais : ", essai);
        scanf("\n%127[^\n]", password);
        if (compare_char(livreur_dans_bd->mdp, password) == 1){
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
        printf("Veuillez entrer votre nom : ");
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
    
    printf("Veuillez entrer votre mot de passe : ");
    scanf("\n%127[^\n]", nouv_livreur.mdp);

    printf("Veuillez entrer votre numero de telephone (En entrant les espaces): ");
    scanf("\n%127[^\n]", nouv_livreur.tel);

    cpt = 0;
    do{
        printf("Veuillez entrer un code postal ou vous pouvez livrer (Entrez 0 pour arreter) : ");
        scanf("\n%127[^\n]", cp_actu);
        if (compare_char(cp_actu, "0") != 1){
            nouv_livreur.deplacements[cpt]=malloc(sizeof(cp_actu));
            strcpy(nouv_livreur.deplacements[cpt], cp_actu);
            cpt += 1;
        }
    }while(compare_char(cp_actu, "0") != 1 && cpt<MAX_CP);
    nouv_livreur.nb_deplacements = cpt;

    while (trouve == 0){
        printf("Dependez vous d'un restaurant ? Si oui entrez 1 sinon entrez 0 : ");
        scanf("%d[^\n] \n", &check);
        if(check){
            printf("Entrez le nom du restaurant dont vous dependez : ");
            scanf("\n%127[^\n]", nom_restau);

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
                printf("Erreur, restaurant non présent dans la base de donnee\n");
            }
        }
        else{
            nouv_livreur.restaurant = 0;
            trouve = 1;
        }
    }

    if(nouvfichier == 1){
        nouv_livreur.id = 1;
        clear(&dblivreur);
    }
    else{
        increment (&fin, -1);
        dernier_livreur = (struct livreur*)fin.element;
        nouv_livreur.id = dernier_livreur->id + 1;
    } 

    FILE* ecriturelivreur = fopen("database/livreurs.csv", "w");

    push_back(&dblivreur, &nouv_livreur);
    ecriture_table_livreurs(ecriturelivreur, &dblivreur);

    FILE* flog = fopen("log.txt","a+");
    fprintf(flog,"Le livreur %s a créé un nouveau compte (id %i)",nouv_livreur.nom, (int)nouv_livreur.id);
    fclose(flog); 

    destroy(&dblivreur);
    destroy(&dbrestau);
    fclose(ecriturelivreur);
    fclose(fichierrestau);

    return nouv_livreur.id;
}

//Permet a un liveur de supprimer son compte et toutes les information y etant contenues
void supprimer_compte_livreur(int id){
    //On ouvre le fichier csv des livreurs, on cherche l'index du compte ou on est 
    //connecté et on supprime la ligne dans le fichier csv
    //Ensuite, on fait "remonter" les lignes d'apres en réduisant tous leurs indexs
    //de 1
    FILE* fichierlivreur;
    fichierlivreur = fopen("database/livreurs.csv","r");
    vector dblivreur = lecture_table_livreurs(fichierlivreur);
    fclose(fichierlivreur);

    iterator iterateur, debut;

    debut = begin(&dblivreur);
    
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

    if(debut.element == iterateur.element){
        remove("database/livreurs.csv");
    }
    else{
        erase(&dblivreur, iterateur);
    }
    

    FILE* ecriturelivreur = fopen("database/livreurs.csv","w");
    ecriture_table_livreurs(ecriturelivreur, &dblivreur);

    FILE* flog = fopen("log.txt","a+");
    fprintf(flog,"Le compte livreur d'id %i a supprimé son compte", id);
    fclose(flog); 

    fclose(ecriturelivreur);
    destroy(&dblivreur);

    return;
}

//Permet a un livreur de modifier les cp ou il lui est possible de livrer
void modifier_cp_livreur(int id){
    //On ouvre le fichier csv des livreurs, on récupere la ligne correspondant a 
    //l'index du compte ou on est connecté, on récupere la liste
    //des codes postaux.
    //On remplace avec la nouvelle liste de ses codes postaux

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
    do{
        printf("Veuillez entrer un code postal ou vous pouvez livrer (Entrez 0 pour arreter) : ");
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

    return;
}

//Permet a un livreur de modifier son numero de telephone 
void modifier_tel_livreur(int id){
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

    printf("Veuillez entrer votre numero de telephone (En entrant les espaces): ");
    scanf("\n%127[^\n]", livreur_connecte->tel);

    set(actu, (void *)livreur_connecte);
    FILE* ecriturelivreur = fopen("database/livreurs.csv","w");
    ecriture_table_livreurs(ecriturelivreur, &dblivreur);

    FILE* flog = fopen("log.txt","a+");
    fprintf(flog,"Le livreur d'id %i a modifié son numéro de téléphone", id);
    fclose(flog); 

    fclose(ecriturelivreur);
    destroy(&dblivreur);

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

    while(trouve == 0){
        printf("Dependez vous d'un restaurant ? Si oui entrez 1 sinon entrez 0 :");
        scanf("%d[^\n] \n", &check);
        if(check){
            printf("Entrez le nom du restaurant dont vous dependez : ");
            scanf("\n%127[^\n]", nom_restau);

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
                printf("Erreur, restaurant non présent dans la base de donnee\n");
            }
        }
        else{
            livreur_connecte->restaurant = 0;
            trouve = 1;
        }
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

    return;
}

//Permet a un livreur de modifier les differents elements vu ci dessus
void modifier_compte_livreur(int id){
    //On combine les trois fonctions au dessus en demandant a l'utilisateur ce qu'il
    //souhaite  modifier
    int value;
    printf("Veuillez taper 1, 2 ou 3 selon ce que vous souhaitez modifier : \n1) Codes postaux de livraison\n2) Numero de tel\n3) Restaurant affilié\n");
    scanf("%i",&value);

    switch (value)
    {
    case 1:
        modifier_cp_livreur(id);
        break;
    case 2:
        modifier_tel_livreur(id);
        break;
    case 3:
        modifier_resto_livreur(id);
        break;
    default:
        printf("Valeur incorrecte, veuillez recommencer\n");
        break;
    }

    return;
}


//Permet à un liveur de consulter la somme d'argent qu'il a sur son solde 
void consulter_solde_livreur(int id){
    //On ouvre la bd livreur et on recup la valeur de la solde a la ligne du compte
    //auquel on est connecté
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
    printf("Le solde sue ce compte est de %.2f euros (Appuyez sur q pour quitter)", livreur_dans_bd->solde);

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
            supprimer_compte_livreur(idlivreur);
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
    char choice, r = ' ';

    printf("* Menu Livreur *\n\nVous voulez :\n1. Vous connecter à un compte\n2. Créer un nouveau compte\n\nVotre choix ('q' pour quitter) : ");
    choice = getchar();

    switch(choice){
        //On recupere l'id du livreur dans les deux cas pour appeler la fonction de gestion de compte
        case '1':
            //On verifie qu'il existe des livreurs avant de tenter de se connecter
            if (access("database/livreurs.csv", F_OK) == 0){
                idlivreur = connecter_compte_livreur();
                quite = gestion_compte_livreur(idlivreur);
                if(quite == 0){
                    menu_livreur();
                }
            }else{
                printf("Aucun compte enregistré, impossible de se connecter(Appuyez sur q pour revenir en arriere)");
                while(r != 'q'){
                    scanf("%c", &r);
                }
                menu_livreur();
            }
            break;
        case '2':
            idlivreur = creer_compte_livreur();
            quite = gestion_compte_livreur(idlivreur);
            //Selon les choix de l'utilisateur dans gestion_compte_livreur, on le fait revenir a ce menu ou non
            if(quite == 0){
                menu_livreur();
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


