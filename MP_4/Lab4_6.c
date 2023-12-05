#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef enum 
{
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE
} status_code;

typedef struct _node 
{
    char data;
    struct _node *parent;
    struct _node *left;
    struct _node *right;
} Node;

typedef struct _st_elem
{
    char data;
    struct _st_elem *prev;
} St_elem;

typedef struct
{
    St_elem *tail;
} Stack;

typedef struct _st_node
{
    Node* tree;
    struct _st_node *prev;
} St_node;

typedef struct
{
    St_node *tail;
} Stack_Node;


status_code creat_node (char c, Node** node) {
    *node = (Node*)malloc(sizeof(Node));
    if (*node == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    (*node)->data = c;
    (*node)->parent = NULL;
    (*node)->left = NULL;
    (*node)->right = NULL;

    return OK;
}

// status_code add_l (Node* node, char c)
// {
//     Node* new_node = (Node*)malloc(sizeof(Node));
//     if (new_node == NULL) {
//         return MEM_NOT_ALLOC;
//     }

//     new_node->data = c;
//     new_node->parent = node;
//     new_node->left = NULL;
//     new_node->right = NULL;
//     node->left = new_node;

//     return OK;
// }

// status_code add_r (Node* node, char c)
// {
//     Node* new_node = (Node*)malloc(sizeof(Node));
//     if (new_node == NULL) {
//         return MEM_NOT_ALLOC;
//     }

//     new_node->data = c;
//     new_node->parent = node;
//     new_node->left = NULL;
//     new_node->right = NULL;
//     node->right = new_node;

//     return OK;
// }

void print_tree(Node *root, int tab) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < tab; i++) {
        printf("  ");
    }
    printf("%c\n", root->data);

    print_tree(root->left, 1 + tab);
    print_tree(root->right, 1 + tab);
}

status_code dell_tree (Node* tree)
{
    if (tree->left != NULL)
    {
        dell_tree(tree->left);
    }

    if (tree->right != NULL)
    {
        dell_tree(tree->right);
    }

    free(tree);

    return OK;
}


status_code creat_stack (Stack** stack)
{
    *stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        return MEM_NOT_ALLOC;
    }
    (*stack)->tail = NULL;

    return OK;
}

