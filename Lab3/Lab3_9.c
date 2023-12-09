#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


typedef enum 
{
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE
} status_code;


typedef struct _node 
{
    char* data;
    size_t amount;
    size_t len;
    struct _node *parent;
    struct _node *left;
    struct _node *right;
} Node;

typedef struct 
{
    char* elems;
    size_t size;
    size_t capacity;
} Vector;


status_code create_vector (Vector* vec) 
{
    vec->size = 0;
    vec->capacity = 8;
    vec->elems = (char*)malloc(sizeof(char) * vec->capacity);
    if (vec->elems == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code add (Vector* vec, char sep) 
{
    if (vec->size >= vec->capacity) 
    {
        vec->capacity *= 2;
        char* tmp;
        
        tmp = realloc(vec->elems, sizeof(char) * vec->capacity);
        if (tmp == NULL) 
        {
            free(vec->elems);
            return MEM_NOT_ALLOC;
        }
        vec->elems = tmp;
    }
    vec->elems[vec->size++] = sep;

    return OK;
}

status_code del_vec (Vector* vector)
{
    free(vector->elems);
    free(vector);
    return OK;
}


status_code creat_node (char* str, size_t size, Node** node) {
    *node = (Node*)malloc(sizeof(Node));
    if (*node == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    (*node)->data = (char*)malloc(sizeof(char) * (size + 1));
    if ((*node)->data == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    strcpy((*node)->data, str);
    (*node)->amount = 1;
    (*node)->len = size;

    (*node)->parent = NULL;
    (*node)->left = NULL;
    (*node)->right = NULL;

    return OK;
}

void print_tree(Node *root, int tab) 
{
    if (root == NULL) 
    {
        return;
    }
    for (int i = 0; i < tab; i++) 
    {
        printf("  ");
    }
    printf("%s\n", root->data);

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

    free(tree->data);
    free(tree);

    return OK;
}


int comparator (const void* e_1, const void* e_2) {
    char e1 = *(char*) e_1;
    char e2 = *(char*) e_2;

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

status_code bin_search (char* arr, int len, char cur)
{
    int a = 0, b = len - 1, c;
    c = b / 2;
    while (b > a)
    {
        if (arr[c] == cur)
        {
            return OK;
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
        return OK;
    }

    return INVALID_ARG;
}


status_code separator_add (Vector* separators, char* argv[], size_t amount)
{
    for (int i = 2; i < amount; i++)
    {
        if (argv[i][1] == '\0')
        {
            if (add(separators, argv[i][0]))
            {
                return MEM_NOT_ALLOC;
            }
        }
        else if (argv[i][2] == '\0')
        {
            if (argv[i][0] != '\\')
            {
                return INVALID_ARG;
            }
            else
            {
                if (argv[i][1] == 'n')
                {
                    if (add(separators, '\n'))
                    {
                        return MEM_NOT_ALLOC;
                    }
                }
                else if (argv[i][1] == 't')
                {
                    if (add(separators, '\t'))
                    {
                        return MEM_NOT_ALLOC;
                    }
                }
            }
        }
        else if (argv[i][3] == '\0')
        {
            if (argv[i][0] == '"' && argv[i][2] == '"')
            {
                if (add(separators, argv[i][1]))
                {
                    return MEM_NOT_ALLOC;
                }
            } 
            else
            {
                return INVALID_ARG;
            }
        }
        else
        {
            return INVALID_ARG;
        }
    }

    return OK;
}

status_code add_node (Node** tree, char* str, int len)
{
    if ((*tree) == NULL)
    {
        if (creat_node(str, len, tree) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
        
        return OK;
    } 

    Node* cur = *tree;
    Node* new;

    while (cur != NULL)
    {
        if (strcmp(cur->data, str) == 0)
        {
            cur->amount++;
            return OK;
        } 
        else if (len > cur->len || (len == cur->len && strcmp(str, cur->data) > 0))
        {
            if (cur->right == NULL)
            {
                if (creat_node(str, len, &new) == MEM_NOT_ALLOC)
                {
                    return MEM_NOT_ALLOC;
                }

                cur->right = new;
                new->parent = cur;

                return OK;
            }

            cur = cur->right;
        }
        else
        {
            if (cur->left == NULL)
            {
                if (creat_node(str, len, &new) == MEM_NOT_ALLOC)
                {
                    return MEM_NOT_ALLOC;
                }

                cur->left = new;
                new->parent = cur;

                return OK;
            }

            cur = cur->left;
        }
    }

    return INVALID_ARG;
}

status_code read (FILE* file, Node** root, Vector* sep)
{
    size_t size = 8;
    char c;
    int i = 0;

    char* buf = (char*)malloc(sizeof(char) * size);
    if (buf == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    while ((c = getc(file)) != EOF)
    {
        if (bin_search(sep->elems, sep->size, c))
        {
            if (i >= size){
                size *= 2;

                char* tmp = (char*)realloc(buf, sizeof(char) * size);
                if (tmp == NULL)
                {
                    free(buf);
                    return MEM_NOT_ALLOC;
                }

                buf = tmp;
            }

            buf[i] = c;
            i++;
        }
        else if (i != 0)
        {
            buf[i] = '\0';

            if (add_node(root, buf, i) == MEM_NOT_ALLOC)
            {
                free(buf);
                return MEM_NOT_ALLOC;
            }

            i = 0;
        }   
    }

    if (i != 0)
    {
        buf[i] = '\0';

        if (add_node(root, buf, i) == MEM_NOT_ALLOC)
        {
            free(buf);
            return MEM_NOT_ALLOC;
        }
    }   

    free(buf);
    return OK;
}

status_code find (Node* tree, char* str, size_t* res)
{
    size_t len = strlen(str);
    Node* cur = tree;

    while (cur != NULL)
    {
        if (strcmp(cur->data, str) == 0)
        {
            *res = cur->amount;
            return OK;
        } 
        else if (len > cur->len || (len == cur->len && strcmp(str, cur->data) > 0))
        {
            cur = cur->right;
        }
        else
        {
            cur = cur->left;
        }
    }

    *res = 0;

    return OK;
}

status_code find_min (Node* tree, Node** res)
{
    if (tree == NULL)
    {
        return INVALID_ARG;
    }

    Node* cur = tree;

    while (cur->left != NULL)
    {
        cur = cur->left;
    }

    *res = cur;

    return OK;
}

status_code find_max (Node* tree, Node** res)
{
    if (tree == NULL)
    {
        return INVALID_ARG;
    }

    Node* cur = tree;

    while (cur->right != NULL)
    {
        cur = cur->right;
    }

    *res = cur;

    return OK;
}

void most_frequent(Node *root, Node* arr[], int n) 
{
    if (root == NULL) 
    {
        return;
    }

    Node* cur = root;
    Node* t;

    for (int i = 0; i < n; i++) 
    {
        if (arr[i] == NULL)
        {
            arr[i] = cur;
            break;
        }
        else if (arr[i]->amount < cur->amount)
        {
            t = arr[i];
            arr[i] = cur;
            cur = t;
        }
        
    }

    most_frequent(root->left, arr, n);
    most_frequent(root->right, arr, n);
}

void depth(Node *root, int* cur, int* max) 
{
    if (root == NULL) 
    {
        if (*max < *cur)
        {
            *max = *cur;
        }

        return;
    }

    (*cur)++;

    depth(root->left, cur, max);

    depth(root->right, cur, max);

    (*cur)--;
}

void to_file(FILE* file, Node *root, char* sep) 
{
    if (root == NULL) 
    {
        return;
    }

    for (int j = 0; j < root->amount; j++)
    {
        fprintf(file, "%s%c", root->data, sep);
    }

    to_file(file, root->left, sep);
    to_file(file, root->right, sep);
} 


status_code interface (FILE* input, Node* tree, char sep)
{
    int e = 1;
    int cur, dep, n;
    char ent;
    size_t res;
    status_code status;

    char buf[256];
    Node* node;

    while (e) 
    {
        printf("Enter:\ne - exit\nf - find\nm - min\nM - max\nс - find most common\nd - depth\np - to file\nr - read tree\n");

        if (scanf("%c", &ent) != 1)
        {
            printf("Invalid input\n");
            break;
        }
        getchar();

        switch (ent) 
        {
            case 'e':
                e = 0;
                break;

            case 'f':
                printf("Find:\n");

                if (scanf("%s", buf) != 1)
                {
                    printf("Invalid input\n");
                    break;
                }
                getchar();

                find(tree, buf, &res);
                printf("The word occurs: %d\n", res);
                
                break;

            case 'm':
                if (find_min(tree, &node) == INVALID_ARG)
                {
                    printf("Empty\n");
                    return INVALID_ARG;
                }
                printf("min: %s\n", node->data);

                break;
            
            case 'M':
                if (find_max(tree, &node) == INVALID_ARG)
                {
                    printf("Empty\n");
                    return INVALID_ARG;
                }
                printf("max: %s\n", node->data);

                break;

            case 'd':
                cur = 0;
                dep = 0;
                
                depth(tree, &cur, &dep);
                printf("Tree depth: %d\n", dep);

                break;

            case 'c':
                printf("n:\n");

                if (scanf("%d", &n) != 1)
                {
                    printf("Invalid input\n");
                    break;
                }
                getchar();

                if (n < 1)
                {
                    printf("Invalid input\n");
                    break;
                }

                Node** arr = (Node**)calloc(n, sizeof(Node*));
                if (arr == NULL)
                {
                    printf("Memory was not allocated\n");
                    return MEM_NOT_ALLOC;
                }

                most_frequent(tree, arr, n);

                int j = 0;
                while (arr[j] != NULL && j < n)
                {
                    printf("%s\n", arr[j]->data);
                    j++;
                }

                free(arr);
                break;

            case 'p':
            {
                FILE* out = fopen("C:\\VSCode\\Lab\\MP_3\\9_tree.txt", "w");
                if (out == NULL)
                {
                    return NOT_OPEN_FILE;
                }

                if (tree != NULL)
                {
                    to_file(out, tree, 0);
                }

                dell_tree(tree);
                tree = NULL;

                fclose(out);
                break;
            }

            case 'r':
            {
                FILE* out = fopen("C:\\VSCode\\Lab\\MP_3\\9_tree.txt", "r");
                if (out == NULL)
                {
                    return NOT_OPEN_FILE;
                }
                
                Vector* separators = (Vector*)malloc(sizeof(Vector));
                if (separators == NULL)
                {
                    printf("Memory was not allocated\n");
                    return MEM_NOT_ALLOC;
                }

                if (create_vector(separators) == MEM_NOT_ALLOC)
                {
                    printf("Memory was not allocated\n");
                    return MEM_NOT_ALLOC;
                }

                separators->elems[0] = sep;
                separators->size = 1;


                status = read(out, &tree, separators);
                if (status != OK)
                {
                    del_vec(separators);
                    fclose(out);
                    return status;
                }

                del_vec(separators);
                fclose(out);
                break;
            }

            default:
                printf("Invalid input\n");
                break;
        }
    }

    return OK;
}


int main (int argc, char* argv[]) 
{
    if (argc < 3)
    {
        printf("Invalid input\n");
        return INVALID_ARG;
    }


    Vector* separators = (Vector*)malloc(sizeof(Vector));
    if (separators == NULL)
    {
        printf("Memory was not allocated\n");
        return MEM_NOT_ALLOC;
    }

    if (create_vector(separators) == MEM_NOT_ALLOC)
    {
        printf("Memory was not allocated\n");
        return MEM_NOT_ALLOC;
    }

    status_code status = separator_add(separators, argv, argc);

    if (status != OK)
    {
        printf("Error adding separator\n");
        del_vec(separators);
        return status;
    }

    qsort(separators->elems, separators->size, sizeof(char), comparator);

    // for (int i = 0; i < separators->size; i++)
    // {
    //     printf("%c ", separators->elems[i]);
    // }
    // printf("\n");


    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("The file was not opened\n");
        del_vec(separators);
        return NOT_OPEN_FILE;
    }

    Node* root = NULL;

    status = read(file, &root, separators);
    if (status != OK)
    {
        printf("Reading error\n");
        del_vec(separators);
        fclose(file);
        return status;
    }

    print_tree(root, 0);

    status = interface(file, root, separators->elems[0]);
    if (status != OK)
    {
        printf("Error\n");
    }


    del_vec(separators);
    dell_tree(root);

    free(root->data);

    fclose(file);

    return status;
}