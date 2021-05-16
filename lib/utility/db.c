#include "db.h"

#include "vector_api.h"
#include "vector_types.h"

#include <stdbool.h>
#include <stdio.h>




vector lecture_table_client(
    FILE* file)
{
    float const growth_factor_doubling = 2.;

    client user;

    vector base_donnee = make_vector(sizeof(user),0, growth_factor_doubling);
    
    while(fscanf(file, "%zu,%127[^,],%zu,%127[^,],%f,%127[^,]", &user.id, user.nom, &user.code_postal, user.telephone, user.solde, user.mot_de_passe) == 6)
    {
        push_back(&base_donnee, &user);
    }
    return base_donnee;
}

vector lecture_table_livreur(
    FILE* file)
{
    float const growth_factor_doubling = 2.;

    livreur livreur;

    vector base_donnee = make_vector(sizeof(livreur),0, growth_factor_doubling);
    
    while(fscanf(file, "%zu,%127[^,],%zu,%127[^,],%f,%127[^,]", &livreur.id, livreur.nom, &livreur.code_postal, livreur.telephone, livreur.solde, livreur.mot_de_passe) == 6)
    {
        push_back(&base_donnee, &livreur);
    }
    return base_donnee;
}

vector lecture_table_restaurant(
    FILE* file)
{
    float const growth_factor_doubling = 2.;

    restaurant restau;

    vector base_donnee = make_vector(sizeof(restau),0, growth_factor_doubling);
    
    while(fscanf(file, "%zu,%127[^,],%127[^,],%zu,%127[^,],%127[^,],%zu,%f", &restau.id, restau.nom, restau.mdp, &restau.cp, restau.tel, restau.type, &restau.menu, restau.solde) == 8)
    {
        push_back(&base_donnee, &restau);
    }
    return base_donnee;
}

void ecriture_table_client(
    FILE* file,
    vector const* db)
{
        for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
        {
            client const* user = (client*)i.element;
            fprintf(file, "%zu,%127[^,],%zu,%127[^,],%f,%127[^,]\n", user->id, user->nom, user->code_postal, user->telephone, user->solde, user->mot_de_passe);
        }


}

void ecriture_table_livreur(
    FILE* file,
    vector const* db)
{
        for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
        {
            livreur const* livreur = (livreur*)i.element;
            fprintf(file, "%zu,%127[^,],%zu,%127[^,],%f,%127[^,]\n", livreur->id, livreur->nom, livreur->code_postal, livreur->telephone, livreur->solde, livreur->mot_de_passe);
        }


}

void ecriture_table_restaurant(
    FILE* file,
    vector const* db)
{
        for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
        {
            restaurant const* restau = (restaurant*)i.element;
            fprintf(file, "%zu,%127[^,],%127[^,],%zu,%127[^,],%127[^,],%zu,%f\n", restau->id, restau->nom, restau->mdp, restau->cp, restau->tel, restau->type, restau->menu, restau->solde);
        }


}
