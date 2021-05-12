#include <stdio.h>
#include "livreur.h"

//Permet a un livreur de se connecter a son compte, renvoie l'id du compte ou on se 
//connecte si on y parvient, 0 sinon
int connecter_compte_livreur(){
    //On demande au livreur son nom, puis on ouvre la db livreur, on demande ensuite
    //a l'utilisateur d'entrer son mot de passe et on verifie si il correspond au mdp
    //present dans la db a la ligne correspondant au nom entré.
    //Si nom entré pas dans la db on redemande, idem pour mdp si il correspond pas
    return 0;
}

//Permet a un livreur de se creer un compte en entrant toutes ses informations
void creer_compte_livreur(){
    //On commence par creer une structure livreur.
    //On fait entrer a l'utilisateur les valeurs de nom,mdp,tel et 
    //on cree une struct vector dans laquelle l'utilisateur va rentrer
    //les differents codes postaux (apres avoir rentré au préalable 
    //le nombre de codes postaux ?)
    //On finit par transformer la struct livreur en csv pour ouvrir le fichier
    //et stocker les informations dedans (on l'ajoute a la fin de la db avec index
    //=indexmax + 1 et solde = 0)
}

//Permet a un liveur de supprimer son compte et toutes les information y etant contenues
void supprimer_compte_livreur(){
    //On ouvre le fichier csv des livreurs, on cherche l'index du compte ou on est 
    //connecté et on supprime la ligne dans le fichier csv
    //Ensuite, on fait "remonter" les lignes d'apres en réduisant tous leurs indexs
    //de 1
}

//Permet a un livreur de modifier les cp ou il lui est possible de livrer
void modifier_cp_livreur(){
    //On ouvre le fichier csv des livreurs, on récupere la ligne correspondant a 
    //l'index du compte ou on est connecté, on récupere sous forme de vecteur la liste
    //des codes postaux.
    //On remplace ensuite ce vecteur par un nouveau dans lequel l'utilisateur va rentrer 
    //la nouvelle liste de ses codes postaux
}

//Permet a un livreur de modifier son numero de telephone 
void modifier_tel_livreur(){
    //Idem qu'au dessus sauf qu'on modifie le string du telephone
}

//Permet a un livreur de modifier ou retirer son exclusivité à un restaurateur
void modifier_resto_livreur(){
    //On commence par faire la meme chose qu'au dessus
    //Ensuite on demande si on veut retirer ou changer l'exclu
    //Si on retire, on change la valeur a 0, et sinon on demande a l'utilisateur
    //de rentrer le nom du resto, on cherche ce nom sur la bd des restos et on
    //recup l'id de ce resto
}

//Permet a un livreur de modifier les differents elements vu ci dessus
void modifier_compte_livreur(){
    //On combine les trois fonctions au dessus en demandant a l'utilisateur ce qu'il
    //souhaite  modifier
}

//Permet à un liveur de consulter la somme d'argent qu'il a sur son solde 
void consulter_solde_livreur(){
    //On ouvre la bd livreur et on recup la valeur de la solde a la ligne du compte
    //auquel on est connecté
}