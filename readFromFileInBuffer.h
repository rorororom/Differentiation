#ifndef READ
#define READ

struct Buffer {
    char *buffer;
    size_t size;
};

void GetFileSize (FILE* file, struct Buffer* array);
void ReadFileInBuffer (FILE *fp, struct Buffer* array);
void FillText (struct Buffer* array);

#endif
