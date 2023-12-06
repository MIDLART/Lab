#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <malloc.h>

typedef enum {
    ok,
    invalid_arguments,
    not_open_file,
    memory_not_allocated,
    overflow
} status_code;

status_code Roman (char* buf, int num) {
    if (num > 3999 || num < 1) {
        return invalid_arguments;
    }

    int k = 0;
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char* roman_symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int i = 0;
    while (num > 0) {
        if (num >= values[i]) {
            buf[k] = roman_symbols[i][0];
            k++;
            if (i & 1) {
                buf[k] = roman_symbols[i][1];
                k++;
            }
            num -= values[i];

        } else {
            i++;
        }
    }
    buf[k] = '\0';

    return ok;
}

status_code Zeckendorf (unsigned int num, char* res, size_t *len) {
    if (num == 0) {
        strcpy(res, "0");
        *len = 1;
        return ok;
    } else if (num == 1) {
        strcpy(res, "11");
        *len = 2;
        return ok;
    }

    unsigned int size = 16;
    unsigned int* fibonacci = (unsigned int*)malloc(sizeof(unsigned int) * size);
    if (fibonacci == NULL) {
        return memory_not_allocated;
    }

    fibonacci[0] = 0;
    fibonacci[1] = 1;
    int i = 1;

    while (fibonacci[i] < num) {
        i++;
        if (i >= size) {
            size *= 2;
            unsigned int* tmp = (unsigned int*)realloc(fibonacci, size);
            if (tmp == NULL) {
                free(fibonacci);
                return memory_not_allocated;
            }
            fibonacci = tmp;
        }
        fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
    }

    unsigned int size_r = 4;

    if (fibonacci[i] != num) {
        i--;
    }
    int j = 0;
    for (; i > 1; i--) {
        if (j >= size_r - 1) {
            size_r *= 2;
            char* tmp = (char*)realloc(res, size_r);
            if (tmp == NULL) {
                free(res);
                free(fibonacci);
                return memory_not_allocated;
            }
            res = tmp;
        }

        if (num >= fibonacci[i]){
            num -= fibonacci[i];
            res[j] = '1';
        } else {
            res[j] = '0';
        }
        j++;
    }

    res[j] = '\0';
    *len = j - 1;
    free(fibonacci);
    return ok;
}

status_code check (char c, int system, char reg) {
    if (c < '0' || (c > '9' && c < reg)) {
        return invalid_arguments;
    }
    if (c > '9') {
        int p = (reg == 'a') ? 39 : 7;
        c -= p + '0';
        if (c >= system) {
            return invalid_arguments;
        }
    }

    return ok;
}

status_code Conversion_10_to_system (char *str_num, int num, int system, int* len) {
    int i = 64, otr = 0;
    char character;
    if (num < 0) {
        num *= -1;
        otr = 1;
    }
    while (num != 0) {
        character = num % system;
        if (character > 9) {
            character += 7;
        }
        character += '0';

        str_num[i] = character;
        num /= system;
        i--;
    }
    if (otr) {
        str_num[i] = '-';
        i--;
    }
    i++;
    *len = i;

    return ok;
}

status_code conversion_10_to_system (char *str_num, int num, int system, int* len) {
    int i = 64, otr = 0;
    char character;
    if (num < 0) {
        num *= -1;
        otr = 1;
    }
    while (num != 0) {
        character = num % system;
        if (character > 9) {
            character += 39;
        }
        character += '0';

        str_num[i] = character;
        num /= system;
        i--;
    }
    if (otr) {
        str_num[i] = '-';
        i--;
    }
    i++;
    *len = i;

    return ok;
}

status_code conv_to_dec (char* buf, char num[], int num_system) {
    if (num_system < 2 || num_system > 36) {
        num_system = 10;
    }
    long long res = 0;
    int i, cur, otr = 0;
    if (num[0] == '-') {
        otr = 1;
    }
    i = otr;
    while (num[i] != '\0') {
        if (check(num[i], num_system, 'a') == invalid_arguments) {
            return invalid_arguments;
        }
        cur = num[i];
        if (num[i] >= 'a') {
            cur -= 39;
        }
        cur -= '0';

        if ((LLONG_MAX - cur) / num_system < res) {
            return overflow;
        }
        res = res * num_system + cur;
        i++;
    }
    if (otr) {
        res *= -1;
    }
    snprintf(buf, sizeof(buf), "%lld", res);

    return ok;
}

