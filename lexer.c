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
        else
        { // nao era espaco nem comentario
            ungetc(c, sourceFile);
            return;
        }
    }
}

Token getNextToken(void)
{
    Token token;
    token.lexeme[0] = '\0';

    skipSpaceAndComments();

    token.line = currentLine;

    int c = fgetc(sourceFile);
    if (c == EOF)
    {
        token.type = TOK_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    switch (c)
    { // operadores
    case '+':
        token.type = TOK_PLUS;
        strcpy(token.lexeme, "+");
        return token;
    case '-':
        token.type = TOK_MINUS;
        strcpy(token.lexeme, "-");
        return token;
    case '*':
        token.type = TOK_MULT;
        strcpy(token.lexeme, "*");
        return token;
    case '/':
        token.type = TOK_DIV;
        strcpy(token.lexeme, "/");
        return token;
    case '(':
        token.type = TOK_LPAREN;
        strcpy(token.lexeme, "(");
        return token;
    case ')':
        token.type = TOK_RPAREN;
        strcpy(token.lexeme, ")");
        return token;
    case '{':
        token.type = TOK_LBRACE;
        strcpy(token.lexeme, "{");
        return token;
    case '}':
        token.type = TOK_RBRACE;
        strcpy(token.lexeme, "]");
        return token;
    case ';':
        token.type = TOK_SEMI;
        strcpy(token.lexeme, ";");
        return token;
    }

    if (isalpha(c))
    { // palavras chave e ids
        int len = 0;
        token.lexeme[len++] = (char)c;

        while ((c = fgetc(sourceFile)) != EOF && isalpha(c))
        {
            if (len < MAX_LEXEM_LEN - 1)
            {
                token.lexeme[len++] = (char)c;
            }
        }

        ungetc(c, sourceFile);

        token.lexeme[len] = '\0';

        if (strcmp(token.lexeme, "Matexpr") == 0)
        {
            token.type = TOK_MATEXPR;
        }
        else if (strcmp(token.lexeme, "int") == 0 || strcmp(token.lexeme, "float") == 0)
        {
            token.type = TOK_TYPE;
        }
        else
        {
            token.type = TOK_ID;
        }
        return token;
    }

    if (isdigit(c))
    { // inteiros e floats (numeros)
        int len = 0;
        token.lexeme[len++] = (char)c;
        int hasDot = 0;

        while ((c = fgetc(sourceFile)) != EOF && (isdigit(c) || (c == '.' && !hasDot)))
        {
            if (c == '.')
            {
                hasDot = 1;
            }
            if (len < MAX_LEXEM_LEN - 1)
            {
                token.lexeme[len++] = (char)c;
            }
        }

        ungetc(c, sourceFile);

        token.lexeme[len] = '\0';
        token.type = TOK_NUM;
        return token;
    }

    // caso de erro
    token.type = TOK_ERROR;
    token.lexeme[0] = (char)c;
    token.lexeme[1] = '\0';
    return token;
}