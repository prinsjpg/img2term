CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = img2term
OBJ = main.o bmp.o render.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)