#ifndef DIFFER
#define DIFFER

#include <stdlib.h>
#include "readFromFileInBuffer.h"

#define CREAT_NODE(node) Node* node = (Node*)malloc(sizeof(Node))

const int MAX_LEN = 256;
const int MAX_VARIABLES = 10;
const int capacity = 10;

typedef struct Node {
    int type;
    int value;
    Node* left;
    Node* right;
    Node* parent;
    int flagDirection;
} Node;

typedef struct Tree {
    Node* rootTree;
    int size;
} Tree;

typedef struct VariableData {
    char* name;
    int value;
} VariableData;

typedef struct Variables {
    VariableData* data;
    int capacity;
    int size;
} Variables;

enum OPERATION{
    OperError = -1,
    SUB,
    DIV,
    ADD,
    MUL
};

enum TYPE{
    INT,
    OPERAT,
    VARIEBL
};

enum DIRECTION{
    LEFT,
    RIGHT
};

void CtorRootAndVariebles(Tree* tree, Variables* arrayVar);
void TreeAndVarieblesDtor(Tree* tree, Variables* arrayVar);

Node* ReadFromBufferInf(Buffer* array, Node* currentNode, Variables* arrayVar);

void BuildTreeFromFile(const char* filename, Tree* tree, Variables* arrayVar);

//Node* ReadFromBuffer(Buffer* array, Node* currentNode);
char* GetWord(Buffer* array);

//Node* BuildTreeFromBuffer(struct Buffer* array, int* index, Variables* arrayVar);
void SetNodeTypeAndValue(Node* node, char* value, Variables* arrayVar);

char IssuesOperation(Node* node);
void GenerateImage(Tree* tree, Variables* arrayVar);
void GenerateGraphImage();

int EvaluateExpression(Node* node, Variables* arrayVar);

void PrintTreeToFileWithoutBrackets(Node* node, Variables* arrayVar);
void PrintNode(Node* node, FILE* file, Variables* arrayVar);
void PrintIntNode(Node* node, FILE* file, Variables* arrayVar);
void PrintParentNorNull(Node* node, FILE* file, Variables* arrayVar);
void PrintParentNull(Node* node, FILE* file, Variables* arrayVar);

int CheckingPriorityOperation(int operation);

void PrintInFileInfForm(Node* node, Variables* arrayVar);
void PrintNodeInfForm(Node* node, FILE* file, Variables* arrayVar);

char* FromOperationToWord(int operation);

void PrintTreeLaTex(Node* node, Variables* arrayVar);
void PrintParentNorNullTex(Node* node, FILE* file, Variables* arrayVar);
void PrintNodeTex(Node* node, FILE* file, Variables* arrayVar);
void PrintParentNullTex(Node* node, FILE* file, Variables* arrayVar);
void PrintIntNodeTex(Node* node, FILE* file, Variables* arrayVar);

//VariableData* findOrCreateVariable(Variables* arrayVar, const char* name);

#endif
