#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

typedef enum 
{
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE,
    DIV_0,
    OVERFLOW_,
    UNCERTAINTY
} status_code;

typedef struct _st_elem
{
    char data;
    struct _st_elem *prev;
} St_elem;

typedef struct
{
    St_elem *tail;
} Stack;

typedef struct _st_num
{
    int data;
    struct _st_num *prev;
} St_num;

typedef struct
{
    St_num *tail;
} Stack_num;


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


status_code creat_stack_num (Stack_num** stack)
{
    *stack = (Stack_num*)malloc(sizeof(Stack_num));
    if (stack == NULL)
    {
        return MEM_NOT_ALLOC;
    }
    (*stack)->tail = NULL;

    return OK;
}

status_code push_back_num (Stack_num* stack, int num)
{
    St_num* new = (St_num*)malloc(sizeof(St_num));
    if (new == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    new->data = num;
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

status_code pop_back_num (Stack_num* stack, int* num)
{
    if (stack->tail == NULL)
    {
        return INVALID_ARG;
    }
    
    *num = stack->tail->data;

    St_num* new_tail = stack->tail->prev;
    free(stack->tail);
    stack->tail = new_tail;

    return OK;
}

status_code dell_stack_num (Stack_num* stack)
{
    if (stack->tail == NULL)
    {
        return INVALID_ARG;
    }

    while (stack->tail != NULL)
    {
        St_num* elem = stack->tail;
        stack->tail = elem->prev;
        free(elem);
    }
    free(stack);
    
    return OK;
}


bool is_operation (char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^')
    {
        return true;
    }
    
    return false;
}

bool priority (char cur, char top)
{
    short int p_cur, p_top;

    if (top == '(')
    {
        return true;
    }

    if (cur == '^') 
    {
        p_cur = 3;
    } 
    else if (cur == '*' || cur == '/' || cur == '%')
    {
        p_cur = 2;
    }
    else
    {
        p_cur = 1;
    }
    
    if (top == '^')
    {
        p_top = 3;
    } 
    else if (top == '*' || top == '/' || top == '%')
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

status_code operation (int num_1, int num_2, char op, int* res)
{
    switch (op)
    {
        case '+':
            if ((num_1 < 0 && num_2 > 0 || num_1 > 0 && num_2 < 0) 
                && (INT_MAX - abs(num_1) < abs(num_2)))
            {
                return OVERFLOW_;
            }

            *res = num_1 + num_2;

            break;

        case '-':
            if ((num_1 < 0 && num_2 < 0 || num_1 > 0 && num_2 > 0) 
                && (INT_MAX - abs(num_1) < abs(num_2)))
            {
                return OVERFLOW_;
            }

            *res = num_1 - num_2;

            break;

        case '*':
            if (INT_MAX / num_1 <= num_2)
            {
                return OVERFLOW_;
            }

            *res = num_1 * num_2;

            break;
        
        case '/':
            if (num_2 == 0)
            {
                return DIV_0;
            }

            *res = num_1 / num_2;

            break;

        case '%':
            if (num_2 == 0)
            {
                return DIV_0;
            }
            
            *res = num_1 % num_2;

            break;

        case '^':
            if (num_2 < 0)
            {
                return INVALID_ARG;
            }

            if (num_1 == 0 && num_2 == 0)
            {
                return UNCERTAINTY;
            }
            
            *res = pow(num_1, num_2);

            break;
    }

    // printf("%c %d %d %d ", op, num_1, num_2, *res);

    return OK;
}

status_code conv_to_dec (char num[], int *decimal) 
{
    int i = 0, cur, res = 0;

    while (num[i] != '\0') 
    {
        cur = num[i] - '0';

        if ((INT_MAX - cur) / 10 < res) 
        {
            return OVERFLOW_;
        }

        res = res * 10 + cur;
        i++;
    }

    *decimal = res;
    return OK;
}


status_code reverse_polish_entry (char* str, char* rev_pol)
{
    Stack* stack;
    if (creat_stack(&stack) == MEM_NOT_ALLOC)
    {
        return MEM_NOT_ALLOC;
    }

    int i = 0, j = 0;
    char c;

    while (str[i] != '\0')
    {
        if (str[i] == '(')
        {
            if (push_back(stack, str[i]) == MEM_NOT_ALLOC)
            {
                dell_stack(stack);
                return MEM_NOT_ALLOC;
            }
        }
        else if (is_operation(str[i]))
        {
            if (str[i] == '-' && (i == 0 || str[i - 1] == '('))
            {
                if (i != 0 && !isdigit(str[i - 1]))
                {
                    rev_pol[j] = ' ';
                    rev_pol[j + 1] = '0';

                    j += 2;
                }
                else
                {
                    rev_pol[j] = '0';
                    j++;
                }
            }

            if (stack->tail != NULL)
            {
                if (priority(str[i], stack->tail->data))
                {
                    if (push_back(stack, str[i]) == MEM_NOT_ALLOC)
                    {
                        dell_stack(stack);
                        return MEM_NOT_ALLOC;
                    }
                }
                else
                {
                    while (stack->tail != NULL && !priority(str[i], stack->tail->data))
                    {
                        pop_back(stack, &c);

                        rev_pol[j] = c;
                        j++;
                    }

                    if (push_back(stack, str[i]) == MEM_NOT_ALLOC)
                    {
                        dell_stack(stack);
                        return MEM_NOT_ALLOC;
                    }
                }
            }
            else
            {
                if (push_back(stack, str[i]) == MEM_NOT_ALLOC)
                {
                    dell_stack(stack);
                    return MEM_NOT_ALLOC;
                }
            }
        }
        else if (isdigit(str[i]))
        {
            if (i != 0 && !isdigit(str[i - 1]))
            {
                rev_pol[j] = ' ';
                rev_pol[j + 1] = str[i];

                j += 2;
            }
            else
            {
                rev_pol[j] = str[i];
                j++;
            }
        }
        else if (str[i] == ')')
        {
            while (stack->tail->data != '(')
            {
                pop_back(stack, &c);

                rev_pol[j] = c;
                j++;
            }

            pop_back(stack, &c);
        }
        else
        {
            dell_stack(stack);
            return INVALID_ARG;
        }
        
        i++;
    }

    while (stack->tail != NULL)
    {
        pop_back(stack, &c);

        rev_pol[j] = c;
        j++;
    }

    rev_pol[j] = '\0';

    if (i == 0)
    {
        dell_stack(stack);
        return INVALID_ARG;
    }


    dell_stack(stack);
    return OK;
}

status_code solve (char* str, int* res)
{
    char buf[11];
    int i = 0, j;
    int num, n_1, n_2;
    status_code status;

    *res = 0;

    Stack_num* stack;
    if (creat_stack_num(&stack) == MEM_NOT_ALLOC)
    {
        return MEM_NOT_ALLOC;
    }

    while (str[i] != '\0')
    {
        j = 0;
        while (isdigit(str[i]))
        {
            buf[j] = str[i];

            j++;
            i++;
        }
        buf[j] = '\0';

        if (j > 11)
        {
            dell_stack_num(stack);
            return OVERFLOW_;
        }
        else if (j != 0)
        {
            if (conv_to_dec(buf, &num) == OVERFLOW_)
            {
                dell_stack_num(stack);
                return OVERFLOW_;
            }

            if (push_back_num(stack, num) == MEM_NOT_ALLOC)
            {
                dell_stack_num(stack);
                return MEM_NOT_ALLOC;
            }
        }

        if (str[i] == ' ')
        {
            i++;
        }
        else if (is_operation(str[i]))
        {
            if (pop_back_num(stack, &n_2) == INVALID_ARG)
            {
                dell_stack_num(stack);
                return INVALID_ARG;
            }

            if (pop_back_num(stack, &n_1) == INVALID_ARG)
            {
                dell_stack_num(stack);
                return INVALID_ARG;
            }

            status = operation(n_1, n_2, str[i], res);
            if (status != OK)
            {
                dell_stack_num(stack);
                return status;
            }
            else
            {
                if (push_back_num(stack, *res) == MEM_NOT_ALLOC)
                {
                    dell_stack_num(stack);
                    return MEM_NOT_ALLOC;
                }
            }

            i++;
        }
        else
        {
            if (pop_back_num(stack, res) == INVALID_ARG)
            {
                dell_stack_num(stack);
                return INVALID_ARG;
            }

            break;
        }
    }
    
    dell_stack_num(stack);
    return OK;
}


status_code read (FILE* file, FILE* out)
{
    char c;
    char buf[256];
    int flag = 1;
    int str = 0;
    int i, staples;
    status_code status;

    while (flag)
    {
        i = 0;
        staples = 0;

        while ((c = getc(file)) != EOF && c != '\n')
        {
            buf[i] = c;
            i++;

            if (c == '(')
            {
                staples++;
            }
            else if (c == ')')
            {
                staples--;
            }

            if (staples < 0)
            {
                staples = -260;
            }    
        }

        buf[i] = '\0';
        printf("%s  ", buf);

        if (staples != 0)
        {
            fprintf(out, "%s The balance of the brackets is broken in %d line\n", buf, str);
        }
        else
        {
            char rev[512];

            status = reverse_polish_entry(buf, rev);

            if (status == MEM_NOT_ALLOC)
            {
                return status;
            }
            else if (status == INVALID_ARG)
            {
                fprintf(out, "%s Invalid input in %d line\n", buf, str);
                printf("\n");
            }
            else
            {
                printf("%s  ", rev);

                int res;
                status = solve(rev, &res);

                if (status == MEM_NOT_ALLOC)
                {
                    return MEM_NOT_ALLOC;
                }
                else if (status == INVALID_ARG)
                {
                    fprintf(out, "%s Invalid input in %d line\n", buf, str);
                    printf("\n");
                }
                else if (status == OVERFLOW_)
                {
                    fprintf(out, "%s Overflow in %d line\n", buf, str);
                    printf("\n");
                }
                else if (status == DIV_0)
                {
                    fprintf(out, "%s Division by zero in %d line\n", buf, str);
                    printf("\n");
                }
                else if (status == UNCERTAINTY)
                {
                    fprintf(out, "%s Zero to the power of zero in %d line\n", buf, str);
                    printf("\n");
                }
                else
                {
                    printf("res: %d\n", res);
                }
            }
        }

        if (c == EOF)
        {
            flag = 0;
        }
        
        str++;
    }
    

    return OK;
}

status_code create_out (char* name, FILE** output_file)
{
    size_t file_name_size = strlen(name);
    char* file_name = (char*)malloc(sizeof(char)*(file_name_size + 5));
    if(file_name == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    size_t index_current = file_name_size - 1;

    memcpy(file_name, name, sizeof(char) * index_current);
    file_name[index_current + 5] = '\0';

    while (name[index_current] != '\\' && index_current >= 0) 
    {
        file_name[index_current + 4] = name[index_current];
        index_current--;
    }

    memcpy(file_name + index_current + 1, "out_", sizeof(char) * 4);

    printf("%s\n", file_name);
    
    *output_file = fopen(file_name, "w");
    if (*output_file == NULL)
    {
        return NOT_OPEN_FILE;
    }

    free(file_name);

    return OK;
}


int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Invalid input\n");
        return INVALID_ARG;
    }

    status_code status = OK;

    FILE* file, * out;
    for (int i = 1; i < argc; i++)
    {
        file = fopen(argv[i], "r");
        if (file == NULL)
        {
            printf("Not open file\n");
            return NOT_OPEN_FILE;
        }

        printf("%s\n", argv[i]);

        if (create_out(argv[i], &out) != OK)
        {
            printf("Output file error\n");
            fclose(file);
            return NOT_OPEN_FILE;
        }

        status = read(file, out);


        printf("\n");
        fclose(file);
        fclose(out);
    }

    return OK;
}