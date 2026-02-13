/*
 * Lab 3 - Sudoku Validator
 * This is the code skeleton for Lab 3. If you do not need it, you may also build your code from scratch.
 *
 * Input : 9x9 integers from stdin (1..9, well-formed)
 * Output: print 1 if valid, else print 0
 * Do not print extra text (debug messages). Print only 0\n or 1\n.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sudoku[9][9];

typedef struct {
    int row;
    int column;
    int index;
    int *results;
} parameters;

/* thread entry functions */
void *check_row(void *arg) {
    parameters *p = (parameters *)arg;
    
    int seen[10] = {0}; // seen[i] tracks if number i has been seen
    int row = p->row;
    
    for (int col = 0; col < 9; col++) {
        int num = sudoku[row][col];
        if (num < 1 || num > 9 || seen[num]) {
            p->results[p->index] = 0;
            free(p);
            return NULL;
        }
        seen[num] = 1;
    }
    
    p->results[p->index] = 1;
    free(p);
    return NULL;
}

void *check_column(void *arg) {
    parameters *p = (parameters *)arg;
    
    int seen[10] = {0}; // seen[i] tracks if number i has been seen
    int col = p->column;
    
    for (int row = 0; row < 9; row++) {
        int num = sudoku[row][col];
        if (num < 1 || num > 9 || seen[num]) {
            p->results[p->index] = 0;
            free(p);
            return NULL;
        }
        seen[num] = 1;
    }
    
    p->results[p->index] = 1;
    free(p);
    return NULL;
}

void *check_grid(void *arg) {
    parameters *p = (parameters *)arg;
    
    int seen[10] = {0}; // seen[i] tracks if number i has been seen
    int start_row = p->row;
    int start_col = p->column;
    
    for (int row = start_row; row < start_row + 3; row++) {
        for (int col = start_col; col < start_col + 3; col++) {
            int num = sudoku[row][col];
            if (num < 1 || num > 9 || seen[num]) {
                p->results[p->index] = 0;
                free(p);
                return NULL;
            }
            seen[num] = 1;
        }
    }
    
    p->results[p->index] = 1;
    free(p);
    return NULL;
}

int main(void) {
    /* Standard legal Sudoku, 
     * you can use it to test your program

    int test_sudoku[9][9] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };*/

    // Read sudoku[9][9] from stdin
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            scanf("%d", &sudoku[i][j]);
        }
    }

    int results[27] = {0};
    pthread_t threads[27];
    int thread_index = 0;

    // Create threads for rows (0-8)
    for (int row = 0; row < 9; row++) {
        parameters *p = (parameters *)malloc(sizeof(parameters));
        p->row = row;
        p->column = 0;
        p->index = thread_index;
        p->results = results;
        pthread_create(&threads[thread_index], NULL, check_row, (void *)p);
        thread_index++;
    }

    // Create threads for columns (9-17)
    for (int col = 0; col < 9; col++) {
        parameters *p = (parameters *)malloc(sizeof(parameters));
        p->row = 0;
        p->column = col;
        p->index = thread_index;
        p->results = results;
        pthread_create(&threads[thread_index], NULL, check_column, (void *)p);
        thread_index++;
    }

    // Create threads for 3x3 grids (18-26)
    for (int box_row = 0; box_row < 3; box_row++) {
        for (int box_col = 0; box_col < 3; box_col++) {
            parameters *p = (parameters *)malloc(sizeof(parameters));
            p->row = box_row * 3;
            p->column = box_col * 3;
            p->index = thread_index;
            p->results = results;
            pthread_create(&threads[thread_index], NULL, check_grid, (void *)p);
            thread_index++;
        }
    }

    // Join all threads and wait for completion
    for (int i = 0; i < 27; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check all results
    int valid = 1;
    for (int i = 0; i < 27; i++) {
        if (results[i] == 0) {
            valid = 0;
            break;
        }
    }

    printf("%d\n", valid);

    return 0;
}
