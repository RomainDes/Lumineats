#include "utility/db.h"
#include "utility/vector.h"
#include "link.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


float const growth_factor_doubling = 2.;


vector lecture_table_clients(
    FILE* file)
{
    char c;
    client user;

    vector base_donnee = make_vector(sizeof(user),0, growth_factor_doubling);
    
    while(c != EOF)
    {
        fscanf(file, "%zu,%127[^,],%127[^,],%127[^,],%f,%127[^\n]", &user.id, user.nom, user.code_postal, user.tel, &user.solde, user.mdp);
        c = fgetc(file);
        push_back(&base_donnee, &user);
    }
    return base_donnee;
}

vector lecture_table_livreurs(
    FILE* file)
{
    
    char c;
    int i = 0;
    int j = 0;

    livreur livreurs;
    vector base_donnee = make_vector(sizeof(livreurs),0, growth_factor_doubling);

    while(c != EOF){
        
        fscanf(file, "%zu,%127[^,],", &livreurs.id, livreurs.nom);

        while(c != ','){
            if(j==0)    livreurs.deplacements[i] = malloc(sizeof(char));
            c = fgetc(file);
            if(c == ';' || c == ','){
                livreurs.deplacements[i][j] = '\0';
                i++;
                j = 0;
            }
            else{
                livreurs.deplacements[i][j] = c;
                j++;
            }
        }
        livreurs.deplacements[i] = malloc(sizeof(char));
        livreurs.deplacements[i][j] = '\0';

        livreurs.nb_deplacements = i;
        i=0;
        fscanf(file,"%127[^,],%zu,%f,%127[^\n]", livreurs.tel, &livreurs.restaurant, &livreurs.solde, livreurs.mdp);
        push_back(&base_donnee, &livreurs);
        c = fgetc(file);
    }
    return base_donnee;
}

vector lecture_table_restaurants(
    FILE* file)
{

    int c = 0;

    int i = 0;

    restaurant resto;

    //resto = make_restaurant();

    vector base_donnee = make_vector(sizeof(resto),0, growth_factor_doubling);
    
    while(c != EOF){
        
        fscanf(file, "%zu,%127[^,],%127[^,],%127[^,],%127[^,],%127[^,],", &resto.id, resto.nom, resto.mdp, resto.code_postal, resto.tel, resto.type);

        while(fscanf(file,"%i;",&c) == 1){
            resto.menu[i] = c;
            i++;
        }
        c = fgetc(file);
        resto.menu[i] = 0;
        resto.nb_menu = i;

        i = 0;
        fscanf(file,"%f", &resto.solde);
        push_back(&base_donnee, &resto);
        c = fgetc(file);
    }
    return base_donnee;
}

vector lecture_table_items(
    FILE* file)
{
    char c;
    int i = 0;
    int j = 0;

    item items;
    vector base_donnee = make_vector(sizeof(items),0, growth_factor_doubling);

    while(c != EOF){
        
        fscanf(file, "%zu,%zu,%127[^,],", &items.id, &items.restaurant, items.nom);

        while(c != ',' && c != EOF){
            if(j==0)    items.ingredients[i] = malloc(sizeof(char));
            c = fgetc(file);
            if(c == ';' || c == ','){
                items.ingredients[i][j] = '\0';
                i++;
                j = 0;
            }
            else{
                items.ingredients[i][j] = c;
                j++;
            }
        }
        items.ingredients[i] = malloc(sizeof(char));
        items.ingredients[i][j] = '\0';

        items.nb_ingr = i;
        i=0;
        fscanf(file,"%f", &items.prix);
        push_back(&base_donnee, &items);
        c = fgetc(file);
    }
    return base_donnee;
}
    


void ecriture_table_clients(
    FILE* file,
    vector const* db)
{
    int j = 0;

    for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1)){
        if(j != 0) fputc('\n', file);
        client const* user = (client*)i.element;
        fprintf(file, "%zu,%s,%s,%s,%.2f,%s", user->id, user->nom, user->code_postal, user->tel, user->solde, user->mdp);
        j++;
    }


}

void ecriture_table_livreurs(
    FILE* file,
    vector const* db)
{
    int j = 0;
    for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
    {
        if(j != 0)  fputc('\n', file);
        livreur const* livreurs = (livreur*)i.element;
        fprintf(file, "%zu,%s,", livreurs->id, livreurs->nom);
        for(int j = 0; j < livreurs->nb_deplacements; j++){
            fputs(livreurs->deplacements[j], file);
            if(j != livreurs->nb_deplacements-1)    fputc(';', file);   
        }
        fprintf(file, ",%s,%zu,%.2f,%s", livreurs->tel, livreurs->restaurant, livreurs->solde, livreurs->mdp);
        j++;
    }
}

void ecriture_table_restaurants(
    FILE* file,
    vector const* db)
{
    int cpt = 0;
    for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1)){
        if(cpt != 0)  fputc('\n', file);
        restaurant const* resto = (restaurant*)i.element;
        fprintf(file, "%zu,%s,%s,%s,%s,%s,", resto->id, resto->nom, resto->mdp, resto->code_postal, resto->tel, resto->type);
        for(int j = 0; j < resto->nb_menu; j++){
            fprintf(file,"%i",resto->menu[j]);
            if(j != resto->nb_menu-1)    fputc(';', file);   
        }
        fprintf(file,",%.2f",  resto->solde);
        cpt++;
    }
}

void ecriture_table_items(
    FILE* file,
    vector const* db)
{
    int j = 0;
    for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
    {
        if(j != 0)  fputc('\n', file);
        item const* items = (item*)i.element;
        fprintf(file, "%zu,%zu,%s,", items->id, items->restaurant, items->nom);
        for(int j = 0; j < items->nb_ingr; j++){
            fputs(items->ingredients[j], file);
            if(j != items->nb_ingr-1)    fputc(';', file);   
        }
        fprintf(file, ",%.2f", items->prix);
        j++;
    }
}
