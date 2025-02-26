#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

struct twinBuffer B;
int cno=0;
int lno=0;
//running count of line and character, so as to populate the fields in the tokenInfo structure in getNextToken(twinBuffer B) 


void removeComments(char *testcaseFile, char *cleanFile){//assumes char array terminates with \0, and returns a char array terminated with \0 as well
    free(cleanFile);

    cleanFile=malloc((strlen(testcaseFile)+1)*sizeof(char));
    int i=0,j=0;
    while(testcaseFile[i]!='\0'){
        if(testcaseFile[i]=='%'){
            while((testcaseFile[i]!='\0')&&(testcaseFile[i]!='\n')){i++;}
        }
        cleanFile[j]=testcaseFile[i];
        i++;j++;
    }
    cleanFile[j]='\0';
}

FILE *getStream(FILE *fp) {//populates twin buffer(a global variable) with 200 characters  
    fgets(B.buf1,100,fp);
    fgets(B.buf2,100,fp);
    return fp;
}

//simple function that converts a string to an integer
int stringToInt(const char *str) {
    int result = 0;
    int sign = 1;
    

    if (*str == '-') {
        sign = -1;
        str++; 
    }

    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    return result * sign;
}


/*tokenInfo getNextToken(twinBuffer B){

}*/

void main(){
    printf("hello");
}