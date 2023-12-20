#include <stdio.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum 
{
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE,
    OVERFLOW
} status_code;

typedef struct _elem
{
    char* def_name;
    char* value;
    struct _elem* next;
} Elem;

typedef struct _hash
{
    Elem** table;
    size_t HASHSIZE;
    size_t* list_sizes;
    status_code (*func) (char*, size_t, int*, long long*);
} Hash_table;

typedef struct _cash
{
    long long cash;
    char* def_name;
    char* value;
    struct _cash* next;
} Cash;

typedef struct _cash_list
{
    Cash *head, *tail;
} Cash_list;


status_code conv_to_dec (char num[], int num_system, long long *decimal) 
{
    long long res = 0;
    int i, cur, otr = 0;

    if (num[0] == '-') 
    {
        otr = 1;
    }
    i = otr;

    while (num[i] != '\0') 
    {
        cur = num[i];

        if (num[i] >= 'a') 
        {
            cur -= 39;
        } 
        else if (num[i] >= 'A') 
        {
            cur -= 7;
        }
        cur -= '0';

        if ((LLONG_MAX - cur) / num_system < res) 
        {
            return OVERFLOW;
        }

        res = res * num_system + cur;
        i++;
    }

    if (otr) 
    {
        res *= -1;
    }

    *decimal = res;

    return OK;
}

status_code Hash_func (char* def_name, size_t HASHSIZE, int* res, long long* cash)
{
    long long dec;

    if (conv_to_dec(def_name, 62, &dec) == OVERFLOW)
    {
        return OVERFLOW;
    }

    *cash = dec;
    *res = dec % HASHSIZE;

    return OK;
}

