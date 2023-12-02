#include "stdio.h"

#define MAX_IMG_FILE_LEN 256
#define MAX_CMD_LEN 256

static size_t      IMG_CNT        = 1;
static const char* IMG_FOLDER_DIR = "img/";

int VisCreat(const char* equation);
void CreatePlotScript(const char* script_file, const char* equation);
void MakeImgFromGpl(const char* gpl_file, const char* img_name);
char* GenImgName();
void StartGraphic(const char* gpl_file, const char* img_name);
void EndGraphic(FILE* gnuf);
void StartGraph(FILE* dotf);
void MakeImgFromDot(const char* dot_file);
void StartGraph(FILE* dotf);
void EndGraph(FILE* dotf);

