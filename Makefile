# Definiamo il compilatore e le "flags" (opzioni di avviso e ottimizzazione)
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Il nome del nostro file eseguibile finale
TARGET = img2term

# La regola predefinita che viene eseguita quando scrivi "make"
all: $(TARGET)

# Come costruire il programma
$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

# Un comando utile per fare "pulizia"
clean:
	rm -f $(TARGET)