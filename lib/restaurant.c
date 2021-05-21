#include "restaurant.h"

//Permet de mettre en place l'index, à chaque fois qu'on appelle la fonction l'id incrémente de 1
int index_counter(vector const* dbresto)
{
    restaurant *r = (restaurant*)value(at(dbresto, dbresto->num_elements - 1));
    return r->id + 1;
}

//On demande les informations suivante : le nom, mot de passe, le code
//postal, le téléphone et le type de cuisine en les stockant 
//dans un vecteur puis on copie ce vecteur dans la db restaurant.
void creer_compte_resto(){

    restaurant restaurants;

    int exist;
    
      
    if( access("database/restaurants.csv", F_OK ) == -1){
        exist = 0;
        FILE *fichierresto_create = fopen("database/restaurants.csv", "w");
        fclose(fichierresto_create);
    }
    else    exist = 1;

    FILE * fichierresto_read = fopen("database/restaurants.csv", "r");
    vector dbresto = lecture_table_restaurants(fichierresto_read);
    fclose(fichierresto_read);

    if(exist == 0){
        erase(&dbresto, begin(&dbresto));
        restaurants.id = 1;
    }
    else    restaurants.id = index_counter(&dbresto);

    printf("Nom :");
    scanf("%s", restaurants.nom); //LaBoucherie

    while(nom_resto_exist(&dbresto, restaurants.nom) == 1){
        //demander de nouveau le nom du restaurant
        printf("Nom :");
        scanf("%s", restaurants.nom);
    }

    //demander le mdp, code_postal, téléphone, type de cuisine (possible de créer une fonction create_resto)
    
    printf("MotdePasse :");
    scanf("%s", restaurants.mdp);
    printf("Code postal :");
    scanf("%s", restaurants.code_postal); //13180
    printf("Téléphone :");
    scanf("%s", restaurants.tel); //0695942642
    printf("Type :");
    scanf("%s", restaurants.type); //Viande
    restaurants.menu[0] = 0;
    restaurants.nb_menu = 0;
    restaurants.solde = 0.00;
    push_back(&dbresto, &restaurants);

    FILE *fichierresto_write = fopen("database/restaurants.csv", "w");
    ecriture_table_restaurants(fichierresto_write, &dbresto);
    fclose(fichierresto_write);

    destroy(&dbresto);
}

//On lit toute la base de donnée pour savoir si il existe déjà le nom
int nom_resto_exist(vector const* dbresto, char nom[TAILLE_NOM]){

    int error = 0;
    int i = 0;
    for(iterator r = begin(dbresto), e = end(dbresto); compare(r, e) != 0; increment(&r, 1)){
        restaurant const* resto = (restaurant*)r.element;

        while(resto->nom[i] != '\0' && nom[i] != '\0'){
            if( resto->nom[i] == nom[i] )  error = 1;
            else error = 0;
            i++;
        }

        if( resto->nom[i] == nom[i] )  error = 1;
        else error = 0;

        if(error == 1)   return error;
    
    }

    return error;
}


//On demande au restaurateur le nom du restaurant, puis on ouvre la db livreur, on demande ensuite
//a l'utilisateur d'entrer son mot de passe et on verifie si il correspond au mdp
//present dans la db a la ligne correspondant au nom entré.
//Si nom entré pas dans la db on redemande, idem pour mdp si il correspond pa
int connecter_compte_resto(){
    return 0;
}

void supprimer_compte(){
//On
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
