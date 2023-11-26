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
    MUL,
    POW,
    NEG,
    SIN,
    COS
};

enum TYPE{
    INT,
    OPERAT,
    VAR
};

enum DIRECTION{
    LEFT,
    RIGHT
};

void CtorRootAndVariebles(Tree* tree, Variables* arrayVar);
void TreeAndVarieblesDtor(Tree* tree, Variables* arrayVar);
void ClearFile(const char* filename);

Node* NewNode(int type, int value, Node* left, Node* right);
void SetParentPointers(Node* node, Node* parent);
Node* Copy(Node* nowNode);
Node* Dif(Node* nowNode);
void TransformationNode(Node** nowNode, int* changeCount, Variables* arrayVar, Tree* treeDif, Lines* text);

Node* ReadFromBufferInf(Buffer* array, Node* currentNode, Variables* arrayVar);

void BuildTreeFromFile(const char* filename, Tree* tree, Variables* arrayVar);

//Node* ReadFromBuffer(Buffer* array, Node* currentNode);
char* GetWord(Buffer* array);

//Node* BuildTreeFromBuffer(struct Buffer* array, int* index, Variables* arrayVar);
void SetNodeTypeAndValue(Node* node, char* value, Variables* arrayVar);

char* IssuesOperation(Node* node);
void GenerateImage(Tree* tree, Variables* arrayVar);
void GenerateGraphImage();

int EvaluateExpression(Node* node, Variables* arrayVar);

int CheckingPriorityOperation(int operation);

char* FromOperationToWord(int operation);

int GenerateRandomNumber(int min, int max);

void CreateNewGraph();

//VariableData* findOrCreateVariable(Variables* arrayVar, const char* name);

#endif
