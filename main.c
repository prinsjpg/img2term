#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include "render.h"

typedef enum
{
    MODALITA_COLOR = 1,
    MODALITA_BG = 2,
    MODALITA_HTML = 3
} Modalita;

int main(int argc, char *argv[])
{
    // 1. Controllo base degli argomenti
    if (argc < 2)
    {
        printf("Uso: %s <immagine.bmp> [-color | -bg]\n", argv[0]);
        return 1;
    }

    // --- NUOVO: CONTROLLO OPZIONI (FAIL FAST) ---
    Modalita scelta = MODALITA_COLOR; // Default: Testo colorato

    if (argc == 3)
    {
        if (strcmp(argv[2], "-bg") == 0)
        {
            scelta = MODALITA_BG;
        }
        else if (strcmp(argv[2], "-color") == 0)
        {
            scelta = MODALITA_COLOR;
        }
        else if (strcmp(argv[2], "-html") == 0)
        {
            scelta = MODALITA_HTML;
        }
        else
        {
            // ERRORE BLOCCANTE: stampiamo l'errore e fermiamo subito il programma
            printf("Errore: Opzione '%s' non riconosciuta.\n", argv[2]);
            printf("Usa '-color' per il testo colorato, '-bg' per i pixel pieni o '-html' per l'output HTML.\n");
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

    short bit_count;
    fseek(file, 28, SEEK_SET);
    if (fread(&bit_count, 2, 1, file) != 1)
    {
        printf("Errore: impossibile leggere il formato BMP.\n");
        fclose(file);
        return 1;
    }
    if (bit_count != 24)
    {
        printf("Errore: solo BMP a 24-bit supportati (trovato: %d bit).\n", bit_count);
        fclose(file);
        return 1;
    }

    // ALLOCAZIONE DINAMICA DELLA MATRICE PER SALVARE I CARATTERI ASCII DELL'IMMAGINE
    Pixel **immagine = alloca_immagine(larghezza, altezza);

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
                libera_immagine(immagine, altezza);
                fclose(file);
                return 1; // Chiude il programma immediatamente
            }
            immagine[r][c].r = bgr[2];
            immagine[r][c].g = bgr[1];
            immagine[r][c].b = bgr[0];
        }
        fseek(file, padding, SEEK_CUR); // Saltiamo i byte di troppo
    }

    // STAMPA DELL'IMMAGINE ASCII NEL TERMINALE
    if (scelta == MODALITA_HTML)
    {
        esporta_html(immagine, larghezza, altezza);
    }
    else
    {
        stampa_ascii(immagine, larghezza, altezza, scelta);
    }

    // PULIZIA DELLA MEMORIA ALLOCATA E CHIUSURA DEL FILE
    for (int i = 0; i < altezza; i++)
    {
        free(immagine[i]);
    }
    free(immagine);
    fclose(file);

    return 0;
}