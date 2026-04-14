#ifndef BMP_H
#define BMP_H

typedef struct
{
    char carattere;
    int r, g, b;
} Pixel;

Pixel **alloca_immagine(int larghezza, int altezza);
void libera_immagine(Pixel **immagine, int altezza);

#endif