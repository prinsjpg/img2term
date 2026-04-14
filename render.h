#ifndef RENDER_H
#define RENDER_H

#include "bmp.h"

void calcola_media(Pixel **immagine, int r, int c, int step,
                   int larghezza,
                   int *out_r, int *out_g, int *out_b);

void stampa_ascii(Pixel **immagine, int larghezza, int altezza, int scelta);

void esporta_html(Pixel **immagine, int larghezza, int altezza);

#endif