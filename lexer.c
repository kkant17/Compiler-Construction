#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "lexerDef.h"

const char* terminalNames[MAX_TERMINALS] = {
    "TK_ASSIGNOP",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RUID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "TK_EPS",
    "TK_DOLLAR",
    "TK_COMMENT"
};


//running count of line and character, so as to populate the fields in the tokenInfo structure in getNextToken(twinBuffer B) 


void removeComments(char *testcaseFile, char *cleanFile){//assumes char array terminates with \0, and returns a char array terminated with \0 as well
    FILE* tcf=fopen(testcaseFile,"r");
    FILE*  cf=fopen(cleanFile,"w");

    if(tcf==NULL){
        printf("Error: file %s not found in directory",testcaseFile);
    }
    
    int flag=0;
    char temp=fgetc(tcf);
    while(temp!=EOF){
        if(temp=='%'){
           flag=1;
        }
        if((flag==1)&&(temp=='\n')){flag=0;}
        else if(!flag) {
            if(cf!=NULL)fputc(temp,cf);
            fputc(temp,stdout);
        }
        temp=fgetc(tcf);
    }
    fclose(tcf);
}

FILE *getStream(twinBuffer* buffer, FILE *fp, int bufno) {//populates twin buffer(a global variable) with 200 characters  
    int charsread;

    if (fp == NULL) {
        printf("Error: File pointer is NULL\n");
        return NULL;
    }
    if (bufno==1){
        //printf("0 to 1\n");
        charsread=fread(buffer->buf+30, sizeof(char), 30, fp);
        if(charsread!=30){buffer->buf[30+charsread]='\0';}
        //buffer->index=30;
    }
    else if (bufno==0){
        //printf("1 to 0\n");
        charsread=fread(buffer->buf, sizeof(char), 30, fp);
        if(charsread!=30){buffer->buf[charsread]='\0';}
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
    //printf("fwdptr is %d",fwdptr);
    tokenInfo token;
    token.err=0;
    token.lno=B->lno;  
    token.tkid=toktype;
    if(fwdptr<B->index){
        strncpy(token.strlex,B->buf+B->index,60-B->index);
        strncpy(token.strlex+60-B->index,B->buf,fwdptr+1);
        token.strlex[len]='\0';
    }
    else{
        strncpy(token.strlex,B->buf+B->index,len);
        token.strlex[len]='\0';
    }
    return token;
}


//start index in wraparound case is (B->index-len+1+60)%60

tokenInfo int_token(twinBuffer* B, int len){
    tokenInfo token;
    token=set_lexeme(B,len,TK_NUM);
    int value=0;
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
    
    token.val.rval=0;
    token.val.ival=value;
    return token;
}

tokenInfo float_token(twinBuffer* B, int len){
    tokenInfo token;    
    token=set_lexeme(B,len,TK_RNUM);
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

    token.val.ival=0;
    token.val.rval=value;
    return token;
    
}



int isunknown(char c){
            if((c<=8)&&(c>0)||((c>=11)&&(c<=31))||c==27||c==34||c==36||c==63||c==92||c==94||c==96||((c>=123)&&(c<=125))||c>125){return 1;}
            else return 0;
}

//symbol table strcmp binary search
tokenInfo lookup(twinBuffer* B, int len, lookuptbl* table, tk toktype){
    tokenInfo tkinf=set_lexeme(B,len,toktype);
    //check if len exceeded, if so just return tkinf
    int low=0, high=27, mid=13;
    int cmp;

    while(low<=high){
        mid=(low+high)/2;
        cmp=strcmp(tkinf.strlex,table[mid].keyw);
        if(cmp==0){
            tkinf.tkid=table[mid].tkid;
            high=low-1;
        }
        else if(cmp<0){
            high=mid-1;
        }
        else{
            low=mid+1;
        }
    }

    return tkinf;
}





//NEED ERROR FUNCTION for incorrect pattern to check if the other flags are there first and work only if they're off

tokenInfo getNextToken(twinBuffer* B, FILE* fp, lookuptbl* table){
    tokenInfo token;
    //lexeme lex;
    int state=0;
    char currchar;
    int len=0;
    int errflag=0;
    int contflag=1;
    int tokentype;
    while (contflag){
        //printf("B->index is %d",B->index);

        if(((B->index+len)%30)==0){//Load a buffer if needed
            int buftoload=((B->index+len)%60==0)?0:1;
            if(B->loadedbuf!=buftoload) fp=getStream(B,fp,buftoload);
            B->loadedbuf=buftoload;
        }
        currchar=B->buf[(B->index+len)%60];//character to examine- len characters of the token have already been examined
        //printf("currchar is %c",currchar);



//0-9,11-31,34,27,63,92,94,96,123-125
        len++;
        switch (state){
            case 0: 
                //printf("In state 0\n");
                //printf("B->index is %d\n",B->index);
                //printf("currchar is %c\n\n",currchar);
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
                if(currchar=='a'){state=56;break;}//fixed this
                if(currchar=='#'){state=50;break;}
                if(currchar==9){state=72;break;}
                if(isunknown(currchar)){
                    //printf("errflag set condition reached\n");
                    errflag=1;
                    contflag=0;
                    break;
                }
                state=75;
                break;
            case 1:
                //printf("In state 1\n");
                //printf("B->index is %d\n",B->index);
                //printf("currchar is %c\n\n",currchar);
                if((48<=currchar)&&(57>=currchar)){state=1;break;}
                if(currchar=='.'){state=3;break;}
                len--;
                if(len>30){
                contflag=0;
                token.tkid=TK_NUM;
                break;
                }
                token=int_token(B,len);
                B->index=(B->index+len)%60;
                return token;
            case 3:
                if((48<=currchar)&&(57>=currchar)){state=4;break;}
                len-=2;
                if(len>30){
                contflag=0;
                token.tkid=TK_NUM;
                break;
                }
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
                if(len>30){
                contflag=0;
                token.tkid=TK_RNUM;
                break;
                }
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
                    if(len>30){
                      contflag=0;
                      token.tkid=TK_RNUM;
                      break;
                    }
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
                    if(len>30){
                      contflag=0;
                      token.tkid=TK_RNUM;
                      break;
                    }
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
                    tokentype=TK_GE;
                    break;
                }
                len--;
                contflag=0;
                tokentype=TK_GT;//earlier this was errflag=2;
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
                contflag=0;
                tokentype=TK_DOLLAR;
                token=set_lexeme(B,len,tokentype);
                return token;
            case 76:
                B->index=(B->index+1)%60;
                len=0;
                state=0;
                B->lno++;
                break;
            case 69://nice
                if(currchar==' '){state=69;break;}
                len--;//we need a retract here so as not to see the next char
                B->index=(B->index+len)%60;
                //printf("here index is %d\n",B->index);
                len=0;
                state=0;
                break;
            case 71:
                if(currchar=='\0'){
                    tokentype=TK_COMMENT;
                    token.lno=B->lno;
                    token.strlex[0]='%';
                    token.strlex[1]='\0';
                    token.tkid=tokentype;
                    len--;
                    token.err=errflag;
                    B->index=(B->index+len)%60;
                    return token;
                }
                if(currchar=='\n'){
                    tokentype=TK_COMMENT;
                    token.lno=B->lno;
                    token.strlex[0]='%';
                    token.strlex[1]='\0';
                    token.tkid=tokentype;
                    token.err=errflag;
                    B->index=(B->index+len)%60;
                    B->lno++;
                    return token;//break was used here earlier
                }
                state=71;
                break;
            //started from here
            case 50:
                if((97<=currchar)&&(122>=currchar)){state=51;break;}
                errflag=2;
                len--;
                contflag=0;
                break;
            case 51:
                if((97<=currchar)&&(122>=currchar)){state=51;break;}
                len--;
                if(len>30){
                contflag=0;
                token.tkid=TK_RUID;
                break;
                }
                contflag=0;
                tokentype=TK_RUID;
                break;
            case 53:
                if(((97<=currchar)&&(122>=currchar))||((65<=currchar)&&(90>=currchar))){state=54;break;}
                errflag=2;
                len--;
                contflag=0;
                break;
            case 54:
                if(((97<=currchar)&&(122>=currchar))||((65<=currchar)&&(90>=currchar))||((48<=currchar)&&(57>=currchar))){state=54;break;}
                len--;
                if(len>30){
                contflag=0;
                token.tkid=TK_FUNID;
                break;
                }
                token=lookup(B,len,table,TK_FUNID);//could be _main here
                B->index=(B->index+len)%60;

                return token;
            case 56:
                if(((97<=currchar)&&(122>=currchar))||((65<=currchar)&&(90>=currchar))){state=56;break;}
                len--;
                if(len>30){
                contflag=0;
                token.tkid=TK_FIELDID;
                break;
                }
                token=lookup(B,len,table,TK_FIELDID);
                B->index=(B->index+len)%60;
                return token;
            case 58:
                if((97<=currchar)&&(122>=currchar)){state=60;break;}
                if((50<=currchar)&&(55>=currchar)){state=62;break;}
                len--;
                if(len>30){
                contflag=0;
                token.tkid=TK_FIELDID;
                break;
                }
                token=lookup(B,len,table,TK_FIELDID);
                B->index=(B->index+len)%60;
                return token;
            case 60:
                if((97<=currchar)&&(122>=currchar)){state=60;break;}
                len--;
                if(len>30){
                contflag=0;
                token.tkid=TK_FIELDID;
                break;
                }
                token=lookup(B,len,table,TK_FIELDID);
                B->index=(B->index+len)%60;
                return token;
            case 62:
                if((currchar<=100)&&(currchar>=98)){state=64;break;}
                len--;
                contflag=0;
                tokentype=TK_ID;
                break;
            case 64:
                if((currchar<=100)&&(currchar>=98)){state=64;break;}
                if((50<=currchar)&&(55>=currchar)){state=66;break;}
                len--;
                if(len>20){
                B->index=(B->index+len)%60;
                token.tkid=TK_ID;
                token.err=3;
                token.lno=B->lno;
                return token;
                }
                tokentype=TK_ID;
                contflag=0;
                break;
            case 66:
                if((50<=currchar)&&(55>=currchar)){state=66;break;}
                len--;
                if(len>20){
                B->index=(B->index+len)%60;
                token.tkid=TK_ID;
                token.err=3;
                token.lno=B->lno;
                return token;
                }
                tokentype=TK_ID;
                contflag=0;
                break;
            case 72://nice
                if(currchar==9){state=72;break;}
                len--;
                B->index=(B->index+len)%60;
                len=0;
                state=0;
                break;

        }
    }
    
    if(len>30){
        B->index=(B->index+len)%60;
        token.err=3;
        token.lno=B->lno;
        printf("Token is too long\n\n");
        return token;
    }

    //printf("loop broken\n");
    token=set_lexeme(B,len,tokentype);
    B->index=(B->index+len)%60;
    token.err=errflag;
    return token;

}

