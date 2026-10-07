#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static FILE *sourceFile = NULL;
static int currentLine = 1;

void initLexer(FILE *file)
{
    sourceFile = file;
    currentLine = 1;
}

static void skipSpaceAndComments(void)
{
    int c;

    while ((c = fgetc(sourceFile)) != EOF)
    {
        if (c == '\n')
        {
            currentLine++;
        }
        else if (isspace(c))
        {
            continue;
        }
        else if (c == '/')
        {
            int next = fgetc(sourceFile);

            if (next == '/')
            { // descarta a linha de comentario
                while ((c = fgetc(sourceFile)) != EOF && c != '\n')
                    ;
                if (c == '\n')
                {
                    currentLine++;
                }
            }
            else if (next == '*')
            { // descarta blocos de comentários
                int prev = 0;

                while ((c = fgetc(sourceFile)) != EOF)
                {
                    if (c == '\n')
                    {
                        currentLine++;
                    }
                    if (prev == '*' && c == '/')
                    {
                        break;
                    }
                    prev = c;
                }
            }
            else
            { // era só uma barra
                ungetc(next, sourceFile);
                ungetc(c, sourceFile);
                return;
            }
        }
    }
}
