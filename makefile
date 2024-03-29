# La cible par défault sera le programme de test.
.DEFAULT_GOAL := check  #'check' afin de lancer directement le programme
 						# de test à l'invite de commandes en tapant 'make'.

.PHONY: clean

# Liste des cibles.

# Efface tous les fichiers temporaires et les produits finaux.
clean:
	rm -rf build

# Crée le répertoire qui acceuille les fichiers temporaires et les produits 
# finaux
build:
	mkdir -p build

# Ajout des compilations du code

build/db.o: lib/utility/db.c lib/utility/db.h lib/utility/vector.h lib/link.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/utility/db.c -I ./lib -o build/db.o

build/vector_api.o: lib/utility/vector_api.c lib/utility/vector_api.h lib/utility/vector_types.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/utility/vector_api.c -I ./lib/utility -o build/vector_api.o

build/livreur.o: lib/livreur.c lib/livreur.h lib/utility/vector.h lib/utility/db.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/livreur.c -I ./lib -o build/livreur.o

build/client.o: lib/client.c lib/client.h lib/restaurant.h lib/utility/vector.h lib/utility/db.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/client.c -I ./lib -o build/client.o

build/restaurant.o: lib/restaurant.c lib/restaurant.h lib/utility/vector.h lib/utility/db.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/restaurant.c -I ./lib -o build/restaurant.o

# build/interface.o: lib/interface.c | build
# 	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/interface.c -I ./lib -o build/interface.o

# build/interface: build/interface.o build/libvector.a build/libdb.a build/liblink.a | build
# 	gcc build/interface.o -L build -l vector -l db -l link -o build/interface


# Lier les bibliothèques statique (ajout des fichiers objets dans la ligne de commande)


build/libdb.a: lib/utility/db.h build/db.o | build
	ar crs build/libdb.a build/db.o

build/libvector.a: lib/utility/vector.h build/vector_api.o | build
	ar crs build/libvector.a build/vector_api.o

build/liblink.a: lib/link.h build/restaurant.o build/client.o build/livreur.o| build
	ar crs build/liblink.a build/restaurant.o build/client.o build/livreur.o


# Programme de test.

build/test.o: test/test.c | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c test/test.c -I ./lib -o build/test.o

build/test: build/test.o build/libvector.a build/libdb.a build/liblink.a | build
	gcc build/test.o -L build -l vector -l db -l link -o build/test

check: build/test
	./build/test

# Progamme main

build/main.o: main.c | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c main.c -I ./lib -o build/main.o

build/main: build/main.o build/libvector.a build/libdb.a build/liblink.a | build
	gcc build/main.o -L build -l link -l db -l vector -o build/main

# S'assure que le programme build/main existe et le lance à l'invite de commande.
main: build/main
	./build/main

