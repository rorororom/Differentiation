#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

#include "differ.h"

static void InitializeAkinatorNode(Node* node, int value, Node* left, Node* right);
static void PrintNodeDump(FILE* dotFile, Node* node, const char* fillColor);

void CtorRoot(Tree* tree)
{
    CREAT_NODE(newNode);
    newNode->type = 0;
    newNode->value= 0;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->flagDirection = 0;
    tree->rootTree = newNode;
}


void BuildTreeFromFile(const char* filename, Tree* tree)
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
    newNode = ReadFromBuffer(&array, NULL);
    tree->rootTree = newNode;

    if (tree->rootTree == NULL)
    {
        printf("Файл пустой или содержит только пробелы. Дерево не заполнено.\n");
        CtorRoot(tree);
    }
}


static void InitializeAkinatorNode(Node* node, int value, Node* left, Node* right)
{
    assert(node);

    node->value = value;
    node->left = left;
    node->right = right;
}

void ProverkaStr(Node* node, const char* value)
{
    if ('0' <= value[0] && value[0] <= '9')
    {
        int number = atoi(value);
        node->type = INT;
        node->value = number;
        return;
    }
    if (strcmp(value, "div") == 0)
    {
        node->type = OPERAT;
        node->value = DIV;
        return;
    }
    if (strcmp(value, "sub") == 0)
    {
        node->type = OPERAT;
        node->value = SUB;
        return;
    }
    if (strcmp(value, "add") == 0)
    {
        node->type = OPERAT;
        node->value = ADD;
        return;
    }
    if (strcmp(value, "mul") == 0)
    {
        node->type = OPERAT;
        node->value = MUL;
        return;
    }
    node->value = OperError;
    node->type = OPERAT;
}

void GenerateImage(Tree* tree)
{
    assert(tree);

    FILE* dotFile = fopen("grapth.dot", "w");
    if (dotFile)
    {
        fprintf(dotFile, "digraph tree {\n");
        fprintf(dotFile, "\tnode [shape=Mrecord, style=filled, fillcolor=\"#bba6cd\", color=\"#552d7b\"];\n");

        PrintNodeDump(dotFile, tree->rootTree, "#d5a1a7");

        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }
    else
    {
        printf("Ошибка при открытии файла graph.dot\n");
    }
}

static void PrintNodeDump(FILE* dotFile, Node* node, const char* fillColor)
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

    if (node->left != NULL)
    {
        fprintf(dotFile, "\t%d -> %d;\n", node, node->left);
        PrintNodeDump(dotFile, node->left, "#6495ed");
    }

    if (node->right != NULL)
    {
        fprintf(dotFile, "\t%d -> %d;\n", node, node->right);
        PrintNodeDump(dotFile, node->right, "#bba6cd");
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

int AAA = 0;

Node* ReadFromBuffer(Buffer* array, Node* currentNode)
{
    char* token = ReadSlovo(array);
    if (strcmp(token, "(") == 0)
    {
        CREAT_NODE(newNode);
        token = ReadSlovo(array);
        ProverkaStr(newNode, token);

        newNode->parent = currentNode;

        newNode->left = ReadFromBuffer(array, newNode);
        newNode->right = ReadFromBuffer(array, newNode);
        token = ReadSlovo(array);
        return newNode;
    }
    else if (strcmp(token, "nil") == 0)
        return NULL;

    return NULL;
}

char* ReadSlovo(Buffer* array)
{
    while (AAA < array->size && isspace(array->buffer[AAA])) {
        AAA++;
    }
    int cnt = 0;
    while (AAA+cnt < array->size && !isspace(array->buffer[AAA + cnt])) {
        cnt++;
    }

    char* token = (char*)malloc(cnt + 1);
    memcpy(token, array->buffer + AAA, cnt);
    token[cnt] = '\0';

    AAA = AAA + cnt;

    return token;
}

void ReadFileInBuffer (FILE *fp, struct Buffer* array)
{
    assert (array != NULL);
    assert (array -> buffer != NULL);

    size_t nread = fread (array -> buffer, sizeof(char), array -> size, fp);
    assert(nread == array -> size);

    fclose (fp);
}

void GetFileSize (FILE* file, struct Buffer* array)
{
    assert (file != NULL);

    struct stat st = {};

    if (fstat(fileno(file), &st) == -1)
    {
        printf ("Error: could not stat the file");
    }

    array -> size = st.st_size;
}

void FillText (struct Buffer* array)
{
    assert (array != NULL);
    assert (array -> buffer != NULL);


    for (int i = 0; i < array -> size; i++)
    {
        if (array -> buffer[i] == '\n')
        {
            array -> buffer[i] = '\0';
        }
    }

    if (array -> size > 0 && array -> buffer[array -> size - 1] != '\n')
    {
        array -> buffer[array -> size] = '\0';
    }
}

int EvaluateExpression(struct Node* node)
{
    assert(node);
    if (node->type == INT) {
        return node->value;
    }

    int leftValue = EvaluateExpression(node->left);
    int rightValue = EvaluateExpression(node->right);

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

void PrintTreeToFile(Node* node)
{
    assert(node);

    FILE* file = fopen("fileOutput.txt", "w");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    PrintNode(node, file);
    fclose(file);
}

void PrintNode(Node* node, FILE* file)
{
    if (node == NULL)
    {
        return;
    }
    if (node->type == INT)
    {
        PrintIntNode(node, file);
    }
    else
    {
        if (node->parent != NULL)
        {
            PrintParentNorNull(node, file);
        }
        else
        {
            PrintParentNull(node, file);
        }
    }
}

void PrintIntNode(Node* node, FILE* file)
{
    PrintNode(node->left, file);
    fprintf(file, "%d ", node->value);
    PrintNode(node->right, file);
}

void PrintParentNorNull(Node* node, FILE* file)
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
    PrintNode(node->left, file);
    char operation = IssuesOperation(node);

    fprintf(file, "%c ", operation);

    node->flagDirection = RIGHT;
    PrintNode(node->right, file);

    if (operationParent> operationNowNode ||
        ((node->parent->flagDirection == RIGHT) && (operationParent == operationNowNode) && (node->value == DIV)))
    {
        fprintf(file, ") ");
    }
}

void PrintParentNull(Node* node, FILE* file)
{
    node->flagDirection = LEFT;
    PrintNode(node->left, file);

    char operation = IssuesOperation(node);

    node->flagDirection = RIGHT;
    fprintf(file, "%c ", operation);
    PrintNode(node->right, file);
}

int CheckingPriorityOperation(int operation)
{
    if (operation == ADD || operation == SUB) return 0;
    if (operation == MUL || operation == DIV) return 1;
}
