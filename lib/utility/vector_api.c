#include "vector_api.h"

#include "vector_types.h"

#include <stdlib.h>
#include <string.h>

vector make_vector(
    size_t const element_size,
    size_t const num_elements,
    float const growth_factor)
{
    if(num_elements > 0){
        return (vector){
                .data = malloc(num_elements * element_size),
                .element_size = element_size,
                .num_elements = num_elements,
                .growth_factor = growth_factor,
                .capacity = num_elements,
                };
    }
    else{
        return (vector){
                .data = NULL,
                .element_size = element_size,
                .num_elements = num_elements,
                .growth_factor = growth_factor,
                .capacity = num_elements,
                };
    }
}

void destroy(
    vector* v)
{
    if(v->data != NULL)  v->data = NULL;
}

size_t size(
    vector const v)
{
    return v.num_elements;
}

size_t capacity(
    vector const v)
{
    return v.capacity;
}

void push_back(
    vector* v,
    void const* data)
{
    
    if(v->capacity == 0){
        v->capacity = 1;
        v->data = malloc(v->capacity * v->element_size);
    }
    else if(size(*v) == capacity(*v)){
            v->capacity = v->capacity * v->growth_factor;
            v->data = realloc(v->data,  v->capacity * v->element_size);
    }
  

    memcpy(v->data + v->num_elements*v->element_size, data, v->element_size);
    v->num_elements = v->num_elements + 1;


    return;
}

void* pop_back(
    vector* v)
{
    void * p = malloc(v->element_size);

    if(v->num_elements == 0){
        return NULL;
    }
    else{
        memcpy(p, v->data + (v->num_elements-1)*v->element_size, v->element_size);
        v->num_elements = v->num_elements - 1;
    }
    return p;

}

void insert(
    vector* v,
    iterator const i,
    void const* data)
{
    int j;
    //Si v est NULL, on push_back i dans v
    if(v->capacity == 0){
        v->capacity = 1;
        v->data = malloc(v->capacity * v->element_size);
        memcpy(v->data + v->num_elements*v->element_size, data, v->element_size);
        v->num_elements = v->num_elements + 1;
    }
    //Sinon on décale tout ceux après d'un (si on dépasse la capacité on realloc) et on met data à i
    else{

        
        if(size(*v) == capacity(*v)){ //Si le nbre d'élément est égale à la capacité alors on realloue de la mémoire
            v->capacity = v->capacity * v->growth_factor;
            v->data = realloc(v->data,  v->capacity * v->element_size);
        }

        v->num_elements = v->num_elements + 1; //On rajoute 1 aux nbre d'élément pour savoir s'il faut rallouer avant d'insérer

        for(j = v->num_elements-1; v->data + j*v->element_size != *&i.element; j--){ //On décale tout ceux après l'index
            memcpy(v->data + j*v->element_size, v->data + (j - 1)*v->element_size, v->element_size);
        }
        memcpy(v->data + j*v->element_size, data, v->element_size); //On rajoute data à l'emplacement index 
        
    }
    return;
}

void erase(
    vector* v,
    iterator const i)
{
    //Si v est NULL, on push_back i dans v
    if(v->capacity == 0){
        return;
    }
    //Sinon on décale tout ceux après d'un (si on dépasse la capacité on realloc) et on met data à i
    else{

        for(int j = 0; j < v->num_elements; j++){
            if(v->data + j*v->element_size == *&i.element){//On cherche l'index de l'iterator i
                for(;j<v->num_elements;j++){ //A partir de celui ci on remplace la case par celle d'aprés jusqu'à num_element
                    memcpy(v->data + j*v->element_size, v->data + (j + 1)*v->element_size, v->element_size);
                }
            }
        }

        v->num_elements = v->num_elements - 1; //On rajoute 1 aux nbre d'élément pour savoir s'il faut rallouer avant d'insérer

        if(size(*v) == capacity(*v)/v->growth_factor){ //Si le nbre d'élément est égale à la capacité alors on realloue la mémoire divisé par growth
            v->capacity = v->capacity / v->growth_factor;
            v->data = realloc(v->data,  v->capacity * v->element_size);
        }
    }
    return;
}

