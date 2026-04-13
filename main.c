#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <immagine.bmp>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
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

    // 2. Prepariamo la griglia in memoria (usiamo static per sicurezza)
    static char immagine[2000][2000];
    char tavolozza[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^";
    int padding = (4 - (larghezza * 3) % 4) % 4;

    // 3. FASE DI LETTURA: Riempiamo la matrice
    fseek(file, offset_dati, SEEK_SET);
    for (int riga = 0; riga < altezza; riga++) {
        for (int colonna = 0; colonna < larghezza; colonna++) {
            unsigned char pixel[3];
            fread(pixel, 1, 3, file);

            int grigio = (pixel[0] + pixel[1] + pixel[2]) / 3;
            int indice = (grigio * (strlen(tavolozza) - 1)) / 255;
            
            // Salviamo nella matrice invece di stampare
            immagine[riga][colonna] = tavolozza[indice];
        }
        fseek(file, padding, SEEK_CUR); // Saltiamo i byte di troppo
    }

    // 4. FASE DI STAMPA: Partiamo dall'ultima riga salvata (altezza-1)
    // Questo raddrizza l'immagine che nel BMP è sottosopra! 🔄
    for (int riga = altezza - 1; riga >= 0; riga--) {
        for (int colonna = 0; colonna < larghezza; colonna++) {
            // Stampiamo due volte per correggere le proporzioni del terminale
            printf("%c%c", immagine[riga][colonna], immagine[riga][colonna]);
        }
        printf("\n");
    }

    fclose(file);
    return 0;
}