status_code create_table (
    Hash_table** table, size_t size, status_code (*func) (char*, size_t, int*, long long*))
{
    *table = (Hash_table*)malloc(sizeof(Hash_table));
    if (*table == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    (*table)->HASHSIZE = size;
    (*table)->func = func;

    (*table)->table = (Elem**)calloc(size, sizeof(Elem*));
    if ((*table)->table == NULL)
    {
        free(*table);
        return MEM_NOT_ALLOC;
    }

    (*table)->list_sizes = (size_t*)calloc(size, sizeof(size_t));
    if ((*table)->table == NULL)
    {
        free((*table)->table);
        free(*table);

        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code create_cash_list (Cash_list** list)
{
    *list = (Cash_list*)malloc(sizeof(Cash_list));
    if (*list == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    (*list)->head = NULL;
    (*list)->tail = NULL;

    return OK;
}

status_code dell_cash_list (Cash_list* list)
{
    Cash* head = list->head;
    Cash* tmp;

    while (head != NULL)
    {
        free(head->def_name);
        free(head->value);

        tmp = head;
        head = head->next;

        free(tmp);
    }

    free(list);

    return OK;
}

status_code dell_list (Elem* head)
{
    Elem* tmp;
    while (head != NULL)
    {
        free(head->def_name);
        free(head->value);

        tmp = head;
        head = head->next;

        free(tmp);
    }

    return OK;
}

status_code dell_table (Hash_table* table)
{
    for (int i = 0; i < table->HASHSIZE; i++)
    {
        if (table->table[i] != NULL)
        {
            dell_list(table->table[i]);
        }
    }

    free(table->list_sizes);

    return OK;
}

status_code add_cash (Cash_list* list, long long cash, char* name, int s_name, char* val, int s_val)
{
    Cash *new = (Cash*)malloc(sizeof(Cash));
    if (new == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    new->next = NULL;

    char* new_name = (char*)malloc(sizeof(char) * s_name);
    if (new_name == NULL)
    {
        free(new);
        return MEM_NOT_ALLOC;
    }

    char* new_val = (char*)malloc(sizeof(char) * s_val);
    if (new_val == NULL)
    {
        free(new);
        free(new_name);
        return MEM_NOT_ALLOC;
    }

    strcpy(new_name, name);
    strcpy(new_val, val);

    new->def_name = new_name;
    new->value = new_val;

    new->cash = cash;

    if (list->head == NULL)
    {
        list->head = new;
    }
    else
    {
        list->tail->next = new;
    }

    list->tail = new;

    return OK;
}

bool check_collision (size_t* sizes, size_t hash_size)
{
    size_t min = 128, max = 0;
    
    for (int i = 0; i < hash_size; i++)
    {
        if (sizes[i] != 0)
        {
            if (sizes[i] < min)
            {
                min = sizes[i];
            }

            if (sizes[i] > max)
            {
                max = sizes[i];
            }
        }
    }

    if (max / 2 > min)
    {
        return true;
    }

    return false;
}

status_code add_from_cash (Hash_table* hash, Cash* cash)
{
    Elem *new = (Elem*)malloc(sizeof(Elem));
    if (new == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    new->next = NULL;

    char* new_name = (char*)malloc(sizeof(char) * strlen(cash->def_name));
    if (new_name == NULL)
    {
        free(new);
        return MEM_NOT_ALLOC;
    }

    char* new_val = (char*)malloc(sizeof(char) * strlen(cash->value));
    if (new_val == NULL)
    {
        free(new);
        free(new_name);
        return MEM_NOT_ALLOC;
    }

    strcpy(new_name, cash->def_name);
    strcpy(new_val, cash->value);

    new->def_name = new_name;
    new->value = new_val;

    int ind = cash->cash % hash->HASHSIZE;

    if (hash->table[ind] == NULL)
    {
        hash->table[ind] = new;
        hash->list_sizes[ind] = 1;
    }
    else
    {
        Elem* cur = hash->table[ind];
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        
        hash->list_sizes[ind] += 1;
        cur->next = new;
    }

    return OK;
}

status_code rebuild (Hash_table* hash, Cash_list* cash_list)
{
    size_t size = hash->HASHSIZE * 2;

    dell_table(hash);

    hash->HASHSIZE = size;

    size_t* tmp = (size_t*)calloc(size, sizeof(size_t));
    if (tmp == NULL)
    {
        return MEM_NOT_ALLOC;
    }
    hash->list_sizes = tmp;

    Elem** tmp_table = (Elem**)calloc(size, sizeof(Elem*));
    if (tmp_table == NULL)
    {
        return MEM_NOT_ALLOC;
    }
    hash->table = tmp_table;

    Cash* cur = cash_list->head;
    while (cur != NULL)
    {
        if (add_from_cash(hash, cur) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }

        cur = cur->next;
    }

    if (hash->HASHSIZE < 2048 && check_collision(hash->list_sizes, hash->HASHSIZE))
    {
        if (rebuild(hash, cash_list) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
    }

    return OK;
}

status_code add_elem (Hash_table* hash, char* name, int s_name, char* val, int s_val, Cash_list* cash_list)
{
    Elem *new = (Elem*)malloc(sizeof(Elem));
    if (new == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    new->next = NULL;

    char* new_name = (char*)malloc(sizeof(char) * s_name);
    if (new_name == NULL)
    {
        free(new);
        return MEM_NOT_ALLOC;
    }

    char* new_val = (char*)malloc(sizeof(char) * s_val);
    if (new_val == NULL)
    {
        free(new);
        free(new_name);
        return MEM_NOT_ALLOC;
    }

    strcpy(new_name, name);
    strcpy(new_val, val);

    new->def_name = new_name;
    new->value = new_val;

    int ind;
    long long cash;
    hash->func(name, hash->HASHSIZE, &ind, &cash);

    if (add_cash(cash_list, cash, name, s_name, val, s_val) == MEM_NOT_ALLOC)
    {
        return MEM_NOT_ALLOC;
    }

    if (hash->table[ind] == NULL)
    {
        hash->table[ind] = new;
        hash->list_sizes[ind] = 1;
    }
    else
    {
        Elem* cur = hash->table[ind];
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        
        hash->list_sizes[ind] += 1;
        cur->next = new;
    }

    if (check_collision(hash->list_sizes, hash->HASHSIZE))
    {
        if (rebuild(hash, cash_list) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
    }

    return OK;
}


status_code print_hash (Hash_table* hash)
{
    for (int i = 0; i < hash->HASHSIZE; i++)
    {
        if (hash->table[i] != NULL)
        {
            printf("%d %s %s\n", i, hash->table[i]->def_name, hash->table[i]->value);
        }
    }

    return OK;
}

status_code print_cash (Cash_list* cash_list)
{
    Cash* cur = cash_list->head;

    while (cur != NULL)
    {
        printf("%lld %s %s\n", cur->cash, cur->def_name, cur->value);

        cur = cur->next;
    }

    return OK;
}


status_code read (FILE* file, Hash_table* hash, Cash_list* cash_list)
{
    FILE* out = fopen("C:\\VSCode\\Lab\\MP_4\\out_1", "w");
    if (out == NULL)
    {
        printf("The output file was not opened\n");
        return NOT_OPEN_FILE;
    }

    char c;

    char def[7], buf_name[256], buf_val[256];
    char buf[256];

    int flag = 1, flag_h, i, j, ind;
    long long t;
    Elem* cur;

    while ((c = getc(file)) != EOF)
    {
        if (flag && c == '#')
        {
            i = 0;
            for (; i < 6; i++)
            {
                c = getc(file);
                if (c == EOF)
                {
                    def[i] = '\0';
                    fprintf(out, "#%s", def);

                    return OK;
                }

                def[i] = c;
            }

            def[i] = '\0';

            if (strcmp("define", def) == 0)
            {
                c = getc(file);
                if (c != ' ')
                {
                    fclose(out);
                    return INVALID_ARG;
                }
                
                c = getc(file);
                i = 0;
                while (isalnum(c))
                {
                    buf_name[i] = c;
                    c = getc(file);
                    i++;
                }

                if (c == EOF)
                {
                    fclose(out);
                    return INVALID_ARG;
                }

                buf_name[i] = '\0';

                if (c != ' ')
                {
                    fclose(out);
                    return INVALID_ARG;
                }
                c = getc(file);

                j = 0;
                while (isalnum(c))
                {
                    buf_val[j] = c;
                    c = getc(file);
                    j++;
                }

                if (c == EOF)
                {
                    return OK;
                }
                buf_val[j] = '\0';

                if (add_elem(hash, buf_name, i, buf_val, j, cash_list) == MEM_NOT_ALLOC)
                {
                    fclose(out);
                    return MEM_NOT_ALLOC;
                }

                if (c != '\n')
                {
                    fclose(out);
                    return INVALID_ARG;
                }
            }
            else
            {
                flag = 0;
            }
        } 
        else
        {
            flag = 0;
            
            i = 0;
            while (isalnum(c))
            {
                buf[i] = c;
                c = getc(file);
                i++;
            }

            buf[i] = '\0';

            if (i != 0)
            {
                if (hash->func(buf, hash->HASHSIZE, &ind, &t) == OVERFLOW)
                {
                    fprintf(out, "%s", buf);
                } 
                else if (hash->table[ind] == NULL)
                {
                    fprintf(out, "%s", buf);
                }
                else
                {
                    cur = hash->table[ind];
                    flag_h = 0;

                    while (cur != NULL && !flag_h)
                    {
                        if (strcmp(cur->def_name, buf) == 0)
                        {
                            fprintf(out, "%s", cur->value);
                            flag_h = 1;
                        }

                        cur = cur->next;
                    }

                    if (!flag_h)
                    {
                        fprintf(out, "%s", buf);
                    }
                }
            }

            if (c == EOF)
            {
                fclose(out);
                return OK;
            } 
            else 
            {
                fprintf(out, "%c", c);
            }
        }
    }

    fclose(out);
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

    status_code status = OK;
    Cash_list* cash;

    status = create_cash_list(&cash);
    if (status != OK)
    {
        printf("Hash error\n");
        fclose(file);

        return status;
    }

    Hash_table* table;
    status = create_table(&table, 128, Hash_func );
    if (status != OK)
    {
        printf("Hash error\n");
        dell_cash_list(cash);
        fclose(file);

        return status;
    }

    status = read(file, table, cash);
    if (status != OK)
    {
        printf("Reading error\n");
    }

    print_hash(table);
    print_cash(cash);

    // if (rebuild(table, cash) != OK)
    // {
    //     printf("Rebuild error\n");
    // }

    // printf("%d\n", table->HASHSIZE);

    // print_hash(table);
    // print_cash(cash);

    dell_cash_list(cash);
    dell_table(table);
    free(table);
    fclose(file);
    return status;
}