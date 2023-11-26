#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "print_tree.h"
#include "differentiation.h"

void PrintTreeToFileWithoutBrackets(Node* node, Variables* arrayVar)
{
    assert(node);

    FILE* file = fopen("./file/fileOutput.txt", "w");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    PrintNode(node, file, arrayVar);
    fclose(file);
}

void PrintNode(Node* node, FILE* file, Variables* arrayVar)
{
    if (node == NULL)
    {
        return;
    }
    if (node->type == INT || node->type == VAR)
    {
        PrintIntNode(node, file, arrayVar);
    }
    else
    {
        if (node->parent != NULL)
        {
            PrintParentNorNull(node, file, arrayVar);
        }
        else
        {
            PrintParentNull(node, file, arrayVar);
        }
    }
}

void PrintIntNode(Node* node, FILE* file, Variables* arrayVar)
{
    PrintNode(node->left, file, arrayVar);
    if (node->type ==VAR)
    {
        char* nameValue = strdup(arrayVar->data[node->value].name);
        fprintf(file, "%s ", nameValue);
    }
    else
    {
        fprintf(file, "%d ", node->value);
    }
    PrintNode(node->right, file, arrayVar);
}

void PrintParentNorNull(Node* node, FILE* file, Variables* arrayVar)
{
    int operationParent = CheckingPriorityOperation(node->parent->value);
    int operationNowNode = CheckingPriorityOperation(node->value);

    if (operationParent > operationNowNode ||
        ((node->parent->flagDirection == RIGHT) && (operationParent == operationNowNode) && (node->value == DIV)))
    {
        fprintf(file, "( ");
    }
    else if ((node->parent->flagDirection == RIGHT) && (operationParent == operationNowNode) && (node->value == SUB))
    {
        node->value = ADD;
    }

    node->flagDirection = LEFT;
    PrintNode(node->left, file, arrayVar);
    char* operation = IssuesOperation(node);

    fprintf(file, "%s ", operation);

    node->flagDirection = RIGHT;
    PrintNode(node->right, file, arrayVar);

    if (operationParent> operationNowNode ||
        ((node->parent->flagDirection == RIGHT) && (operationParent == operationNowNode) && (node->value == DIV)))
    {
        fprintf(file, ") ");
    }
}

void PrintParentNull(Node* node, FILE* file, Variables* arrayVar)
{
    node->flagDirection = LEFT;
    PrintNode(node->left, file, arrayVar);

    char* operation = IssuesOperation(node);

    node->flagDirection = RIGHT;
    fprintf(file, "%s ", operation);
    PrintNode(node->right, file, arrayVar);
}

void PrintInFileInfForm(Node* node, Variables* arrayVar)
{
    assert(node);

    FILE* file = fopen("./file/defInf.txt", "w");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    PrintNodeInfForm(node, file, arrayVar);
    fclose(file);
}

void PrintNodeInfForm(Node* node, FILE* file, Variables* arrayVar)
{
    assert(file);

    if (node == NULL)
    {
        fprintf(file, "nil ");
        return;
    }

    fprintf(file, "( ");

    PrintNodeInfForm(node->left, file, arrayVar);

    if (node->type == INT)
    {
        fprintf(file, "%d ", node->value);
    }
    else if (node->type ==VAR)
    {
        char* operation = strdup(arrayVar->data[node->value].name);
        fprintf(file, "%s ", operation);
    }
    else
    {
        char* operation = FromOperationToWord(node->value);
        fprintf(file, "%s ", operation);
    }

    PrintNodeInfForm(node->right, file, arrayVar);
    fprintf(file, ") ");
}

void PrintTreeLaTex(Node* node, Variables* arrayVar, Lines* text)
{
    assert(node);

    FILE* file = fopen("./file/tex.md", "a");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    int min_value = 0;
    int max_value = 4;
    int random_number = GenerateRandomNumber(min_value, max_value);
    fprintf(file, "$\\text{%s}$\n", text->text[random_number]);

    fprintf(file, "$$ ");
    PrintNodeTex(node, file, arrayVar);
    fprintf(file, "$$\n");

    fclose(file);
}


