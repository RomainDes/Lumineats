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

build/db.o: lib/utility/db.c lib/utility/db.h lib/utility/vector_api.h lib/utility/vector_types.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/utility/db.c -I ./lib/utility -o build/db.o

build/vector_api.o: lib/utility/vector_api.c lib/utility/vector_api.h lib/utility/vector_types.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/utility/vector_api.c -I ./lib/utility -o build/vector_api.o

build/livreur.o: lib/livreur.c lib/livreur.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/livreur.c -I ./lib -o build/livreur.o

build/client.o: lib/client.c lib/client.h lib/restaurant.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/client.c -I ./lib -o build/client.o

build/restaurant.o: lib/restaurant.c lib/restaurant.h | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c lib/restaurant.c -I ./lib -o build/restaurant.o

# Lier les bibliothèques statique (ajout des fichiers objets dans la ligne de commande)


build/libdb.a: lib/utility/db.h build/db.o | build
	ar crs build/libdb.a build/db.o

build/libvector.a: lib/utility/vector.h build/vector_api.o | build
	ar crs build/libvector.a build/vector_api.o

# Programme de test.

build/test.o: test/test.c | build
	gcc -Wno-pointer-arith -Wall -Werror -pedantic --debug -c test/test.c -I ./lib/utility -o build/test.o

build/test: build/test.o build/libvector.a build/libdb.a | build
	gcc build/test.o -L build -l vector -l db -o build/test

# S'assure que le programme build/test existe et le lance à l'invite de commande.
check: build/test
	./build/test

