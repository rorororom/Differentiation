#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

#include "readFromFileInBuffer.h"
#include "differ.h"

static void InitializeAkinatorNode(Node* node, int value, Node* left, Node* right);
static void PrintNodeDump(FILE* dotFile, Node* node, Variables* arrayVar, const char* fillColor);
static void DestroyNode(Node* node);

static int GetVariableIdByName(Variables* arrayVar, const char* variableName);
static int AddVariable(Variables* arrayVar, const char*  variableName, const int variableValue);

void CtorRootAndVariebles(Tree* tree, Variables* arrayVar)
{
    CREAT_NODE(newNode);
    newNode->type = 0;
    newNode->value= 0;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->flagDirection = 0;
    tree->rootTree = newNode;

    arrayVar->capacity = capacity;
    arrayVar->size = 0;
    arrayVar->data = (VariableData*)calloc(arrayVar->capacity, sizeof(VariableData));
}

void TreeAndVarieblesDtor(Tree* tree, Variables* arrayVar)
{
    assert(tree);
    DestroyNode(tree->rootTree);
    tree->rootTree = nullptr;

    for (size_t i = 0; i < arrayVar->capacity; ++i)
    {
        if (arrayVar->data->name) free(arrayVar->data->name);

        arrayVar->data[i].name = nullptr;
        arrayVar->data[i].value = 0;
    }

    free(arrayVar->data);
    arrayVar->data     = nullptr;
    arrayVar->size     = 0;
    arrayVar->capacity = 0;
}

static void DestroyNode(Node* node)
{
    assert(node);

    if (node->left != nullptr) {
        DestroyNode(node->left);
    }
    if (node->right != nullptr) {
        DestroyNode(node->right);
    }

    free(node);
}

void BuildTreeFromFile(const char* filename, Tree* tree, Variables* arrayVar)
{
    FILE* file = fopen(filename, "r");
    if (file == nullptr)
    {
        printf("Ошибка при открытии файла.\n");
    }

    Buffer array = {
                    NULL,
                    0,
    };

    GetFileSize(file, &array);
    array.buffer = (char*)calloc(array.size + 1, sizeof(char));
    ReadFileInBuffer(file, &array);
    FillText(&array);

    for (int i = 0; i < array.size; i++)
    {
        printf("%c", array.buffer[i]);
    }
    printf("\n");

    CREAT_NODE(newNode);
    newNode = ReadFromBufferInf(&array, NULL, arrayVar);
    tree->rootTree = newNode;

    if (tree->rootTree == NULL)
    {
        printf("Файл пустой или содержит только пробелы. Дерево не заполнено.\n");
    }
}


static void InitializeAkinatorNode(Node* node, int value, Node* left, Node* right)
{
    assert(node);

    node->value = value;
    node->left = left;
    node->right = right;
}

void SetNodeTypeAndValue(Node* node, char* value, Variables* arrayVar)
{
    if ('0' <= value[0] && value[0] <= '9')
    {
        int number = atoi(value);
        node->type = INT;
        node->value = number;
    }
    else if (strcmp(value, "div") == 0 || strcmp(value, "sub") == 0 || strcmp(value, "add") == 0 || strcmp(value, "mul") == 0)
    {
        node->type = OPERAT;
        if (strcmp(value, "div") == 0) node->value = DIV;
        else if (strcmp(value, "sub") == 0) node->value = SUB;
        else if (strcmp(value, "add") == 0) node->value = ADD;
        else if (strcmp(value, "mul") == 0) node->value = MUL;
    }
    else
    {
        printf("AAAAAA\n");
        if (GetVariableIdByName(arrayVar, value) == -1)
        {
            node->type = VARIEBL;
            node->value = AddVariable(arrayVar, value, 0);
        }
    }
}

