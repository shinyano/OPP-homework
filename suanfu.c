#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

char stack[1010]="#\0";
int top=0;
char input[1010]="\0";
int pointer=0;
int length;
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
void error();
void success();
void reduceError();
void reduceSuccess();

void analyse()
{
    length = strlen(input);
    while(1){
        pushIn();
    }
    
    // if(!(top == 1 && stack[top] == 'E') || pointer != length-1){
    //     printf("RE\n");
    //     exit(0);
    // }
}

void pushIn(){
    int inner = find(stack[top]);
    int outer = find(input[pointer]);

    if(outer == -2){
        error();
    }
    if(inner == -1 && top > 0){
        inner=find(stack[top-1]);
        if(inner < 0){
            error();
        }
    } else if(inner == -2){
        error();
    }

    if(inner == outer && outer == Border){
        success();
    }

    int relation = priority[inner][outer];
    // printf("%s\n",stack);
    // printf("%d%c %d%c\n",top,stack[top],pointer,input[pointer]);
    // printf("%d %d %d\n",inner,outer,Border);

    if(relation == -2){
        //printf("%s\n",stack);
        error();
    } else if(relation == -1 || relation == 0){
        stack[++top]=input[pointer++];
        printf("I%c\n",stack[top]);
        if(relation == 0){
            if(reduce() == 0)
                printf("R\n");
            // else{
            //     printf("RE\n");
            //     exit(0);
            // }
        }
    } else {
        if(reduce() == 0)
            printf("R\n");
    }

}


int reduce(){
    if(stack[top]=='i'){
        stack[top]='E';
        return 0;
    } else if(top>=2 && (stack[top-2] == '(' && stack[top-1] == 'E' && stack[top] == ')')){
        top-=2;
        stack[top] = 'E';
        return 0;
    } else if(top>=2 && (stack[top-2] == 'E' && stack[top-1] == '*' && stack[top] == 'E')){
        top-=2;
        stack[top] = 'E';
        return 0;
    } else if(top>=2 && (stack[top-2] == 'E' && stack[top-1] == '+' && stack[top] == 'E')){
        top-=2;
        stack[top] = 'E';
        return 0;
    } else {
        reduceError();
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
        return symbol;
    case '*':
        symbol=Mult;
        return symbol;
    case '(':
        symbol=LPar;
        return symbol;
    case ')':
        symbol=Rpar;
        return symbol;
    case 'i':
        symbol=I;
        return symbol;
    case '#':
        symbol=Border;
        return symbol;

    case 'E':
        return -1;
    
    default:
        return -2;
    }
}

void error(){
    printf("E\n");
    exit(0);
}

void reduceSuccess(){
    printf("R\n");
}

void reduceError(){
    printf("RE\n");
    exit(0);
}

void success(){
    exit(0);
}