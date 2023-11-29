#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "differentiation.h"
#include "log_funcs.h"
#include "print_tree.h"

void FuncFromFile(char* filename, Lines* text)
{
    Differ differ_before = {};
    Tree tree = {};
    Variables array = {};
    differ_before.tree = &tree;
    differ_before.variables = &array;

    CtorRootAndVariebles(&differ_before);

    BuildTreeFromFile(filename, &differ_before);
    GenerateImage(&differ_before);

    double result = 0;
    differ_before.variables->data[0].value = 3.14;
    differ_before.variables->data[1].value = 3.14;
    printf("%.2lf\n", differ_before.variables->data[1].value);
    result = EvaluateExpression(differ_before.tree->rootTree, differ_before.variables);

    printf("answer = %.2lf\n", result);
    PrintTreeLaTex("f(x) = ", differ_before.tree->rootTree, differ_before.variables, text);

    Differ differ_after = {};
    Tree treeDif = {};
    Variables arrayDif = {};
    differ_after.tree = &treeDif;
    differ_after.variables = &arrayDif;

    CtorRootAndVariebles(&differ_after);
    differ_after.variables = differ_before.variables;

    differ_after.tree->rootTree = Dif(differ_before.tree->rootTree);
    SetParentPointers(differ_after.tree->rootTree, NULL);
    GenerateImage(&differ_after);
    differ_after.tree->rootTree->parent = NULL;
    PrintTreeLaTex("f'(x) = ", differ_after.tree->rootTree, differ_before.variables, text);

    int changeCount = 0;
    do {
        changeCount = 0;
        TransformationNode(&differ_after.tree->rootTree, &changeCount, differ_after.variables, differ_after.tree, text);
        GenerateImage(&differ_after);
    } while (changeCount > 0);

    printf("%.2lf\n", differ_before.tree->rootTree->left->value);
    result = EvaluateExpression(differ_after.tree->rootTree, differ_after.variables);
    printf("answer = %.2lf\n", result);

    TreeAndVarieblesDtor(&differ_after);
    TreeAndVarieblesDtor(&differ_before);
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

    FuncFromFile("./file/defInf3.txt", &text);
    FuncFromFile("./file/defInf.txt", &text);
    FuncFromFile("./file/defInf1.txt", &text);
    FuncFromFile("./file/defInf2.txt", &text);
    return 0;
}
