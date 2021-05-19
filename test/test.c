//Tester toutes les fonctions de client.c, livreur.c et 
//restaurant.c et vérifer la base de donnée à chaque 
//changement.

//Commencer par l'interface principale où l'on doit 
//choisir ce que l'on est.

#include "test_harness/test_harness.h"

#include "utility/db.h"
#include "utility/vector.h"
#include "link.h"


#include <sys/stat.h>
#include <unistd.h>

#if defined(__linux__)
    #define  _POSIX_C_SOURCE 200809L
#endif
#include <stdio.h>

// Valeurs pour le harnais de test spécifiques à ce programme.
int const tests_total = 76;
int const test_column_width = 60;

int main()
{
    system("rm -rf build/test-db && mkdir -p build/test-db && cp -a test/db/. build/test-db");

    // Tests de lecture et d'écriture de la table 'restaurants'.
    {
        FILE *test_db_restaurants = fopen("database/restaurants.csv", "r");
        vector restaurants = lecture_table_restaurants(test_db_restaurants);
        fclose(test_db_restaurants);

        TEST(size(restaurants) == 3);

        restaurant *r = (restaurant*)value(begin(&restaurants));
        TEST(r->id == 1);
        TEST(strcmp(r->nom, "Chez Michel") == 0);
        TEST(strcmp(r->mdp, "please") == 0);
        TEST(strcmp(r->code_postal, "13001") == 0);
        TEST(strcmp(r->tel, "04 13 13 13 13") == 0);
        TEST(strcmp(r->type, "Provencal") == 0);
        TEST(r->menu[0] == 1);
        TEST(r->menu[1] == 4);
        TEST(r->menu[2] == 5);
        TEST(r->menu[3] == 0);
        TEST(r->solde == 50.00);

        r = (restaurant*)value(at(&restaurants, 2));
        TEST(r->id == 3);
        TEST(strcmp(r->nom, "Joe's International House of Pancakes") == 0);
        TEST(strcmp(r->mdp, "jetensuppli") == 0);
        TEST(strcmp(r->code_postal, "13010") == 0);
        TEST(strcmp(r->tel, "04 22 33 44 55") == 0);
        TEST(strcmp(r->type, "Americain") == 0);
        TEST(r->menu[0] == 6);
        TEST(r->menu[1] == 7);
        TEST(r->menu[2] == 0);
        TEST(r->solde == 44.00);

        FILE *test_db_restaurants_copie = fopen("database/copie-restaurants.csv", "w");
        ecriture_table_restaurants(test_db_restaurants_copie, &restaurants);
        fclose(test_db_restaurants_copie);
        TEST_FILE("database/restaurants.csv", "database/copie-restaurants.csv");

        destroy(&restaurants);
    }

    // Tests de lecture et d'écriture de la table 'items'.
    {
        FILE *test_db_items = fopen("database/items.csv", "r");
        vector items = lecture_table_items(test_db_items);
        fclose(test_db_items);

        TEST(size(items) == 7);

        item *i = (item*)value(begin(&items));
        TEST(i->id == 1);
        TEST(strcmp(i->nom, "bouillabaisse") == 0);
        TEST(strcmp(i->ingredients[0], "poissons de roche") == 0);
        TEST(strcmp(i->ingredients[1], "pommes de terre") == 0);
        TEST(strcmp(i->ingredients[2], "") == 0);
        TEST(i->prix == 25.00);

        i = (item*)value(at(&items, 6));
        TEST(i->id == 7);
        TEST(strcmp(i->nom, "petit-dej du champion") == 0);
        TEST(strcmp(i->ingredients[0], "oeufs") == 0);
        TEST(strcmp(i->ingredients[1], "toast") == 0);
        TEST(strcmp(i->ingredients[2], "bacon") == 0);
        TEST(strcmp(i->ingredients[3], "pomme de terre") == 0);
        TEST(strcmp(i->ingredients[4], "") == 0);
        TEST(i->prix == 12.0);

        FILE *test_db_items_copie = fopen("database/items-copie.csv", "w");
        ecriture_table_items(test_db_items_copie, &items);
        fclose(test_db_items_copie);
        TEST_FILE("database/items.csv", "database/items-copie.csv");

        destroy(&items);
    }

    // Tests de lecture et d'écriture de la table 'livreurs'.
    {
        FILE *test_db_livreurs = fopen("database/livreurs.csv", "r");
        vector livreurs = lecture_table_livreurs(test_db_livreurs);
        fclose(test_db_livreurs);

        TEST(size(livreurs) == 3);

        livreur *l = (livreur*)value(begin(&livreurs));
        TEST(l->id == 1);
        TEST(strcmp(l->nom, "Francois Pignon") == 0);
        TEST(strcmp(l->tel, "06 00 00 00 00") == 0);
        TEST(strcmp(l->deplacements[0], "13001") == 0);
        TEST(strcmp(l->deplacements[1], "13002") == 0);
        TEST(strcmp(l->deplacements[2], "13003") == 0);
        TEST(strcmp(l->deplacements[3], "") == 0);
        TEST(l->restaurant == 1);
        TEST(l->solde == 20);
        TEST(strcmp(l->mdp, "lafranceauxfrancais") == 0);

        l = (livreur*)value(at(&livreurs, 2));
        TEST(l->id == 3);
        TEST(strcmp(l->nom, "Mickey Mouse") == 0);
        TEST(strcmp(l->tel, "06 11 22 33 44") == 0);
        TEST(strcmp(l->deplacements[0], "13008") == 0);
        TEST(strcmp(l->deplacements[1], "13009") == 0);
        TEST(strcmp(l->deplacements[2], "13010") == 0);
        TEST(strcmp(l->deplacements[3], "13011") == 0);
        TEST(strcmp(l->deplacements[4], "") == 0);
        TEST(l->restaurant == 0);
        TEST(l->solde == 0);
        TEST(strcmp(l->mdp, "jeanneausecours") == 0);

        FILE *test_db_livreurs_copie = fopen("database/copie-livreurs.csv", "w");
        ecriture_table_livreurs(test_db_livreurs_copie, &livreurs);
        fclose(test_db_livreurs_copie);
        TEST_FILE("database/livreurs.csv", "database/copie-livreurs.csv");

        destroy(&livreurs);
    }

    // Tests de lecture et d'écriture de la table 'clients'.
    {
        FILE *test_db_clients = fopen("database/clients.csv", "r");
        vector clients = lecture_table_clients(test_db_clients);
        fclose(test_db_clients);

        TEST(size(clients) == 3);

        client *c = (client*)value(begin(&clients));
        TEST(c->id == 1);
        TEST(strcmp(c->nom, "Francoise Perrin") == 0);
        TEST(strcmp(c->mdp, "jaimegemo") == 0);
        TEST(strcmp(c->code_postal, "13005") == 0);
        TEST(strcmp(c->tel, "04 10 20 30 40") == 0);
        TEST(c->solde == 0);

        c = (client*)value(at(&clients, 2));
        TEST(c->id == 3);
        TEST(strcmp(c->nom, "Quentin Tarantino") == 0);
        TEST(strcmp(c->mdp, "jaimeguillaume") == 0);
        TEST(strcmp(c->code_postal, "13008") == 0);
        TEST(strcmp(c->tel, "06 99 88 77 66") == 0);
        TEST(c->solde == 15);

        FILE *test_db_clients_copie = fopen("database/clients-copie.csv", "w");
        ecriture_table_clients(test_db_clients_copie, &clients);
        fclose(test_db_clients_copie);
        TEST_FILE("database/clients.csv", "database/clients-copie.csv");

        destroy(&clients);
    }

    return tests_total - tests_successful;
}