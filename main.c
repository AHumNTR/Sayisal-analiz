#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include"sayisalAnaliz.h"

int main(){
    char input[1000],**elementsChar=(char**)malloc(0);
    baseElement *elements;//=malloc(sizeof(baseElement)),*derivitive;
    int elementCount=0,constantCount=0,c=0;
    double x=0;
    input[0]='(';
    elements=readAndCreateMatrix(4,4);
    printf("%lf",findDeterminentOfMatrix(elements));
    /*while(input[0]=='('){
        printf("Please enter the function\n");
        fgets(input,1000,stdin);
        checkAndConvertToParanthesesElements(input,elements);
        //numericalDerivative(elements);
        printf("Please enter the value of x\n");
        scanf("%lf",&x);
        checkAndConvertToParanthesesElements(input,elements);
        printf("%lf\n",getValueOfElement(makeCopyOfElement(elements),x));
        derivitive=derivate(elements);
        printf("derivitive =%lf\n",getValueOfElement(derivitive,x));
        printElement(derivitive);
        printf("\n");
        freeMemoryOfElement(elements);
        freeMemoryOfElement(derivitive);
        elements=malloc(sizeof(baseElement));
        elementCount=0,constantCount=0;
        while ((c = getchar()) != '\n' && c != EOF);
    }*/
    return 0;
}

