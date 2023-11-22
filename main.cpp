#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "differ.h"

int main()
{
    Tree tree = {};
    Variables array = {};

    CtorRootAndVariebles(&tree, &array);

    BuildTreeFromFile("./file/defInf.txt", &tree, &array);

    int result = EvaluateExpression(tree.rootTree, &array);

    printf("%d\n", result);
    GenerateImage(&tree, &array);
    GenerateGraphImage();

    PrintTreeToFileWithoutBrackets(tree.rootTree, &array);
    PrintInFileInfForm(tree.rootTree, &array);
    PrintTreeLaTex(tree.rootTree, &array);

    TreeAndVarieblesDtor(&tree, &array);
    return 0;
}
