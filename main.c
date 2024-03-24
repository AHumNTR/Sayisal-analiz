#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include"sayisalAnaliz.h"




int main(){
    char input[1000],**elementsChar=(char**)malloc(0);
    baseElement *elements=malloc(sizeof(baseElement));
    int elementCount=0,constantCount=0,x=0,c=0;
    input[0]='(';
    while(input[0]=='('){
        printf("Please enter the function\n");
        fgets(input,1000,stdin);
        printf("Please enter the value of x\n");
        scanf("%d",&x);
        checkAndConvertToParanthesesElements(input,elements);
        printf("%lf\n",getValueOfElement(elements,x));
        freeMemoryOfElement(elements);
        elements=malloc(sizeof(baseElement));
        elementCount=0,constantCount=0;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return 0;
}

