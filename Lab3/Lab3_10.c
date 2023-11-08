#include <stdio.h>
#include <ctype.h>
#include <malloc.h>

typedef enum {
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE
} status_code;

typedef struct _node {
    char data;
    struct _node *parent;
    struct _node *child;
    struct _node *brother;
} Node;

status_code creat_r (char c, Node** root) {
    *root = (Node*)malloc(sizeof(Node));
    if (*root == NULL) {
        return MEM_NOT_ALLOC;
    }

    (*root)->data = c;
    (*root)->parent = NULL;
    (*root)->brother = NULL;
    (*root)->child = NULL;

    return OK;
}

status_code add_child (Node* node, char c) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return MEM_NOT_ALLOC;
    }

    new_node->data = c;
    new_node->parent = node;
    new_node->brother = NULL;
    new_node->child = NULL;
    node->child = new_node;

    return OK;
}

status_code add_brother (Node* node, char c) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return MEM_NOT_ALLOC;
    }

    new_node->data = c;
    new_node->parent = node->parent;
    new_node->brother = NULL;
    new_node->child = NULL;
    node->brother = new_node;

    return OK;
}

void print_tree(Node *root, int tab) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < tab; i++) {
        printf("  ");
    }
    printf("%c\n", root->data);

    print_tree(root->child, 1 + tab);
    print_tree(root->brother, tab);
}

void fprint_tree(FILE* file, Node *root, int tab) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < tab; i++) {
        fprintf(file, "  ");
    }
    fprintf(file, "%c\n", root->data);

    fprint_tree(file, root->child, 1 + tab);
    fprint_tree(file, root->brother, tab);
}

status_code creat_tree (char* str, Node** tree){ 

    if (creat_r (str[0], tree) == MEM_NOT_ALLOC) {
        return MEM_NOT_ALLOC;
    }

    Node* cur = *tree;
    int i = 1;
    while (str[i] != '\0') {
        if (str[i] == '(') {
            i++;
            if (isalnum(str[i]) == 0) {
                return INVALID_ARG;
            }
            if (add_child(cur, str[i]) == MEM_NOT_ALLOC) {
                return MEM_NOT_ALLOC;
            }

            cur = cur->child;

        } else if (str[i] == ')') {
            cur = cur->parent;
            
        } else if (str[i] == ',') {
            i++;
            if (isalnum(str[i]) == 0) {
                return INVALID_ARG;
            }
            if (add_brother(cur, str[i]) == MEM_NOT_ALLOC) {
                return MEM_NOT_ALLOC;
            }

            cur = cur->brother;
        } else {
            return INVALID_ARG;
        }
        i++;
    }

    return OK;
}

void del_tree (Node* tree) {
    if (tree == NULL) {
        return;
    }

    del_tree(tree->child);
    del_tree(tree->brother);

    free(tree);
}

status_code read_info (FILE* file, FILE* output) {
    char buf[256];
    char c;
    int er_check, i, flag = 1, check = 0;
    size_t read_line, len = 0;
    while (flag) {
        i = 0;
        while ((c = fgetc(file)) != '\n') {
            if (c == EOF) {
                flag = 0;
                break;
            }
            if (isalnum(c) == 0 && c != ',' && c != '(' && c != ')') {
                return INVALID_ARG;
            }

            if (c == '(') {
                check++;
            } else if (c == ')') {
                check--;
            }
            if (check < 0) {
                return INVALID_ARG;
            }

            buf[i] = c;
            i++;
        }

        buf[i] = '\0';
        if (check != 0) {
            return INVALID_ARG;
        }
        if (buf[0] == '(' || buf[0] == ',') {
            return INVALID_ARG;
        }

        printf("%s\n", buf);
        fprintf(output, "%s\n", buf);
        Node* tree;

        int cr_res = creat_tree(buf, &tree);
        if (cr_res == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        } else if (cr_res == INVALID_ARG) {
            return INVALID_ARG;
        } else {
            print_tree(tree, 0);
            fprint_tree(output, tree, 0);
        }

        del_tree(tree);
    }

    return OK;
}

int main (int argc, char* argv[]) {
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Not open input file\n");
        return NOT_OPEN_FILE;
    }

    FILE* output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        fclose(input_file);
        printf("Not open output file\n");
        return NOT_OPEN_FILE;
    }

    int read_res = read_info(input_file, output_file);
    if (read_res == INVALID_ARG) {
        printf("Invalid input\n");
        fclose(input_file);
        fclose(output_file);
        return INVALID_ARG;
    } else if (read_res == MEM_NOT_ALLOC) {
        printf("Memory was not allocated\n");
        fclose(input_file);
        fclose(output_file);
        return MEM_NOT_ALLOC;
    }

    fclose(input_file);
    fclose(output_file);

    return OK;
}