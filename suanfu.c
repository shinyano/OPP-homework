#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

char stack[1010]="#\0";
int top=0;
char input[1010]="\0";
int pointer=0;
enum symbols{
    Plus,Mult,LPar,Rpar,I,Border,
} symbol;
int priority[6][6]={
    {1,-1,-1,1,-1,1},
    {1,1,-1,1,-1,1},
    {-1,-1,-1,0,-1,-2},
    {1,1,-2,1,-2,1},
    {1,1,-2,1,-2,1},
    {-1,-1,-1,-2,-1,-2},
};
FILE *fp;

void analyse();
int find(char);
int reduce();
void pushIn();

void analyse()
{
    int length = strlen(input);
    while(pointer<length){
        pushIn();
    }
    if(strcmp(stack,"#E")!=0 || strcmp(input, "#")!=0){
        printf("RE\n");
    }
}

void pushIn(){
    int inner = find(stack[top]);
    int outer = find(input[pointer]);

    if(outer == -1){
        printf("E\n");
        exit(0);
    }
    if(inner == -1 && top > 0){
        inner=find(stack[top-1]);
        if(inner == -1){
            printf("E\n");
            exit(0);
        }
    } else if(inner == -1){
        printf("E\n");
        exit(0);
    }

    int relation = priority[inner][outer];

    if(relation == -2){
        printf("E\n");
        exit(0);
    } else if(relation == -1){
        stack[++top]=input[pointer++];
        printf("I%c\n",stack[top]);
    } else {
        if(relation == 0){
            stack[++top]=input[pointer++];
            printf("I%c\n",stack[top]);
        }
        while(reduce() == 0)
            printf("R\n");
    }

}


int reduce(){
    if(stack[top]=='i'){
        stack[top]='F';
        return 0;
    } else if(top>=2 && (stack[top-2] == '(' && stack[top-1] == 'E' && stack[top] == ')')){
        top-=2;
        stack[top] = 'F';
        return 0;
    } else if(top>=2 && (stack[top-2] == 'T' && stack[top-1] == '*' && stack[top] == 'F')){
        top-=2;
        stack[top] = 'T';
        return 0;
    } else if(top>=2 && (stack[top-2] == 'E' && stack[top-1] == '+' && stack[top] == 'T')){
        top-=2;
        stack[top] = 'E';
        return 0;
    } else {
        return -1;
    }
}



int main(int argc, char **argv)
{
    fp=fopen(argv[1],"r");
    if(fp==NULL){
        printf("error:%s\n",strerror(errno));
        return -1;
    }
    fgets(input,1010,fp);
    int i=0;
    while(input[i]!='\r' && input[i]!='\n' && input[i]!='\0') i++;
    input[i]='#';input[i+1]='\0';

    analyse();
    
    fclose(fp);
    return 0;
}

int find(char c){
    switch (c)      
    {
    case '+':
        symbol=Plus;
    case '*':
        symbol=Mult;
    case '(':
        symbol=LPar;
    case ')':
        symbol=Rpar;
    case 'i':
        symbol=I;
    case '#':
        symbol=Border;
        return symbol;
    
    default:
        return -1;
    }
}
