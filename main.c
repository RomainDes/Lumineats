#include "utility/db.h"
#include "utility/vector.h"
#include "link.h"

#include <stdio.h>

int main()
{
    int again = 0;
    do{
        printf("\n");
        system("clear");
        //Page principal

        printf("*** Bienvenue sur Lumineats, la livraison à vitesse luminique ***\n\n* Menu Principal *\n\nVous êtes :\n1. Un·e restaurateur(trice)\n2. Un·e livreur(se)\n3. Un·e client(e)\nVotre choix ('q' pour quitter) : ");

        char operation = getchar();
        switch(operation)
        {
                    
            case '1':
                again = menu_restaurant();
                break;
            case '2':
                printf("Livreur\n");
                break;
            case '3':
                printf("Client\n");
                break;
            case 'q':
                again = 1;
                break;
            default :
                again = 0;
                        
        }
    }   while(again == 0);

    return 0;
}