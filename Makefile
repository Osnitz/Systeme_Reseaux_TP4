# Nom du fichier exécutable
CLIENT_EXEC = client

# Fichiers source et d'en-tête
SRC = client.c
OBJ = $(SRC:.c=.o)
LIBS = -L. -lseg

# Options du compilateur
CC = gcc
CFLAGS = -Wextra -Wall -I.  # -I. pour inclure le fichier segdef.h dans le répertoire courant
LDFLAGS = -L. -lseg  # Liens avec la bibliothèque libseg.a

# Règle par défaut : compiler le client
all: $(CLIENT_EXEC)

# Compilation du programme client
$(CLIENT_EXEC): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

# Compilation du fichier source client.c
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJ) $(CLIENT_EXEC)

# Règle pour exécuter le programme
run: $(CLIENT_EXEC)
	./$(CLIENT_EXEC)
