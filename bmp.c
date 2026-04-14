#include <stdlib.h>
#include "bmp.h"

Pixel **alloca_immagine(int larghezza, int altezza)
{
    Pixel **immagine = malloc(altezza * sizeof(Pixel *));
    for (int i = 0; i < altezza; i++)
        immagine[i] = malloc(larghezza * sizeof(Pixel));
    return immagine;
}

void libera_immagine(Pixel **immagine, int altezza)
{
    for (int i = 0; i < altezza; i++)
        free(immagine[i]);
    free(immagine);
}