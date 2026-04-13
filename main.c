#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s <immagine.bmp>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        printf("Errore: Impossibile aprire il file.\n");
        return 1;
    }

    // 1. Estraiamo le dimensioni e l'offset
    int larghezza, altezza, offset_dati;

    fseek(file, 18, SEEK_SET);
    fread(&larghezza, 4, 1, file);
    fread(&altezza, 4, 1, file);

    fseek(file, 10, SEEK_SET);
    fread(&offset_dati, 4, 1, file);

    // ALLOCAZIONE DINAMICA DELLA MATRICE PER SALVARE I CARATTERI ASCII DELL'IMMAGINE
    char **immagine = (char **)malloc(altezza * sizeof(char *));
    if (immagine == NULL)
    {
        return 1;
    }

    for (int i = 0; i < altezza; i++)
    {
        immagine[i] = (char *)malloc(larghezza * sizeof(char));
        if (immagine[i] == NULL)
        {
            return 1;
        }
    }

    char tavolozza[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^";
    int padding = (4 - (larghezza * 3) % 4) % 4;

    // LETTURA DEI PIXEL E SALVATAGGIO NELLA MATRICE
    fseek(file, offset_dati, SEEK_SET);
    for (int riga = 0; riga < altezza; riga++)
    {
        for (int colonna = 0; colonna < larghezza; colonna++)
        {
            unsigned char pixel[3];
            fread(pixel, 1, 3, file);

            int grigio = (pixel[0] + pixel[1] + pixel[2]) / 3;
            int indice = (grigio * (strlen(tavolozza) - 1)) / 255;

            immagine[riga][colonna] = tavolozza[indice];
        }
        fseek(file, padding, SEEK_CUR); // Saltiamo i byte di troppo
    }

    // STAMPA DELL'IMMAGINE ASCII NEL TERMINALE
    // Stampa l'immagine al contrario per correggere l'orientamento
    for (int riga = altezza - 1; riga >= 0; riga--)
    {
        for (int colonna = 0; colonna < larghezza; colonna++)
        {
            // Stampa ogni carattere due volte per migliorare la proporzione
            printf("%c%c", immagine[riga][colonna], immagine[riga][colonna]);
        }
        printf("\n");
    }

    // PULIZIA DELLA MEMORIA ALLOCATA E CHIUSURA DEL FILE
    for (int i = 0; i < altezza; i++)
    {
        free(immagine[i]);
    }
    free(immagine);
    immagine = NULL; // Evitare dangling pointer

    fclose(file);
    return 0;
}