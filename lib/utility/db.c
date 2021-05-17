#include "utility/db.h"
#include "utility/vector.h"
#include "link.h"

#include <stdbool.h>
#include <stdio.h>


 float const growth_factor_doubling = 2.;

vector lecture_table_clients(
    FILE* file)
{

    client user;

    vector base_donnee = make_vector(sizeof(user),0, growth_factor_doubling);
    
    while(fscanf(file, "%zu,%127[^,],%127[^,],%127[^,],%f,%127[^,]", &user.id, user.nom, user.code_postal, user.tel, &user.solde, user.mdp) == 6)
    {
        push_back(&base_donnee, &user);
    }
    return base_donnee;
}

vector lecture_table_livreurs(
    FILE* file)
{
   
    livreur livreurs;

    vector base_donnee = make_vector(sizeof(livreurs),0, growth_factor_doubling);
    
    while(fscanf(file, "%zu,%s,%s,%s,%zu,%f,%s", &livreurs.id, livreurs.nom, *livreurs.deplacements, livreurs.tel, &livreurs.restaurant, &livreurs.solde, livreurs.mdp) == 7)
    {
        push_back(&base_donnee, &livreurs);
    }
    return base_donnee;
}

vector lecture_table_restaurants(
    FILE* file)
{


    restaurant resto;

    vector base_donnee = make_vector(sizeof(resto),0, growth_factor_doubling);
    
    while(fscanf(file, "%zu,%s,%s,%s,%s,%s,%s,%f", &resto.id, resto.nom, resto.mdp, resto.code_postal, resto.tel, resto.type, resto.menu, &resto.solde) == 8)
    {
        push_back(&base_donnee, &resto);
    }
    return base_donnee;
}

vector lecture_table_items(
    FILE* file)
{
    float const growth_factor_doubling = 2.;

    item items;

    vector base_donnee = make_vector(sizeof(items),0, growth_factor_doubling);
    
    while(fscanf(file, "%zu,%s,%s,%f", &items.id, items.nom, *items.ingredients, &items.prix) == 4)
    {
        push_back(&base_donnee, &items);
    }
    return base_donnee;
}

void ecriture_table_clients(
    FILE* file,
    vector const* db)
{
        for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
        {
            client const* user = (client*)i.element;
            fprintf(file, "%zu,%s,%s,%s,%f,%s", user->id, user->nom, user->code_postal, user->tel, user->solde, user->mdp);
        }


}

void ecriture_table_livreurs(
    FILE* file,
    vector const* db)
{
        for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
        {
            livreur const* livreurs = (livreur*)i.element;
            fprintf(file, "%zu,%s,%s,%s,%zu,%f,%s", livreurs->id, livreurs->nom, (char*)livreurs->deplacements, livreurs->tel, livreurs->restaurant, livreurs->solde, livreurs->mdp);
        }


}

void ecriture_table_restaurants(
    FILE* file,
    vector const* db)
{
        for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
        {
            restaurant const* resto = (restaurant*)i.element;
            fprintf(file, "%zu,%s,%s,%s,%s,%s,%s,%lf", resto->id, resto->nom, resto->mdp, resto->code_postal, resto->tel, resto->type, resto->menu, resto->solde);
        }


}

void ecriture_table_items(
    FILE* file,
    vector const* db)
{
        for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
        {
            item const* items = (item*)i.element;
            fprintf(file, "%zu,%s,%s,%lf", items->id, items->nom, (char*)items->ingredients, items->prix);
        }


}
