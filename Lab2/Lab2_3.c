#include <stdio.h>
#include <stdarg.h>
#include <string.h>

typedef enum {
    ok,
    invalid_arguments,
    no,
    not_open_file,
    memory_not_allocated
} status_code;

void print_info (int line, int symbol) {
    printf("Line № %d symbol № %d\n", line, symbol);
}
status_code substr_search (char* substr, FILE** file) {
    char c, check_n;
    size_t len = strlen(substr);
    int num_str = 1, place = 0, i, no_occur = 1, flag = 0;

    while ((c = fgetc(*file)) != EOF && flag == 0) {
        if (check_n == '\n') {
            num_str++;
            place = 0;
        }
        i = 0;
        check_n = c;
        while (c == substr[i] && i < len) {
            c = fgetc(*file);
            i++;

            if (c == EOF) {
                flag = 1;
            }

        }

        if (i == len) {
            print_info(num_str, place);
            no_occur = 0;
        }

        if (i >= 1) {
            fseek(*file, -i, SEEK_CUR);
        }

        place++;

    }

    if (no_occur == 1) {
        return no;
    } else {
        return ok;
    }
}

status_code file_search (char* substr, ...) {
    va_list(ptr);
    va_start(ptr, substr);
    FILE* file;
    int file_num = 0;
    int no_occur = 1;

    while ((file = fopen(va_arg(ptr, char*), "r")) != NULL) {
        file_num++;
        printf("File № %d:\n", file_num);

        if (substr_search(substr, &file) == no) {
            printf("No occurrences\n");
        }
        no_occur = 1;

        fclose(file);
    }

    va_end(ptr);
    return ok;
}

int main () {
    //"c\na"
    file_search("\nabc", "C:\\VSCode\\files\\mp_2\\test1.txt", 
                "C:\\VSCode\\files\\mp_2\\test2.txt", "C:\\VSCode\\files\\mp_2\\test3.txt");
    return ok;
}
