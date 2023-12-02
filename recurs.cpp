#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "recurs.h"
#include "read_from_file_in_buffer.h"

int GetG(const char* str)
{
    s = strdup(str);
    p = 0;
    int val =  GetE();
    if (s[p] != '\0') printf("ошибка\n");
    return val;
}

int GetE()
{
    int val = GetT();
    while (s[p] == '+' || s[p] == '-')
    {
        char op = s[p];
        p++;
        int val2 = GetT();
        switch(op)
        {
            case '+' : val += val2; break;
            case '-' : val -= val2; break;
            default: printf("ошибка");
        }
    }
    return val;
}

int GetT()
{
    int val = GetP();
    while (s[p] == '*' || s[p] == '/')
    {
        char op = s[p];
        p++;
        int val2 = GetP();
        switch(op)
        {
            case '*' : val *= val2; break;
            case '/' :
                if (val2 != 0) {
                    val /= val2;
                } else {
                    printf("ошибка: деление на ноль\n");
                }
                break;
            default: printf("ошибка");
        }
    }
    return val;
}

int GetP()
{
    int val = 0;
    if (s[p] == '(')
    {
        p++;
        val = GetE();
        if (s[p] != ')') printf("ошибка\n");
        p++;
        return val;
    }
    else
    {
        return GetN();
    }
}

int GetN()
{
    int old_p = p;
    int val = 0;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + s[p] - '0';
        p++;
        if (p <= old_p) printf("ошибка\n");
    }
    return val;
}

void BuildTREEEE(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == nullptr)
    {
        printf("Ошибка при открытии файла.\n");
    }

    Buffer array = {NULL, 0};

    GetFileSize(file, &array);
    array.buffer = (char*)calloc(array.size + 1, sizeof(char));
    ReadFileInBuffer(file, &array);
    FillText(&array);
    int val = GetG(array.buffer);
    printf("%d\n", val);
}
