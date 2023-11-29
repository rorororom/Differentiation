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
    if (node == NULL) return;
    if (node->type == INT || node->type == VAR)
        PrintIntNode(node, file, arrayVar);
    else {
        if (node->parent != NULL)
            PrintParentNorNull(node, file, arrayVar);
        else
            PrintParentNull(node, file, arrayVar);
    }
}

void PrintIntNode(Node* node, FILE* file, Variables* arrayVar)
{
    PrintNode(node->left, file, arrayVar);
    if (node->type ==VAR) {
        char* nameValue = strdup(arrayVar->data[int(node->value)].name);
        fprintf(file, "%s ", nameValue);
    }
    else {
        fprintf(file, "%.2lf ", node->value);
    }
    PrintNode(node->right, file, arrayVar);
}

void PrintParentNorNull(Node* node, FILE* file, Variables* arrayVar)
{
    int operationParent = CheckingPriorityOperation(node->parent->value);
    if (operationParent == ERROR_OP) printf("error operation\n");
    int operationNowNode = CheckingPriorityOperation(node->value);
    if (operationNowNode == ERROR_OP) printf("error operation\n");

    if (operationParent > operationNowNode   || ((node->parent->flagDirection == RIGHT) &&
       (operationParent == operationNowNode) && (node->value == DIV)))
        fprintf(file, "( ");
    else if ((node->parent->flagDirection == RIGHT) && (operationParent == operationNowNode)  &&
             (node->value == SUB))
        node->value = ADD;

    node->flagDirection = LEFT;
    PrintNode(node->left, file, arrayVar);
    char* operation = IssuesOperation(node);

    fprintf(file, "%s ", operation);

    node->flagDirection = RIGHT;
    PrintNode(node->right, file, arrayVar);

    if (operationParent> operationNowNode || ((node->parent->flagDirection == RIGHT) &&
       (operationParent == operationNowNode) && (node->value == DIV)))
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

    if (node == NULL) {
        fprintf(file, "nil ");
        return;
    }

    fprintf(file, "( ");

    PrintNodeInfForm(node->left, file, arrayVar);

    if (node->type == INT || node->type == CONST) {
        if (node->value == E_CONST) fprintf(file, "e ");
        else fprintf(file, "%.2lf ", node->value);
    }
    else if (node->type ==VAR) {
        char* operation = strdup(arrayVar->data[int(node->value)].name);
        fprintf(file, "%s ", operation);
    }
    else {
        char* operation = FromOperationToWord(node->value);
        fprintf(file, "%s ", operation);
    }

    PrintNodeInfForm(node->right, file, arrayVar);
    fprintf(file, ") ");
}

void PrintTreeLaTex(const char* expression, Node* node, Variables* arrayVar, Lines* text)
{
    assert(node);
    assert(arrayVar);

    FILE* file = fopen("./file/tex.md", "a");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    int min_value = 0;
    int max_value = text->line_count - 1;
    int random_number = GenerateRandomNumber(min_value, max_value);

    fprintf(file, "$\\text{%s}$\n", text->text[random_number]);

    fprintf(file, "$$%s ", expression);
    PrintNodeTex(node, file, arrayVar);
    fprintf(file, "$$\n");

    fclose(file);
}

static void PrintDivisionTexNoPar(Node* node, FILE* file, Variables* arrayVar)
{
    fprintf(file, "\\frac ");

    fprintf(file, "{ ");
    PrintNodeTex(node->left, file, arrayVar);
    fprintf(file, "} ");

    fprintf(file, "{ ");
    PrintNodeTex(node->right, file, arrayVar);
    fprintf(file, "} ");
}

static void PrintDivisionTex(Node* node, FILE* file, Variables* arrayVar)
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

static void PrintSpecialOperationTex(Node* node, FILE* file, Variables* arrayVar)
{
    char* operation = IssuesOperation(node);
    fprintf(file, "%s( ", operation);

    PrintNodeTex(node->left, file, arrayVar);
    fprintf(file, ") ");
}