status_code push_back (Stack* stack, char c)
{
    St_elem* new = (St_elem*)malloc(sizeof(St_elem));
    if (new == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    new->data = c;
    if (stack->tail == NULL){
        new->prev = NULL;
    }
    else 
    {
        new->prev = stack->tail;
    }
    stack->tail = new;

    return OK;
}

status_code pop_back (Stack* stack, char* c)
{
    if (stack->tail == NULL)
    {
        return INVALID_ARG;
    }
    
    *c = stack->tail->data;

    St_elem* new_tail = stack->tail->prev;
    free(stack->tail);
    stack->tail = new_tail;

    return OK;
}

status_code dell_stack (Stack* stack)
{
    if (stack->tail == NULL)
    {
        return INVALID_ARG;
    }

    while (stack->tail != NULL)
    {
        St_elem* elem = stack->tail;
        stack->tail = elem->prev;
        free(elem);
    }
    free(stack);
    
    return OK;
}

status_code creat_stack_node (Stack_Node** stack)
{
    *stack = (Stack_Node*)malloc(sizeof(Stack_Node));
    if (stack == NULL)
    {
        return MEM_NOT_ALLOC;
    }
    (*stack)->tail = NULL;

    return OK;
}

status_code push_back_node (Stack_Node* stack, char c)
{
    St_node* new = (St_node*)malloc(sizeof(St_node));
    if (new == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    if (creat_node(c, &(new->tree)) == MEM_NOT_ALLOC)
    {
        return MEM_NOT_ALLOC;
    }

    if (stack->tail == NULL){
        new->prev = NULL;
    }
    else 
    {
        new->prev = stack->tail;
    }
    stack->tail = new;

    return OK;
}

status_code pop_back_node (Stack_Node* stack, Node** node)
{
    if (stack->tail == NULL)
    {
        return INVALID_ARG;
    }
    
    *node = stack->tail->tree; 

    St_node* new_tail = stack->tail->prev;
    free(stack->tail);
    stack->tail = new_tail;

    return OK;
}

status_code dell_stack_node (Stack_Node* stack)
{
    if (stack->tail == NULL)
    {
        return INVALID_ARG;
    }

    while (stack->tail != NULL)
    {
        St_node* elem = stack->tail;
        stack->tail = elem->prev;
        free(elem);
    }
    free(stack);
    
    return OK;
}


int comparator (const void* a, const void* b)
{
    char e1 = *(char*)a;
    char e2 = *(char*)b;

    if (e1 > e2)
    {
        return 1;
    }
    else if (e1 < e2)
    {
        return -1;
    } 

    return 0;
}

int is_operation (char c)
{
    if (c == '&' || c == '|' || c == '~' || c == '=' || c == '!' || c == '?')
    {
        return 1;
    }
    else if (c == '-' || c == '+' || c == '<')
    {
        return 2;
    }

    return 0;
}


int bin_search (char* arr, int len, char cur)
{
    int a = 0, b = len - 1, c;
    c = b / 2;
    while (b > a)
    {
        if (arr[c] == cur)
        {
            return c;
        }
        else if (cur > arr[c])
        {
            a = c + 1;
        }
        else
        {
            b = c;
        }
        c = (a + b) / 2;
    }
    if (arr[c] == cur)
    {
        return c;
    }

    return 0;
}

bool operation (bool a, bool b, char c)
{
    switch (c)
    {
        case '&':
            return a && b;
        
        case '|':
            return a || b;

        case '~':
            return !b;

        case '-':
            return !a || b;

        case '+':
            return a && !b;

        case '<':
            return a ^ b;

        case '=':
            return a == b;

        case '!':
            return !(a && b);

        case '?':
            return !(a || b);
    }
}

bool calculate_res (Node* l, Node* r, char op, char* vars, bool* val, int len, FILE* file)
{
    bool n_1 = 0, n_2 = 0, res;
    int ind;

    if ((l == NULL || !is_operation(l->data)) && (!is_operation(r->data)))
    {
        if (l != NULL && isalpha(l->data))
        {
            ind = bin_search(vars, len, l->data);
            n_1 = val[ind];
        }
        else if (l != NULL)
        {
            n_1 = l->data - '0';
        }

        if (isalpha(r->data))
        {
            ind = bin_search(vars, len, r->data);
            n_2 = val[ind];
        }
        else
        {
            n_2 = r->data - '0';
        }

        res = operation(n_1, n_2, op);
        fprintf(file, "%d ", res);

    }
    else if (l != NULL && is_operation(l->data) && is_operation(r->data))
    { 
        res = operation(calculate_res(l->left, l->right, l->data, vars, val, len, file), 
                         calculate_res(r->left, r->right, r->data, vars, val, len, file), op);

        fprintf(file, "%d ", res);

    }
    else if (l == NULL || !is_operation(l->data))
    {
        if (l != NULL && isalpha(l->data))
        {
            ind = bin_search(vars, len, l->data);
            n_1 = val[ind];
        }
        else if (l != NULL)
        {
            n_1 = l->data - '0';
        }

        res = operation(n_1, calculate_res(r->left, r->right, r->data, vars, val, len, file), op);
        fprintf(file, "%d ", res);

    }
    else
    {
        if (isalpha(r->data))
        {
            ind = bin_search(vars, len, r->data);
            n_2 = val[ind];
        }
        else
        {
            n_2 = r->data - '0';
        }

        res = operation(calculate_res(l->left, l->right, l->data, vars, val, len, file), n_2, op);
        fprintf(file, "%d ", res);

    }
    
    return res;
}

status_code creat_expr (Node* l, Node* r, char op, FILE* file)
{
    fprintf(file, "%c", op);

    if (op == '-' || op == '+' || op == '<')
    {
        fprintf(file, ">");
    }

    if (l != NULL && is_operation(l->data))
    {
        creat_expr (l->left, l->right, l->data, file);
    }
    else if (l != NULL)
    {
        fprintf(file, "%c", l->data);
    }

    if (is_operation(r->data))
    {
        creat_expr (r->left, r->right, r->data, file);
    }
    else
    {
        fprintf(file, "%c", r->data);
    }

    return OK;
}

status_code subexpr (Node* l, Node* r, char op, FILE* file)
{
    if ((l == NULL || !is_operation(l->data)) && (!is_operation(r->data)))
    {   
        creat_expr(l, r, op, file);
        fprintf(file, " ");
    }
    else if (l != NULL && is_operation(l->data) && is_operation(r->data))
    { 
        subexpr(l->left, l->right, l->data, file); 
        subexpr(r->left, r->right, r->data, file);

        creat_expr(l, r, op, file);
        fprintf(file, " ");
    }
    else if (l == NULL || !is_operation(l->data))
    {
        subexpr(r->left, r->right, r->data, file);

        creat_expr(l, r, op, file);
        fprintf(file, " ");
    }
    else
    {
        subexpr(l->left, l->right, l->data, file); 

        creat_expr(l, r, op, file);
        fprintf(file, " ");
    }
    
    return OK;
}

status_code next_case (bool* arr, int len)
{
    for (int i = len - 1; i >= 0; i--)
    {
        if (arr[i] == 0)
        {
            arr[i] = 1;
            break;
        }
        else
        {
            arr[i] = 0;
        }
    }

    return OK;
}

status_code print_header (Node* tree, FILE* file, char vars[], int len)
{
    for (int i = 0; i < len; i++)
    {
        fprintf(file, "%c ", vars[i]);
    }

    subexpr(tree->left, tree->right, tree->data, file);

    fprintf(file, "\n\n");

    return OK;
}

status_code creat_table (Node* tree, FILE* output, char vars[], int len)
{
    bool* val;

    print_header (tree, output, vars, len);

    if (len == 0)
    {
        calculate_res(tree->left, tree->right, tree->data, vars, val, len, output);
    }
    else
    {
        val = (bool*)calloc(len, sizeof(bool));
        if (val == NULL)
        {
            return MEM_NOT_ALLOC;
        }

        int columns = pow(2, len);
        for (int i = 0; i < columns; i++)
        {
            for (int j = 0; j < len; j++)
            {
                fprintf(output, "%d ", val[j]);
            }

            calculate_res(tree->left, tree->right, tree->data, vars, val, len, output);
            fprintf(output, "\n");

            next_case(val, len);
        }
    }

    return OK;
}


status_code find (char arr[], char c, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == c)
        {
            return OK;
        }
    }

    return INVALID_ARG;
}

