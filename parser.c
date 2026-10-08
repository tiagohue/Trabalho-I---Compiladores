#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

static Token lookahead;

static void error(const char *message)
{
    fprintf(stderr, "Erro sintatico na linha %d: %s (token encontrado: '%s')\n",
            lookahead.line, message, lookahead.lexeme);
    exit(EXIT_FAILURE);
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
static void program(void);
static void block(void);
static void decls(void);
static void decl(void);
static void stmts(void);
static void stmt(void);
static void expr(void);
static void expr1(void);
static void term(void);
static void term1(void);
static void fact(void);

// program -> Matexpr block
static void program(void)
{
    match(TOK_MATEXPR);
    block();
    if (lookahead.type != TOK_EOF)
    {
        error("conteudo extra apos o termino do bloco principal");
    }
}

// block -> { decls stmts }
static void block(void)
{
    match(TOK_LBRACE);
    decls();
    stmts();
    match(TOK_RBRACE);
}

// MODIFICADO
// decls -> decl decls
//        | epsilon
static void decls(void)
{
    while (lookahead.type == TOK_TYPE)
    {
        decl();
    }
}

// decl -> type id ;
static void decl(void)
{
    match(TOK_TYPE);
    match(TOK_ID);
    match(TOK_SEMI);
}

// MODIFICADO
// stmts -> stmt stmts
//        | epsilon
static void stmts(void)
{
    while (lookahead.type == TOK_LBRACE || lookahead.type == TOK_LPAREN || lookahead.type == TOK_NUM || lookahead.type == TOK_ID)
    {
        stmt();
    }
}

// MODIFICADO
// stmt -> block
//       | expr
static void stmt(void)
{
    if (lookahead.type == TOK_LBRACE)
    {
        block();
    }
    else
    {
        expr();
        printf("\n"); // nova linha após a expressão ser traduzida
    }
}

// MODIFICADO
// expr -> term expr1
static void expr(void)
{
    term();
    expr1();
}

// MODIFICADO
// expr1 -> + term { print("+ ") } expr1
//        | - term { print("- ") } expr1
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

// MODIFICADO
// term -> fact term1("
static void term(void)
{
    fact();
    term1();
}

// MODIFICADO
// term1 -> * fact { print("* ") } term1
//        | / fact { print("/ ") } term1
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

// fact -> ( expr )
//       | num { print(num) }
//       | id { print(id) }
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

void parse(FILE *file)
{
    initLexer(file);
    lookahead = getNextToken();
    program();
}