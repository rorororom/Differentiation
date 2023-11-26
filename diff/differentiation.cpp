#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "../common/read_from_file_in_buffer.h"
#include "differentiation.h"
#include "../common/log_funcs.h"
#include "print_tree.h"

#define _ADD(left, right) NewNode(OPERAT, ADD, left, right)
#define _MUL(left, right) NewNode(OPERAT, MUL, left, right)
#define _SUB(left, right) NewNode(OPERAT, SUB, left, right)
#define _DIV(left, right) NewNode(OPERAT, DIV, left, right)
#define _IN(left, right) NewNode(OPERAT, IN, left, right)
#define _POW(base, exponent) NewNode(OPERAT, POW, base, exponent)
#define _COS(expr) NewNode(OPERAT, COS, expr, NULL)
#define _SIN(expr) NewNode(OPERAT, SIN, expr, NULL)
#define _NEG(expr) NewNode(OPERAT, NEG, expr, NULL)
#define COPYL Copy(nowNode->left)
#define COPYR Copy(nowNode->right)
#define DIFL  Dif(nowNode->left)
#define DIFR  Dif(nowNode->right)

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
    else if (value[0] == '-')
    {
        int number = atoi(value + 1);
        node->type = INT;
        node->value = -1 * number;
    }
    else if (strcmp(value, "div") == 0 || strcmp(value, "sub") == 0 || strcmp(value, "add") == 0 || strcmp(value, "mul") == 0 || strcmp(value, "pow") == 0 || strcmp(value, "sin") == 0 || strcmp(value, "cos") == 0)
    {
        node->type = OPERAT;
        if (strcmp(value, "div") == 0) node->value = DIV;
        else if (strcmp(value, "sub") == 0) node->value = SUB;
        else if (strcmp(value, "add") == 0) node->value = ADD;
        else if (strcmp(value, "mul") == 0) node->value = MUL;
        else if (strcmp(value, "pow") == 0) node->value = POW;
        else if (strcmp(value, "sin") == 0) node->value = SIN;
        else if (strcmp(value, "cos") == 0) node->value = COS;
    }
    else
    {
        int answer = GetVariableIdByName(arrayVar, value);
        if (answer == -1)
        {
            node->type =VAR;
            node->value = AddVariable(arrayVar, value, 0);
        }
        else
        {
            node->type =VAR;
            node->value = answer;
        }
    }
}

