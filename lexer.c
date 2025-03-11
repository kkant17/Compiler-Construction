#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "leggser.h"



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

FILE *getStream(twinBuffer* buffer, FILE *fp, int bufno) {//populates twin buffer(a global variable) with 200 characters  
    if (fp == NULL) {
        printf("Error: File pointer is NULL\n");
        return NULL;
    }
    if (bufno==1){
        //printf("0 to 1\n");
        fread(buffer->buf+30, sizeof(char), 30, fp);
        //buffer->index=30;
    }
    else if (bufno==0){
        //printf("1 to 0\n");
        fread(buffer->buf, sizeof(char), 30, fp);
        //buffer->index=0;
    }
    return fp;
}


/*void retract(twinBuffer*B){
    B->index=(B->index+29)%30;
    return;
}*/

tokenInfo set_lexeme(twinBuffer* B, int len,tk toktype){
    int fwdptr=(B->index+len+59)%60;
    printf("fwdptr is %d",fwdptr);
    tokenInfo token;
    token.err=0;
    token.lno=B->lno;  
    token.tkid=toktype;
    if(fwdptr<B->index){
        strncpy(token.lex.strlex,B->buf+B->index,60-B->index);
        strncpy(token.lex.strlex+60-B->index,B->buf,fwdptr+1);
        token.lex.strlex[len]='\0';
    }
    else{
        strncpy(token.lex.strlex,B->buf+B->index,len);
        token.lex.strlex[len]='\0';
    }
    return token;
}


//start index in wraparound case is (B->index-len+1+60)%60

tokenInfo int_token(twinBuffer* B, int len){
    tokenInfo token;
    token.err=0;
    token.lno=B->lno;
    token.tkid=TK_INT;
    int value=0;
    int multiplier=1;
    int i=B->index;

    /*if(len>30){
        fprintf(stderr,"Line %d: Integer length limit exceeded. Truncated to lowest 30 digits",B->lno);
        i=(i+len-30)%60;
    }*/

    while(i!=(B->index+len+59)%60){
        value=value*10+(B->buf[i]-48);
        i=(i+1)%60;
    }
    value=value*10+(B->buf[i]-48);

    token.lex.ival=value;
    return token;
}

tokenInfo float_token(twinBuffer* B, int len){
    tokenInfo token;    
    token.err=0;
    token.lno=B->lno;
    token.tkid=TK_REAL;
    float value=0;
    float dec=0;
    float exp=0;
    int i=B->index;

    /*if(len>30){
        fprintf(stderr,"Line %d: Real number length limit exceeded. Integral part truncated to lowest 30 digits",B->lno);
        i=(i+len-30)%60;
    }*/
    
    while(B->buf[i]!='.'){
        value=(value*10)+(B->buf[i]-48);
        i=(i+1)%60;
    }

    //printf("value is %f\n",value);
    //printf("exp is %c%c\n",B->buf[(B->index+len+58)%60],B->buf[(B->index+len+59)%60]);
    dec=(B->buf[(i+1)%60]-48)*10+(B->buf[(i+2)%60]-48);
    value+=dec/100;
    i+=3;
    
    if((B->index+len+59)%60>i){
        exp=(B->buf[(B->index+len+58)%60]-48)*10+(B->buf[(B->index+len+59)%60]-48);
        if(B->buf[(i+1)%60]=='-'){exp=-exp;}
        value=value*pow(10,exp);
    }
    
    //printf("exp is %f\n",exp);

    token.lex.rval=value;
    return token;
    
}



int isunknown(int lno, char c){
            if((c<=9)||
            ((c>=11)&&(c<=31))||
            c==34||
            c==27||
            c==63||
            c==92||
            c==94||
            c==96||
            ((c>=123)&&(c<=125))||
            c>125){
               
               return 1; 
            }
            else return 0;

}




//NEED ERROR FUNCTION for incorrect pattern to check if the other flags are there first and work only if they're off

