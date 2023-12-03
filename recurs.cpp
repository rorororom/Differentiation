#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "recurs.h"
#include "read_from_file_in_buffer.h"
#include "differentiation.h"
#include "print_tree.h"

char* s = NULL;
int p = 0;
Variables arrayVar;
int size = 0;

Node* GetG(const char* str)
{
    s = strdup(str);
    p = 0;
    Node* node = GetE();
    if (s[p] != '\0') printf("ошибкаG\n");
    return node;
}

Node* GetE()
{

    Node* node = GetT();
    while (s[p] == '+' || s[p] == '-')
    {
        int old_p = p;
        char op = s[p];
        p++;
        Node* node2 = GetT();

        CREAT_NODE(operationNode);
        switch (s[old_p]) {
            case '+':
                InitializeNode(operationNode, OPERAT, ADD, NULL, NULL, NULL);
                break;
            case '-':
                InitializeNode(operationNode, OPERAT, SUB, NULL, NULL, NULL);
                break;
            default:
                printf("ошибкаE\n");
        }

        operationNode->left = node;
        operationNode->right = node2;

        node = operationNode;
    }
    return node;
}

Node* GetT()
{
    Node* node = GetP();

    while (s[p] == '*' || s[p] == '/' || s[p] == '^')
    {
        int old_p = p;
        char op = s[p];
        p++;
        Node* node2 = GetP();

        CREAT_NODE(operationNode);
        switch (s[old_p]) {
            case '*':
                InitializeNode(operationNode, OPERAT, MUL, NULL, NULL, NULL);
                break;
            case '/':
                InitializeNode(operationNode, OPERAT, DIV, NULL, NULL, NULL);
                break;
            case '^':
                InitializeNode(operationNode, OPERAT, POW, NULL, NULL, NULL);
                break;
            default:
                printf("ошибкаP\n");
        }

        operationNode->left = node;
        operationNode->right = node2;
        node = operationNode;
    }
    return node;
}

Node* GetP()
{
    Node* node = NULL;

    if (s[p] == '(')
    {
        p++;
        node = GetE();
        while (s[p] == ' ') {
            p++;
        }
        if (s[p] != ')') printf("ошибкаP\n");
        p++;
    }
    else if ('a' <= s[p] && s[p] <= 'z')
    {
        node = GetO();
        if (node->type == OPERAT)
            node->left = GetP();
    }
    else
    {
        node = GetN();
    }
    return node;
}

Node* GetN()
{
    int old_p = p;
    int val = 0;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + s[p] - '0';
        p++;
        if (p <= old_p) printf("ошибкаN\n");
    }

    CREAT_NODE(node);
    InitializeNode(node, INT, val, NULL, NULL, NULL);

    return node;
}

#define SET_OPERATOR(op, OP, ...)                           \
    if (5 <= OP && OP <= 13 && strcmp(token, op) == 0)      \
    {                                                       \
        InitializeNode(node, OPERAT, OP, NULL, NULL, NULL); \
        return node;                                        \
    }

Node* GetO()
{
    char token[OP_LEN] = "";
    int i = 0;
    while ('a' <= s[p] && s[p] <= 'z')
    {
        token[i] = s[p];
        i++;
        p++;
    }

    CREAT_NODE(node);

    #include "operation.dsl"
    #undef SET_OPERATOR

    int val = AddVariable(&arrayVar, token, 0);
    InitializeNode(node, VAR, val, NULL, NULL, NULL);
    return node;
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

    size = array.size;
    arrayVar.capacity = CAPACITY;
    arrayVar.size = ZERO;
    arrayVar.data = (VariableData*)calloc(arrayVar.capacity, sizeof(VariableData));

    Node* node = GetG(array.buffer);
    Differ differ_before = {};
    Tree tree = {};
    Variables arrayV = {};
    differ_before.tree = &tree;
    differ_before.variables = &arrayV;

    CtorRootAndVariebles(&differ_before);
    differ_before.tree->rootTree = node;
    differ_before.variables = &arrayVar;
    GenerateImage(&differ_before);
    PrintTreeToFileWithoutBrackets(node, &arrayVar);

    double result = EvaluateExpression(differ_before.tree->rootTree, differ_before.variables);
    printf("%lg\n", result);
}
