# Tradutor de Expressões Aritméticas para Notação Pós-fixada

Este documento contém a especificação e arquitetura do tradutor de linguagens de programação desenvolvido em C para a disciplina de Compiladores.

O programa realiza a leitura de arquivos de entrada contendo declarações de variáveis e expressões aritméticas, executa a análise léxica e a análise sintática preditiva descendente, e traduz as expressões para a notação pós-fixada utilizando Tradução Dirigida por Sintaxe (SDT).


## 1. Gramática e Esquema de Tradução (SDT)

A gramática livre de contexto que rege a linguagem do tradutor é definida pelas seguintes regras de produção:

**OBSERVAÇÃO**: As regras de produção estão um pouco diferentes das especificadas no arquivo pois dessa forma evitam a recursão à esquerda.

```text
program -> Matexpr block
block   -> { decls stmts }
decls   -> decl decls | ε
decl    -> type id ;
stmts   -> stmt stmts | ε
stmt    -> block | expr
expr    -> term expr'
expr'   -> + term { print('+ ') } expr' 
         | - term { print('- ') } expr' 
         | ε
term    -> fact term'
term'   -> * fact { print('* ') } term' 
         | / fact { print('/ ') } term' 
         | ε
fact    -> ( expr ) 
         | num { print(num) } 
         | id { print(id) }
```

## 2. Estrutura do Projeto

* `lexer.h`: Interface do analisador léxico e definições de tokens.
* `lexer.c`: Implementação do analisador léxico, tratamento de espaços, comentários e identificação de lexemas.
* `parser.h`: Interface do analisador sintático preditivo LL(1).
* `parser.c`: Análise sintática preditiva descendente e tradução para a notação pós-fixada.
* `main.c`: Ponto de entrada do sistema e gerenciamento de arquivos.

## 3. Instruções de Compilação e Execução

### Compilação
Para compilar o tradutor utilizando o GCC:

```bash
gcc -Wall lexer.c parser.c main.c -o tradutor
```

### Execução
Para executar o tradutor passando um arquivo de código-fonte como entrada:

```bash
./tradutor testes/entrada.txt
```

---

## 4. Exemplos de Teste

### Exemplo 1 (`testes/entrada.txt`)

**Entrada:**
```text
Matexpr {
    int x;
    float y;
    a + 5
    (x - 2) * 3
}
```

**Saída:**
```text
a 5 + 
x 2 - 3 * 
```

### Exemplo 2 (`testes/entrada2.txt`)

**Entrada:**
```text
Matexpr {
    // Declaracao de variaveis
    int a;
    int b;
    float resultado;

    /* Expressao 1: precedencia de operadores */
    a + b * 4 - 10 / 2

    // Expressao 2: parenteses e decimais
    (a + b) * (resultado - 3.14)

    /* Bloco aninhado */
    {
        float temp;
        temp + a * 2
    }
}
```

**Saída:**
```text
a b 4 * + 10 2 / - 
a b + resultado 3.14 - * 
temp a 2 * + 
```
### Exemplo 3 (`testes/entrada3.txt`)

**Entrada:**
```text
Matexpr {
    int x;
    a + * 5
}
```

**Saída:**
```text
Erro sintatico na linha 3: esperado numero, identificador ou '(' (token encontrado: '*')
```

### Exemplo 4 (`testes/entrada3.txt`)

**Entrada:**
```text
Matexpr {
    int x;
    a + 5
}
```

**Saída:**
```text
Erro sintatico na linha 1: token inesperado (token encontrado: '{')
```
