#include <stdio.h>
#include <string.h>
#include "render.h"

const char tavolozza[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^";

void calcola_media(Pixel **immagine, int r, int c, int step,
                   int larghezza, 
                   int *out_r, int *out_g, int *out_b)
{
    int somma_r = 0, somma_g = 0, somma_b = 0;
    int pixel_contati = 0;

    for (int i = 0; i < step && (r - i) >= 0; i++)
        for (int j = 0; j < step && (c + j) < larghezza; j++) {
            somma_r += immagine[r - i][c + j].r;
            somma_g += immagine[r - i][c + j].g;
            somma_b += immagine[r - i][c + j].b;
            pixel_contati++;
        }

    *out_r = somma_r / pixel_contati;
    *out_g = somma_g / pixel_contati;
    *out_b = somma_b / pixel_contati;
}

void stampa_ascii(Pixel **immagine, int larghezza, int altezza, int scelta)
{
    const int lunghezza_tavolozza = strlen(tavolozza);
    int max_larghezza = 80;
    int step = 1;
    if (larghezza > max_larghezza)
    {
        step = larghezza / max_larghezza;
    }

    for (int r = altezza - 1; r >= 0; r -= step)
    {
        for (int c = 0; c < larghezza; c += step)
        {
            int media_r, media_g, media_b;
            calcola_media(immagine, r, c, step, larghezza,
            &media_r, &media_g, &media_b);

            int grigio = (media_r + media_g + media_b) / 3;
            char char_medio = tavolozza[(grigio * lunghezza_tavolozza) / 255];

            switch (scelta)
            {
            case 1:
                printf("\033[38;2;%d;%d;%dm%c%c\033[0m",
                       media_r, media_g, media_b,
                       char_medio, char_medio);
                break;
            case 2:
                printf("\033[48;2;%d;%d;%dm  \033[0m",
                       media_r, media_g, media_b);
                break;
            }
        }
        printf("\n");
    }
}

void esporta_html(Pixel **immagine, int larghezza, int altezza) {
    FILE *html = fopen("risultato.html", "w");
    if (!html) {
        printf("Errore: impossibile creare il file risultato.html\n");
        return;
    }

    // Intestazione della pagina web (sfondo nero e font monospazio)
    fprintf(html, "<html><body style='background-color: black; font-family: monospace; white-space: pre; line-height: 8px; font-size: 8px;'>\n");

    int max_larghezza = 150; // In HTML possiamo fare immagini un po' più larghe!
    int step = 1; 
    if (larghezza > max_larghezza) step = larghezza / max_larghezza;

    for (int r = altezza - 1; r >= 0; r -= step) {
        for (int c = 0; c < larghezza; c += step) {
            int media_r, media_g, media_b;
            calcola_media(immagine, r, c, step, larghezza,
              &media_r, &media_g, &media_b);

            int grigio = (media_r + media_g + media_b) / 3;
            char char_medio = tavolozza[(grigio * (strlen(tavolozza) - 1)) / 255];

            // Stampiamo un singolo carattere colorato dentro l'HTML!
            fprintf(html, "<span style='color: rgb(%d,%d,%d)'>%c</span>", media_r, media_g, media_b, char_medio);
        }
        fprintf(html, "<br>\n"); // Andiamo a capo nell'HTML
    }

    fprintf(html, "</body></html>\n");
    fclose(html);
    printf("Immagine esportata con successo in 'risultato.html'! Aprilo nel tuo browser.\n");
}