static void PrintPowerOpTexNoPar(Node* node, FILE* file, Variables* arrayVar)
{
    if (node->right->type == INT && node->right->value == 0.5) {
        fprintf(file, "\\sqrt{ ");
        PrintNodeTex(node->left, file, arrayVar);
        fprintf(file, "} ");
    }
    else {
        if (node->left->left == NULL && node->left->right == NULL)
            PrintNodeTex(node->left, file, arrayVar);
        else {
            fprintf(file, "( ");
            PrintNodeTex(node->left, file, arrayVar);
            fprintf(file, ") ");
        }
        char* operation = IssuesOperation(node);
        fprintf(file, "%s ", operation);
        fprintf(file, "{ ");
        PrintNodeTex(node->right, file, arrayVar);
        fprintf(file, "} ");
    }
}

static void PrintPowerOpTex(Node* node, FILE* file, Variables* arrayVar)
{
    char* operation = IssuesOperation(node);
    fprintf(file, "%s ", operation);
    fprintf(file, "{ ");
    PrintNodeTex(node->right, file, arrayVar);
    fprintf(file, "} ");
}

static void PrintMultiplicationWithAddSubLeft(Node* node, FILE* file, Variables* arrayVar)
{
    fprintf(file, "( ");
    PrintNodeTex(node->left, file, arrayVar);
    fprintf(file, ") ");
    fprintf(file, "* ");
    PrintNodeTex(node->right, file, arrayVar);
}

static void PrintMultiplicationWithAddSubRight(Node* node, FILE* file, Variables* arrayVar)
{
    fprintf(file, "( ");
    PrintNodeTex(node->right, file, arrayVar);
    fprintf(file, ") ");
    fprintf(file, "* ");
    PrintNodeTex(node->right, file, arrayVar);
}

static void ProcessDefaultOperationTex(Node* node, FILE* file, Variables* arrayVar)
{
    node->flagDirection = LEFT;
    PrintNodeTex(node->left, file, arrayVar);

    if (node->value == MUL) {
        fprintf(file, "\\cdot ");
    }
    else {
        char* operation = IssuesOperation(node);
        fprintf(file, "%s ", operation);
    }

    node->flagDirection = RIGHT;
    PrintNodeTex(node->right, file, arrayVar);
}

void PrintParentNorNullTex(Node* node, FILE* file, Variables* arrayVar)
{
    int operationParent = 0;
    if (node->parent != NULL) {
        operationParent = CheckingPriorityOperation(node->parent->value) ;
        if (operationParent == ERROR_OP) printf("error operation\n");
    }

    int operationNowNode = CheckingPriorityOperation(node->value);
    if (operationNowNode == ERROR_OP) printf("error operation\n");

    if ((node->parent->flagDirection == RIGHT) &&
        (operationParent == operationNowNode) && (node->value == SUB)) {
        node->value = ADD;
    }

    if (node->value == DIV) {
        PrintDivisionTexNoPar(node, file, arrayVar);
    }
    else if (5 <= node->value && node->value <= 13) {
        PrintSpecialOperationTex(node, file, arrayVar);
    }
    else if (node->value == POW) {
        PrintPowerOpTexNoPar(node, file, arrayVar);
    }

    else if (node->value       == MUL  && (((node->left->value == ADD ||
             node->left->value == SUB) &&    node->left->type  == OPERAT))) {
        PrintMultiplicationWithAddSubLeft(node, file, arrayVar);
    }
    else if (node->value        == MUL  && (((node->right->value == ADD ||
             node->right->value == SUB) &&    node->right->type  == OPERAT))) {
        PrintMultiplicationWithAddSubRight(node, file, arrayVar);
    }
    else {
        ProcessDefaultOperationTex(node, file, arrayVar);
    }
}

void PrintNodeTex(Node* node, FILE* file, Variables* arrayVar)
{
    if (node == NULL) {
        return;
    }
    if (node->type == INT || node->type == VAR || node->type == CONST) {
        PrintIntNodeTex(node, file, arrayVar);
    }
    else {
        if (node->parent != NULL) {
            PrintParentNorNullTex(node, file, arrayVar);
        }
        else {
            PrintParentNullTex(node, file, arrayVar);
        }
    }
}

void PrintIntNodeTex(Node* node, FILE* file, Variables* arrayVar)
{
    PrintNodeTex(node->left, file, arrayVar);
    if (node->type == CONST) {
        fprintf(file, "e ");
    }
    else if (node->type == INT) {
        fprintf(file, "%.2lf ", node->value);
    }
    else if (node->type ==VAR) {
        char* nameVar = strdup(arrayVar->data[int(node->value)].name);
        fprintf(file, "%s ", nameVar);
    }
    PrintNodeTex(node->right, file, arrayVar);
}

