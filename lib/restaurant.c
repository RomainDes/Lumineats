#include "restaurant.h"

// //Initialise la structure d'un restaurant
// restaurant make_restaurant(){
//     return (restaurant){
//             .code_postal = malloc(6*sizeof(char)),
//             .tel = malloc(15*sizeof(char)),
//             .type = malloc(TAILLE_TYPE*sizeof(char)),
//             .menu[0] = 0,
//             .nb_menu = 0,
//             .solde = 0.00,
//             };
// }

//On demande les informations suivante : le nom, mot de passe, le code
//postal, le téléphone et le type de cuisine en les stockant 
//dans un vecteur puis on copie ce vecteur dans la db restaurant.
void creer_compte_resto(){

    restaurant restaurants;
    FILE * fichierresto = fopen("database/restaurants.csv", "r");
    vector dbresto = lecture_table_livreurs(fichierresto);

    //char nom[TAILLE_NOM];

    scanf("%s\n", restaurants.nom); //LaBoucherie

    while(nom_resto_exist(&dbresto, restaurants.nom) == 1){
        //demander de nouveau le nom du restaurant
        scanf("%s\n", restaurants.nom);
    }

    //restaurants = make_restaurant();
    //demander le mdp, code_postal, téléphone, type de cuisine (possible de créer une fonction create_resto)
    
    scanf("%s\n", restaurants.code_postal); //13180
    scanf("%s\n", restaurants.tel); //06 95 94 26 42
    scanf("%s\n", restaurants.type); //Viande
    restaurants.menu[0] = 0;
    restaurants.nb_menu = 0;
    restaurants.solde = 0.00;
    push_back(&dbresto, &restaurants);
}

//On lit toute la base de donnée pour savoir si il existe déjà le nom
int nom_resto_exist(vector const* dbresto, char nom[TAILLE_NOM]){

    int error = 0;

    for(iterator r = begin(dbresto), e = end(dbresto); compare(r, e) != 0 || error != 0; increment(&r, 1)){
        restaurant const* resto = (restaurant*)r.element;
    
        if( resto->nom == nom )  error = 1;
        
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