void PrintParentNorNullTex(Node* node, FILE* file, Variables* arrayVar)
{
    int operationParent = 0;
    if (node->parent != NULL)
    {
        operationParent = CheckingPriorityOperation(node->parent->value) ;
    }
    //int operationParent = (node->parent != NULL) ? CheckingPriorityOperation(node->parent->value) ;

    int operationNowNode = CheckingPriorityOperation(node->value);

    if ((node->parent->flagDirection == RIGHT) && (operationParent == operationNowNode) && (node->value == SUB))
    {
        node->value = ADD;
    }

    if (node->value == DIV)
    {
        fprintf(file, "\\frac ");

        fprintf(file, "{ ");
        PrintNodeTex(node->left, file, arrayVar);
        fprintf(file, "} ");

        fprintf(file, "{ ");
        PrintNodeTex(node->right, file, arrayVar);
        fprintf(file, "} ");
    }
    else if (node->value == SIN || node->value == COS)
    {
        char* operation = IssuesOperation(node);
        fprintf(file, "%s( ", operation);

        PrintNodeTex(node->left, file, arrayVar);
        fprintf(file, ") ");
    }
    else if (node->value == POW)
    {
        PrintNodeTex(node->left, file, arrayVar);
        char* operation = IssuesOperation(node);
        fprintf(file, "%s ", operation);
        fprintf(file, "{ ");
        PrintNodeTex(node->right, file, arrayVar);
        fprintf(file, "} ");
    }
    else
    {
        node->flagDirection = LEFT;
        PrintNodeTex(node->left, file, arrayVar);

        if (node->value == MUL)
        {
            fprintf(file, "\\cdot ");
        }
        else
        {
            char* operation = IssuesOperation(node);
            fprintf(file, "%s ", operation);
        }

        node->flagDirection = RIGHT;
        PrintNodeTex(node->right, file, arrayVar);
    }
}

void PrintNodeTex(Node* node, FILE* file, Variables* arrayVar)
{
    if (node == NULL)
    {
        return;
    }
    if (node->type == INT || node->type ==VAR)
    {
        PrintIntNodeTex(node, file, arrayVar);
    }
    else
    {
        if (node->parent != NULL)
        {
            PrintParentNorNullTex(node, file, arrayVar);
        }
        else
        {
            PrintParentNullTex(node, file, arrayVar);
        }
    }
}

void PrintParentNullTex(Node* node, FILE* file, Variables* arrayVar)
{
    if (node->value == DIV)
    {
        fprintf(file, "\\frac ");

        fprintf(file, "{ ");
        node->flagDirection = LEFT;
        PrintNodeTex(node->left, file, arrayVar);
        fprintf(file, "} ");

        fprintf(file, "{ ");
        node->flagDirection = RIGHT;
        PrintNodeTex(node->right, file, arrayVar);
        fprintf(file, "} ");
    }
    else if (node->value == POW)
    {
        PrintNodeTex(node->left, file, arrayVar);
        char* operation = IssuesOperation(node);
        fprintf(file, "%s( ", operation);
        fprintf(file, "{ ");
        PrintNodeTex(node->right, file, arrayVar);
        fprintf(file, "} ");
    }
    else if (node->value == SIN || node->value == COS)
    {
        char* operation = IssuesOperation(node);
        fprintf(file, "%s ", operation);

        PrintNodeTex(node->left, file, arrayVar);
        fprintf(file, ") ");
    }
    else
    {
        node->flagDirection = LEFT;
        PrintNodeTex(node->left, file, arrayVar);

        if (node->value == MUL)
        {
            fprintf(file, "\\cdot ");
        }
        else
        {
            char* operation = IssuesOperation(node);
            fprintf(file, "%s ", operation);
        }

        node->flagDirection = RIGHT;
        PrintNodeTex(node->right, file, arrayVar);
    }
}


void PrintIntNodeTex(Node* node, FILE* file, Variables* arrayVar)
{
    PrintNodeTex(node->left, file, arrayVar);
    if (node->type == INT)
    {
        fprintf(file, "%d ", node->value);
    }
    else if (node->type ==VAR)
    {
        char* nameVar = strdup(arrayVar->data[node->value].name);
        fprintf(file, "%s ", nameVar);
    }
    PrintNodeTex(node->right, file, arrayVar);
}