/*HEAD tokenlist(twinBuffer* B, FILE* fp, lookuptbl* table){
     HEAD list=(HEAD)malloc(sizeof(struct llhead));
     list->head=NULL;
     list->tail=NULL;
     tokenInfo tkinf=getNextToken(B,fp,table);
     NODE nextnode;
     while(tkinf.tkid!=TK_DOLLAR){
     nextnode=(NODE)malloc(sizeof(struct llnode));
     nextnode->tkinf=tkinf;
     nextnode->next=NULL;
     if(list->tail==NULL){list->tail=nextnode;list->head=nextnode;}
     else {list->tail->next=nextnode; list->tail=nextnode;}
     tkinf=getNextToken(B,fp,table);
     while(tkinf.err){tkinf=getNextToken(B,fp,table);}
     }
     return list;
}*/

/*void printtokenlist(HEAD list){
    NODE node= list->head;
    while(node!=NULL){
        printf("Token lexeme is %s and type is %s\n",node->tkinf.strlex,terminalNames[node->tkinf.tkid]);
        node=node->next;
    }
}*/

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

/*
int main(){
    lookuptbl table[28];
    strcpy(table[0].keyw,"_main");table[0].tkid=TK_MAIN;
    strcpy(table[1].keyw,"as");table[1].tkid=TK_AS;
    strcpy(table[2].keyw,"call");table[2].tkid=TK_CALL;
    strcpy(table[3].keyw,"definetype");table[3].tkid=TK_DEFINETYPE;
    strcpy(table[4].keyw,"else");table[4].tkid=TK_ELSE;
    strcpy(table[5].keyw,"end");table[5].tkid=TK_END;
    strcpy(table[6].keyw,"endif");table[6].tkid=TK_ENDIF;
    strcpy(table[7].keyw,"endrecord");table[7].tkid=TK_ENDRECORD;
    strcpy(table[8].keyw,"endunion");table[8].tkid=TK_ENDUNION;
    strcpy(table[9].keyw,"endwhile");table[9].tkid=TK_ENDWHILE;
    strcpy(table[10].keyw,"global");table[10].tkid=TK_GLOBAL;
    strcpy(table[11].keyw,"if");table[11].tkid=TK_IF;
    strcpy(table[12].keyw,"input");table[12].tkid=TK_INPUT;
    strcpy(table[13].keyw,"int");table[13].tkid=TK_INT;
    strcpy(table[14].keyw,"list");table[14].tkid=TK_LIST;
    strcpy(table[15].keyw,"output");table[15].tkid=TK_OUTPUT;
    strcpy(table[16].keyw,"parameter");table[16].tkid=TK_PARAMETER;
    strcpy(table[17].keyw,"parameters");table[17].tkid=TK_PARAMETERS;
    strcpy(table[18].keyw,"read");table[18].tkid=TK_READ;
    strcpy(table[19].keyw,"real");table[19].tkid=TK_REAL;
    strcpy(table[20].keyw,"record");table[20].tkid=TK_RECORD;
    strcpy(table[21].keyw,"return");table[21].tkid=TK_RETURN;
    strcpy(table[22].keyw,"then");table[22].tkid=TK_THEN;
    strcpy(table[23].keyw,"type");table[23].tkid=TK_TYPE;
    strcpy(table[24].keyw,"union");table[24].tkid=TK_UNION;
    strcpy(table[25].keyw,"while");table[25].tkid=TK_WHILE;
    strcpy(table[26].keyw,"with");table[26].tkid=TK_WITH;
    strcpy(table[27].keyw,"write");table[27].tkid=TK_WRITE;


    twinBuffer* buffer;
    buffer=(twinBuffer *)malloc(sizeof(twinBuffer));
    buffer->lno=1; 
    buffer->index=0;
    buffer->loadedbuf=-1;
    int len=0;
    FILE* test=fopen("test.txt","r");
    //test=getStream(buffer,test,0);
    //test=getStream(buffer,test,1);
    
    int i=5;
    tokenInfo tkinf;


    //HEAD list= tokenlist(buffer,test,table);
    //printtokenlist(list);



    

    while(true){
    tkinf= getNextToken(buffer,test,table);
    if(tkinf.tkid==TK_DOLLAR){break;}
    if(tkinf.err==0)printf("Line no. %-5d    Lexeme %-35s   Token %-20s\n",tkinf.lno,tkinf.strlex,terminalNames[tkinf.tkid]);
    else if(tkinf.err==3)
        {
         if(tkinf.tkid==TK_NUM){
          printf("Line No %-5d : Error :Integer is longer than the maximum length of 30 digits\n",tkinf.lno);
         }
         else if(tkinf.tkid==TK_RNUM){
          printf("Line No %-5d : Error :Floating-point number is longer than the maximum length of 30 characters\n",tkinf.lno);
         }
         else if(tkinf.tkid==TK_ID){
          printf("Line No %-5d : Error :Variable is longer than the prescribed length of 20 characters\n",tkinf.lno);
         }
         else{
          printf("Line No %-5d : Error :Record, union, field or function identifier is longer than the maximum length of 30 characters\n",tkinf.lno);
         }
        }
    else if(tkinf.err==2)printf("Line no: %-5d : Error: Unknown pattern <%s>\n",tkinf.lno,tkinf.strlex);
    else printf("Line No %-5d : Error: Unknown Symbol <%s>\n",tkinf.lno,tkinf.strlex); 
    }

    return 0;
}*/
