# Biliardo 2D
![immagine](https://raw.githubusercontent.com/Programmazione-per-la-Fisica/progetto2023/e3516bd9d6801fe48479eff680af87f8d113eca7/biliardo.svg) <br>
Il programma simula un biliardo simmetrico bidimensionale come in figura.

## Funzionalità

Il programma calcola e mostra a schermo (con tanto di animazione) gli esiti del lancio di una palline con condizioni iniziali come fornite dall'utente. Per attivare questa funzionalità premere il pulsante throw. 
Premendo normal distribution invece vengono effettuati n lanci in cui y_0 e theta_0 vengono generate randomicamente secondo disribuzioni normali con parametri decisi dall'utente e vengono mostrati, nella parte destra della finestra, gli istogrammi rappresentanti le distribuzioni degli esiti di tali lanci.

## Utilizzo

### Librerie utilizzate

SFML, TGui, Root

### Installazione

Se si hanno già scaricate tutte le librerie sopracitate, per clonare l'intera repository bisogna utilizzare:
```
git clone https://github.com/feynman77/biliardo.git

```
Se non si è interessati ai commit precedenti ma solo al programma finito invece:
```
git clone --depth 1 https://github.com/feynman77/biliardo.git

```


### Esecuzione

A seguito dell'installazione, basterà utilizzare sul terminale i seguenti comandi in sequenza:
```
cd biliardo
mkdir build
cmake -S . -B build
cmake --build build
cd build
.\biliardo
```

## Autori

Simone Cervellera
Colombo Francesco

