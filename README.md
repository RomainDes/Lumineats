![](https://github.com/Amu-DevCommeLesPros-2021/DevCommeLesPros-2021-Projet/workflows/Run%20tests/badge.svg)

<!-- TOC ignore:true -->
# DevCommeLesPros-2021-Projet

Bienvenue sur la page du Projet final de la matiere Développer comme les pros de Romain DESMARECAUX, Emilien MARCHAL et Guillaume POLVERELLI, réalisé en 2021. Vous trouverez dans cette page le manuel d'utilisation de l'application.

<!-- TOC ignore:true -->
# Table des matières

<!-- TOC -->
 - [Lancement du programme](#lancement-du-programme)

- [Protocole de tests](#protocole-de-tests)

- [Interface utilisateur](#interface-utilisateur)    
    - [Interface de compte restaurant](#interface-de-compte-restaurant)
        - [Gestion de compte restaurant](#gestion-de-compte-restaurant)
    - [Interface de compte livreur](#interface-de-compte-livreur) 
        - [Gestion de compte livreur](#gestion-de-compte-livreur)  
    - [Interface de compte client](#interface-de-compte-client)
        - [Gestion de compte client](#gestion-de-compte-client)
        
<!-- /TOC -->
# Lancement du programme

Le programme peut être lancé suivant 2 modes distincts, le premier est celui de test, qui nous a permi de tester les différentes fonctions du programme, il se lance en entrant dans le terminal la fonction :
make test
Ensuite vient le mode utilisateur, celui ci permet d'utiliser le programme au travers de l'interface utilisateur. Pour lancer ce mode, la commande à entrer dans le terminal est 
make main

# Protocole de tests

Le protocole de test se compose d'abord d'une batterie de tests sur les fonctions de gestion des bases de données et des vecteurs qui s'acheve sur une impression du nombre de tests reussis par rapport au nombre de tests. Suite a cela, on peut trouver dans le fichier de test 3 groupes de tests commentés nous ayant servi a tester les fonctions des 3 modules (Client, Livreur, Utilisateur).

# Interface utilisateur

Au lancement de l'interface utilisateur, ce dernier devra choisir le type de compte qui lui correspond : 
1 - Restaurant
2 - Livreur
3 - Client

Pour selectionner, taper 1, 2 ou 3 au clavier puis entrée. 

Selon le type choisi, l'interface correspondante apparaitra alors.
On notera que dans chaque menu de ce type, il est possible de taper q pour quitter.

## Interface de compte restaurant

Une fois dans l'interface restaurant, un nouveau choix s'offre à nous. Il est possible de :
1 - Se connecter à un compte existant afin d'en consulter et/ou modifier les informations
2 - Creer un nouveau compte qui sera enregistré sur la plateforme

La encore, on navigue en tapant 1 ou 2 puis entrée

### Gestion de compte restaurant

Si on a choisi de se connecter à son compte restaurant, après avoir rentré correctement son nom et son mot de passe, on se retrouve dans l'interface de gestion de compte restaurant, qui présente un nouveau menu :
1 - Supprimer son compte, effaçant toutes les données de l'application.
2 - Modifier le menu, ce qui ouvre une nouvelle interface.
3 - Consultation du solde présent sur le compte.

On pourra également taper d pour se deconnecter et revenir au menu précédent.

Gestion du menu d'un restaurant

Si on se connecte à un compte, ou apres en avoir créé un, on se retrouve dans l'interface de gestion des menus, qui nous permet de 
1 - Créer un nouvel "item" (Produit) dans la base de donnée
2 - Ajouter un "item" au menu du restaurant
3 - Supprimer un item existant dans le menu et si il a été créé par le restaurant connécté, on le supprime de la base de donnée

Dans ces fonctions, il suffit de suivre les instructions apparaissant à l'ecran pour insérer les informations nécéssaires

## Interface de compte livreur

Une fois dans l'interface livreur, on se retrouve dans un nouveau menu présentant 2 choix :
1 - Se connecter à un compte existant afin d'en consulter et/ou modifier les informations
2 - Creer un nouveau compte qui sera enregistré sur la plateforme

La encore, on navigue en tapant 1 ou 2 puis entrée

### Gestion de compte livreur

Si on a choisi de se connecter à un compte livreur(ou apres en avoir créé un), on se retrouve la aussi devant un menu de gestion de compte 
, il y est possible de 
1 - Suppression du compte auquel on est connecté
2 - Modifier le compte auquel on est connecté, il est possible de modifier son numéro de télephone, les codes postaux où on livre et le restaurant auquel on est affilié (On peut également devenir indépendant ou arreter de l'être)
3 - Consultation du solde du compte auquel on est connecté

## Interface de compte client 

De même que pour les menus précédents, l'interface pour les comptes client propose 
1 - De se connecter à un compte existant
2 - De créer un nouveau compte

### Gestion de compte client

Une fois encore, apres s'etre connecté ou après avoir créé un nouveau compte, on peut acceder au menu suivant :
1 - Suppression du compte
2 - Modification du compte (Téléphone ou code postal de son adresse)
3 - Consultation du solde du compte
4 - Commander, qui permet de rechercher par plat ou par restaurant, et qui permet ensuite de passer commande de ce que l'on souhaite dans la liste nouvellement affichée.
