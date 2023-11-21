#ifndef DIFFER
#define DIFFER

#include <stdlib.h>

const int MAX_LEN = 256;

struct Buffer {
    char *buffer;
    size_t size;
};

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

enum OPERATION{
    OperError = -1,
    SUB,
    DIV,
    ADD,
    MUL
};

enum TYPE{
    INT,
    OPERAT
};

enum DIRECTION{
    LEFT,
    RIGHT
};

void CtorRoot(Tree* tree);

void BuildTreeFromFile(const char* filename, Tree* tree);
void GetFileSize (FILE* file, struct Buffer* array);
void ReadFileInBuffer (FILE *fp, struct Buffer* array);
void FillText (struct Buffer* array);

Node* ReadFromBuffer(Buffer* array, Node* currentNode);
char* ReadSlovo(Buffer* array);

Node* BuildTreeFromBuffer(struct Buffer* array, int* index);
void ProverkaStr(Node* node, const char* value);

char IssuesOperation(Node* node);
void GenerateImage(Tree* tree);
void GenerateGraphImage();

int EvaluateExpression(struct Node* node);

void PrintTreeToFile(Node* node);
void PrintNode(Node* node, FILE* file);
void PrintIntNode(Node* node, FILE* file);
void PrintParentNorNull(Node* node, FILE* file);
void PrintParentNull(Node* node, FILE* file);

int CheckingPriorityOperation(int operation);

#define CREAT_NODE(node) Node* node = (Node*)malloc(sizeof(Node))

#endif
