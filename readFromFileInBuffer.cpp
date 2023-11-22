#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "readFromFileInBuffer.h"

void ReadFileInBuffer (FILE *fp, Buffer* array)
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