static int AddVariable(Variables* arrayVar,
                              const char*  variableName,
                              const int variableValue)
{
    assert(arrayVar);
    assert(variableName);

    arrayVar->data[arrayVar->size].name  = strdup(variableName);
    printf("AAA = %s\n", arrayVar->data[arrayVar->size].name);

    assert(arrayVar->data[arrayVar->size].name);
    if (arrayVar->data[arrayVar->size].name == nullptr)
        return -1;

    arrayVar->data[arrayVar->size].value = variableValue;
    arrayVar->size++;

    return (int)arrayVar->size - 1;
}

static int GetVariableIdByName(Variables* arrayVar,
                               const char* variableName)
{
    assert(arrayVar);
    assert(variableName);

    for (size_t i = 0; i < arrayVar->size; ++i)
    {
        if (strcmp(arrayVar->data[i].name, variableName) == 0)
            return (int)i;
    }

    return -1;
}

void GenerateImage(Tree* tree, Variables* arrayVar)
{
    assert(tree);

    FILE* dotFile = fopen("grapth.dot", "w");
    if (dotFile)
    {
        fprintf(dotFile, "digraph tree {\n");
        fprintf(dotFile, "\tnode [shape=Mrecord, style=filled, fillcolor=\"#bba6cd\", color=\"#552d7b\"];\n");

        PrintNodeDump(dotFile, tree->rootTree, arrayVar, "#d5a1a7");

        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }
    else
    {
        printf("Ошибка при открытии файла graph.dot\n");
    }
}

static void PrintNodeDump(FILE* dotFile, Node* node, Variables* arrayVar, const char* fillColor)
{
    assert(dotFile);
    assert(node);
    assert(fillColor);

    if (node == NULL)
    {
        return;
    }

    if (node->type == INT)
    {
        fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\", color=\"#552d7b\",\
                          fillcolor=\"%s\", fontsize=14, label=\" %d \"];\n",
                          node, fillColor, node->value);
    }
    else if (node->type == OPERAT)
    {
        char operation = IssuesOperation(node);
        fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\", color=\"#552d7b\",\
                          fillcolor=\"%s\", fontsize=14, label=\" %c \"];\n",
                          node, fillColor, operation);
    }
    else if(node->type = VARIEBL)
    {
        char* value = strdup(arrayVar->data[node->value].name);
        fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\", color=\"#552d7b\",\
                          fillcolor=\"%s\", fontsize=14, label=\" %s \"];\n",
                          node, fillColor, value);
    }

    if (node->left != NULL)
    {
        fprintf(dotFile, "\t%d -> %d;\n", node, node->left);
        PrintNodeDump(dotFile, node->left, arrayVar, "#6495ed");
    }

    if (node->right != NULL)
    {
        fprintf(dotFile, "\t%d -> %d;\n", node, node->right);
        PrintNodeDump(dotFile, node->right, arrayVar, "#bba6cd");
    }
}

char IssuesOperation(Node* node)
{
    switch(node->value)
    {
        case DIV:
            return '/';
        case MUL:
            return '*';
        case ADD:
            return '+';
        case SUB:
            return '-';
        default:
            return '.';
    }
}

void GenerateGraphImage()
{
    char command[MAX_LEN] = "";
    sprintf(command, "dot -Tpng /Users/aleksandr/Desktop/differentiation/grapth.dot -o /Users/aleksandr/Desktop/differentiation/file.png");
    system(command);
}

int indexInBufer = 0;

// Node* ReadFromBuffer(Buffer* array, Node* currentNode)
// {
//     char* token = GetWord(array);
//     if (strcmp(token, "(") == 0)
//     {
//         CREAT_NODE(newNode);
//         token = GetWord(array);
//         SetNodeTypeAndValue(newNode, token);
//
//         newNode->parent = currentNode;
//
//         newNode->left = ReadFromBuffer(array, newNode);
//         newNode->right = ReadFromBuffer(array, newNode);
//         token = GetWord(array);
//         return newNode;
//     }
//     else if (strcmp(token, "nil") == 0)
//         return NULL;
//
//     return NULL;
// }

