#include "restaurant.h"

int where = 0;

//Permet de mettre en place l'index, à chaque fois qu'on appelle la fonction l'id incrémente de 1
int index_counter(vector const* db, char structure)
{
    if(structure == 'r'){
        restaurant *r = (restaurant*)value(at_origin(db, db->num_elements - 1));
        return r->id + 1;
    }
    else if(structure == 'i'){
        item *i = (item*)value(at_origin(db, db->num_elements - 1));
        return i->id + 1;
    }
    else return 0; 
}

//On demande les informations suivante : le nom, mot de passe, le code
//postal, le téléphone et le type de cuisine en les stockant 
//dans un vecteur puis on copie ce vecteur dans la db restaurant. renvoie l'index du compte crée
int creer_compte_resto(){

    FILE * fichierlog = fopen("log.txt", "a+");

    printf("\n");
    system("clear");

    printf("* Menu Restaurateur *\n\n* Créer un compte *\n\n");

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

        printf("Entrez votre nom :");
        scanf("\n%127[^\n]", restaurants.nom); //LaBoucherie

        while(nom_resto_exist(&dbresto, restaurants.nom) >= 1){
            //demander de nouveau le nom du restaurant
            printf("\nNom déjà existant, veuillez réessayer :");
            scanf("\n%127[^\n]", restaurants.nom);
        }

        //demander le mdp, code_postal, téléphone, type de cuisine (possible de créer une fonction create_resto)
        
        printf("Entrez votre mot de passe :");
        scanf("\n%127[^\n]", restaurants.mdp);
        printf("Entrez votre code postal (XXXXX):");
        scanf("\n%127[^\n]", restaurants.code_postal); //13180
        printf("Entrez votre téléphone (XX XX XX XX XX):");
        scanf("\n%127[^\n]", restaurants.tel); //0695942642
        printf("Entrez votre type de votre restaurant :");
        scanf("\n%127[^\n]", restaurants.type); //Viande
        restaurants.menu[0] = 0;
        restaurants.nb_menu = 0;
        restaurants.solde = 0.00;

        //Confirmer la création du compte
        printf("\nConfirmer la création de votre compte ('y' pour valider, 'r' pour recommencer, 'q' pour quitter) : ");
        char operation;

        operation = getchar();
        operation = getchar();

        switch(operation)
        {   
            case 'y':
                push_back(&dbresto, &restaurants);

                FILE *fichierresto_write = fopen("database/restaurants.csv", "w");
                ecriture_table_restaurants(fichierresto_write, &dbresto);
                fclose(fichierresto_write);

                destroy(&dbresto);

                fprintf(fichierlog, "Le restaurant %s a crée un compte.\n", restaurants.nom);
                fclose(fichierlog);

                return restaurants.id;
                break;
            case 'r':
                if(exist == 0)  remove("database/restaurants.csv");
                return creer_compte_resto();
                break;
            case 'q':
                if(exist == 0)  remove("database/restaurants.csv");
                break;
        }
        return index_resto;
    }
    else{
        printf("Vous êtes déjà connecter à un compte.\n\nLoading...");
        sleep(4);
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
    
    printf("\n");
    system("clear");

    printf("* Menu Restaurateur *\n\n* Se connecter à un compte *\n\n");

    if(index_resto == 0){

        FILE * fichierlog = fopen("log.txt", "a+");
        
        int index = 0;

        if( access("database/restaurants.csv", F_OK ) != -1){
            char nom[TAILLE_NOM];
            char mdp[TAILLE_MDP];

            FILE * fichierresto = fopen("database/restaurants.csv", "r");
            vector dbresto = lecture_table_restaurants(fichierresto);
            fclose(fichierresto);

            printf("Entrez votre nom ('q' pour quitter) :");
            scanf("\n%127[^\n]", nom);
            while(index == 0){
                if(compare_char(nom, "q") == 1)  return 0;


                index = nom_resto_exist(&dbresto, nom);
                //demander de nouveau le nom du restaurant

                if(index == 0){
                    printf("Nom invalide, veuillez réessayer ('q' pour quitter) :");
                    scanf("\n%127[^\n]", nom);
                }
            }

            restaurant *r = (restaurant*)value(at(&dbresto,index));
        
            printf("Entrez votre mot de passe ('q' pour quitter) :");
            scanf("\n%127[^\n]", mdp);

            while(compare_char(r->mdp, mdp) != 1){
                printf("Mot de passe invalide, veuillez réessayer ('q' pour quitter) :");
                scanf("\n%127[^\n]", mdp);
                if(compare_char(mdp, "q") == 1)  return 0;
            }
            fprintf(fichierlog, "Le restaurant %s s'est connecté a son compte.\n", r->nom);
            fclose(fichierlog);
        }
        else{
            printf("Félicitation, vous êtes la premiere personne sur cette application, veuillez créer un compte !\n\nLoading...\n");
            sleep(4);
        }

        

        return index;
    }
    else{
        printf("Vous êtes déjà connecter à un compte.\n\nLoading...");
        sleep(4);
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


int supprimer_compte_resto(char nom[TAILLE_NOM]){
    printf("\n");
    system("clear");

    printf("* Menu Restaurateur *\n\n-- %s --\n\n* Supprimer votre compte *\n\n", nom);


    if(index_resto > 0){
        FILE * fichierlog = fopen("log.txt", "a+");

        printf("Confirmer la suppression du compte ('y' pour valider, 'r' pour refuser) : ");
        char choix;
        scanf("\n%c", &choix);
        if(choix == 'y'){
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
            
            fprintf(fichierlog, "Le restaurant d'index %s a été supprimé et les items qu'il a crée.\nLoading...\n", nom);
            fclose(fichierlog);

            index_resto = 0;
            printf("Le restaurant %s a été supprimé et les items qu'il a crée.\nLoading...\n", nom);
            sleep(4);
            
        }
        else if(choix == 'r'){
            printf("Compte non supprimé.\nLoading...\n");
            sleep(4);
            return 0;//retourne 0 pour dire que le compte n'a pas été supprimé
        }
        else{
            return supprimer_compte_resto(nom);
        }
    }
    else{
        printf("Vous n'êtes pas connecter à un compte.\nLoading...\n");
        sleep(4);
    }
    
    return 1;
}

void modifier_menu(char nom[TAILLE_NOM]){

//On appelle les trois sous-fonctions suivante
    printf("\n");
    system("clear");

    printf("* Menu Restaurateur *\n\n-- %s --\n\n* Modifier votre menu *\n\n", nom);

    if(index_resto > 0){
        printf("1. Créer nouvel item\n2. Ajouter un item\n3. Supprimer un item\n\nVotre choix ('q' pour quitter) :");
    
        char operation = getchar();
        switch(operation)
        {
                
            case '1':
                creer_nouvel_item(nom);
                modifier_menu(nom);
                break;
            case '2':
                ajouter_item(nom);
                modifier_menu(nom);
                break;
            case '3':
                supprimer_item(nom);
                modifier_menu(nom);
                break;
            case 'q':
                break;
            default :
                modifier_menu(nom);
            }
    }
    else{    
        printf("Vous n'êtes pas connecté à un compte.\nLoading...\n");
        sleep(4);
    }
}

void creer_nouvel_item(char nom[TAILLE_NOM]){
//On demande les informations suivante : le nom, les ingrédiants principaux et le prix en les stockant 
//dans un vecteur puis on copie ce vecteuir dans la db menu. 
//Et ensuite on ajoute l'id auquel il réfere dans la ligne 
//du resto que l'on traite dans db restaurant.
    printf("\n");
    system("clear");

    printf("* Menu Restaurateur *\n\n-- %s --\n\n* Créer un nouvel item à votre menu *\n\n", nom);

    FILE * fichierlog = fopen("log.txt", "a+");

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
    vector dbitems = lecture_table_items(fichieritem_read);
    fclose(fichieritem_read);

    if(exist == 0){
        erase(&dbitems, begin(&dbitems));
        items.id = 1;
    }
    else    items.id = index_counter(&dbitems, 'i');

    
    items.restaurant = index_resto;

    //demander le nom, les ingrédients, et le prix
    printf("Entrez le nom de l'item :");
    scanf("\n%127[^\n]", items.nom); 

    char ingredient[TAILLE_INGRE] = " ";
    int i = 1;
    int j;
    printf("\nListe des ingrédients ('0' pour plus d'ingrédients) :\n");
    while(compare_char(ingredient, "0") == 0){
        items.ingredients[i-1] = malloc(sizeof(char));
        printf("Entrez le nom de l'ingrédients n°%i :", i);
        scanf("\n%127[^\n]", ingredient);
        if(compare_char(ingredient, "0")== 0) {
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
    
    printf("Entrez le prix de l'item (X.XX):");
    scanf("%f", &items.prix); 

    push_back(&dbitems, &items);
    
    FILE * fichieritem_write = fopen("database/items.csv", "w");
    ecriture_table_items(fichieritem_write, &dbitems);
    fclose(fichieritem_write);

    //Ajouter l'index menu au restaurant index_resto
    int menu;

    menu = items.id;

    ajouter_item_menu(menu);

    fprintf(fichierlog, "Le restaurant d'index %li a crée un item nommé %s et a été ajouté.\n", index_resto, items.nom);
    fclose(fichierlog);

    printf("\nLoading...\n");
    sleep(4);
}

int compare_int(int *a, int b){
    int cpt = 0;
    while(a[cpt] != 0){
        if(a[cpt] == b)  return 0;
        cpt++;
    }
    return 1;//quand il y en a un b dans a sinon retourne 1
}
void ajouter_item(char nom[TAILLE_NOM]){
//On ajoute l'id d'un item de la db menu dans les menus du 
//restaurant que l'on traite dans la db restaurant.
    printf("\n");
    system("clear");

    printf("* Menu Restaurateur *\n\n-- %s --\n\n* Ajouter un item à votre menu *\n\n", nom);

    if( access("database/items.csv", F_OK ) != -1){
        
        //Prends la liste des menus pouvant être ajouté
        int *liste_menu = malloc(sizeof(int));

        int menu=-1;
        FILE * fichieritem_read = fopen("database/items.csv", "r");
        vector dbitems = lecture_table_items(fichieritem_read);
        fclose(fichieritem_read);

        printf("Liste des items :\n\n");

        int affiche = 0;
        for(iterator i = begin(&dbitems), e = end(&dbitems); compare(i, e) != 0; increment(&i, 1))
        {
            
            item const* items = (item*)i.element;
            if(items->restaurant != index_resto){
                printf("Items n°%zu : %s\n", items->id, items->nom);
                liste_menu[affiche] = items->id;
                liste_menu[affiche + 1] = 0;
                affiche++;
            }
            
        }
        if( affiche == 0){
            printf("Aucun item par défaut. Veuillez en créer un.\nLoading...\n");
            sleep(4);
        }           
        else{
            printf("\nAjouter Item n° (0 pour quitter) : ");
            scanf("\n%i", &menu);
            if (compare_int(liste_menu, menu) == 0){
                ajouter_item_menu(menu);
                printf("\nLoading...\n");
                sleep(4);
                ajouter_item(nom);
            }
            else if(menu == 0){
                printf("\n\nAucun Item ajouté.\nLoading...\n");
                sleep(4);
            }
            else{
                printf("\n\nItem inexistant.\nLoading...\n");
                sleep(4);
                ajouter_item(nom);
            }
            
        }
    }
    else{
        printf("Aucun item par défaut. Veuillez en créer un.\nLoading...\n");
        sleep(4);
    }
}

int ajouter_item_menu(int menu){
    FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
    vector dbresto = lecture_table_restaurants(fichierresto_read);
    fclose(fichierresto_read);

    FILE * fichierlog = fopen("log.txt", "a+");

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

        printf("\nLe menu a été ajouté à votre restaurant.");

        fprintf(fichierlog, "Le restaurant d'index %li a ajouté l'item d'index %i.\n", index_resto, menu);
        fclose(fichierlog);
    }

    return exist;
    
}

void supprimer_item(char nom[TAILLE_NOM]){
//On supprime un ligne dans la db menu
    char choix = ' ';

    printf("\n");
    system("clear");

    printf("* Menu Restaurateur *\n\n-- %s --\n\n* Supprimer un item de votre menu *\n\n", nom);
    if(index_resto > 0){  
        
        
        if( access("database/items.csv", F_OK ) != -1){

            FILE * fichierlog = fopen("log.txt", "a+");

          
            //Ouverture de la db item
            FILE * fichieritem_read = fopen("database/items.csv", "r");
            vector dbitems = lecture_table_items(fichieritem_read);
            fclose(fichieritem_read);

            //Ouverture de la db resto
            FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
            vector dbresto = lecture_table_restaurants(fichierresto_read);
            fclose(fichierresto_read);

            //items prend la ligne du restaurant
            iterator r = at(&dbresto,index_resto);
            restaurant * resto = (restaurant*)r.element;

            printf("Liste des items a supprimer :\n");

            //Afficher tout les items du menu du restaurant
            for(int j = 0; j < resto->nb_menu; j++){
                

                iterator i = at(&dbitems, resto->menu[j]);
                item * items = (item*)i.element;

                printf("- %s ('y' pour supprimer, autre pour ne pas supprimer) : ",items->nom);
                scanf("\n%c", &choix);
                if(choix == 'y'){
                    resto->menu[j] = resto->menu[resto->nb_menu - 1];
                    resto->menu[resto->nb_menu - 1] = 0;
                    resto->nb_menu--;
                    j--;
                    if(items->restaurant == index_resto){
                        erase(&dbitems, at(&dbitems,items->id));
                    }
                    printf("\nItem supprimé.\n\n");
                    fprintf(fichierlog, "Le restaurant d'index %li a supprimé un item nommé %s.\n", index_resto, items->nom);
                    
                }
                else{
                    printf("Item non supprimé.\n\n");
                    
                }
            }
            if(dbitems.num_elements != 0){
                FILE * fichieritem_write = fopen("database/items.csv", "w");
                ecriture_table_items(fichieritem_write, &dbitems);
                fclose(fichieritem_write);
            }
            else    remove("database/items.csv");
            

            FILE * fichierresto_write = fopen("database/restaurants.csv", "w");
            ecriture_table_restaurants(fichierresto_write, &dbresto);
            fclose(fichierresto_write);
            fclose(fichierlog);
            
            printf("Loading...\n");
            sleep(4);
        }
        else{
            printf("Aucun item par défaut. Veuillez en créer un.\nLoading...\n");
            sleep(4);
        }
    }
    else{
        printf("Vous n'êtes pas connecté à un compte.\nLoading...\n");
        sleep(4);
    }
}

void consulter_solde_restaurant(char nom[TAILLE_NOM]){

    printf("\n");
    system("clear");

    printf("* Menu Restaurateur *\n\n-- %s --\n\n* Consulter Solde *\n\n", nom);


//On affiche le solde du restaurant que l'on traite
    if(index_resto > 0){

        FILE * fichierlog = fopen("log.txt", "a+");

        FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
        vector dbresto = lecture_table_restaurants(fichierresto_read);
        fclose(fichierresto_read);

        iterator i = at(&dbresto,index_resto);
        restaurant *const resto = (restaurant*)i.element;

        printf("Solde : %.2f ('q' pour quitter) : ", resto->solde);
        fprintf(fichierlog, "Le restaurant d'index %li consulte son solde de %.2f€.\n", index_resto, resto->solde);
        fclose(fichierlog);
    }
    else {  
        printf("\n\nVous n'êtes pas connecté à un compte.\nLoading...\n");
        sleep(4);
    }

    /* attente de saisie */
    char c =' ';

    while( c != 'q')    scanf("%c", &c);
}


/////INTERFACE//////

int menu_restaurant(){

    printf("\n");
    system("clear");

    


    int compare;

    printf("* Menu Restaurateur *\n\nVous voulez :\n1. Vous connecter à un compte\n2. Créer un nouveau compte\n\nVotre choix ('p' pour retourner au menu principal) : ");

    char operation;
    

    operation = getchar();
    operation = getchar();

    switch(operation)
    {
        case '1':
            index_resto = connecter_compte_resto();
            if(index_resto != 0){
                FILE * fichierresto = fopen("database/restaurants.csv", "r");
            vector dbresto = lecture_table_restaurants(fichierresto);
            fclose(fichierresto);
                restaurant *r = (restaurant*)value(at(&dbresto, index_resto));
                destroy(&dbresto);
                compare = menu_restaurant_compte(r->nom);
            }
            else    compare = 1;
            if(compare == 1)   menu_restaurant();
            break;
        case '2':
            index_resto = creer_compte_resto();
            if(index_resto != 0){
                FILE * fichierresto = fopen("database/restaurants.csv", "r");
                vector dbresto = lecture_table_restaurants(fichierresto);
                fclose(fichierresto);
                restaurant *r = (restaurant*)value(at(&dbresto, index_resto));
                destroy(&dbresto);
                compare = menu_restaurant_compte(r->nom);
            }
            else    compare = 1;
            if(compare == 1)   menu_restaurant();
            break;
        case 'p':
            break;
        default :
            menu_restaurant();
    }
    
    return 0;
}

int menu_restaurant_compte(char nom[TAILLE_NOM]){
    printf("\n");
    system("clear");

    int where;
    
    printf("* Menu Restaurateur *\n\n-- %s --\n\nVous voulez :\n1. Supprimer votre compte\n2. Modifier votre menu (ajouter/modifier/supprimer)\n3. Consulter votre solde\n\nVotre choix ('q' pour quitter, 'd' pour se déconnecter) : ", nom);

    int again = 1;

    char operation = getchar();
    switch(operation)
    {
            
        case '1':
            again = supprimer_compte_resto(nom);
            if(again == 0)  where = menu_restaurant_compte(nom);
            else    where = 1;
            break;
        case '2':
            modifier_menu(nom);
            where = menu_restaurant_compte(nom);
            break;
        case '3':
            consulter_solde_restaurant(nom);
            where = menu_restaurant_compte(nom);
            break;
        case 'd':
            index_resto = 0;
            where = 1;
            break;
        case 'q':
            index_resto = 0;
            where = 0;
            break;
        default :
            where = menu_restaurant_compte(nom);
        }

    

    return where;
}