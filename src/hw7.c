#include "hw7.h"
#include <stdlib.h>

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    bst_sf *new_node = malloc(sizeof(bst_sf));
    new_node->mat = mat;
    new_node->left_child = NULL;
    new_node->right_child = NULL;

    bst_sf *parent = NULL;
    bst_sf *curr = root;

    while (curr) {
        parent = curr;
        if (mat->name < curr->mat->name) curr = curr->left_child;
        else curr = curr->right_child;
    }
    
    if (!parent) root = new_node;
    else if (mat->name < parent->mat->name) parent->left_child = new_node;
    else parent->right_child = new_node;

    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    bst_sf *node = root;

    while (node) {
        char node_name = node->mat->name;
        if (name == node_name) return node->mat;
        else if (name < node_name) node = node->left_child;
        else node = node->right_child;
    }

    return NULL;
}

void free_bst_sf(bst_sf *root) {
    if (root) {
        free_bst_sf(root->left_child);
        free_bst_sf(root->right_child);
        free(root->mat);
        free(root);
    }
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

int prec(char operator) {
    switch (operator) {
        case '\'': return 3;
        case '*': return 2;
        case '+': return 1;
        default: return -1;
    }
}

char* infix2postfix_sf(char *infix) {
    int len = strlen(infix);
    char *postfix = malloc(len + 1);
    int postfix_end = 0;

    char *stack = malloc(len + 1);
    int top = -1;

    for (int i = 0; i < len; i++) {
        char c = infix[i];

        if (isalpha(c)) postfix[postfix_end++] = c;

        else if (c == '(') stack[++top] = c;

        else if (c == ')') {
            while (top >= 0 && stack[top] != '(') 
                postfix[postfix_end++] = stack[top--];
            
            top--;
        } 

        else if (c == ' ') continue;
        
        else {
            while (top >= 0 && prec(c) <= prec(stack[top])) 
                postfix[postfix_end++] = stack[top--];
            
            stack[++top] = c;
        }
    }

    while (top >= 0) {
        postfix[postfix_end++] = stack[top--];
    }
    postfix[postfix_end] = '\0';

    free(stack);
    return postfix;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char* postfix = infix2postfix_sf(expr);
    int postfix_len = strlen(postfix);
    matrix_sf** stack = malloc(postfix_len * sizeof(matrix_sf*));
    int top = -1;

    matrix_sf* operand1;
    matrix_sf* operand2;

    for (int i = 0; i < postfix_len; i++) {
        char c = postfix[i];
        switch (c) {
            case '\'': 
                operand1 = stack[top--];
                stack[++top] = transpose_mat_sf(operand1);
                
                if (!isalpha(operand1->name)) free(operand1);

                stack[top]->name = '?';
                break;

            case '*':
                operand2 = stack[top--];
                operand1 = stack[top--];
                stack[++top] = mult_mats_sf(operand1, operand2);

                if (!isalpha(operand1->name)) free(operand1);
                if (!isalpha(operand2->name)) free(operand2);

                stack[top]->name = '?';
                break;

            case '+':
                operand1 = stack[top--];
                operand2 = stack[top--];
                stack[++top] = add_mats_sf(operand1, operand2);

                if (!isalpha(operand1->name)) free(operand1);
                if (!isalpha(operand2->name)) free(operand2);

                stack[top]->name = '?';
                break;

            default:
                operand1 = find_bst_sf(c, root);
                stack[++top] = operand1;
                break;
        }
    }

    matrix_sf* res = stack[0];
    res->name = name;
    free(stack);
    free(postfix);
    return res;
}

matrix_sf *execute_script_sf(char *filename) {
    char *str = NULL;
    FILE *file = fopen(filename, "r");
    size_t max_line_size = MAX_LINE_LEN; // defined in hw7.h
    int mode = 0; // create mode
    bst_sf* root = NULL;
    char name;

    while (getline(&str, &max_line_size, file) != -1) {
        char *s = str;
        while (*s == ' ') // iterate to name
            s++;

        name = *s;

        while (*s++ != '=') // iterate past the equals to the main expr
            ;

        char* end = s;
        strtol(s, &end, 10);
        if (s == end) mode = 1; // compute mode
        
        // printf("Mode is %s\n", mode ? "compute" : "create");

        if (mode == 0) {
            root = insert_bst_sf(create_matrix_sf(name, s), root);
        } else {
            root = insert_bst_sf(evaluate_expr_sf(name, s, root), root);
        }
    }

    matrix_sf *res = find_bst_sf(name, root);
    int res_len = res->num_rows * res->num_cols;
    matrix_sf* res_copy = malloc(sizeof(matrix_sf) + res_len * sizeof(int));

    res_copy->num_rows = res->num_rows;
    res_copy->num_cols = res->num_cols;
    memcpy(res_copy->values, res->values, res_len * sizeof(int));

    free_bst_sf(root);
    fclose(file);
    free(str);
    return res_copy;
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
