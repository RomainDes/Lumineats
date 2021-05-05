#include "db.h"

#include "vector_api.h"
#include "vector_types.h"

#include <stdbool.h>
#include <stdio.h>




vector lecture_table(
    FILE* file)
{
    float const growth_factor_doubling = 2.;

    docteur doctor;

    vector base_donnee = make_vector(sizeof(doctor),0, growth_factor_doubling);
    
    while(fscanf(file, "%zu,%127[^,],%i,%zu", &doctor.index, doctor.nom, &doctor.specialites, &doctor.index_hopital) == 4)
    {
        push_back(&base_donnee, &doctor);
    }
    return base_donnee;
}

void ecriture_table(
    FILE* file,
    vector const* db)
{
        for(iterator i = begin(db), e = end(db); compare(i, e) != 0; increment(&i, 1))
        {
            docteur const* doctor = (docteur*)i.element;
            fprintf(file, "%zu,%s,%i,%zu\n", doctor->index, doctor->nom, doctor->specialites, doctor->index_hopital);
        }


}

bool est_generaliste(
    void const* d)
{

    enum specialite_t generaliste = 0x8;
    
    if((char)generaliste <= *(char*)(d+44) && *(char*)(d+44) != (char)16 )    return true;//Je ne sais pas comment savoir s'il est MEDECINE_GENERALE
    else    return false;
}