status_code conv_TO_dec (char* buf, char num[], int num_system) {
    if (num_system < 2 || num_system > 36) {
        num_system = 10;
    }
    long long res = 0;
    int i, cur, otr = 0;
    if (num[0] == '-') {
        otr = 1;
    }
    i = otr;
    while (num[i] != '\0') {
        if (check(num[i], num_system, 'A') == invalid_arguments) {
            return invalid_arguments;
        }
        cur = num[i];
        if (num[i] >= 'A') {
            cur -= 7;
        }
        cur -= '0';

        if ((LLONG_MAX - cur) / num_system < res) {
            return overflow;
        }
        res = res * num_system + cur;
        i++;
    }
    if (otr) {
        res *= -1;
    }
    snprintf(buf, 12, "%lld", res);

    return ok;
}

status_code memory_dump(char* buf, void *ptr, size_t size) {
    unsigned char *p = (unsigned char*)ptr;
    int k = 0;
    for (size_t i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            buf[k] = ((p[i] >> j) & 1) + '0';
            k++;
        }
        buf[k] = ' ';
        k++;
    }
    buf[k] = '\0';

    return ok;
}

int overfprintf (FILE* stream, const char* format, ...) {
    int char_record = 0, i = 0, record;
    char buf[73];
    int result;

    va_list args;
    va_start(args, format);

    while (format[i] != '\0') {
        if (format[i] == '%' && format[i + 1] != '\0') {
            if (format[i + 1] == 'R' && format[i + 2] == 'o') {
                int num = va_arg(args, int);
                if (Roman(buf, num) == invalid_arguments) {
                    return -1;
                }
                fputs(buf, stream);
                char_record += 1;
                i += 2;

            } else if (format[i + 1] == 'Z' && format[i + 2] == 'r') {
                int num = va_arg(args, int);

                unsigned int size_r = 4;
                char* res = (char*)malloc(sizeof(char) * size_r);
                if (res == NULL) {
                    return memory_not_allocated;
                }
                size_t len;

                if (Zeckendorf(num, res, &len) == memory_not_allocated) {
                    return -1;
                }

                for (int j = len; j >= 0; j--) {
                    fputc(res[j], stream);
                }
                fputc('1', stream);
                free(res);

                char_record += 1;
                i += 2;
            
            } else if (format[i + 1] == 'C' && format[i + 2] == 'v') {
                int num = va_arg(args, int);
                int system = va_arg(args, int);
                int len;
                if (conversion_10_to_system(buf, num, system, &len) != ok) {
                    return -1;
                }

                for (int l = len; l < 65; l++) {
                    fputc(buf[l], stream);
                    char_record++;
                }
                char_record += 1;
                i += 2;

            } else if (format[i + 1] == 'C' && format[i + 2] == 'V') {
                int num = va_arg(args, int);
                int system = va_arg(args, int);
                int len;
                if (Conversion_10_to_system(buf, num, system, &len) != ok) {
                    return -1;
                }

                for (int l = len; l < 65; l++) {
                    fputc(buf[l], stream);
                    char_record++;
                }
                char_record += 1;
                i += 2;

            } else if (format[i + 1] == 't' && format[i + 2] == 'o') {
                char* num = va_arg(args, char*);
                int system = va_arg(args, int);
                if (conv_to_dec(buf, num, system) != ok) {
                    return -1;
                }
                fputs(buf, stream);
                char_record += 1;
                i += 2;

            } else if (format[i + 1] == 'T' && format[i + 2] == 'O') {
                char* num = va_arg(args, char*);
                int system = va_arg(args, int);
                if (conv_TO_dec(buf, num, system) != ok) {
                    return -1;
                }
                fputs(buf, stream);
                char_record += 1;
                i += 2;

            } else if (format[i + 1] == 'm' && format[i + 2] == 'i') {
                int num = va_arg(args, int);
                memory_dump(buf, &num, sizeof(signed int));
                fputs(buf, stream);
                char_record += 1;
                i += 2;

            } else if (format[i + 1] == 'm' && format[i + 2] == 'u') {
                unsigned int num = va_arg(args, unsigned int);
                memory_dump(buf, &num, sizeof(unsigned int));
                fputs(buf, stream);
                char_record += 1;
                i += 2;

            } else if (format[i + 1] == 'm' && format[i + 2] == 'd') {
                double num = va_arg(args, double);
                memory_dump(buf, &num, sizeof(double));
                fputs(buf, stream);
                char_record += 1;
                i += 2;

            } else if (format[i + 1] == 'm' && format[i + 2] == 'f') {
                float num = va_arg(args, double);
                memory_dump(buf, &num, sizeof(float));
                fputs(buf, stream);
                char_record += 1;
                i += 2;

            } else {
                char* st_str = (char*)malloc(sizeof(char) * (strlen(format) + 1));
                if (st_str == NULL) {
                    return -1;
                }

                int j = 0, k = i;
                do {
                    st_str[j] = format[k];
                    j++;
                    k++;
                } while (format[k] != '%' && format[k] != '\0');
                st_str[j] = '\0';

                void* std = va_arg(args, void*);
                
                result = fprintf(stream, st_str, std);
                char_record += result;
                i = k - 1;

                free(st_str);
            }

        } else if (format[i] == '%') {
            return -1;

        } else {
            fputc(format[i], stream);
            char_record++;
        }

        i++;
    }

    va_end(args);
    return char_record;
}

