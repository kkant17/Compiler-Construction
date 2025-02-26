#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

twinBuffer* buffer;


//running count of line and character, so as to populate the fields in the tokenInfo structure in getNextToken(twinBuffer B) 


void removeComments(char *testcaseFile, char *cleanFile){//assumes char array terminates with \0, and returns a char array terminated with \0 as well
    FILE* tcf=fopen(testcaseFile,"r");
    FILE*  cf=fopen(cleanFile,"w");

    if(tcf==NULL||cf==NULL){
        printf("Error: file %s or %s not found in directory",testcaseFile,cleanFile);
    }
    
    int flag=0;
    char temp=fgetc(tcf);
    while(temp!=EOF){
        if(temp=='%'){
           flag=1;
        }
        if((flag==1)&&(temp=='\n')){flag=0;}
        else if(!flag) fputc(temp,cf);
        temp=fgetc(tcf);
    }
    fclose(tcf);
}

FILE *getStream(FILE *fp) {//populates twin buffer(a global variable) with 200 characters  
    if (fp == NULL) {
        printf("Error: File pointer is NULL\n");
        return NULL;
    }
    if (buffer->currbuf==0){
        fread(buffer->buf2, sizeof(char), 30, fp);
        buffer->index=0;
    }
    if (buffer->currbuf==1){
        fread(buffer->buf1, sizeof(char), 30, fp);
        buffer->index=0;
    }
    return fp;
}

tokenInfo getNextToken(twinBuffer B){
    tokenInfo token;
    lexeme lex;
    int state=0;
    while (1){
        switch (state){
            case 
        }

    }
}

int main(){
    buffer=(twinBuffer *)malloc(sizeof(twinBuffer));
    buffer->lno=1;
    buffer->cno=1;
    buffer->index=0;
    buffer->currbuf=1;
    FILE* test=fopen("test.txt","r");
    test=getStream(test);
    printf("Buffer 1: %s\n", buffer->buf1);
    printf("Buffer 2: %s\n", buffer->buf2);
    printf("Current Buffer: %d\n", buffer->currbuf);
    printf("Index: %d\n", buffer->index);
    printf("Character Number: %d\n", buffer->cno);
    printf("Line Number: %d\n", buffer->lno);
    return 0;
}


