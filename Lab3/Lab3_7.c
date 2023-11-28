#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

#define Name_size 15

typedef enum 
{
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE
} status_code;

typedef struct _liver
{   
    bool flag;
    char surname[Name_size];
    char name[Name_size];
    char patronymic[Name_size];
    char birth[11];
    char gender;
    double income;
    struct _liver* next;
} Liver;

typedef struct 
{
    Liver *head, *tail;
} List;

typedef struct _change_info
{
    char flag;
    char buf[Name_size];
    double income;
    Liver* ptr;
    struct _change_info *next, *prev;
} Change_info;

typedef struct
{
    int elems;
    int modif;
    Change_info *head, *tail;
} Modif_list; 


status_code push_back (List *list, Liver* new_liver) 
{
    if (list -> tail != NULL) 
    {
        list -> tail -> next = new_liver;
    }
    if (list -> head == NULL) 
    {
        list -> head = new_liver;
    }
    list -> tail = new_liver;
    new_liver -> next = NULL;

    return OK;
}

status_code push_front (List *list, Liver* new_liver) 
{
    if (list -> head != NULL) 
    {
        new_liver -> next = list -> head;
    }
    if (list -> tail == NULL) 
    {
        list -> tail = new_liver;
    }
    list -> head = new_liver;

    return OK;
}

int age_comparison (char* cur, char* liver)
{
    if (cur[6] < liver[6])
    {
        return 1;
    }
    else if (cur[6] > liver[6])
    {
        return 0;
    }

    int cur_age = 0, liv_age = 0;
    for (int i = 6; i < 10; i++)
    {
        cur_age = cur_age * 10 + cur[i] - '0';
        liv_age = liv_age * 10 + liver[i] - '0';
    }
    if (cur_age < liv_age) 
    {
        return 1;
    }
    else if (cur_age > liv_age)
    {
        return 0;
    }

    cur_age = 0, liv_age = 0;
    for (int i = 3; i < 5; i++)
    {
        cur_age = cur_age * 10 + cur[i] - '0';
        liv_age = liv_age * 10 + liver[i] - '0';
    }
    if (cur_age < liv_age) 
    {
        return 1;
    }
    else if (cur_age > liv_age)
    {
        return 0;
    }

    cur_age = 0, liv_age = 0;
    for (int i = 0; i < 2; i++)
    {
        cur_age = cur_age * 10 + cur[i] - '0';
        liv_age = liv_age * 10 + liver[i] - '0';
    }
    if (cur_age < liv_age) 
    {
        return 1;
    }
    else if (cur_age > liv_age)
    {
        return 0;
    }

    return 0;
}

status_code add_by_age (List *list, Liver* liver)
{
    liver->flag = true;
    if (list -> head == NULL) 
    {
        push_back(list, liver);
        return OK;
    }

    if (age_comparison(list->head->birth, liver->birth))
    {
        push_front (list, liver);
        return OK;
    }

    Liver* cur = list -> head;
    while (cur -> next != NULL) 
    {
        if (age_comparison(cur->next->birth, liver->birth))
        {
            break;
        }

        cur = cur -> next;
    }

    if (cur -> next == NULL) 
    {
        push_back(list, liver);
        return OK;
    }

    liver -> next = cur -> next;
    cur -> next = liver;

    return OK;
}

status_code del_list (List *list)
{
    if (list->head == NULL) 
    {
        return OK;
    }
    if (list -> head == list -> tail) 
    {
        free(list -> tail);
        list -> head = list -> tail = NULL;
        return OK;
    }

    Liver* cur = list->head;
    while (cur != NULL)
    {   
        list -> head = cur->next;
        free(cur);
        cur = list->head;
    }

    return OK;
}

status_code print_list (List* list)
{
    Liver* cur = list->head;
    while (cur != NULL) 
    {
        if (cur->flag)
        {
            printf("%s %s %s %s %c %.3f", cur->surname, cur->name, 
                    cur->patronymic, cur->birth, cur->gender, cur->income);
            printf("\n");
        }
        cur = cur->next;
    }

    return OK;
}

