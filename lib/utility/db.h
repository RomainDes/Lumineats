#pragma once

#include "vector_api.h"
#include "vector_types.h"

#include <stdbool.h>
#include <stdio.h>

#define TAILLE_NOM 30
#define TAILLE_MDP 50
#define TAILLE_TYPE 20
#define TAILLE_MENU 40

#define MAX_CP 15
#define MAX_INGRE 10




vector lecture_table_clients(
    FILE* file);

vector lecture_table_livreurs(
    FILE* file);

vector lecture_table_restaurants(
    FILE* file);

vector lecture_table_items(
    FILE* file);

void ecriture_table_clients(
    FILE* file,
    vector const* db);

void ecriture_table_livreurs(
    FILE* file,
    vector const* db);

void ecriture_table_restaurants(
    FILE* file,
    vector const* db);

void ecriture_table_items(
    FILE* file,
    vector const* db);


