#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


#define MAX_SIZE 100
bool hasError=false;

void pushN(int num);
int popN();
void pushO(char op);
char popO();
int priority(char ex);
int calculate(int num1,int num2,char op);
void parseNumber(char *exp,int *index,bool *isoperator);
int eval(char *exp);
int numS[MAX_SIZE];
int topn=-1;
int opS[MAX_SIZE];
int topo=-1;

void pushN(int num){
    if(topn>=MAX_SIZE-1){
        printf("Stack Overflow\n");
        hasError=true;
    }else{
    topn++;
    numS[topn] = num;
    }
}
int popN(){
    int num ;
    if(topn<0){
        printf("Stack Underflow\n");
        hasError=true;
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
        hasError=true;
    }else{
        topo++;
        opS[topo]=op;
    }
}
char popO(){
    char ex;
    if(topo<0){
        printf("Stack UnderFLow\n");
        hasError=true;
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
                hasError=true;
                printf("Division by zero\n");
                return 0;
            }else{
                res=num1/num2;

            }
            break;
        default:
            printf("Invalid operator\n");
            hasError=true;
            res=0;
            break;
        }
        return res;
}
void parseNumber(char *exp,int *index,bool *isoperator){
    int num =0;
    int i=*index;
            while(isdigit(exp[i])){
                num=num*10+(exp[i]-'0');
                i++;
                int nextNonSpaceIndex=i;
                while(exp[nextNonSpaceIndex]==' '){
            nextNonSpaceIndex++;
        }
        if(isdigit(exp[nextNonSpaceIndex])){
            i=nextNonSpaceIndex;
        }
        }
            pushN(num);
            *index=i;
            *isoperator=false;
        
}
int eval(char *exp){
    int i=0;
    int res=0;
    hasError=false;
    bool isoperator=true;
    while(exp[i]!='\0'){
        
        if(exp[i]==' '||exp[i]=='\n'){
            i++;
            continue;
        }
        if(isdigit(exp[i])){
            parseNumber(exp,&i,&isoperator);
            continue;
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
            isoperator=true;
        }else{
            hasError=true;
            printf("Invalid character: %c\n",exp[i]);
            return 0;
        }
        i++;

    }
    if(isoperator){
        printf("Expression cannot end with an operator\n");
        hasError=true;
        return 0;
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
    if(hasError){
        printf("Error occurred during evaluation.\n");
        return 1;
    }else{
        printf("Result: %d\n",Res);
    }
    
    return 0;
}
