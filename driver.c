#include "leggser.h"
#include "parser.h"

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
    tokenInfo tkinf;
    
    int option=-1;
    while(1){
        printf("Enter a number according to the following options:\n\t0 - Exit\n\t1 - Print comment-free code\n\t2 - Print token list\n\t3 - Print parse tree\n\t4 - Print time taken\n");
        scanf("%d",&option);
    

    if(option==1){removeComments("test.txt","NONE");}
    else if(option==2){
        while(1){
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
    }
    else if(option==0)return 0;
    else if(option==3){
        printf("hello testing\n");
    FirstFollowSet* firstFollowSets = ComputeFirstFollowSets();
    //printf("\nPrinting FIRST sets:\n");
    //printf("===================\n");
   // printFirstSets();
    storeGrammarRules(); 
    //printGrammarRules();
    createParseTable(firstFollowSets, &parseTable);
    // printParseTable(parseTable, nonterminalNames, terminalNames); 
    char* myfile="t6.txt";
    ParseTreeNode *mytree=parseInputSourceCode(myfile,parseTable,firstFollowSets);
    char* parseTreeFile="t1.txt";
    printParseTree(mytree,parseTreeFile);

    //test=getStream(buffer,test,0);
    //test=getStream(buffer,test,1);
    }
    }
}
