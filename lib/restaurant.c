#include "restaurant.h"

//Permet de mettre en place l'index, à chaque fois qu'on appelle la fonction l'id incrémente de 1
int index_counter(vector const* db, char structure)
{
    if(structure == 'r'){
        restaurant *r = (restaurant*)value(at(db, db->num_elements));
        return r->id + 1;
    }
    else if(structure == 'i'){
        item *i = (item*)value(at(db, db->num_elements));
        return i->id + 1;
    }
    else return 0; 
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
        else    restaurants.id = index_counter(&dbresto, 'r');

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
        printf("Vous êtes déjà connecter à un compte.\n");
        return index_resto;
    }

}

//On lit toute la base de donnée pour savoir si il existe déjà le nom, renvoie 0 si il n'existe pas sinon l'index où il existe
int nom_resto_exist(vector const* dbresto, char nom[TAILLE_NOM]){

    for(iterator r = begin(dbresto), e = end(dbresto); compare(r, e) != 0; increment(&r, 1)){
        restaurant const* resto = (restaurant*)r.element;

        if(compare_char(resto->nom,nom) == 1)   return resto->id;
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

            restaurant *r = (restaurant*)value(at(&dbresto,index));
        
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
        printf("Vous êtes déjà connecter à un compte.\n");
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


void supprimer_compte_resto(){
    if(index_resto > 0){
        //Supprimer les items créer par le resto index
        if( access("database/items.csv", F_OK ) != -1){
            FILE * fichieritems_read = fopen("database/items.csv", "r");
            vector dbitems = lecture_table_items(fichieritems_read);
            fclose(fichieritems_read);

            int j = 1;
            for(iterator i = begin(&dbitems), e = end(&dbitems); compare(i, e) != 0; increment(&i, 1)){
                item const* items = (item*)i.element;

                if(items->restaurant == index_resto){
                    erase(&dbitems, at(&dbitems,items->id));
                    decrement(&i,1);
                }
                else j++;
                
            }
            if(dbitems.num_elements != 0){
                FILE * fichieritem_write = fopen("database/items.csv", "w");
                ecriture_table_items(fichieritem_write, &dbitems);
                fclose(fichieritem_write);
            }
            else    remove("database/items.csv");
        }
        //Supprimer le resto index
        
        
        FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
        vector dbresto = lecture_table_restaurants(fichierresto_read);
        fclose(fichierresto_read);

        erase(&dbresto, at(&dbresto,index_resto));

        if(dbresto.num_elements != 0){
            FILE * fichierresto_write = fopen("database/restaurants.csv", "w");
            ecriture_table_restaurants(fichierresto_write, &dbresto);
            fclose(fichierresto_write);
        }
        else    remove("database/restaurants.csv");
        

        index_resto = 0;
    }
    else    printf("Vous n'êtes pas connecter à un compte.\n");
    
}

void modifier_menu(){
    char choix;
//On appelle les trois sous-fonctions suivante
    if(index_resto > 0){
        while(choix != 'q'){
            printf("1 - Créer nouvel item\n2 - Ajouter un item\n3 - Supprimer un item\n'q' pour quitter\n");
            scanf("\n%c", &choix);
            if(choix == '1')  creer_nouvel_item();
            else if(choix == '2') ajouter_item();
            else if(choix == '3') supprimer_item();
            else if(choix == 'q'){}
            else{
                printf("Erreur, veuillez recommencer.\n");
                modifier_menu();
            }
        }
    }
    else    printf("Vous n'êtes pas connecté à un compte.\n");
}

void creer_nouvel_item(){
//On demande les informations suivante : le nom, les ingrédiants principaux et le prix en les stockant 
//dans un vecteur puis on copie ce vecteuir dans la db menu. 
//Et ensuite on ajoute l'id auquel il réfere dans la ligne 
//du resto que l'on traite dans db restaurant.

    item items;

    int exist = 0;

    if( access("database/items.csv", F_OK ) == -1){
            FILE * fichieritem_create = fopen("database/items.csv", "w+");
            fclose(fichieritem_create);
            exist = 0;
    }
    else{
            exist = 1;
    }

    FILE * fichieritem_read = fopen("database/items.csv", "r");
    vector dbitem = lecture_table_items(fichieritem_read);
    fclose(fichieritem_read);

    if(exist == 0){
        erase(&dbitem, begin(&dbitem));
        items.id = 1;
    }
    else    items.id = index_counter(&dbitem, 'i');

    
    items.restaurant = index_resto;

    //demander le nom, les ingrédients, et le prix
    printf("Nom :");
    scanf("\n%127[^\n]", items.nom); 

    char ingredient[TAILLE_INGRE] = "0";
    int i = 1;
    int j;
    while(compare_char(ingredient, "-1") == 0){
        items.ingredients[i-1] = malloc(sizeof(char));
        printf("Ingrédients n°%i :", i);
        scanf("\n%127[^\n]", ingredient);
        if(compare_char(ingredient, "-1")== 0) {
            for(j = 0; ingredient[j] != '\0'; j++){
                items.ingredients[i-1][j] = ingredient[j];
            }
            items.ingredients[i-1][j] = '\0';
            i++;
        }
    }
    items.nb_ingr = i-1;
    items.ingredients[i-1] = malloc(sizeof(char));
    items.ingredients[i-1][0] = '\0';
    
    printf("Prix :");
    scanf("%f", &items.prix); 

    push_back(&dbitem, &items);
    
    FILE * fichieritem_write = fopen("database/items.csv", "w");
    ecriture_table_items(fichieritem_write, &dbitem);
    fclose(fichieritem_write);

    //Ajouter l'index menu au restaurant index_resto
    int menu;

    menu = items.id;

    ajouter_item_menu(menu);

    printf("Le menu a été crée et ajouté à votre restaurant.\n");
}

void ajouter_item(){
//On ajoute l'id d'un item de la db menu dans les menus du 
//restaurant que l'on traite dans la db restaurant.
    if( access("database/items.csv", F_OK ) != -1){
        int menu=-1;
        FILE * fichieritem_read = fopen("database/items.csv", "r");
        vector dbitem = lecture_table_items(fichieritem_read);
        fclose(fichieritem_read);

        printf("Ajoutez un menu :\n");

        for(iterator i = begin(&dbitem), e = end(&dbitem); compare(i, e) != 0; increment(&i, 1))
        {
            
            item const* items = (item*)i.element;
            if(items->restaurant != index_resto){
                printf("Menu n°%zu : %s\n", items->id, items->nom);
            }
            
        }
        
        while( menu != 0 ){
            printf("Vous ajoutez le menu n° (0 pour quitter):\n");
            scanf("%i", &menu);
            if (menu != 0){
                ajouter_item_menu(menu);
                
            }
            else    printf("Aucun menu n'a été ajouté.\n");
        }
    }
    else    printf("Aucun item par défaut.\n");
}

int ajouter_item_menu(int menu){
    FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
    vector dbresto = lecture_table_restaurants(fichierresto_read);
    fclose(fichierresto_read);

    //items prend la ligne du restaurant
    iterator i = at(&dbresto,index_resto);
    restaurant * resto = (restaurant*)i.element;
    //Mettre les nouveaux menus
    int exist = 0;
    int nb_menu = 0;
    while(resto->menu[nb_menu] != 0 && exist == 0){
        if (menu == resto->menu[nb_menu]){
            exist = 1;
            printf("Cet item existe déjà dans votre menu.\n");
            return exist;
        }
        nb_menu++;
    }

    if(exist == 0){
        resto->menu[nb_menu] = menu;
        resto->menu[nb_menu + 1] = 0;
        resto->nb_menu = nb_menu+1;

        FILE * fichierresto_write = fopen("database/restaurants.csv", "w");
        ecriture_table_restaurants(fichierresto_write, &dbresto);
        fclose(fichierresto_write);

        printf("Le menu a été ajouté à votre restaurant.\n");
    }

    return exist;
    
}

void supprimer_item(){
//On supprime un ligne dans la db menu
    char choix = ' ';
    if( access("database/items.csv", F_OK ) != -1){
        if(index_resto > 0){    
            //Ouverture de la db item
            FILE * fichieritem_read = fopen("database/items.csv", "r");
            vector dbitem = lecture_table_items(fichieritem_read);
            fclose(fichieritem_read);

            //Ouverture de la db resto
            FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
            vector dbresto = lecture_table_restaurants(fichierresto_read);
            fclose(fichierresto_read);

            //items prend la ligne du restaurant
            iterator r = at(&dbresto,index_resto);
            restaurant * resto = (restaurant*)r.element;

            printf("Supprimer item :\n");

            //Afficher tout les items du menu du restaurant
            for(int j = 0; j < resto->nb_menu; j++){
                

                iterator i = at(&dbitem, resto->menu[j]);
                item * items = (item*)i.element;

                printf("Menu n°%zu : %s\nVoulez-vous le supprimer ? Y/N\n", items->id, items->nom);
                scanf("\n%c", &choix);
                if(choix == 'Y'){
                    resto->menu[j] = resto->menu[resto->nb_menu - 1];
                    resto->menu[resto->nb_menu - 1] = 0;
                    resto->nb_menu--;
                    j--;
                    if(items->restaurant == index_resto){
                        erase(&dbitem, at(&dbitem,items->id));
                    }
                    printf("Item supprimé.\n");
                }
                else    printf("Item non supprimé.\n");
            }
            if(dbitem.num_elements != 0){
                FILE * fichieritem_write = fopen("database/items.csv", "w");
                ecriture_table_items(fichieritem_write, &dbitem);
                fclose(fichieritem_write);
            }
            else    remove("database/items.csv");
            

            FILE * fichierresto_write = fopen("database/restaurants.csv", "w");
            ecriture_table_restaurants(fichierresto_write, &dbresto);
            fclose(fichierresto_write);
        }
        else    printf("Vous n'êtes pas connecté à un compte.\n");
    }
    else    printf("Aucun item par défaut.\n");
}

void consulter_solde_restaurant(){
//On affiche le solde du restaurant que l'on traite
    if(index_resto > 0){
        FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
        vector dbresto = lecture_table_restaurants(fichierresto_read);
        fclose(fichierresto_read);

        iterator i = at(&dbresto,index_resto);
        restaurant *const resto = (restaurant*)i.element;

        printf("Solde : %.2f\n", resto->solde);
    }
    else    printf("Vous n'êtes pas connecté à un compte.\n");
}