int oversprintf (char* res_buf, const char* format, ...) {
    int char_record = 0, i = 0, record;
    char buf[73];

    int result; 
    size_t len;

    va_list args;
    va_start(args, format);

    while (format[i] != '\0') {
        if (format[i] == '%' && format[i + 1] != '\0') {
            if (format[i + 1] == 'R' && format[i + 2] == 'o') {
                int num = va_arg(args, int);
                if (Roman(buf, num) == invalid_arguments) {
                    return -1;
                }

                len = strlen(buf);
                memcpy(res_buf + char_record, buf, sizeof(char) * len);
                char_record += len;
                i += 2;

            } else if (format[i + 1] == 'Z' && format[i + 2] == 'r') {
                int num = va_arg(args, int);

                unsigned int size_r = 4;
                char* res = (char*)malloc(sizeof(char) * size_r);
                if (res == NULL) {
                    return memory_not_allocated;
                }
                size_t len;

                if (Zeckendorf(num, res, &len) == memory_not_allocated) {
                    return -1;
                }

                for (int j = len; j >= 0; j--) {
                    res_buf[char_record] = res[j];
                    char_record++;
                }
                res_buf[char_record] = '1';
                free(res);

                char_record++;
                i += 2;

             } else if (format[i + 1] == 'C' && format[i + 2] == 'v') {
                int num = va_arg(args, int);
                int system = va_arg(args, int);
                int len;
                if (conversion_10_to_system(buf, num, system, &len) != ok) {
                    return -1;
                }

                for (int l = len; l < 65; l++) {
                    res_buf[char_record] = buf[l];
                    char_record++;
                }
                i += 2;

            } else if (format[i + 1] == 'C' && format[i + 2] == 'V') {
                int num = va_arg(args, int);
                int system = va_arg(args, int);
                int len;
                if (Conversion_10_to_system(buf, num, system, &len) != ok) {
                    return -1;
                }

                for (int l = len; l < 65; l++) {
                    res_buf[char_record] = buf[l];
                    char_record++;
                }
                i += 2;

            } else if (format[i + 1] == 't' && format[i + 2] == 'o') {
                char* num = va_arg(args, char*);
                int system = va_arg(args, int);
                if (conv_to_dec(buf, num, system) != ok) {
                    return -1;
                }

                len = strlen(buf);
                memcpy(res_buf + char_record, buf, sizeof(char) * len);
                char_record += len;
                i += 2;

            } else if (format[i + 1] == 'T' && format[i + 2] == 'O') {
                char* num = va_arg(args, char*);
                int system = va_arg(args, int);
                if (conv_TO_dec(buf, num, system) != ok) {
                    return -1;
                }

                len = strlen(buf);
                memcpy(res_buf + char_record, buf, sizeof(char) * len);
                char_record += len;
                i += 2;

            } else if (format[i + 1] == 'm' && format[i + 2] == 'i') {
                int num = va_arg(args, int);
                memory_dump(buf, &num, sizeof(signed int));

                len = strlen(buf);
                memcpy(res_buf + char_record, buf, sizeof(char) * len);
                char_record += len;
                i += 2;

            } else if (format[i + 1] == 'm' && format[i + 2] == 'u') {
                unsigned int num = va_arg(args, unsigned int);
                memory_dump(buf, &num, sizeof(unsigned int));

                len = strlen(buf);
                memcpy(res_buf + char_record, buf, sizeof(char) * len);
                char_record += len;
                i += 2;

            } else if (format[i + 1] == 'm' && format[i + 2] == 'd') {
                double num = va_arg(args, double);
                memory_dump(buf, &num, sizeof(double));
                
                len = strlen(buf);
                memcpy(res_buf + char_record, buf, sizeof(char) * len);
                char_record += len;
                i += 2;

            } else if (format[i + 1] == 'm' && format[i + 2] == 'f') {
                float num = va_arg(args, double);
                memory_dump(buf, &num, sizeof(float));
                
                len = strlen(buf);
                memcpy(res_buf + char_record, buf, sizeof(char) * len);
                char_record += len;
                i += 2;

            } else {
                char* st_str = (char*)malloc(sizeof(char) * (strlen(format) + 1));
                if (st_str == NULL) {
                    return -1;
                }

                int j = 0, k = i;
                do {
                    st_str[j] = format[k];
                    j++;
                    k++;
                } while (format[k] != '%' && format[k] != '\0');
                st_str[j] = '\0';

                void* std = va_arg(args, void*);
                
                len = strlen(buf);
                char* st_buf[310];
                result = sprintf(buf, st_str, std);

                len = strlen(buf);
                memcpy(res_buf + char_record, buf, sizeof(char) * len);

                char_record += result;
                i = k - 1;

                free(st_str);
            }

        } else if (format[i] == '%') {
            return -1;

        } else {
            res_buf[char_record] = format[i];
            char_record++;
        }

        i++;
    }
    res_buf[char_record] = '\0';
    char_record++;

    va_end(args);
    return char_record;
}

