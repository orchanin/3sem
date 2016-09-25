 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GenerateString(int n, char* string);

int main (void)
{
    int n;
    scanf("%d", &n);
/*
 * Форматируйте код аккуратнее. Следующая строка должна быть такой:
 * char* string = (char*)malloc(sizeof(char) * (1 << (n + 1)));
 * Вокруг любого бинарного оператора ставьте пробелы. Лишних ставить не надо.
 */
    char* string = (char*)malloc(sizeof(char)* (1 << (n+1) ));
    GenerateString(n, string);
    free(string);
}

void GenerateString(int n, char* string)
{  
    int count = 1;
    int len;
    *string = 'a';  //string[0] = 'a'
    while (count != n + 1)
    {
        printf("%s\n", string);
        int len = (1 << count) - 1;
        
        string[len] = 'a' + count;

        strncpy(string + len + 1, string, len);
        string[2 * len + 1] = '\0';
        count++;
    } 

}