char* FromOperationToWord(int operation)
{
    switch(operation)
    {
        case DIV:
            return "div";
        case MUL:
            return "mul";
        case ADD:
            return "add";
        case SUB:
            return "sub";
    }
}

char* GetWord(Buffer* array)
{
    while (indexInBufer < array->size && isspace(array->buffer[indexInBufer])) {
        indexInBufer++;
    }
    int cnt = 0;
    while (indexInBufer+cnt < array->size && !isspace(array->buffer[indexInBufer + cnt])) {
        cnt++;
    }

    char* token = (char*)malloc(cnt + 1);
    memcpy(token, array->buffer + indexInBufer, cnt);
    token[cnt] = '\0';

    indexInBufer = indexInBufer + cnt;

    return token;
}

int EvaluateExpression(struct Node* node, Variables* arrayVar)
{
    assert(node);

    if (node->type == INT) {
        return node->value;
    }
    else if (node->type == VARIEBL)
    {
        return arrayVar->data[node->value].value;
    }

    int leftValue = EvaluateExpression(node->left, arrayVar);
    int rightValue = EvaluateExpression(node->right, arrayVar);

    switch (node->value) {
        case ADD:
            return leftValue + rightValue;
        case SUB:
            return leftValue - rightValue;
        case MUL:
            return leftValue * rightValue;
        case DIV:
            return leftValue / rightValue;
        default:
            printf("Некорректная операция\n");
            return 0;
    }
}

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
    if (node->type == INT || node->type == VARIEBL)
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
    if (node->type == VARIEBL)
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
    char operation = IssuesOperation(node);

    fprintf(file, "%c ", operation);

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

    char operation = IssuesOperation(node);

    node->flagDirection = RIGHT;
    fprintf(file, "%c ", operation);
    PrintNode(node->right, file, arrayVar);
}

int CheckingPriorityOperation(int operation)
{
    if (operation == ADD || operation == SUB) return 0;
    if (operation == MUL || operation == DIV) return 1;
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
    else if (node->type == VARIEBL)
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

Node* ReadFromBufferInf(Buffer* array, Node* currentNode, Variables* arrayVar)
{
    char* token = GetWord(array);

    if (strcmp(token, "(") == 0)
    {
        CREAT_NODE(newNode);
        newNode->left = ReadFromBufferInf(array, newNode, arrayVar);

        token = GetWord(array);
        SetNodeTypeAndValue(newNode, token, arrayVar);

        newNode->parent = currentNode;

        newNode->right = ReadFromBufferInf(array, newNode, arrayVar);
        token = GetWord(array);
        return newNode;
    }
    else if (strcmp(token, "nil") == 0)
    {
        return NULL;
    }
}

void PrintTreeLaTex(Node* node, Variables* arrayVar)
{
    assert(node);

    FILE* file = fopen("./file/TEX.txt", "w");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    PrintNodeTex(node, file, arrayVar);
    fclose(file);

}

void PrintParentNorNullTex(Node* node, FILE* file, Variables* arrayVar)
{
    int operationParent = CheckingPriorityOperation(node->parent->value);
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
    else
    {
        node->flagDirection = LEFT;
        PrintNodeTex(node->left, file, arrayVar);
        char operation = IssuesOperation(node);

        fprintf(file, "%c ", operation);

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
    if (node->type == INT || node->type == VARIEBL)
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
    else
    {
        node->flagDirection = LEFT;
        PrintNodeTex(node->left, file, arrayVar);

        char operation = IssuesOperation(node);

        node->flagDirection = RIGHT;
        fprintf(file, "%c ", operation);
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
    else if (node->type == VARIEBL)
    {
        char* nameVar = strdup(arrayVar->data[node->value].name);
        fprintf(file, "%s ", nameVar);
    }
    PrintNodeTex(node->right, file, arrayVar);
}
