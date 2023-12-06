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

status_code Roman (const char* buf, int *num) {
    int res = 0;

    int i = 0, ch = 0;
    while (buf[i] == 'M') {
        res += 1000;
        i++;
        ch++;
    }
    if (ch > 3) {
        return invalid_arguments;
    }
    ch = 0;

    if (buf[i] == 'C' && buf[i + 1] == 'M') {
        res += 900;
        i += 2;
    }

    if (buf[i] == 'D') {
        res += 500;
        i++;
    }

    if (buf[i] == 'C' && buf[i + 1] == 'D') {
        res += 400;
        i += 2;
    }

    while (buf[i] == 'C') {
        res += 100;
        i++;
        ch++;
    }
    if (ch > 3) {
        return invalid_arguments;
    }
    ch = 0;

    if (buf[i] == 'X' && buf[i + 1] == 'C') {
        res += 90;
        i += 2;
    }

    if (buf[i] == 'L') {
        res += 50;
        i++;
    }

    if (buf[i] == 'X' && buf[i + 1] == 'L') {
        res += 40;
        i += 2;
    }

    while (buf[i] == 'X') {
        res += 10;
        i++;
        ch++;
    }
    if (ch > 3) {
        return invalid_arguments;
    }
    ch = 0;

    if (buf[i] == 'I' && buf[i + 1] == 'X') {
        res += 9;
        i += 2;
    }

    if (buf[i] == 'V') {
        res += 5;
        i++;
    }

    if (buf[i] == 'I' && buf[i + 1] == 'V') {
        res += 4;
        i += 2;
    }

    while (buf[i] == 'I') {
        res += 1;
        i++;
        ch++;
    }
    if (ch > 3) {
        return invalid_arguments;
    }

    if (buf[i] != '\0') {
        return invalid_arguments;
    } 

    *num = res;

    return ok;
}

status_code Zeckendorf (char* num, unsigned int* res) {
    size_t len = strlen(num);
    unsigned int prev_1, prev_2, t;
    *res = 0;

    if (num[len - 1] != '1') {
        return invalid_arguments;
    }

    if (len == 0) {
        return invalid_arguments;
    } else if (len == 1) {
        *res = 0;
        return ok;
    } else {
        prev_2 = 0; 
        prev_1 = 1;
    }

    len--;
    for (int i = 0; i < len; i++) {
        if (num[0] != '0' && num[0] != '1') {
            return invalid_arguments;
        }

        if (num[i] == '1') {
            (*res) += prev_1 + prev_2;
        }

        t = prev_2;
        prev_2 = prev_1;
        prev_1 += t;
    }
    
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

status_code conv (int* cv, char num[], int num_system) {
    if (num_system < 2 || num_system > 36) {
        num_system = 10;
    }
    int res = 0;
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

        if ((INT_MAX - cur) / num_system < res) {
            return overflow;
        }
        res = res * num_system + cur;
        i++;
    }
    if (otr) {
        res *= -1;
    }
    
    *cv = res;
    return ok;
}

status_code Conv (int* cv, char num[], int num_system) {
    if (num_system < 2 || num_system > 36) {
        num_system = 10;
    }
    int res = 0;
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

        if ((INT_MAX - cur) / num_system < res) {
            return overflow;
        }
        res = res * num_system + cur;
        i++;
    }
    if (otr) {
        res *= -1;
    }
    
    *cv = res;
    return ok;
}

int overfscanf (FILE* stream, const char* format, ...) {
    int record = 0, i = 0, n, rec;
    char c = ' ';
    size_t size = 8;

    char* buf = (char*)malloc(sizeof(char) * size);
    if (buf == NULL) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    while (format[i] != '\0') {
        if (format[i] == '%' && format[i + 1] != '\0') {
            n = 0;
            buf[0] = '\0';

            if (c == EOF) {
                free(buf);
                return record;
            }

            while ((c = getc(stream)) != EOF && c != ' ' && c != '\n' && c != '\t') {
                if (n >= size - 1) {
                    size *= 2;
                    char* tmp = realloc(buf, size);
                    if (tmp == NULL) {
                        va_end(args);
                        return -1;
                    }

                    buf = tmp;
                }

                buf[n] = c;
                n++;
            }
            buf[n] = '\0';

            if (format[i + 1] == 'R' && format[i + 2] == 'o') {
                int* num = va_arg(args, int*);
                if (Roman(buf, num) == invalid_arguments) {
                    free(buf);
                    return -1;
                }

                record += 1;
                i += 2;

            } else if (format[i + 1] == 'Z' && format[i + 2] == 'r') {
                unsigned int *res = va_arg(args, unsigned int*);

                if (Zeckendorf(buf, res) == invalid_arguments) {
                    free(buf);
                    return -1;
                }

                record += 1;
                i += 2;

            } else if (format[i + 1] == 'C' && format[i + 2] == 'v') {
                int* num = va_arg(args, int*);
                int system = va_arg(args, int);
                if (conv(num, buf, system) != ok) {
                    free(buf);
                    return -1;
                }

                record += 1;
                i += 2;

            } else if (format[i + 1] == 'C' && format[i + 2] == 'V') {
                int* num = va_arg(args, int*);
                int system = va_arg(args, int);
                if (Conv(num, buf, system) != ok) {
                    free(buf);
                    return -1;
                }

                record += 1;
                i += 2;

            } else {
                void* std = va_arg(args, void*);
                char st_flag[10];
                st_flag[0] = '%';
                int k = 1;

                while (format[i + 1] != '%' && format[i + 1] != '\0') {
                    st_flag[k] = format[i + 1];
                    i++;
                    k++;
                }
                st_flag[k] = '\0';

                rec = sscanf(buf, st_flag, std);
                if (rec == -1) {
                    return -1;
                }
                record += rec;
            }

        } else if (format[i] == '%') {
            free(buf);
            return -1;
        }

        i++;
    }

    va_end(args);
    free(buf);
    return record;
}

