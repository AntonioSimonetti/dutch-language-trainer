# Dutch Language Trainer

Un'applicazione terminale per imparare l'olandese (A1), costruita in C99 con PDCurses.

## Obiettivo

Tool di apprendimento interattivo per vocabolario olandese con tracking dei progressi, sviluppato utilizzando tecnologie e tecniche degli anni '80.

## Tech Stack

- **Linguaggio:** C99
- **UI Library:** PDCurses (ncurses per Windows)
- **Compilatore:** GCC 15.2.0 (MinGW via MSYS2)
- **Build System:** Make
- **Compilazione:** Statica (eseguibile portabile)

## Requisiti di Sviluppo

- MSYS2 con ambiente UCRT64
- GCC
- PDCurses
- Make

## Compilazione
```bash
gcc -std=c99 -Wall -Wextra -static -o dutch_trainer.exe main.c -lpdcurses_wincon -lwinmm
```

Oppure con Make:
```bash
make
```

## Distribuzione

L'eseguibile è compilato staticamente e **non richiede DLL esterne**. Funziona su qualsiasi Windows senza installazioni aggiuntive.

## Contesto del Progetto

Parte di una serie di progetti che esplorano la storia della programmazione, ricreando software con le tecnologie e i vincoli delle rispettive epoche.

## License

MIT

## Autore

Antonio Simonetti - Progetto personale di apprendimento