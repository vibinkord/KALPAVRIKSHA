#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_SIZE 100

int numS[MAX_SIZE];
int topn=-1;
int opS[MAX_SIZE];
int topo=-1;

void pushN(int num){
    if(topn>=MAX_SIZE-1){
        printf("Stack Overflow\n");
    }else{
    topn++;
    numS[topn] = num;
    }
}
int popN(){
    int num ;
    if(topn<0){
        printf("Stack Underflow\n");
        num = 0;
    }else{
        num= numS[topn];
        topn--;
    }
    return num;
}
void pushO(char op){
    if(topo>=MAX_SIZE-1){
        printf("Stack Overflow\n");
    }else{
        topo++;
        opS[topo]=op;
    }
}
char popO(){
    char ex;
    if(topo<0){
        printf("Stack UnderFLow\n");
        ex=' ';
    }else{
        ex=opS[topo];
        topo--;
    }
    return ex;
}
int priority(char ex){
    if(ex=='+'||ex=='-'){
        return 1;
    }else if(ex=='*'||ex=='/'){
        return 2;
    }else{
        return 0;
    }
}
int calculate(int num1,int num2,char op){
    int res;
    switch(op){
        case'+':
            res=num1+num2;
            break;
        case'-':
            res=num1-num2;
            break;
        case'*':
            res=num1*num2;
            break;
        case'/':
            if(num2==0){
                printf("Division by zero\n");
                return 0;
            }else{
                res=num1/num2;

            }
            break;
        default:
            printf("Invalid operator\n");
            res=0;
            break;
        }
        return res;
}
int eval(char *exp){
    int i=0;
    int res=0;
    while(exp[i]!='\0'){
        if(exp[i]==' '){
            i++;
            continue;
        }
        if(isdigit(exp[i])){
            int num =0;
            while(isdigit(exp[i])){
                num=num*10+(exp[i]-'0');
                i++;
            }
            pushN(num);
        }
        else if(exp[i]=='+'||exp[i]=='-'||exp[i]=='*'||exp[i]=='/'){
            while(topo!=-1 &&priority(exp[i])<=priority(opS[topo])){
                int num2=popN();
                int num1=popN();
                char ex=popO();
                res=calculate(num1,num2,ex);
                pushN(res);
            }
            pushO(exp[i]);
        }else{
            printf("Invalid character: %c\n",exp[i]);
            return 0;
        }
        i++;

    }
    while(topo!=-1){
        int num2=popN();
        int num1=popN();
        char ex=popO();
        res=calculate(num1,num2,ex);
        pushN(res);
    }
    return popN();
}
int main(){
    char exp[100];
    fgets(exp,sizeof(exp),stdin);
    int Res=eval(exp);
    printf("Result: %d\n",Res);
    return 0;
}