bool priority (char cur, char top)
{
    short int p_cur, p_top;

    if (top == '(')
    {
        return true;
    }

    if (cur == '~') 
    {
        p_cur = 3;
    } 
    else if (cur == '&' || cur == '?' || cur == '!' || cur == '+')
    {
        p_cur = 2;
    }
    else
    {
        p_cur = 1;
    }
    
    if (top == '~')
    {
        p_top = 3;
    } 
    else if (top == '&' || top == '?' || top == '!' || top == '+')
    {
        p_top = 2;
    }
    else
    {
        p_top = 1;
    }

    if (p_cur > p_top)
    {
        return true;
    } 

    return false;
}

status_code read (FILE* file, Stack* stack, Stack_Node* stack_node, char vars[], int *len)
{
    char c, prev_c = 0;
    short int r;
    int staples = 0;

    while (((c = getc(file)) != EOF) && (c != '\n'))
    {
        if (isalpha(c) || c == '1' || c == '0')
        {
            if (isalpha(prev_c) || prev_c == '1' || prev_c == '0')
            {
                return INVALID_ARG;
            }

            if (push_back_node(stack_node, c) == MEM_NOT_ALLOC)
            {
                return MEM_NOT_ALLOC;
            }

            if (c != '1' && c != '0' && (find(vars, c, *len) == INVALID_ARG))
            {
                vars[*len] = c;
                *len += 1;
            }
        } 
        else if (c == '(')
        {
            if (push_back(stack, c) == MEM_NOT_ALLOC)
            {
                return MEM_NOT_ALLOC;
            }

            staples++;
        } 
        else if ((r = is_operation(c)) != 0)
        {
            if (c != '~')
            {   if (is_operation(prev_c))
                {
                    return INVALID_ARG;
                }
            }
            else
            {
                if (isalnum(prev_c))
                {
                    return INVALID_ARG;
                }
            }

            if (stack->tail == NULL)
            {
                if (push_back(stack, c) == MEM_NOT_ALLOC)
                {
                    return MEM_NOT_ALLOC;
                }
            }
            else
            {
                if (priority(c, stack->tail->data))
                {
                    if (push_back(stack, c) == MEM_NOT_ALLOC)
                    {
                        return MEM_NOT_ALLOC;
                    }
                }
                else
                {
                    while (stack->tail != NULL && !priority(c, stack->tail->data))
                    {
                        Node *pR, *pL;
                        if (pop_back_node(stack_node, &pR) == INVALID_ARG)
                        {
                            return INVALID_ARG;
                        }

                        if (c == '~')
                        {
                            pL = NULL;
                        }
                        else
                        {
                            if (pop_back_node(stack_node, &pL) == INVALID_ARG)
                            {
                                return INVALID_ARG;
                            }
                        }

                        char top;
                        if (pop_back(stack, &top) == INVALID_ARG)
                        {
                            return INVALID_ARG;
                        }

                        if (push_back_node(stack_node, top) == MEM_NOT_ALLOC)
                        {
                            return MEM_NOT_ALLOC;
                        }
                        stack_node->tail->tree->right = pR;
                        stack_node->tail->tree->left = pL;

                    }

                    if (push_back(stack, c) == MEM_NOT_ALLOC)
                    {
                        return MEM_NOT_ALLOC;
                    }
                }
            }

            if (r == 2)
            {
                c = getc(file);
                if (c != '>')
                {
                    return INVALID_ARG;
                }
            }

        }
        else if (c == ')')
        {
            staples--;

            while (stack->tail->data != '(')
            {
                Node *pR, *pL;
                if (pop_back_node(stack_node, &pR) == INVALID_ARG)
                {
                    return INVALID_ARG;
                }

                if (stack->tail->data == '~')
                {
                    pL = NULL;
                }
                else
                {
                    if (pop_back_node(stack_node, &pL) == INVALID_ARG)
                    {
                        return INVALID_ARG;
                    }
                }

                char top;
                if (pop_back(stack, &top) == INVALID_ARG)
                {
                    return INVALID_ARG;
                }

                if (push_back_node(stack_node, top) == MEM_NOT_ALLOC)
                {
                    return MEM_NOT_ALLOC;
                }
                stack_node->tail->tree->right = pR;
                stack_node->tail->tree->left = pL;
            }

            char t;
            if (pop_back(stack, &t) == INVALID_ARG)
            {
                return INVALID_ARG;
            }
        }

        if (staples < 0)
        {
            return INVALID_ARG;
        }

        prev_c = c;
    }

    if (staples != 0)
    {
        return INVALID_ARG;
    }

    while (stack->tail != NULL)
    {
        Node *pR, *pL;
        if (pop_back_node(stack_node, &pR) == INVALID_ARG)
        {
            return INVALID_ARG;
        }

        if (stack->tail->data == '~')
        {
            pL = NULL;
        }
        else
        {
            if (pop_back_node(stack_node, &pL) == INVALID_ARG)
            {
                return INVALID_ARG;
            }
        }

        char top;
        if (pop_back(stack, &top) == INVALID_ARG)
        {
            return INVALID_ARG;
        }

        if (push_back_node(stack_node, top) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
        stack_node->tail->tree->right = pR;
        stack_node->tail->tree->left = pL;
    }

    return OK;
}

status_code creat_name (char* name)
{
    srand(time(NULL));
    size_t size = 4 + (rand() % 7);
    char t;
    int i;

    for (i = 7; i < size + 7; i++)
    {
        t = '0' + rand() % 62;

        if (t > '9')
        {
            t += 7;
        }
        if (t > 'Z')
        {
            t += 6;
        }

        name[i] = t;
    }

    memcpy(name, ".\\MP_4\\", sizeof(char) * 7);
    memcpy(name + i, ".txt", sizeof(char) * 4);
    name[i + 4] = '\0';

    return OK;
}

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Invalid input\n");
        return INVALID_ARG;
    }


    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("The file was not opened\n");
        return NOT_OPEN_FILE;
    }

    Stack* stack;
    if (creat_stack(&stack) == MEM_NOT_ALLOC)
    {
        printf("Memory was not allocated\n");
        fclose(file);
        return MEM_NOT_ALLOC;
    }

    Stack_Node* stack_node;
    if (creat_stack_node(&stack_node) == MEM_NOT_ALLOC)
    {
        printf("Memory was not allocated\n");
        fclose(file);
        free(stack_node);

        return MEM_NOT_ALLOC;
    }


    int len = 0;
    char vars[52];

    status_code status;
    status = read(file, stack, stack_node, vars, &len);
    if (status != OK)
    {
        printf("Reading error\n");
        return INVALID_ARG;
    }

    qsort(vars, len, sizeof(char), comparator);

    // print_tree(stack_node->tail->tree, 0);


    char out_name[22];
    creat_name(out_name);
    printf("%s\n", out_name);
    if (!strcmp(argv[1], out_name))
    {
        printf("The input and output files are the same\n");

        dell_tree(stack_node->tail->tree);
        dell_stack(stack);
        dell_stack_node(stack_node);
        fclose(file);

        return INVALID_ARG;
    }

    // FILE* out = fopen(".\\MP_4\\out.txt", "w");
    FILE* out = fopen(out_name, "w");
    if (out == NULL)
    {
        printf("The output file was not opened\n");
        status = NOT_OPEN_FILE;
    }
    else
    {
        creat_table(stack_node->tail->tree, out, vars, len);
    }


    dell_tree(stack_node->tail->tree);
    dell_stack(stack);
    dell_stack_node(stack_node);
    fclose(file);
    fclose(out);

    return status;
}
