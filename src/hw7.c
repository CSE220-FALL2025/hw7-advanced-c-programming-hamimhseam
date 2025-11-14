#include "hw7.h"
#include <stdlib.h>

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    return NULL;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    return NULL;
}

void free_bst_sf(bst_sf *root) {
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    int num_rows = mat1->num_rows;
    int num_cols = mat1->num_cols;

    int res_len = num_rows * num_cols;
    matrix_sf *res = malloc(sizeof(matrix_sf) + res_len * sizeof(int));
    res->num_rows = num_rows;
    res->num_cols = num_cols;

    for (int i = 0; i < res_len; i++) {
        res->values[i] = mat1->values[i] + mat2->values[i];
    }

    return res;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    int m = mat1->num_rows;
    int n = mat2->num_cols;
    int r = mat1->num_cols;

    int res_len = m * n;
    matrix_sf *res = calloc(1, sizeof(matrix_sf) + res_len * sizeof(int));
    res->num_rows = m;
    res->num_cols = n;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < r; k++) {
                res->values[i * n + j] +=
                    mat1->values[i * r + k] * mat2->values[k * n + j];
            }
        }
    }

    return res;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    int m = mat->num_rows;
    int n = mat->num_cols;

    int res_len = n * m;
    matrix_sf *res = malloc(sizeof(matrix_sf) + res_len * sizeof(int));
    res->num_rows = n;
    res->num_cols = m;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            res->values[j * m + i] = mat->values[i * n + j];
        }
    }

    return res;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    char* s = expr;

    int m = strtol(s, &s, 10);
    int n = strtol(s, &s, 10);

    int res_len = m * n;
    matrix_sf *res = malloc(sizeof(matrix_sf) + res_len * sizeof(int));
    res->name = name;
    res->num_rows = m;
    res->num_cols = n;

    int i = 0;
    int j = 0;
    while (*s == ' ') s++; // s points to '['
    s++; // s points past '['

    char* end = s;
    while (*s != ']') {
        while (*s == ' ') s++;

        int k = strtol(s, &end, 10);
        if (end == s && *s == ';') {
            i++;
            j = 0;
            s = end + 1;
        } else if (end != s) {
            res->values[i * n + j] = k;
            s = end;
            j++;
        }

    }

    return res;
}

char* infix2postfix_sf(char *infix) {
    return NULL;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    return NULL;
}

matrix_sf *execute_script_sf(char *filename) {
   return NULL;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
