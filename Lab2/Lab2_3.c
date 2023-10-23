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
    char c;
    size_t len = strlen(substr);
    int num_str = 1, place = 0, i, no_occur = 1; 

    while ((c = fgetc(*file)) != EOF) {
        i = 0;
        while (c == substr[i] && i < len) {
            c = fgetc(*file);
            i++;
        }

        if (i == len) {
            print_info(num_str, place);
            no_occur = 0;
            if (substr[0] == '\n') {
                num_str++;
                place = -1;
            }
        }

        if (substr[1] == '\n') {
            i--;
        }

        if (i > 1) {
            fseek(*file, -i + 1, SEEK_CUR);
            place++;
        } else if (i == 1) {
            fseek(*file, -1, SEEK_CUR);
        }

        if (c == '\n') {
            num_str++;
            place = 0;
        } else {
            place++;
        }

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
    file_search("c\na", "C:\\VSCode\\files\\mp_2\\test1.txt", 
                "C:\\VSCode\\files\\mp_2\\test2.txt", "C:\\VSCode\\files\\mp_2\\test3.txt");
    return ok;
}