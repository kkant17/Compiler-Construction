#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"


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

FILE *getStream(twinBuffer* buffer, FILE *fp) {//populates twin buffer(a global variable) with 200 characters  
    if (fp == NULL) {
        printf("Error: File pointer is NULL\n");
        return NULL;
    }
    if (buffer->currbuf==0){
        printf("0 to 1\n");
        fread(buffer->buf2, sizeof(char), 30, fp);
        buffer->index=0;
        buffer->currbuf=1;
    }
    else if (buffer->currbuf==1){
        printf("1 to 0\n");
        fread(buffer->buf1, sizeof(char), 30, fp);
        buffer->index=0;
        buffer->currbuf=0;
    }
    return fp;
}

tokenInfo getNextToken(twinBuffer* B, FILE* fp){
    tokenInfo token;
    lexeme lex;
    int state=0;
    char currchar;
    int startptr=B->index;
    while (1){
        if(B->index==0){
            fp=getStream(B,fp);
        }
        currchar=(B->currbuf)?B->buf1[B->index]:B->buf2[B->index];

        if((currchar<=9)||((currchar>=11)&&(currchar<=31))||currchar==34||currchar==27||currchar==63||currchar==92||currchar==94||currchar==96||((currchar>=123)&&(currchar<=125))||currchar>125)
            {fprintf(stderr,"Unknown character %c at line %d",currchar,B->lno); B->index++; startptr++; continue;}


//0-9,11-31,34,27,63,92,94,96,123-125
        switch (state){
            case 0: 
                startptr=B->index;
                if((48<=currchar)&&(57>=currchar)){state=1;break;}
                if(currchar=='!'){state=16;break;}
                if(currchar=='.'){state=47;break;}
                if(currchar==';'){state=46;break;}
                if(currchar=='('){state=44;break;}
                if(currchar==')'){state=45;break;}
                if(currchar=='['){state=42;break;}
                if(currchar==']'){state=43;break;}
                if(currchar=='/'){state=41;break;}
                if(currchar=='*'){state=40;break;}
                if(currchar=='-'){state=39;break;}
                if(currchar=='+'){state=38;break;}
                if(currchar==':'){state=48;break;}
                if(currchar==','){state=49;break;}
                if(currchar=='~'){state=29;break;}
                if(currchar=='@'){state=30;break;}
                if(currchar=='&'){state=34;break;}
                if(currchar=='~'){state=29;break;}
                if(currchar=='<'){state=21;break;}
                if(currchar=='>'){state=18;break;}
                if(currchar=='_'){state=53;break;}
                if(currchar==' '){state=69;break;}
                if(currchar=='\0'){state=74;break;}
                if(currchar=='%'){state=71;break;}
                if(currchar=='\n'){state=76;break;}
                if((currchar<=100)&&(currchar>=98)){state=58;break;}
                if((currchar<=122)&&(currchar>=101)){state=56;break;}
                if(currchar=='a'){state=56;break;}
                if(currchar=='#'){state=50;break;}
                state=75;
                break;
            case 1:
                if((48<=currchar)&&(57>=currchar)){state=1;break;}
                if(currchar=='.'){state=3;break;}
                state=2;
                break;

            //case 2:
                
        }
        
        B->index=(B->index+1)%30; 
    }
}


//final state action order- 
//1) retract (reduce B->index, changing currbuf and B->index as needed if the new value is negative)
//2) set parameters of token to return (by calling real_token, int_token, lookup and insert or set_lexeme) / print error
//3) set both pointers= (B->index+1)%30 (change currbuf if necessary) if returning token
//4) inc line number if applicable
//5) return token/set state to 0

//The setting of the forward and start pointer to the required value is taken care of automatically by the increment at the end of the loop and the setting
//of startpointer=B->index in state 0, in the case where we go to state 0 instead of returning token. Otherwise, it must be done before the return statement.                                                         

//installing token/looking up a token in the symbol table could be handled by the invoking parser function. 
//it should know to do this since insertion/lookup is needed if and only if an identifier token is returned.
//Or we could make a symbol table structure here, initialize it in the parser function and pass it into getNextToken().

void retract(twinBuffer*B){
    if(B->index==0){
        B->currbuf==(B->currbuf+1)%2;
        B->index=29;
    }
    else{
        B->index--;
    }
    return;
}

tokenInfo set_lexeme(twinBuffer* B, int startptr,tk toktype){
    tokenInfo token;
    token.lno=B->lno;  
    token.tkid=toktype;
    char *strlex=NULL;
    if(B->index<startptr){
        //strlex=(char*)malloc((30-startptr+B->index+1)*sizeof(char));
        //int i=startptr;
        if(B->currbuf==0){            
            /*while(i<30){
                token.lex[i-startptr]=B->buf2[i];
                i++;
            }
            while(i-30<=B->index){
                token.lex[i-startptr]=B->buf1[i-30];
                i++;
            }*/

            strncpy(token.lex.strlex,B->buf2+startptr,30-startptr);
            strncpy(token.lex.strlex+30-startptr,B->buf1,B->index+1);
            token.lex.strlex[30-startptr+B->index+1]='\0';

        }
        if (B->currbuf==1){
            /*while(i<30){
                token.lex[i-startptr]=B->buf1[i];
                i++;
            }
            while(i-30<=B->index){
                token.lex[i-startptr]=B->buf2[i-30];
                i++;
            }*/

            strncpy(token.lex.strlex,B->buf1+startptr,30-startptr);
            strncpy(token.lex.strlex+30-startptr,B->buf2,B->index+1);
            token.lex.strlex[30-startptr+B->index+1]='\0';
        }
    }
    else{
        //strlex=(char*)malloc((B->index-startptr+1)*sizeof(char));
        //int i=startptr;
        if(B->currbuf==0){
            /*while(i<=B->index){
                token.lex[i-startptr]=B->buf1[i];
            }*/
            strncpy(token.lex.strlex,B->buf1+startptr,B->index-startptr+1);
            token.lex.strlex[B->index-startptr+1]='\0';
        }
        if(B->currbuf==1){
            /*while(i<=B->index){
                token.lex[i-startptr]=B->buf2[i];
            }*/
            strncpy(token.lex.strlex,B->buf1+startptr,B->index-startptr+1);
            token.lex.strlex[B->index-startptr+1]='\0';
        }
    }
    //token.lex=strlex;
    return token;
}


