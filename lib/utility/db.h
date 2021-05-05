#pragma once

#include "vector_api.h"
#include "vector_types.h"

#include <stdbool.h>
#include <stdio.h>

#define TAILLE_CHAMP_NOM 36

vector lecture_table(
    FILE* fichier);

void ecriture_table(
    FILE* fichier,
    vector const* db);

// Renvoie 'vrai' si le docteur compte 'MEDECINE_GENERALE' parmi ses spécialités.
bool est_generaliste(
    void const* d);
