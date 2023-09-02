#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"

#include <string.h>
#include <strings.h>
#define true 1
#define false 0

int main(int argc, char *argv[])
{
    const char *str = "/home/hailey/code/cute_c/scripts/hello._0";
    FILE *f = NULL;
    f = fopen(str, "r");
    if (f == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    int c;
    char buff[1024] = {};
    size_t i = 0;

    while ((c = fgetc(f)) != -1)
    {
        switch (c)
        {

        }
    }

    fclose(f);
    return 0;
}