static int AddVariable(Variables* arrayVar, const char*  variableName, const int variableValue)
{
    assert(arrayVar);
    assert(variableName);

    arrayVar->data[arrayVar->size].name  = strdup(variableName);

    if (arrayVar->data[arrayVar->size].name == nullptr)
        return -1;

    arrayVar->data[arrayVar->size].value = variableValue;
    arrayVar->size++;

    return arrayVar->size - 1;
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

    CreateNewGraph();
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
        char* operation = IssuesOperation(node);
        fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\", color=\" #0000ff \",\
                          fillcolor=\"%s\", fontsize=14, label=\" %s \"];\n",
                          node, fillColor, operation);
    }
    else if(node->type == VAR)
    {
        char* value = strdup(arrayVar->data[node->value].name);
        fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\", color=\"#FF0000\",\
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

char* IssuesOperation(Node* node)
{
    switch(node->value)
    {
        case DIV:
            return "/";
        case MUL:
            return "*";
        case ADD:
            return "+";
        case SUB:
            return "-";
        case POW:
            return "^";
        case SIN:
            return "sin";
        case COS:
            return "cos";
        default:
            return ".";
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
        case POW:
            return "pow";
        case SIN:
            return "sin";
        case COS:
            return "cos";
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

int EvaluateExpression(Node* node, Variables* arrayVar)
{
    assert(node);

    if (node->type == INT) {
        return node->value;
    }
    else if (node->type ==VAR)
    {
        return arrayVar->data[node->value].value;
    }

    int rightValue = 0;
    int leftValue = 0;
    if (node->value == SIN || node->value == COS)
    {
        leftValue = EvaluateExpression(node->left, arrayVar);
    }
    else
    {
        leftValue = EvaluateExpression(node->left, arrayVar);
        rightValue = EvaluateExpression(node->right, arrayVar);
    }

    switch (node->value) {
        case ADD:
            return leftValue + rightValue;
        case SUB:
            return leftValue - rightValue;
        case MUL:
            return leftValue * rightValue;
        case DIV:
            return leftValue / rightValue;
        case SIN:
            return sin(leftValue);
        case COS:
            return cos(leftValue);
        case POW:
        {
            for (int i = 0; i < rightValue - 1; i++)
            {
                leftValue *= leftValue;
            }
            return leftValue;
        }
        default:
            printf("Некорректная операция\n");
            return 0;
    }
}

int CheckingPriorityOperation(int operation)
{
    if (operation == ADD || operation == SUB) return 0;
    if (operation == MUL || operation == DIV || operation == POW) return 1;
    if (operation == SIN || operation == COS) return 2;
    else return -100;
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

Node* Dif(Node* nowNode)
{
    if (nowNode->type == INT)
        return NewNode(INT, 0, NULL, NULL);
    if (nowNode->type == VAR)
        return NewNode(INT, 1, NULL, NULL);

    Node* leftDiff = nowNode->left ? Dif(nowNode->left) : NULL;
    Node* rightDiff = nowNode->right ? Dif(nowNode->right) : NULL;
    switch(nowNode->value)
    {
        case ADD:
            return _ADD(DIFL, DIFR);
        case SUB:
            return _SUB(DIFL, DIFR);
        case MUL:
            return _ADD(_MUL(DIFL, COPYR), _MUL(COPYL, DIFR));
        case POW:
            return _MUL(_MUL(COPYR, _POW(COPYL, _SUB(COPYR, NewNode(INT, 1, NULL, NULL)))), DIFL);
        case DIV:
            return _DIV(_SUB(_MUL(DIFL, COPYR), _MUL(COPYL, DIFR)), _POW(COPYR, NewNode(INT, 2, NULL, NULL)));
        case SIN:
            return _MUL(_COS(COPYL), DIFL);
        case COS:
            return _MUL(_SIN(COPYL), NewNode(INT, -1, NULL, NULL));
    }
}

Node* NewNode(int type, int value, Node* left, Node* right)
{
    CREAT_NODE(node);
    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

Node* Copy(Node* nowNode)
{
    if (nowNode == NULL) {
        return NULL;
    }

    CREAT_NODE(newNode);
    newNode->type = nowNode->type;
    newNode->value = nowNode->value;
    newNode->left = Copy(nowNode->left);
    newNode->right = Copy(nowNode->right);

    return newNode;
}

void TransformationNode(Node** nowNode, int* changeCount, Variables* arrayVar, Tree* treeDif, Lines* text)
{
    if (*nowNode == NULL)
        return;
    if ((*nowNode)->type == OPERAT)
    {
        if ((*nowNode)->right == NULL)
        {
            if (((*nowNode)->left->value == 1 && ((*nowNode)->value == MUL || (*nowNode)->value == DIV)) || ((*nowNode)->left->value == 0 && ((*nowNode)->value == ADD || (*nowNode)->value == SUB)))
            {
                (*nowNode)->right->parent = (*nowNode)->parent;
                *nowNode = (*nowNode)->right;
                (*changeCount)++;
                PrintTreeLaTex(treeDif->rootTree, arrayVar, text);
            }
            else if ((*nowNode)->left->value == 0 && (*nowNode)->value == MUL)
            {
                CREAT_NODE(newNode);
                newNode->value = 0;
                newNode->type = INT;
                newNode->left = NULL;
                newNode->right = NULL;
                newNode->parent = (*nowNode)->parent;
                *nowNode = newNode;
                (*changeCount)++;
                PrintTreeLaTex(treeDif->rootTree, arrayVar, text);
            }
            return;
        }
        if (((*nowNode)->left->type == INT && (*nowNode)->right->type == INT))
        {
            CREAT_NODE(newNode);
            int answer = EvaluateExpression(*nowNode, NULL);
            newNode->value = answer;
            newNode->type = INT;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->parent = (*nowNode)->parent;
            *nowNode = newNode;
            (*changeCount)++;
            CreateNewGraph();
            PrintTreeLaTex(treeDif->rootTree, arrayVar, text);
        }
        else if ((*nowNode)->left->type == INT || (*nowNode)->right->type == INT )
        {
            if (((*nowNode)->left->value == 1 && ((*nowNode)->value == MUL || (*nowNode)->value == DIV)) || ((*nowNode)->left->value == 0 && ((*nowNode)->value == ADD || (*nowNode)->value == SUB)))
            {
                (*nowNode)->right->parent = (*nowNode)->parent;
                *nowNode = (*nowNode)->right;
                (*changeCount)++;
                PrintTreeLaTex(treeDif->rootTree, arrayVar, text);
            }
            else if (((*nowNode)->right->value == 1 && ((*nowNode)->value == MUL || (*nowNode)->value == DIV)) || ((*nowNode)->right->value == 0 && ((*nowNode)->value == ADD || (*nowNode)->value == SUB)))
            {
                (*nowNode)->left->parent = (*nowNode)->parent;
                *nowNode = (*nowNode)->left;
                (*changeCount)++;
                PrintTreeLaTex(treeDif->rootTree, arrayVar, text);
            }
            else if ((((*nowNode)->right->value == 0)  || (*nowNode)->left->value == 0) && (*nowNode)->value == MUL)
            {
                CREAT_NODE(newNode);
                newNode->value = 0;
                newNode->type = INT;
                newNode->left = NULL;
                newNode->right = NULL;
                newNode->parent = (*nowNode)->parent;
                *nowNode = newNode;
                (*changeCount)++;
                PrintTreeLaTex(treeDif->rootTree, arrayVar, text);
            }
        }
    }
    TransformationNode(&(*nowNode)->left, changeCount, arrayVar, treeDif, text);
    TransformationNode(&(*nowNode)->right, changeCount, arrayVar, treeDif, text);
}

static int imageCounter = 0;

void CreateNewGraph()
{
    char filename[100];
    sprintf(filename, "grath_%04d.png", imageCounter);

    char command[1000];
    sprintf(command, "dot -Tpng /Users/aleksandr/Desktop/differentiation/grapth.dot -o /Users/aleksandr/Desktop/differentiation/grapth/%s", filename);
    system(command);

    char path[100] = "/Users/aleksandr/Desktop/differentiation/grapth/";

    imageCounter++;

    fprintf(LOG_FILE, "<div style=\"display: block; margin-bottom: 20px;\">");
    fprintf(LOG_FILE, "<img src=\"%s%s\" alt=\"Graph Image\">", path, filename);
    fprintf(LOG_FILE, "</div>\n");
}

void SetParentPointers(Node* node, Node* parent)
{
    if (node == NULL) {
        return;
    }

    node->parent = parent;

    SetParentPointers(node->left, node);
    SetParentPointers(node->right, node);
}

void ClearFile(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        fclose(file);
    }
}

int GenerateRandomNumber(int min, int max)
{
    srand((unsigned int)time(NULL));

    return min + rand() % (max - min + 1);
}