tokenInfo int_token(twinBuffer* B, int startptr){
    tokenInfo token;
    token.lno=B->lno;
    token.tkid=TK_INT;
    int value=0;
    int multiplier=1;
    int i=B->index;

    if(B->index<startptr){
      if(B->currbuf==0){
        while(i>=0){
           value+=(B->buf1[i]-48)*multiplier;
           multiplier*=10;
           i--;
        }
        i=29;
        while(i>=startptr){
           value+=(B->buf2[i]-48)*multiplier;
           multiplier*=10;
           i--;
        }
      }
      if(B->currbuf==1){
        while(i>=0){
           value+=(B->buf2[i]-48)*multiplier;
           multiplier*=10;
           i--;
        }
        i=29;
        while(i>=startptr){
           value+=(B->buf1[i]-48)*multiplier;
           multiplier*=10;
           i--;
        }
      }
    }
    else{
      if(B->currbuf==0)
      {
        while(i>=startptr){
         value+=(B->buf1[i]-48)*multiplier;
         multiplier*=10;
         i--;
        }
      }
      if(B->currbuf==1)
      {
        while(i>=startptr){
         value+=(B->buf2[i]-48)*multiplier;
         multiplier*=10;
         i--;
        }
      }
    }
    token.lex.ival=value;
    return token;
}

tokenInfo float_token(twinBuffer* B, int startptr){
    tokenInfo token;
    token.lno=B->lno;
    token.tkid=TK_REAL;
    float value=0;
    int multiplier=1;
    int i=B->index;

    if(B->index<startptr){
      if(B->currbuf==0){
        while(i>=0){
           if(B->buf1[i]=='.'){
              value/=multiplier;
              multiplier=1;i--;
              continue;
           }
           value+=(B->buf1[i]-48)*multiplier;
           multiplier*=10;
           i--;
        }
        i=29;
        while(i>=startptr){
            if(B->buf2[i]=='.'){
              value/=multiplier;
              multiplier=1;i--;
              continue;
           }
           value+=(B->buf2[i]-48)*multiplier;
           multiplier*=10;
           i--;
        }
      }
      if(B->currbuf==1){
        while(i>=0){
           if(B->buf2[i]=='.'){
              value/=multiplier;
              multiplier=1;i--;
              continue;
           }
           value+=(B->buf2[i]-48)*multiplier;
           multiplier*=10;
           i--;
        }
        i=29;
        while(i>=startptr){
            if(B->buf1[i]=='.'){
              value/=multiplier;
              multiplier=1;i--;
              continue;
           }
           value+=(B->buf1[i]-48)*multiplier;
           multiplier*=10;
           i--;
        }
      }
    }
    else{
      if(B->currbuf==0)
      {
        while(i>=startptr){
         if(B->buf1[i]=='.'){
              value/=multiplier;
              multiplier=1;i--;
              continue;
         }
         value+=(B->buf1[i]-48)*multiplier;
         multiplier*=10;
         i--;
        }
      }
      if(B->currbuf==1)
      {
        while(i>=startptr){
         if(B->buf2[i]=='.'){
              value/=multiplier;
              multiplier=1;i--;
              continue;
         }
         value+=(B->buf2[i]-48)*multiplier;
         multiplier*=10;
         i--;
        }
      }
    }
    token.lex.rval=value;
    return token;
}

/*tokenInfo lookup(){
    tokenInfo token;
    token->lno=B->lno;
}*/

int main(){
    twinBuffer* buffer;
    buffer=(twinBuffer *)malloc(sizeof(twinBuffer));
    buffer->lno=1;
    //buffer->cno=1; 
    buffer->index=0;
    buffer->currbuf=0;
    FILE* test=fopen("test.txt","r");
    test=getStream(buffer,test);
    //test=getStream(buffer,test);
    printf("Buffer 1: %s\n", buffer->buf1);
    printf("Buffer 2: %s\n", buffer->buf2);
    printf("Current Buffer: %d\n", buffer->currbuf);
    printf("Index: %d\n", buffer->index);
    //printf("Character Number: %d\n", buffer->cno);
    printf("Line Number: %d\n", buffer->lno);


    test=getStream(buffer,test);
    printf("Buffer 1: %s\n", buffer->buf1);
    printf("Buffer 2: %s\n", buffer->buf2);
    printf("Current Buffer: %d\n", buffer->currbuf);
    printf("Index: %d\n", buffer->index);
    //printf("Character Number: %d\n", buffer->cno);
    printf("Line Number: %d\n", buffer->lno);

    buffer->index=2;
    tokenInfo tkinf=float_token(buffer,28);
    printf("%f\n",tkinf.lex.rval);
    return 0;
}
