# img2term

Converte immagini BMP in ASCII art nel terminale.

## Compilazione

    make

## Utilizzo

    ./img2term <immagine.bmp> [opzioni]

## Opzioni

| Opzione      | Effetto                          |
|--------------|----------------------------------|
| `-color`     | Testo colorato (default)         |
| `-bg`        | Pixel pieni colorati             |
| `-html`      | Esporta `risultato.html`         |
| `-w <N>`     | Larghezza in colonne (default 80)|

## Esempi

    ./img2term foto.bmp
    ./img2term foto.bmp -color -w 120
    ./img2term foto.bmp -html -w 300

## Pulizia

    make clean