tokenInfo getNextToken(twinBuffer* B, FILE* fp){
    tokenInfo token;
    lexeme lex;
    int state=0;
    char currchar;
    int len=0;
    int errflag=0;
    int contflag=1;
    int tokentype;
    while (contflag){
        printf("B->index is %d",B->index);

        if(((B->index+len)%30)==0){//Load a buffer if needed
            fp=getStream(B,fp,((B->index+len)%60==0)?0:1);
        }
        currchar=B->buf[(B->index+len)%60];//character to examine- len characters of the token have already been examined
        //printf("currchar is %c",currchar);



//0-9,11-31,34,27,63,92,94,96,123-125
        len++;
        switch (state){
            case 0: 
                printf("In state 0\n");
                printf("B->index is %d\n",B->index);
                printf("currchar is %c\n\n",currchar);
                if((48<=currchar)&&(57>=currchar)){state=1;break;}
                if(currchar=='!'){state=16;break;}
                if(currchar=='='){state=17;break;}
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
                if(isunknown(B->lno,currchar)){
                    errflag=1;
                    contflag=0;
                    break;
                }
                state=75;
                break;
            case 1:
                printf("In state 1\n");
                printf("B->index is %d\n",B->index);
                printf("currchar is %c\n\n",currchar);
                if((48<=currchar)&&(57>=currchar)){state=1;break;}
                if(currchar=='.'){state=3;break;}
                len--;
                token=int_token(B,len);
                B->index=(B->index+len)%60;
                return token;
            case 3:
                if((48<=currchar)&&(57>=currchar)){state=4;break;}
                len-=2;
                token=int_token(B,len);
                B->index=(B->index+len)%60;
                return token;
            case 4:
                if((48<=currchar)&&(57>=currchar)){state=5;break;}
                errflag=2;
                len--;
                contflag=0;
                break;
            case 5:
                if(currchar=='E'){state=7;break;}
                len--;
                token=float_token(B,len);
                B->index=(B->index+len)%60;
                return token;
            case 7:
                if((48<=currchar)&&(57>=currchar)){state=8;break;}
                if(currchar=='+'){state=10;break;}
                if(currchar=='-'){state=10;break;}
                errflag=2;
                len--;
                contflag=0;
                break;
            case 8:
                if((48<=currchar)&&(57>=currchar)){
                    token=float_token(B,len);
                    B->index=(B->index+len)%60;
                    return token;
                }
                errflag=2;
                len--;
                contflag=0;
                break;
            case 10:
                if((48<=currchar)&&(57>=currchar)){state=11;break;}
                errflag=2;
                len--;
                contflag=0;
                break;
            case 11:
                if((48<=currchar)&&(57>=currchar)){
                    token=float_token(B,len);
                    B->index=(B->index+len)%60;
                    return token;
                }
                errflag=2;
                len--;
                contflag=0;
                break;
            case 16:
                if(currchar=='='){
                    contflag=0;
                    tokentype=TK_NE;
                    break;
                }
                errflag=2;
                len--;
                contflag=0;
                break;
            case 17:
                if(currchar=='='){
                    contflag=0;
                    tokentype=TK_EQ;
                    break;
                }
                errflag=2;
                len--;
                contflag=0;
                break;
            case 18:
                if(currchar=='='){
                    contflag=0;
                    tokentype=TK_NE;
                    break;
                }
                errflag=2;
                len--;
                contflag=0;
                break;
            case 21:
                if(currchar=='-'){state=23;break;}
                if(currchar=='='){
                    contflag=0;
                    tokentype=TK_LE;
                    break;
                }
                len--;
                contflag=0;
                tokentype=TK_LT;
                break;
            case 23:
                if(currchar=='-'){state=25;break;}
                len-=2;
                contflag=0;
                tokentype=TK_LT;
                break;
            case 25:
                if(currchar=='-'){
                    contflag=0;
                    tokentype=TK_ASSIGNOP;
                    break;
                }
                errflag=2;
                len--;
                contflag=0;
                break;
            case 30:
                if(currchar=='@'){state=31;break;}
                errflag=2;
                len--;
                contflag=0;
                break;
            case 31:
                if(currchar=='@'){
                    contflag=0;
                    tokentype=TK_OR;
                    break;
                }
                errflag=2;
                len--;
                contflag=0;
                break;
            case 34:
                if(currchar=='&'){state=35;break;}
                errflag=2;
                len--;
                contflag=0;
                break;
            case 35:
                if(currchar=='&'){
                    contflag=0;
                    tokentype=TK_AND;
                    break;
                }
                errflag=2;
                len--;
                contflag=0;
                break;
            case 29:
                len--;
                contflag=0;
                tokentype=TK_NOT;
                break;
            case 38:
                len--;
                contflag=0;
                tokentype=TK_PLUS;
                break;
            case 39:
                len--;
                contflag=0;
                tokentype=TK_MINUS;
                break;
            case 40:
                len--;
                contflag=0;
                tokentype=TK_MUL;
                break;
            case 41:
                len--;
                contflag=0;
                tokentype=TK_DIV;
                break;
            case 42:
                len--;
                contflag=0;
                tokentype=TK_SQL;
                break;
            case 43:
                len--;
                contflag=0;
                tokentype=TK_SQR;
                break;
            case 44:
                len--;
                contflag=0;
                tokentype=TK_OP;
                break;
            case 45:
                len--;
                contflag=0;
                tokentype=TK_CL;
                break;
            case 46:
                len--;
                contflag=0;
                tokentype=TK_SEM;
                break;
            case 47:
                len--;
                contflag=0;
                tokentype=TK_DOT;
                break;
            case 48:
                len--;
                contflag=0;
                tokentype=TK_COLON;
                break;
            case 49:
                len--;
                contflag=0;
                tokentype=TK_COMMA;
                break;
            case 74:
                len--;
                contflag=0;
                tokentype=TK_DOLLAR;
                break;
            case 76:
                B->index=(B->index+1)%60;
                len=0;
                state=0;
                B->lno++;
                break;
            case 69://nice
                if(currchar==' '){state=69;break;}
                B->index=(B->index+len)%60;
                len=0;
                state=0;
                break;
            case 71:
                if(currchar=='\0'){
                    len--;
                    contflag=0;
                    tokentype=TK_COMMENT;
                    break;
                }
                if(currchar=='\n'){
                    tokentype=TK_COMMENT;
                    token=set_lexeme(B,len,tokentype);
                    B->index=(B->index+len)%60;
                    token.err=errflag;
                    B->lno++;
                    break;
                }
                state=71;
                break;
        }
    }
    
    
    token=set_lexeme(B,len,tokentype);
    B->index=(B->index+len)%60;
    
    token.err=errflag;
    return token;

}

