#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char carattere;
    int r, g, b;
} Pixel;

const char tavolozza[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^";

void stampa_ascii(Pixel **immagine, int larghezza, int altezza, int scelta)
{
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
            int somma_r = 0, somma_g = 0, somma_b = 0;
            int pixel_contati = 0;

            for (int i = 0; i < step && (r - i) >= 0; i++)
            {
                for (int j = 0; j < step && (c + j) < larghezza; j++)
                {
                    somma_r += immagine[r - i][c + j].r;
                    somma_g += immagine[r - i][c + j].g;
                    somma_b += immagine[r - i][c + j].b;
                    pixel_contati++;
                }
            }

            int media_r = somma_r / pixel_contati;
            int media_g = somma_g / pixel_contati;
            int media_b = somma_b / pixel_contati;

            int grigio = (media_r + media_g + media_b) / 3;
            char char_medio = tavolozza[(grigio * (strlen(tavolozza) - 1)) / 255];

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

int main(int argc, char *argv[])
{
    // 1. Controllo base degli argomenti
    if (argc < 2)
    {
        printf("Uso: %s <immagine.bmp> [-color | -bg]\n", argv[0]);
        return 1;
    }

    // --- NUOVO: CONTROLLO OPZIONI (FAIL FAST) ---
    int scelta = 1; // Default: Testo colorato

    if (argc == 3)
    {
        if (strcmp(argv[2], "-bg") == 0)
        {
            scelta = 2;
        }
        else if (strcmp(argv[2], "-color") == 0)
        {
            scelta = 1;
        }
        else
        {
            // ERRORE BLOCCANTE: stampiamo l'errore e fermiamo subito il programma
            printf("Errore: Opzione '%s' non riconosciuta.\n", argv[2]);
            printf("Usa '-color' per il testo colorato o '-bg' per i pixel pieni.\n");
            return 1;
        }
    }
    // ---------------------------------------------

    // 2. Solo se i comandi sono corretti, procediamo ad aprire il file...
    FILE *file = fopen(argv[1], "rb");
    if (!file)
    {
        printf("Errore: Impossibile aprire il file '%s'.\n", argv[1]);
        return 1;
    }

    char magic[2];
    if (fread(magic, 1, 2, file) != 2)
    {
        printf("Errore: Impossibile leggere i primi byte del file.\n");
        fclose(file);
        return 1;
    }
    if (magic[0] != 'B' || magic[1] != 'M')
    {
        printf("Errore: Il file fornito non è un'immagine BMP valida!\n");
        fclose(file);
        return 1;
    }

    // 1. Estraiamo le dimensioni e l'offset
    int larghezza, altezza, offset_dati;

    fseek(file, 18, SEEK_SET);
    if (fread(&larghezza, 4, 1, file) != 1 || fread(&altezza, 4, 1, file) != 1)
    {
        printf("Errore: Impossibile leggere le dimensioni dell'immagine.\n");
        fclose(file);
        return 1;
    }

    fseek(file, 10, SEEK_SET);
    if (fread(&offset_dati, 4, 1, file) != 1)
    {
        printf("Errore: Impossibile leggere la posizione dei dati.\n");
        fclose(file);
        return 1;
    }

    // ALLOCAZIONE DINAMICA DELLA MATRICE PER SALVARE I CARATTERI ASCII DELL'IMMAGINE
    Pixel **immagine = (Pixel **)malloc(altezza * sizeof(Pixel *));

    for (int i = 0; i < altezza; i++)
    {
        immagine[i] = (Pixel *)malloc(larghezza * sizeof(Pixel));
    }

    // LETTURA DEI PIXEL E SALVATAGGIO NELLA MATRICE
    int padding = (4 - (larghezza * 3) % 4) % 4;
    fseek(file, offset_dati, SEEK_SET);

    for (int r = 0; r < altezza; r++)
    {
        for (int c = 0; c < larghezza; c++)
        {
            unsigned char bgr[3];
            if (fread(bgr, 1, 3, file) != 3)
            {
                printf("Errore critico: I dati dell'immagine sono tagliati o incompleti!\n");
                return 1; // Chiude il programma immediatamente
            }
            immagine[r][c].r = bgr[2];
            immagine[r][c].g = bgr[1];
            immagine[r][c].b = bgr[0];
            int grigio = (bgr[0] + bgr[1] + bgr[2]) / 3;
            immagine[r][c].carattere = tavolozza[(grigio * (strlen(tavolozza) - 1)) / 255];
        }
        fseek(file, padding, SEEK_CUR); // Saltiamo i byte di troppo
    }

    // STAMPA DELL'IMMAGINE ASCII NEL TERMINALE
    stampa_ascii(immagine, larghezza, altezza, scelta);

    // PULIZIA DELLA MEMORIA ALLOCATA E CHIUSURA DEL FILE
    for (int i = 0; i < altezza; i++)
    {
        free(immagine[i]);
    }
    free(immagine);
    fclose(file);

    return 0;
}