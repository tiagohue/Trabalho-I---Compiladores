#include <stdio.h>

#define MAX_LEXEM_LEN 128

// Tokens reconhecíveis
typedef enum
{
    // básico
    TOK_MATEXPR,
    TOK_TYPE,
    TOK_ID,
    TOK_NUM,

    // operadores
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULT,
    TOK_DIV,

    // estrutural
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_SEMI,

    // especiais
    TOK_EOF,
    TOK_ERROR
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[MAX_LEXEM_LEN];
    int line;
} Token;

Token getNextToken(void);