//final state action order-
//0) One mandatory retract in all final states, so that B->index now points to the last character seen to get to current state and not the next character to examine
//1) retract again if needed (reduce B->index, changing currbuf and B->index as needed if the new value is negative)
//2) set parameters of token to return (by calling real_token, int_token, lookup and insert or set_lexeme) / print error
//3) set both pointers= (B->index+1)%30 (change currbuf if necessary) if returning token (setting startptr=B->index actually not needed; next call to getNextToken will do this for us anyway
//4) inc line number if applicable
//5) return token/set state to 0

//The setting of the forward and start pointer to the required value is taken care of automatically by the increment at the end of the loop and the setting
//of startpointer=B->index in state 0, in the case where we go to state 0 instead of returning token. Otherwise, it must be done before the return statement.                                                         

//installing token/looking up a token in the symbol table could be handled by the invoking parser function. 
//it should know to do this since insertion/lookup is needed if and only if an identifier token is returned.
//Or we could make a symbol table structure here, initialize it in the parser function and pass it into getNextToken().


/*tokenInfo lookup(){
    tokenInfo token;
    token->lno=B->lno;
}*/

int main(){
    twinBuffer* buffer;
    buffer=(twinBuffer *)malloc(sizeof(twinBuffer));
    buffer->lno=1;
    //buffer->cno=1; 
    int len=0;
    buffer->index=0;
    FILE* test=fopen("test.txt","r");
    test=getStream(buffer,test,0);
    

    //tokenInfo tkinf= getNextToken(buffer,test);

    //printf("value of tkn is %d\n",tkinf.lex.ival);

    printf("Buffer: %s\n", buffer->buf);
    printf("Index: %d\n", buffer->index);
    //printf("Character Number: %d\n", buffer->cno);
    printf("Line Number: %d\n", buffer->lno);

    buffer->index=2;
    len=9;
    tokenInfo tkinf=float_token(buffer,len);
    printf("floatval is %f\n",tkinf.lex.rval);
    //tkinf= getNextToken(buffer,test);
    //printf("value of tkn is %d\n",tkinf.lex.ival);

    printf("Buffer: %s\n", buffer->buf);
    printf("Index: %d\n", buffer->index);
    //printf("Character Number: %d\n", buffer->cno);
    printf("Line Number: %d\n", buffer->lno);

    /*buffer->index=4;
    len=7;
    tokenInfo tkinf= set_lexeme(buffer,len,TK_ID);
    printf("value of tkn is %s\n",tkinf.lex.strlex);

    printf("Buffer: %s\n", buffer->buf);
    printf("Index: %d\n", buffer->index);
    //printf("Character Number: %d\n", buffer->cno);
    printf("Line Number: %d\n", buffer->lno);


    //buffer->index=2;
    //tokenInfo tkinf=float_token(buffer,28);
    //printf("%f\n",tkinf.lex.rval);
    */
    return 0;
}