void PrintParentNullTex(Node* node, FILE* file, Variables* arrayVar)
{
    if (node->value == DIV) {
        PrintDivisionTex(node, file, arrayVar);
    }
    else if (node->value == POW) {
        PrintPowerOpTex(node, file, arrayVar);
    }
    else if (SIN <= node->value && node->value <= ARCCOT) {
        PrintSpecialOperationTex(node, file, arrayVar);
    }
    else {
        ProcessDefaultOperationTex(node, file, arrayVar);
    }
}

void Preamble()
{
    FILE* file = fopen("./file/tex.md", "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return;
    }

//     fprintf(file, R"(
// documentclass{article}
// usepackage[a4paper]{geometry}
// geometry{top=1.41cm, bottom=1.41cm, left=1.41cm, right=1.41cm, marginparwidth=1.75cm}
// )");

    fprintf(file, "\\documentclass{article}\n");
    fprintf(file, "\\usepackage[a4paper]{geometry}\n");
    fprintf(file, "\\geometry{top=1.41cm, bottom=1.41cm, left=1.41cm, right=1.41cm, marginparwidth=1.75cm}\n");

    fprintf(file, "\\usepackage[T2A]{fontenc}\n");
    fprintf(file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file, "\\usepackage[english, russian]{babel}\n");

    fprintf(file, "\\usepackage{amsmath}\n");
    fprintf(file, "\\usepackage{graphicx}\n");
    fprintf(file, "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n");
    fprintf(file, "\\usepackage{amsfonts}\n");
    fprintf(file, "\\usepackage{amssymb}\n");
    fprintf(file, "\\usepackage{seqsplit}\n");
    fprintf(file, "\\usepackage[dvipsnames]{xcolor}\n");
    fprintf(file, "\\usepackage{enumitem}\n");
    fprintf(file, "\\usepackage{algorithm}\n");
    fprintf(file, "\\usepackage{algpseudocode}\n");
    fprintf(file, "\\usepackage{algorithmicx}\n");
    fprintf(file, "\\usepackage{mathalfa}\n");
    fprintf(file, "\\usepackage{mathrsfs}\n");
    fprintf(file, "\\usepackage{dsfont}\n");
    fprintf(file, "\\usepackage{caption,subcaption}\n");
    fprintf(file, "\\usepackage{wrapfig}\n");
    fprintf(file, "\\usepackage[stable]{footmisc}\n");
    fprintf(file, "\\usepackage{indentfirst}\n");
    fprintf(file, "\\usepackage{rotating}\n");
    fprintf(file, "\\usepackage{pdflscape}\n");

    fprintf(file, "\\usepackage{MnSymbol,wasysym}\n");

    fprintf(file, "\\begin{document}\n");

    fclose(file);
}

void EndOfDocument()
{
    FILE* file = fopen("./file/tex.md", "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    fprintf(file, "\\end{document}\n");
    fclose(file);
}

void PrintStartProekt()
{
    FILE* file = fopen("./file/tex.md", "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    fprintf(file, "Дифференцировать или не дифференцировать? Вот в чем вопрос.\n");
    fprintf(file, "\n");
    fprintf(file, "Ромашкина Мария\n");
    fprintf(file, "\n");

    fprintf(file, "Мы считаем, что в мире, полном перемен и неожиданностей, умение различать,\
                   анализировать и выделять ключевые моменты – настоящее искусство.\
                   Дифференцирование - это как надежная карта в лабиринте жизни,\
                   которая помогает нам ориентироваться в бурной реальности.\
                   Представьте себе: вы стоите на вершине горы, а ваши знания\
                   в дифференциальных уравнениях – это крылья, позволяющие вам свободно парить\
                   в воздухе перемен. Это не просто математика, это способность видеть красоту\
                   в структурах и взаимосвязях. И если вас когда-то спросят, дифференцировать\
                   или не дифференцировать, вы точно будете знать ответ –\
                   давайте раскрывать потенциал и выделять главные моменты в этом захватывающем\
                   путешествии под названием Жизнь.\n" );
    fprintf(file, "\n");
    fprintf(file, "А вообще, пока вы молоды...\
                   Тусуйтесь на нк и кайфуйте, потому что молодость никто не вернет,\
                   а вот как брать производные, ты всегда сможешь загуглить)\n");
    fprintf(file, "\n");

    fclose(file);
}