status_code fprint_list (List* list)
{
    char name[256];
    if (scanf("%s", name) != 1)
    {
        return INVALID_ARG;
    }
    getchar();

    FILE* file = fopen(name, "w");
    if (file == NULL)
    {
        return NOT_OPEN_FILE;
    }

    Liver* cur = list->head;
    while (cur != NULL) 
    {
        if (cur->flag)
        {
            fprintf(file, "%s %s %s %s %c %.3f", cur->surname, cur->name, 
                    cur->patronymic, cur->birth, cur->gender, cur->income);
            fprintf(file, "\n");
        }
        cur = cur->next;
    }

    fclose(file);
    return OK;
}


status_code real_del (List* livers, Liver* liver)
{
    Liver *cur = livers->head;
    if (strcmp(liver->name, cur->name) == 0 && strcmp(liver->surname, cur->surname) == 0 &&
        strcmp(liver->patronymic, cur->patronymic) == 0 && strcmp(liver->birth, cur->birth) == 0 &&
        liver->gender == cur->gender && fabs(cur->income - liver->income) < 1E-10) 
    {
        livers->head = cur->next;
        free(cur);
        return OK;
    }

    while (cur != NULL) 
    {
        if (strcmp(liver->name, cur->next->name) == 0 && strcmp(liver->surname, cur->next->surname) == 0 &&
            strcmp(liver->patronymic, cur->next->patronymic) == 0 && strcmp(liver->birth, cur->next->birth) == 0 &&
            liver->gender == cur->next->gender && fabs(cur->next->income - liver->income) < 1E-10) 
        {
            Liver* nxt = cur->next->next;
            free(cur->next);
            cur->next = nxt;
            return OK;
        }

        cur = cur->next;
    }

    return INVALID_ARG;
}

status_code modif_clean (Modif_list *list, List* livers)
{
    int lim = (list->modif & 1) ? ((list->modif + 1) / 2) : (list->modif / 2);

    Change_info* cur = list->head;
    while (cur != NULL && list->elems > lim) // flaf d
    {
        if (cur->ptr->flag == 'd')
        {
            real_del(livers, cur->ptr);
        }

        list->head = cur->next;
        cur->next->prev = NULL;
        free(cur);
        cur = list->head;
        list->elems--;
    }

    return OK;
}

