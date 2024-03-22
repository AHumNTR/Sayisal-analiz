#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include"sayisalAnaliz.h"




int main(){
    char input[1000],**elementsChar=(char**)malloc(0);
    baseElement *elements=malloc(sizeof(baseElement));
    fgets(input,1000,stdin);
    int i=0,elementCount=0,constantCount=0;
    checkAndConvertToParanthesesElements(input,elements);
    printf("%lf",getValueOfElement(elements,3));
//checkAndConvertToParanthesesElements(input,elements);
//printf("%lf",getValueOfElement(elements,2));

   /*splitIntoAdditionElements(&elementsChar,input,&elementCount);
   convertAllElementsIntoTypes(&elementsChar,elementCount,&elements,&constantCount);
   float *temp=((float*)(elements[0].ptr));
   for(i=0;i<elementCount;i++){

    printf("%lf\n",*((double*)(elements[i].ptr)));
   }*/
    return 0;
}

