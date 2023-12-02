#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "differentiation.h"
#include "log_funcs.h"
#include "print_tree.h"

char* s = NULL;
int p = 0;

int GetG(const char* str);
int GetE();
int GetN();
int GetT();
int GetP();

void TransformAndEvaluate(Differ* differ, Lines* text)
{
    int changeCount = 0;
    do {
        changeCount = 0;
        TransformationNode(&differ->tree->rootTree, &changeCount, differ->variables, differ->tree, text);
        GenerateImage(differ);
    } while (changeCount > 0);

    double result = EvaluateExpression(differ->tree->rootTree, differ->variables);
    printf("answer = %.2lf\n", result);
}

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

void FuncFromFile(char* filename, Lines* text)
{
    Differ differ_before = {};
    Tree tree = {};
    Variables array = {};
    differ_before.tree = &tree;
    differ_before.variables = &array;

    CtorRootAndVariebles(&differ_before);

    BuildTREEEE(filename);
    // GenerateImage(&differ_before);

//     double result = 0;
//     differ_before.variables->data[0].value = 2;
//     result = EvaluateExpression(differ_before.tree->rootTree, differ_before.variables);
//
//     printf("answer = %.2lf\n", result);
//     PrintTreeLaTex("f(x) = ", differ_before.tree->rootTree, differ_before.variables, text);
//
//     Differ differ_after = {};
//     Tree treeDif = {};
//     Variables arrayDif = {};
//     differ_after.tree = &treeDif;
//     differ_after.variables = &arrayDif;
//
//     CtorRootAndVariebles(&differ_after);
//     differ_after.variables = differ_before.variables;
//
//     differ_after.tree->rootTree = Dif(differ_before.tree->rootTree);
//     SetParentPointers(differ_after.tree->rootTree, NULL);
//     GenerateImage(&differ_after);
//     differ_after.tree->rootTree->parent = NULL;
//     PrintTreeLaTex("f'(x) = ", differ_after.tree->rootTree, differ_before.variables, text);
//
//     TransformAndEvaluate(&differ_after, text);
//
//     TreeAndVarieblesDtor(&differ_after);
//     TreeAndVarieblesDtor(&differ_before);
}

int main()
{
    OpenLogFile("LOGE1111.html", "w");
    ClearFile("./file/tex.md");
    PrintStartProekt();
    srand(time(NULL));

    Lines text = {};
    Buffer phrasesText = {};
    ProcessFile(&phrasesText, &text);

    DifferOperat(&text);

    // FuncFromFile("./file/defInf3.txt", &text);
    // FuncFromFile("./file/defInf.txt", &text);
    // FuncFromFile("./file/defInf1.txt", &text);
    FuncFromFile("./file/defInf4.txt", &text);



    return 0;
}