status_code modif_push (Modif_list *list, Liver* liver, char flag, char* str, double inc, List* livers) 
{
    modif_clean (list, livers);

    Change_info* inf = (Change_info*)malloc(sizeof(Change_info));
    if (inf == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    inf->flag = flag;
    inf->ptr = liver;

    if (flag == 'i')
    {
        inf->income = inc;
    }
    else if (flag != 'd' && flag != 'a')
    {
        strcpy(inf->buf, str);
    }
    
    if (list->tail != NULL) 
    {
        list->tail->next = inf;
        inf->prev = list->tail;
    }
    if (list->head == NULL) 
    {
        list->head = inf;
        inf->prev = NULL;
    }
    list->tail = inf;
    inf->next = NULL;

    list->elems++;
    list->modif++;

    return OK;
}

status_code modif_get (Modif_list* modif)
{
    Change_info* last = modif->tail;
    if (last->prev == NULL)
    {
        modif->tail = NULL;
        modif->head = NULL;
        free(last);

        modif->elems = 0;
        modif->modif = 0;
    }
    else
    {
        modif->tail = last->prev;
        last->prev->next = NULL;
        free(last);

        modif->elems--;
        modif->modif = 0;
    }

    return OK;
}

status_code modif_del (Modif_list *list)
{
    if (list->head == NULL) 
    {
        return OK;
    }
    if (list -> head == list -> tail) 
    {
        free(list -> tail);
        list -> head = list -> tail = NULL;
        return OK;
    }

    Change_info* cur = list->head;
    while (cur != NULL)
    {   
        list -> head = cur->next;
        free(cur);
        cur = list->head;
    }

    return OK;
}


status_code valid_birth (char* b)
{
    if (strlen(b) != 10)
    {
        return INVALID_ARG;
    }
    if (b[2] != '.' || b[5] != '.')
    {
        return INVALID_ARG;
    }

    if (!isdigit(b[0]) && !isdigit(b[1]) && !isdigit(b[3]) && !isdigit(b[4]))
    {
        return INVALID_ARG;
    }
    for (int i = 6; i < 10; i++)
    {
        if (!isdigit(b[i]))
        {
            return INVALID_ARG;
        }
    }
    
    return OK;
}

status_code no_patronymic (Liver* liver, char*str)
{
    if (sscanf(str, "%s %s %s %c %lf", liver->surname, 
        liver->name, liver->birth, &liver->gender, &liver->income) != 5)
    {
        return INVALID_ARG;
    }
    strcpy(liver->patronymic, "n");

    return OK;
}

status_code read_liver (List* list, char* str, Modif_list* modif)
{
    Liver* liver = (Liver*)malloc(sizeof(Liver));
    if (liver == NULL) {
        return MEM_NOT_ALLOC;
    }
    
    if (sscanf(str, "%s %s %s %s %c %lf", liver->surname, liver->name, 
        liver->patronymic, liver->birth, &liver->gender, &liver->income) != 6) 
    {
        if (no_patronymic(liver, str) == INVALID_ARG)
        {
            return INVALID_ARG;
        }
    }

    if (isdigit(liver->patronymic[0]) == 1)
    {
        if (no_patronymic(liver, str) == INVALID_ARG)
        {
            return INVALID_ARG;
        }
    }

    if (valid_birth(liver->birth) == INVALID_ARG)
    {
        return INVALID_ARG;
    }

    if (add_by_age (list, liver) == MEM_NOT_ALLOC) 
    {
        return MEM_NOT_ALLOC;
    }

    if (modif != NULL)
    {
        if (modif_push(modif, liver, 'a', NULL, 0, list) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
    }

    return OK;
}

status_code read_info (FILE* file, List* list) 
{
    char *buf = (char*)malloc(sizeof(Liver) + 5);
    if (buf == NULL) 
    {
        return MEM_NOT_ALLOC;
    }
    
    int er_check, flag = 0;
    size_t read_line, len = 0;
    while ((read_line = getline(&buf, &len, file)) != -1) 
    {
        if (er_check = read_liver (list, buf, NULL) == MEM_NOT_ALLOC) 
        {
            free(buf);
            return MEM_NOT_ALLOC;
        } 
        else if (er_check == INVALID_ARG) 
        {
            break;
        } 
        else 
        {
            flag = 1;
        }
    }
    free(buf);

    if (!flag) 
    {
        return INVALID_ARG;
    }

    return OK;
}


status_code find (List* livers, Liver* liver, Liver** res) 
{
    Liver *cur = livers->head;
    while (cur != NULL) 
    {
        if (strcmp(liver->name, cur->name) == 0 && strcmp(liver->surname, cur->surname) == 0 &&
            strcmp(liver->patronymic, cur->patronymic) == 0 && strcmp(liver->birth, cur->birth) == 0 &&
            liver->gender == cur->gender && fabs(cur->income - liver->income) < 1E-10 && cur->flag) 
        {
            *res = cur;
            return OK;
        }

        cur = cur->next;
    }
    
    return INVALID_ARG;
}

status_code change (Liver** liver, char flag, char* str, double inc, Modif_list* list, List* livers)
{
    if (flag == 'i')
    {
        if (modif_push(list, (*liver), flag, NULL, (*liver)->income, livers) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
        (*liver)->income = inc;
    }
    else if (flag == 'n')
    {
        if (modif_push(list, (*liver), flag, (*liver)->name, 0, livers) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
        strcpy((*liver)->name, str);
    }
    else if (flag == 's')
    {
        if (modif_push(list, (*liver), flag, (*liver)->surname, 0, livers) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
        strcpy((*liver)->surname, str);
    }
    else if (flag == 'p')
    {
        if (modif_push(list, (*liver), flag, (*liver)->patronymic, 0, livers) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
        strcpy((*liver)->patronymic, str);
    }
    else if (flag == 'b')
    {
        if (modif_push(list, (*liver), flag, (*liver)->birth, 0, livers) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
        strcpy((*liver)->birth, str);
    }
    else if (flag == 'g')
    {
        char gender[1];
        gender[0] = (*liver)->gender;
        if (modif_push(list, (*liver), flag, gender, 0, livers) == MEM_NOT_ALLOC)
        {
            return MEM_NOT_ALLOC;
        }
        (*liver)->gender = str[0];
    }

    return OK;
}

status_code change_interface (Liver** liver, Modif_list* list, List* livers)
{
    char ent;
    char buf[Name_size];
    buf[Name_size - 1] = '\0';
    double inc;
    printf("Change:\nn - name\ns - surname\np - patronymic\nb - date of birth (dd.mm.yyyy)\ng - gender\ni - income\n");
    if (scanf("%c", &ent) != 1)
    {
        return INVALID_ARG;
    }
    getchar();

    printf("New: ");
    if (ent == 'i')
    {
        if (scanf("%lf", &inc) != 1)
        {
            return INVALID_ARG;
        }
    }
    else
    {
        if (scanf("%s", buf) != 1 || buf[Name_size - 1] != '\0')
        {
            return INVALID_ARG;
        }
        if (ent == 'g') 
        {
            if (buf[1] != '\0')
            {
                return INVALID_ARG;
            }
        }
        else if (ent == 'b')
        {
            if (buf[10] != '\0')
            {
                return INVALID_ARG;
            }
            if (valid_birth(buf) == INVALID_ARG)
            {
                return INVALID_ARG;
            }
        }
        else if (buf[Name_size - 1] != '\0')
        {
            return INVALID_ARG;
        }
    }
    getchar();

    if (change(liver, ent, buf, inc, list, livers) == MEM_NOT_ALLOC)
    {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code add_new (List* livers, Modif_list* modif)
{
    char *buf = (char*)malloc(sizeof(Liver) + 5);
    if (buf == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    if (scanf("%[^\n]", buf) != 1)
    {
        free(buf);
        return INVALID_ARG;
    }
    getchar();

    status_code status;
    status = read_liver (livers, buf, modif);
    if (status != OK)
    {
        free(buf);
        return status;
    }

    free(buf);
    return OK;
}

status_code del (List* livers, Modif_list* modif, Liver* liver)
{
    if (scanf("%s %s %s %s %c %lf", liver->surname, liver->name, 
        liver->patronymic, liver->birth, &liver->gender, &liver->income) != 6) 
    {
        getchar();
        free(liver);
        return INVALID_ARG;
    }
    getchar();

    Liver* res;
    if (find(livers, liver, &res) == INVALID_ARG) 
    {
        return INVALID_ARG;
    } 
    res->flag = false;

    if (modif_push(modif, res, 'd', NULL, 0, livers) == MEM_NOT_ALLOC)
    {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code undo (Modif_list* modif, List* list)
{
    if (modif->elems == 0)
    {
        return INVALID_ARG;
    }
    Change_info* info = modif->tail;
    switch (info->flag)
    {
        case 'd':
            info->ptr->flag = true;
            break;
        case 'a':
            if (real_del(list, info->ptr) == INVALID_ARG)
            {
                return INVALID_ARG;
            }
            break;
        case 'i':
            info->ptr->income = info->income;
            break;
        case 'n':
            strcpy(info->ptr->name, info->buf);
            break;
        case 's':
            strcpy(info->ptr->surname, info->buf);
            break;
        case 'p':
            strcpy(info->ptr->patronymic, info->buf);
            break;
        case 'b':
            strcpy(info->ptr->birth, info->buf);
            break;
        case 'g':
            info->ptr->gender = info->buf[0];
            break;
        
    }
    modif_get(modif);

    return OK;
}

status_code interface (List* livers, Modif_list* modif, Liver* liver)
{
    int e = 1;
    char ent, ent_2;
    Liver* res;
    status_code status;
    while (e) 
    {
        printf("Enter:\ne - exit\nf - find\nc - change\na - add\nd - dell\nu - undo\np - to file\n");
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
                printf("Find:\nsurname name patronymic(if not: n) (dd.mm.yyyy) gender income\n");
                if (scanf("%s %s %s %s %c %lf", liver->surname, liver->name, 
                    liver->patronymic, liver->birth, &liver->gender, &liver->income) != 6) 
                {
                    getchar();
                    printf("Invalid input\n");
                    free(liver);
                    return INVALID_ARG;
                }
                getchar();

                if (find(livers, liver, &res) == INVALID_ARG) 
                {
                    printf("Not found\n");
                    break;
                } 
                printf("%s %s %s %s %c %.3f", res->surname, res->name, 
                        res->patronymic, res->birth, res->gender, res->income);
                printf("\n");
                break;

            case 'c':
                printf("Find to change:\nsurname name patronymic(if not: n) (dd.mm.yyyy) gender income\n");
                if (scanf("%s %s %s %s %c %lf", liver->surname, liver->name, 
                    liver->patronymic, liver->birth, &liver->gender, &liver->income) != 6) 
                {
                    getchar();
                    printf("Invalid input\n");
                    break;
                }
                getchar();

                if (find(livers, liver, &res) == INVALID_ARG) 
                {
                    printf("Not found\n");
                    break;
                } 
                printf("%s %s %s %s %c %.3f", res->surname, res->name, 
                        res->patronymic, res->birth, res->gender, res->income);
                printf("\n");

                status = change_interface(&res, modif, livers);
                if (status == INVALID_ARG)
                {
                    printf("Invalid input\n");
                    break;
                }
                else if (status == MEM_NOT_ALLOC)
                {
                    return MEM_NOT_ALLOC;
                }
                printf("%s %s %s %s %c %.3f", res->surname, res->name, 
                        res->patronymic, res->birth, res->gender, res->income);
                printf("\n");
                break;

            case 'a':
                printf("Add new:\nsurname name patronymic (dd.mm.yyyy) gender income\n");
                status = add_new(livers, modif);
                if (status == INVALID_ARG)
                {
                    printf("Add error\n");
                    break;
                }
                else if (status == MEM_NOT_ALLOC)
                {
                    printf("Memory was not allocated\n");
                    return MEM_NOT_ALLOC;
                }
                printf("\n");
                print_list(livers);

                break;
            case 'd':
                printf("Find to dell:\nsurname name patronymic(if not: n) (dd.mm.yyyy) gender income\n");
                status = del(livers, modif, liver);
                if (status == INVALID_ARG)
                {
                    printf("Not found\n");
                    break;
                }
                 else if (status == MEM_NOT_ALLOC)
                {
                    printf("Memory was not allocated\n");
                    return MEM_NOT_ALLOC;
                }
                printf("\n");
                print_list(livers);

                break;
            case 'u':
                if (undo(modif, livers) == INVALID_ARG)
                {
                    printf("Impossible to undo\n");
                    break;
                }
                print_list(livers);

                break;
            case 'p':
                printf("File:\n");
                if (fprint_list(livers) != OK)
                {
                    printf("File error\n");
                    break;
                }

                break;

            default:
                printf("Invalid input\n");
                break;
        }
    }

    return OK;
}


int main ()
{
    FILE* file = fopen("C:\\VSCode\\files\\mp_3\\input_7.txt", "r");
    if (file == NULL) 
    {
        printf("The file was not opened\n");
        return NOT_OPEN_FILE;
    }

    List *livers = (List*)malloc(sizeof(List));
    if (livers != NULL)
    {
        livers -> head = NULL;
        livers -> tail = NULL;
    } 
    else 
    {
        fclose(file);
        printf("Memory was not allocated\n");
        return MEM_NOT_ALLOC;
    }

    Modif_list *modifications = (Modif_list*)malloc(sizeof(Modif_list));
    if (modifications != NULL)
    {
        modifications->elems = 0;
        modifications->modif = 0;
        modifications->head = NULL;
        modifications->tail = NULL;
    } 
    else 
    {
        fclose(file);
        del_list(livers);
        free(livers);
        printf("Memory was not allocated\n");
        return MEM_NOT_ALLOC;
    }

    status_code status;
    status = read_info(file, livers);
    if (status == MEM_NOT_ALLOC)
    {
        del_list(livers);
        free(livers);
        modif_del(modifications);
        free(modifications);
        fclose(file);
        printf("Memory was not allocated\n");
        return MEM_NOT_ALLOC;
    }
    else if (status == INVALID_ARG)
    {
        del_list(livers);
        free(livers);
        modif_del(modifications);
        free(modifications);
        fclose(file);
        printf("Invalid input\n");
        return INVALID_ARG;
    }

    print_list(livers);

    Liver *liver = (Liver*)malloc(sizeof(Liver));
    if (liver == NULL) 
    {
        del_list(livers);
        modif_del(modifications);
        free(modifications);
        free(livers);
        fclose(file);
        printf("Memory was not allocated\n");
        return MEM_NOT_ALLOC;
    }

    status_code status_2 = interface(livers, modifications, liver);
    if (status_2 != OK)
    {
        del_list(livers);
        modif_del(modifications);
        free(modifications);
        free(livers);
        free(liver);
        fclose(file);
        printf("Error\n");
        return status_2;
    }

    modif_del(modifications);
    del_list(livers);
    free(modifications);
    free(livers);
    free(liver);
    fclose(file);
    return OK;
}
