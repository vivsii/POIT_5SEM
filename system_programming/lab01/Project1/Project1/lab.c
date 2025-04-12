#include <stdio.h>
#include <locale.h>

int Parse_array(const char* s, char t, char* before, char* after) {
    if (s == NULL || before == NULL || after == NULL) {
        return -1;
    }

    int i = 0, j = 0;
    int found = 0;

    while (s[i] != '\0') {
        if (s[i] == t) {
            found = 1;
            i++; 
            break;
        }
        before[j++] = s[i++];
    }

    before[j] = '\0'; 

    if (found) {
        j = 0;
        while (s[i] != '\0') {
            after[j++] = s[i++];
        }
    }
    after[j] = '\0'; 

    return found ? 0 : -1; 
}

int Parse_pointer(const char* s, char t, char* before, char* after) {
    if (s == NULL || before == NULL || after == NULL) {
        return -1; 
    }

    const char* p = s;
    char* b = before;
    char* a = after;

    while (*p != '\0') {
        if (*p == t) {
            p++; 
            break;
        }
        *b++ = *p++;
    }

    *b = '\0'; 

    while (*p != '\0') {
        *a++ = *p++;
    }

    *a = '\0'; 

    return (p != s) ? 0 : -1; 
}

void test_parse() {
    const char s[] = "Hello,World!";
    char t = ',';
    char before[100], after[100];

    printf("Тест функции Parse_array:\n");
    int result_array = Parse_array(s, t, before, after);
    if (result_array == 0) {
        printf("До символа '%c': %s\n", t, before);
        printf("После символа '%c': %s\n", t, after);
    }
    else {
        printf("Символ '%c' не найден.\n", t);
    }

    printf("\nТест функции Parse_pointer:\n");
    int result_pointer = Parse_pointer(s, t, before, after);
    if (result_pointer == 0) {
        printf("До символа '%c': %s\n", t, before);
        printf("После символа '%c': %s\n", t, after);
    }
    else {
        printf("Символ '%c' не найден.\n", t);
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    test_parse();
    return 0;
}