void assign(
    vector* v,
    iterator first,
    iterator last)
{
    int j = 0;
    while(compare(first,last) != 0){//de first à last on copie colle
        if(v->num_elements == 0){//si le vecteur n'a pas d'élement on copie colle first
            push_back(v, first.element);
        }
        else{
            if(size(*v) == capacity(*v)){ //Si le nbre d'élément est égale à la capacité alors on realloue la mémoire divisé par growth
                v->capacity = v->capacity * v->growth_factor;
                v->data = realloc(v->data,  v->capacity * v->element_size);
            }
            memcpy(v->data + j*v->element_size, first.element, v->element_size); //On copie colle dans v_copy

            v->num_elements = v->num_elements + 1; //On rajoute 1 aux nbre d'élément pour savoir s'il faut rallouer avant d'insérer
        } 
        j++;
        first.element = first.element + v->element_size; //first prend first +1
    }
}

void clear(
    vector* v)
{
    for(; v->num_elements > 0; v->num_elements--){//tant que element n'est pas égal à 0 on supprime le contenu
        *(int*)(v->data + (v->num_elements-1)*v->element_size) = 0;
    }
}

void swap(
    vector* a,
    vector* b)
{
    //On échange les pointeurs vers la data 
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;

    //On échange leurs nombres d'élément
    size_t temp_nb_ele = a->num_elements;
    a->num_elements = b->num_elements;
    b->num_elements = temp_nb_ele;
}

void reserve(
    vector* v,
    size_t const num_elements)
{
    if(v->capacity < num_elements){ // Si la capacité est moindre que demandé, redimensionne le bloc de mémoire 

        v->data = realloc(v->data, num_elements * v->element_size); // Pour contenir *exactement* 'num_elements' éléments
        v->capacity = num_elements; //La capacity prend donc 'num_elements'
    }
    //Sinon aucun effet
}

void resize(
    vector* v,
    size_t const num_elements)
{
    if(v->num_elements < num_elements){ // Si le vecteur contient moins d'éléments que 'num_elements'
        v->data = realloc(v->data, num_elements * v->element_size); //On redimensionne le bloc de mémoire pour contenir *exactement* 'num_elements' éléments
        v->capacity = num_elements; //La capacité devient 'num_elements'.
        v->num_elements = num_elements;  //La valeur des nouveaux éléments introduits n'est pas définie donc le num_elements du vecteur prend 'num_elements'
    }
    else if(v->num_elements > num_elements){// Sinon si le vecteur contient plus d'éléments que 'num_elements'
        v->num_elements = num_elements; //Seul les 'num_elements' premiers éléments sont préservés et le nombre courant d'éléments devient 'num_elements'.
    }
}

void shrink_to_fit(
    vector* v)
{
    if(v->capacity > v->num_elements){ // Si la capacité est plus grande que le nombre d'éléments courant, .
        v->data = realloc(v->data, v->num_elements * v->element_size); //redimensionne le bloc de mémoire pour contenir *exactement* le nombre d'éléments courant
        v->capacity = v->num_elements; //Et donc la capacity prend donc le num_elements
    }
}

iterator begin(
    vector const* v)
{
    //return un iterator qui prend la valeur de la première valeur de v (bloc mémoire)
    //Si v n'est pas NULL sinon renvoie un iterator avec element NULL 
    return (iterator){
                .element = v -> data,
                .elem_size = v -> element_size,
                };
}

iterator end(
    vector const* v)
{
    //return un iterator qui prend la valeur de la DERNIÈRE valeur de 
    //qui signifie *(v.data+size(v)) (bloc mémoire)
    //Si v n'est pas NULL sinon renvoie un iterator avec element NULL
    return (iterator){
            .element = v->data + v->num_elements * v->element_size,
            .elem_size = v -> element_size,
            };
    
}

iterator at(
    vector const* v,
    size_t const index)
{
    return (iterator){
        .element = v->data + index * v->element_size,
        .elem_size = v -> element_size,
    };
}

int compare(
    iterator const a,
    iterator const b)
{
    if(a.element - b.element < 0)  return a.element - b.element;
    else if(a.element - b.element > 0)  return a.element - b.element;
    else    return 0;
}

void increment(
    iterator* i,
    size_t const num_elements)
{
    i->element = i->element + (i->elem_size * num_elements);
}

void decrement(
    iterator* i,
    size_t const num_elements)
{

    if(i != NULL){
        //*(&(i+1)->element) = i->element;
        i->element = i->element - (i->elem_size * num_elements);
    }
}

void* value(
    iterator const i)
{
    return i.element;
}

void set(
    iterator i,
    void const* data)
{
    *(int*)i.element = *(int*)data;

    return;
}