int oversscanf (char* stream, const char* format, ...) {
    int record = 0, i = 0, n, rec, c = 0;
    size_t size = 8;

    char* buf = (char*)malloc(sizeof(char) * size);
    if (buf == NULL) {
        return -1;
    }

    va_list args;
    va_start(args, format);

    while (format[i] != '\0') {
        if (format[i] == '%' && format[i + 1] != '\0') {
            n = 0;
            buf[0] = '\0';

            while (stream[c] != EOF && stream[c] != ' ' && stream[c] != '\n' && stream[c] != '\t') {
                if (n >= size - 1) {
                    size *= 2;
                    char* tmp = realloc(buf, size);
                    if (tmp == NULL) {
                        va_end(args);
                        return -1;
                    }

                    buf = tmp;
                }

                buf[n] = stream[c];
                n++;
                c++;
            }
            c++;
            buf[n] = '\0';

            if (format[i + 1] == 'R' && format[i + 2] == 'o') {
                int* num = va_arg(args, int*);
                if (Roman(buf, num) == invalid_arguments) {
                    free(buf);
                    return -1;
                }

                record += 1;
                i += 2;

            } else if (format[i + 1] == 'Z' && format[i + 2] == 'r') {
                unsigned int *res = va_arg(args, unsigned int*);

                if (Zeckendorf(buf, res) == invalid_arguments) {
                    free(buf);
                    return -1;
                }

                record += 1;
                i += 2;

            } else if (format[i + 1] == 'C' && format[i + 2] == 'v') {
                int* num = va_arg(args, int*);
                int system = va_arg(args, int);
                if (conv(num, buf, system) != ok) {
                    free(buf);
                    return -1;
                }

                record += 1;
                i += 2;

            } else if (format[i + 1] == 'C' && format[i + 2] == 'V') {
                int* num = va_arg(args, int*);
                int system = va_arg(args, int);
                if (Conv(num, buf, system) != ok) {
                    free(buf);
                    return -1;
                }

                record += 1;
                i += 2;

            } else {
                void* std = va_arg(args, void*);
                char st_flag[10];
                st_flag[0] = '%';
                int k = 1;

                while (format[i + 1] != '%' && format[i + 1] != '\0') {
                    st_flag[k] = format[i + 1];
                    i++;
                    k++;
                }
                st_flag[k] = '\0';

                rec = sscanf(buf, st_flag, std);
                if (rec == -1) {
                    return -1;
                }
                record += rec;
            }

        } else if (format[i] == '%') {
            free(buf);
            return -1;
        }

        i++;
    }

    va_end(args);
    free(buf);
    return record;
}

int main () {
    FILE* file = fopen("C:\\VSCode\\files\\mp_2\\test6.txt", "r");
    if (file == NULL) {
        return not_open_file;
    }

    int roman;
    unsigned int zeckendorf;
    int cv, CV, a;

    if (overfscanf(file, "%Ro%Zr%Cv%CV%d", &roman, &zeckendorf, &cv, 16, &CV, 36, &a) == -1) {
        printf("Error\n");         
    }

    fclose(file);

    printf("%d\n%u\n%d %d\n%d\n", roman, zeckendorf, cv, CV, a);

    char buf[] = "XIV 27";
    if (oversscanf(buf, "%Ro%d", &roman, &a) == -1) {
        printf("Error\n");         
    }
    
    printf("%d\n%d\n", roman, a);

    return ok;
}