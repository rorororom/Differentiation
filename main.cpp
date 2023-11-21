#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "differ.h"

int main()
{
    Tree tree = {};
    CtorRoot(&tree);

    BuildTreeFromFile("defPref.txt", &tree);

    int result = EvaluateExpression(tree.rootTree);

    printf("%d\n", result);
    GenerateImage(&tree);
    GenerateGraphImage();

    PrintTreeToFile(tree.rootTree);
    return 0;
}
