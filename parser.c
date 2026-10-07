#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

static Token lookahead;

static void error(const char *message)
{
    printf("implementar depois!!");
}

static void match(TokenType expected)
{
    if (lookahead.type == expected)
    {
        lookahead = getNextToken();
    }
    else
    {
        error("token inesperado");
    }
}

// funcoes dos tokens
static void expr(void);
static void expr1(void);
static void term(void);
static void term1(void);
static void fact(void);

// expr -> term expr1
static void expr(void)
{
    term();
    expr1();
}

// expr1 -> + term expr1
//        | - term expr1
//        | epsilon
static void expr1(void)
{
    if (lookahead.type == TOK_PLUS)
    {
        match(TOK_PLUS);
        term();
        printf("+ ");
        expr1();
    }
    else if (lookahead.type == TOK_MINUS)
    {
        match(TOK_MINUS);
        term();
        printf("- ");
        expr1();
    }
}

// term -> fact term1
static void term(void)
{
    fact();
    term1();
}

// term1 -> * fact term1
//        | / fact term1
//        | epsilon
static void term1(void)
{
    if (lookahead.type == TOK_MULT)
    {
        match(TOK_MULT);
        fact();
        printf("* ");
        term1();
    }
    else if (lookahead.type == TOK_DIV)
    {
        match(TOK_DIV);
        fact();
        printf("/ ");
        term1();
    }
}

// fact -> * ( expr )
//       | num
//       | id
static void fact(void)
{
    if (lookahead.type == TOK_LPAREN)
    {
        match(TOK_LPAREN);
        expr();
        match(TOK_RPAREN);
    }
    else if (lookahead.type == TOK_NUM)
    {
        printf("%s ", lookahead.lexeme);
        match(TOK_NUM);
    }
    else if (lookahead.type == TOK_ID)
    {
        printf("%s ", lookahead.lexeme);
        match(TOK_ID);
    }
    else
    {
        error("esperado numero, identificador ou '('");
    }
}