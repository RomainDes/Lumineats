#pragma once

#include <stdlib.h>

// Contient de zéro à N éléments, chacun de taille 'element_size' dans un seul
// bloc de mémoire contigue.
// Au moment de créer un nouveau vecteur, 'element_size' est donné et ne changera 
// pas pour ce vecteur. Bien entendu, d'autres vecteur peuvent être créés avec un 
// 'element_size' différent.
// Le bloc de mémoire contigue pointé par 'data' est toujours d'au moins 'N * element_size'.
// Si, au moment d'ajouter un nouvel élément, le bloc de mémoire alloué est de 
// taille insuffisante le bloc sera redimensionné par un facteur donné ('growth_factor'
// de la fonction 'make_vector').
// Réf. : https://fr.cppreference.com/w/cpp/container/vector
typedef struct vector
{
    size_t const element_size;
    size_t num_elements; //nombre d'élément dans le vecteur dont chacun à une taille égale
                               //à element_size
    size_t capacity;

    float const growth_factor; //growth_factor est le facteur de croissance par lequel il faut redimensionner
    void *data; //Bloc mémoire du vecteur qui peut être modifié mais qui est alloué de 
                //de num_element * element_size octets

    // D'autre membres peuvent être ajoutés, si nécessaire.
} vector;

// Un itérateur est l'abstraction d'un index dans un vecteur et fait référence 
// à un élément.
// Les fonctions qui utilisent les itérateurs prennent typiquement deux itérateurs 
// 'first' et 'last' et l'itérateur 'last' n'est pas inclus. Il fait office de 
// sentinelle. Exprimé mathématiquement : "[first, last[" plutôt que "[first, last]".
// Il peut être incrémenté, décrémenté, comparé avec un autre itérateur et 
// servira d'interface avec les fonctions algorithmiques dans le prochain exercice.
typedef struct iterator
{
    void *element;
    size_t elem_size; //Ajout de 'elem_size' qui correspond à 'element_size' dans la struct vector

    // D'autre membres peuvent être ajoutés, si nécessaire.
} iterator;
