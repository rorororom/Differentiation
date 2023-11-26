#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "differentiation.h"
#include "log_funcs.h"
#include "print_tree.h"

int main()
{
    OpenLogFile("LOGE1111.html", "w");

    Tree tree = {};
    Variables array = {};

    CtorRootAndVariebles(&tree, &array);

    BuildTreeFromFile("./file/defInf.txt", &tree, &array);
    ClearFile("./file/tex.md");

    Lines text = {};
    Buffer phrasesText = {};
    ProcessFile(&phrasesText, &text);

    GenerateImage(&tree, &array);

    int result = EvaluateExpression(tree.rootTree, &array);

    printf("%d\n", result);
    GenerateImage(&tree, &array);

    PrintTreeToFileWithoutBrackets(tree.rootTree, &array);
    PrintInFileInfForm(tree.rootTree, &array);
    PrintTreeLaTex(tree.rootTree, &array, &text);

    Tree treeDif = {};
    Variables arrayDif = {};

    CtorRootAndVariebles(&treeDif, &arrayDif);
    treeDif.rootTree = Dif(tree.rootTree);
    SetParentPointers(treeDif.rootTree, NULL);
    GenerateImage(&treeDif, &array);
    treeDif.rootTree->parent = NULL;
    PrintTreeLaTex(treeDif.rootTree, &array, &text);

    GenerateImage(&treeDif, &array);

    int changeCount = 0;
    do {
        changeCount = 0;
        TransformationNode(&treeDif.rootTree, &changeCount, &array, &treeDif, &text);
        GenerateImage(&treeDif, &array);
    } while (changeCount > 0);

    result = EvaluateExpression(treeDif.rootTree, &array);

    printf("! = %d\n", result);

    TreeAndVarieblesDtor(&tree, &array);
    return 0;
}
