# Sudoku Validator

A C program to validate sudoku puzzles.

## Files

- `groupX.c` - Source code for the sudoku validator
- `sudoku_validator` - Compiled executable
- `test.txt` - Test input file

## Compilation

```bash
gcc -o sudoku_validator groupX.c
```

## Usage

```bash
./sudoku_validator < test.txt
```

## Features

- Validates sudoku puzzles according to standard rules
- Reads input from stdin
- Outputs validation results
