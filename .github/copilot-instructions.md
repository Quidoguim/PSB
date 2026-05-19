# Copilot Instructions

## Repository Overview

This is a university coursework repository for **Programação de Software Básico (PSB)** — a foundational C programming course. Each exercise list corresponds to a PDF assignment and contains standalone C programs that read from stdin and write to stdout.

## Repository Structure

```
Lista{N}/GuilhermeGuimaraesSantos/Exercicio{NNN}.c   ← exercise solutions
lista_de_exercicios{NNN}_PSB.pdf                      ← assignment PDFs
```

Topics progress across lists:
- **Lista1**: Basic arithmetic and I/O
- **Lista2**: Functions, strings, pointers
- **Lista3**: Structs
- **Lista4**: File I/O (text and binary)
- **Lista5**: Bit manipulation
- **Lista6**: (ongoing)

## Build & Run

Each file is a self-contained program. Compile and run individually:

```sh
gcc ExercicioNNN.c -o ExercicioNNN
echo "input here" | ./ExercicioNNN
```

No Makefile or build system — each exercise compiles independently.

## Code Conventions

### File Header

Every file begins with this comment block (in Portuguese):

```c
/*
 * Nome do arquivo: ExercicioNNN.c
 * Autor: Guilherme G. Santos
 * Propósito: [one-line description in Portuguese]
 * Data: YYYY-MM-DD
 */
```

`Propósito` and `Objetivo` are both used interchangeably for the purpose field.

### Input Handling

Always validate `scanf`/`fgets` return values — never read input unconditionally:

```c
if (scanf("%d", &n) == 1) {
    // process
}
```

For strings, use `fgets` and strip the trailing newline manually:

```c
if (fgets(str, TAM + 1, stdin) != NULL) {
    int len = strlen(str);
    if (str[len - 1] == '\n') str[len - 1] = '\0';
}
```

### Output

All output lines end with `\n`. Use `printf` (not `puts`).

### Naming

- Variables and functions: `snake_case`
- Constants (`#define`): `UPPER_CASE`
- Struct type aliases: `_t` suffix (e.g., `fracao_t`)
- Functions are defined before `main()`

### Types

- Use `int32_t` / `stdint.h` types when working with binary file formats
- Use `long long` for sums that may overflow `int`
- Use `unsigned int` when values are guaranteed non-negative

### Memory

All solutions use stack allocation only — no `malloc`/`free`.
