# Embedded Systems Assignments - UniGe

Collezione di assignment per microcontroller PIC sviluppati con MPLAB X IDE, parte del corso di Embedded Systems all'Università di Genova.

## Progetti

### Assignment 0: LED Basic
- **Cartella**: `assignment-0-LED.X/`
- **Descrizione**: Controllo LED di base

### Assignment 1: LED Toggle
- **Cartella**: `assignment-1-LED-toggle.X/`
- **Descrizione**: Toggle LED con temporizzazione

### Assignment 2: Timers
- **Cartelle**: 
  - `assignment2_timers.X/`
  - `assignment2_2_timers.X/`
  - `assignment2_3_timers.X/`
- **Descrizione**: Utilizzo di timer per generare delay e controllo temporizzato

## Come Usare

### Requisiti
- MPLAB X IDE
- Compilatore XC16 per PIC24

### Apertura di un Progetto
1. Aprire MPLAB X IDE
2. **File → Open Project**
3. Selezionare la cartella `.X` desiderata (es. `assignment-0-LED.X`)

### Compilazione
```bash
make -C <cartella_progetto>
```

Oppure da MPLAB X IDE:
- **Ctrl+Shift+B** (Build)
- **Ctrl+Shift+F11** (Clean and Build)

## Struttura Progetto

Ogni progetto contiene:
- `main.c` - File principale
- Header files (`.h`) - Dichiarazioni e funzioni
- `nbproject/` - Configurazioni MPLAB X
- `Makefile` - Build rules

## Note
Questi assignment sono parte del curriculum dell'Università di Genova.