int main () {
    FILE* file = fopen("C:\\VSCode\\files\\mp_2\\test5.txt", "w");
    if (file == NULL) {
        return not_open_file;
    }
    
    char c = 'A';

    if (overfprintf(file, "int %d\n%c\n%s\n\n\n", 1234, c, "str") == -1) {
        printf("Error\n");         
    }

    if (overfprintf(file, "%Ro\ncfvghb123\n%Zr\n", 109, 25) == -1) {
        printf("Error\n");
    }
    if (overfprintf(file, "%to\n%TO\n\n%Cv\n%CV\n\n", "a", 16, "ABCZ", 36, 10, 16, 36, 36) == -1) {
        printf("Error\n");
    }
    int a = 12345678;
    unsigned int b = 123;
    double d = 12323.4567;
    float f = 0.5;
    if (overfprintf(file, "%mi\n%mu\n%md\n%mf\n", a, b, d, (float)f) == -1) {
        printf("Error\n");
    }

    fclose(file);

    char* buf = (char*)malloc(sizeof(char) * 256);
    if (buf == NULL)
    {
        return memory_not_allocated;
    }
    
    if (oversprintf(buf, "ABcd %d\n%mi\n%Ro\n%Zr\n%TO", 15, a, 10, 13, "F", 16) == -1) {
        printf("Error\n");
    }
    printf("%s\n", buf);
    free(buf);

    return ok;
}
