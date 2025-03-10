#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    TK_ASSIGNOP,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_DOLLAR,
    TK_EPS
} tk;

typedef union lexeme{//from later in the document: 
                     //"If the lexeme is an integer or real number, then its value computed by the lexical analyzer should be printed..."
    int ival;
    float rval;
    char strlex[31];
} lexeme;

typedef struct tokenInfo{//stores lexeme, line number, token type and lexeme's first character's number in line. 
    int lno;
    tk tkid;
    //int cno;
    lexeme lex;
} tokenInfo;

typedef struct twinBuffer{
    char buf1[30];
    char buf2[30];
    int currbuf;
    int index;
    //int cno;
    int lno;
} twinBuffer;


void removeComments(char *testcaseFile, char *cleanFile);
FILE *getStream(twinBuffer* buffer, FILE *fp);
tokenInfo getNextToken(twinBuffer* B, FILE *fp);

#endif
