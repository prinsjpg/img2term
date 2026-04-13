#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char carattere;
    int r, g, b;
} Pixel;

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    FILE *file = fopen(argv[1], "rb");
    if (!file)
        return 1;

    // 1. Estraiamo le dimensioni e l'offset
    int larghezza, altezza, offset_dati;

    fseek(file, 18, SEEK_SET);
    fread(&larghezza, 4, 1, file);
    fread(&altezza, 4, 1, file);

    fseek(file, 10, SEEK_SET);
    fread(&offset_dati, 4, 1, file);

    // ALLOCAZIONE DINAMICA DELLA MATRICE PER SALVARE I CARATTERI ASCII DELL'IMMAGINE
    Pixel **immagine = (Pixel **)malloc(altezza * sizeof(Pixel *));

    for (int i = 0; i < altezza; i++)
    {
        immagine[i] = (Pixel *)malloc(larghezza * sizeof(Pixel));
    }

    // LETTURA DEI PIXEL E SALVATAGGIO NELLA MATRICE
    char tavolozza[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^";
    int padding = (4 - (larghezza * 3) % 4) % 4;
    fseek(file, offset_dati, SEEK_SET);

    for (int r = 0; r < altezza; r++)
    {
        for (int c = 0; c < larghezza; c++)
        {
            unsigned char bgr[3];
            fread(bgr, 1, 3, file);
            immagine[r][c].r = bgr[2];
            immagine[r][c].g = bgr[1];
            immagine[r][c].b = bgr[0];
            int grigio = (bgr[0] + bgr[1] + bgr[2]) / 3;
            immagine[r][c].carattere = tavolozza[(grigio * (strlen(tavolozza) - 1)) / 255];
        }
        fseek(file, padding, SEEK_CUR); // Saltiamo i byte di troppo
    }

    // SCELTA UTENTE
    int scelta;
    printf("1. Testo colorato\n2. Sfondo colorato\nScelta: ");
    scanf("%d", &scelta);

    // STAMPA DELL'IMMAGINE ASCII NEL TERMINALE
    // Stampa l'immagine al contrario per correggere l'orientamento
    for (int r = altezza - 1; r >= 0; r--)
    {
        for (int c = 0; c < larghezza; c++)
        {
            switch (scelta)
            {
            case 1:
                printf("\033[38;2;%d;%d;%dm%c%c\033[0m",
                       immagine[r][c].r, immagine[r][c].g, immagine[r][c].b,
                       immagine[r][c].carattere, immagine[r][c].carattere);
                break;
            case 2:
                printf("\033[48;2;%d;%d;%dm  \033[0m",
                       immagine[r][c].r, immagine[r][c].g, immagine[r][c].b);
                break;
            }
        }
        printf("\n");
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