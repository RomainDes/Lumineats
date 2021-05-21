#include "restaurant.h"

//Permet de mettre en place l'index, à chaque fois qu'on appelle la fonction l'id incrémente de 1
int index_counter(vector const* dbresto)
{
    restaurant *r = (restaurant*)value(at(dbresto, dbresto->num_elements - 1));
    return r->id + 1;
}

//On demande les informations suivante : le nom, mot de passe, le code
//postal, le téléphone et le type de cuisine en les stockant 
//dans un vecteur puis on copie ce vecteur dans la db restaurant. renvoie l'index du compte crée
int creer_compte_resto(){
    //Si on est pas déjà connecter sur un compte
    if(index_resto == 0){

        restaurant restaurants;

        int exist;
        
        //Permet de savoir si la base de donnée existe ou non 
        //S'il elle n'existe pas on créer le fichier et on met l'index à 1
        //Sinon on prend l'inex + 1 du dernier dernier index
        if( access("database/restaurants.csv", F_OK ) == -1){
            FILE * fichierresto_create = fopen("database/restaurants.csv", "w+");
            fclose(fichierresto_create);
            exist = 0;
        }
        else{
            exist = 1;
        }

        FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
        vector dbresto = lecture_table_restaurants(fichierresto_read);
        fclose(fichierresto_read);

        
        if(exist == 0){
            erase(&dbresto, begin(&dbresto));
            restaurants.id = 1;
        }
        else    restaurants.id = index_counter(&dbresto);

        printf("Nom :");
        scanf("\n%127[^\n]", restaurants.nom); //LaBoucherie

        while(nom_resto_exist(&dbresto, restaurants.nom) >= 1){
            //demander de nouveau le nom du restaurant
            printf("Nom :");
            scanf("\n%127[^\n]", restaurants.nom);
        }

        //demander le mdp, code_postal, téléphone, type de cuisine (possible de créer une fonction create_resto)
        
        printf("MotdePasse :");
        scanf("\n%127[^\n]", restaurants.mdp);
        printf("Code postal :");
        scanf("\n%127[^\n]", restaurants.code_postal); //13180
        printf("Téléphone :");
        scanf("\n%127[^\n]", restaurants.tel); //0695942642
        printf("Type :");
        scanf("\n%127[^\n]", restaurants.type); //Viande
        restaurants.menu[0] = 0;
        restaurants.nb_menu = 0;
        restaurants.solde = 0.00;
        push_back(&dbresto, &restaurants);

        FILE *fichierresto_write = fopen("database/restaurants.csv", "w");
        ecriture_table_restaurants(fichierresto_write, &dbresto);
        fclose(fichierresto_write);

        destroy(&dbresto);

        return restaurants.id;
    }
    else{
        printf("Vous êtes déjà connecter à un compte.");
        return index_resto;
    }

}

//On lit toute la base de donnée pour savoir si il existe déjà le nom, renvoie 0 si il n'existe pas sinon l'index où il existe
int nom_resto_exist(vector const* dbresto, char nom[TAILLE_NOM]){

    int index = 1;
    for(iterator r = begin(dbresto), e = end(dbresto); compare(r, e) != 0; increment(&r, 1)){
        restaurant const* resto = (restaurant*)r.element;

        if(compare_char(resto->nom,nom) == 1)   return index;
        else {
            index++;
        }
    }

    return 0;
}


//On demande au restaurateur le nom du restaurant, puis on ouvre la db livreur, on demande ensuite
//a l'utilisateur d'entrer son mot de passe et on verifie si il correspond au mdp
//present dans la db a la ligne correspondant au nom entré.
//Si nom entré pas dans la db on redemande, idem pour mdp si il correspond pa
int connecter_compte_resto(){
    if(index_resto == 0){
        int index = 0;

        if( access("database/restaurants.csv", F_OK ) != -1){
            char nom[TAILLE_NOM];
            char mdp[TAILLE_MDP];

            FILE * fichierresto = fopen("database/restaurants.csv", "r");
            vector dbresto = lecture_table_restaurants(fichierresto);
            fclose(fichierresto);

            while(index == 0){
                printf("Nom :");
                scanf("\n%127[^\n]", nom);
                index = nom_resto_exist(&dbresto, nom);
                //demander de nouveau le nom du restaurant
                
                if(compare_char(nom, "q") == 1)  return 0;
            }

            restaurant *r = (restaurant*)value(at(&dbresto,index-1));
        
            printf("Mdp :");
            scanf("\n%127[^\n]", mdp);

            while(compare_char(r->mdp, mdp) != 1){
                printf("Mdp :");
                scanf("\n%127[^\n]", mdp);
                if(compare_char(mdp, "q") == 1)  return 0;
            }
        }
        return index;
    }
    else{
        printf("Vous êtes déjà connecter à un compte.");
        return index_resto;
    }
}

int compare_char(const char *a,const char*b){
    int same;
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0'){
            if( a[i] == b[i] )  same = 1;
            else return 0;
            i++;
    }
    if( a[i] == b[i] )  same = 1;
    else same = 0;

    return same;
}


void supprimer_compte_resto(int index){
    if(index > 0){
        //Supprimer les items créer par le resto index
        if( access("database/items.csv", F_OK ) != -1){
            FILE * fichieritems_read = fopen("database/items.csv", "r");
            vector dbitems = lecture_table_items(fichieritems_read);
            fclose(fichieritems_read);

            int j = 0;
            for(iterator i = begin(&dbitems), e = end(&dbitems); compare(i, e) != 0; increment(&i, 1)){
                item const* items = (item*)i.element;

                if(items->restaurant == index){
                    erase(&dbitems, at(&dbitems,j));
                    decrement(&i,1);
                }
                else j++;
                
            }
            FILE * fichieritems_write = fopen("database/items.csv", "w");
            ecriture_table_items(fichieritems_write, &dbitems);
            fclose(fichieritems_write);
        }
        //Supprimer le resto index
        if( access("database/restaurants.csv", F_OK ) != -1){
            FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
            vector dbresto = lecture_table_restaurants(fichierresto_read);
            fclose(fichierresto_read);

            erase(&dbresto, at(&dbresto,index-1));

            FILE * fichierresto_write = fopen("database/restaurants.csv", "w");
            ecriture_table_restaurants(fichierresto_write, &dbresto);
            fclose(fichierresto_write);
        }
    }
    else    printf("Vous n'êtes pas connecter à un compte.");
    
}

void modifier_menu(){
//On appelle les trois sous-fonctions suivante
}

void ajouter_nouvel_item(){
//On demande les informations suivante : le nom, les ingrédiants principaux et le prix en les stockant 
//dans un vecteur puis on copie ce vecteuir dans la db menu. 
//Et ensuite on ajoute l'id auquel il réfere dans la ligne 
//du resto que l'on traite dans db restaurant.
}

void ajouter_item_existant(){
//On ajoute l'id d'un item de la db menu dans les menus du 
//restaurant que l'on traite dans la db restaurant.
}

void supprimer_item(){
//On supprime un ligne dans la db menu
}

void consulter_solde_restaurant(){
//On affiche le solde du restaurant que l'on traite
}
