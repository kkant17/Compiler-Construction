#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TERMINALS 59

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
    TK_EPS,
    TK_DOLLAR,
    TK_COMMENT
} tk;

typedef union value{//from later in the document: 
                     //"If the lexeme is an integer or real number, then its value computed by the lexical analyzer should be printed..."
    int ival;
    float rval;
}value;

typedef struct tokenInfo{//stores lexeme, line number, token type and lexeme's first character's number in line. 
    int lno;
    tk tkid;
    //int cno;
    value val;
    char strlex[31];
    int err;
} tokenInfo;

typedef struct twinBuffer{
    char buf[60];
    int index;
    //int cno;
    int lno;
    int loadedbuf;
} twinBuffer;

typedef struct lookuptbl{
    char keyw[11];
    tk tkid;
} lookuptbl;

typedef struct llnode{
    tokenInfo tkinf;
    struct llnode* next;
} * NODE;

typedef struct llhead{
    NODE head;
    NODE tail;
} * HEAD;

#endif
