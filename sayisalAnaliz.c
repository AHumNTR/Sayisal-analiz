#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<math.h>
#include"sayisalAnaliz.h"

void addElementToTheList(char ***elementsString,char *input,int *startIndex,int endIndex,int *elementCount){
            *elementsString= (char**)realloc(*elementsString,(*elementCount+1)*sizeof(char*));
            (*elementsString)[*elementCount]=calloc(endIndex-*startIndex+1,(sizeof(char)));
            memcpy((*elementsString)[*elementCount],input+*startIndex,endIndex-*startIndex);
            (*elementsString)[*elementCount][endIndex-*startIndex]='\0';
            *elementCount+=1;
            *startIndex=endIndex;
}
void splitIntoAdditionElements(char ***elementsString,char *input,int *elementCount){
    int lastElementIndex=0,i=0,paranthesesDeepnes=0;
    while(input[i]!=0 &&input[i]!='\n'){
        if((input[i]=='+' || input[i]=='-')&&paranthesesDeepnes==0){
            addElementToTheList(elementsString,input,&lastElementIndex,i,elementCount);
        }
        else if(input[i]=='(')paranthesesDeepnes+=1;
        else if(input[i]==')')paranthesesDeepnes-=1;
        i++;
    }
    addElementToTheList(elementsString,input,&lastElementIndex,i,elementCount);
}
void splitIntoMultipclationElements(char ***elementsString,char *input,int *elementCount){
    int lastElementIndex=0,i=0,paranthesesDeepnes=0;
    while(input[i]!=0 &&input[i]!='\n'){
        if((input[i]=='*' || input[i]=='/')&&paranthesesDeepnes==0){
            addElementToTheList(elementsString,input,&lastElementIndex,i,elementCount);
        }
        else if(input[i]=='(')paranthesesDeepnes+=1;
        else if(input[i]==')')paranthesesDeepnes-=1;
        i++;
    }
    addElementToTheList(elementsString,input,&lastElementIndex,i,elementCount);
    
}
void splitIntoExponentialElements(char ***elementsString,char *input,int *elementCount){
int lastElementIndex=0,i=0,paranthesesDeepnes=0;//represents how many parantheses deep the thing is in so it dont split a exponential inside a parantheses 
    while(input[i]!=0 &&input[i]!='\n'){
        if((input[i]=='^')&&paranthesesDeepnes==0){//idk if smaller than 0 could happenbut it might be a problem
            addElementToTheList(elementsString,input,&lastElementIndex,i,elementCount);
        }
        else if(input[i]=='(')paranthesesDeepnes+=1;
        else if(input[i]==')')paranthesesDeepnes-=1;

        i++;
    }
    addElementToTheList(elementsString,input,&lastElementIndex,i,elementCount);
}

bool checkAndConvertToConstantElement(char *elementString,baseElement *constant){
    int i =0,j=0,dotsIndex=-1;
    double *temp=malloc(sizeof(double));
    bool isNegative=false,hasSign=true;
    if(elementString[0]=='-')isNegative=true;
    else if (elementString[0]!='+')hasSign=false;
    i=hasSign;
    while(elementString[i]!='\0'){
        if(elementString[i]=='.')dotsIndex=i; //we need to know where the dot is
        else if(!(elementString[i]>='0'&&elementString[i]<='9'))return false; //check if its a number if not return
        i++;
    }
    if(dotsIndex==-1)dotsIndex=i;
    for(j=dotsIndex+1;j<i;j++){//add whats after the dot
        *temp+= pow(10,dotsIndex-j)*(elementString[j]-'0');
    }
    for(j=dotsIndex-1;j>=hasSign;j--){//add whats before the dot
        *temp+= pow(10,dotsIndex-1-j)*(elementString[j]-'0');
    }
    if(isNegative)*temp=*temp*-1;
    
    constant->type=constantElementType;
    constant->ptr=temp;
    return true;
}
bool checkAndConvertToParanthesesElements(char *input, baseElement *parantheses){
    char **elementStrings=(char**)malloc(sizeof(char*));
    bool hasSign=true,isNegative=false;
    int elementStringCount=0,i=0,elementCount=0;
    baseElement *elements=malloc(sizeof(baseElement));//element array to be stored in the parantheseselement
    if(input[0]=='-'){
        isNegative=true;
    }
    else if (input[0]!='+')hasSign=false;
    while(input[i]!='\n'&&input[i]!='\0')i++;
    char* substr= substring(input,1+hasSign,i-2);
    splitIntoAdditionElements(&elementStrings,substr,&elementStringCount);
    convertAllElementsIntoTypes(&elementStrings,elementStringCount,&elements,&elementCount);
    
    paranthesesElement *paranthesesPtr=malloc(sizeof(paranthesesElement)); //create the parantheses element
    parantheses->ptr=paranthesesPtr;//set the pointer for the base element
    parantheses->type= paranthesesElementType;//set the type
    paranthesesPtr->elementArray=elements;//set the array on the parantheses element
    paranthesesPtr->elementCount=elementCount;//set the element count
    for(i=0;i<elementStringCount;i++){//free the string array
        free(elementStrings[i]);
    }
    free(substr);
    free(elementStrings);
    return true;
}
bool checkAndConvertToPolinominal(char *input, baseElement *polinom){
    char **elementsString=malloc(sizeof(char*));
    elementsString[0]=malloc(sizeof(char));
    int elementCount=0,i=0;
    double coefficent=1,exp=0;
    baseElement tempConstant;
    bool isADivider=false,hasSign=true;

    splitIntoMultipclationElements(&elementsString,input,&elementCount);//split the element up into its factors


    for(i =0;i<elementCount;i++){
        if(elementsString[i][0]=='/')isADivider=true;//check for the operator
        else if (elementsString[i][0]!='*')hasSign=false;

        if(checkAndConvertToConstantElement(elementsString[i]+hasSign,&tempConstant))//check if its a constant and multiply or divide coefficent with it
        {
            if(isADivider){
                if(*((double*)tempConstant.ptr)==0)return false;
                else coefficent=coefficent/(*((double*)tempConstant.ptr));
            }
            else coefficent=coefficent*(*((double*)tempConstant.ptr));
            
        } 
        else{//if its not a constant check if it

            if(elementsString[0][0]=='-')
            {
                coefficent=coefficent*-1;//check if x is the first element in which case there will be + or - before it 
                hasSign=true;
            }
            if(elementsString[0][0]=='+')hasSign=true;

            if(elementsString[i][0+hasSign]=='x'){
                if(elementsString[i][1+hasSign]=='^'){
                    if(checkAndConvertToConstantElement(elementsString[i]+2+hasSign,&tempConstant)){
                        exp= exp+(*((double*)tempConstant.ptr))*(isADivider ? -1:1);
                    }
                    else return false;
                }
                else exp= exp+(isADivider ? -1:1);
            }
            else return false;
        }
        isADivider=false;
        hasSign=true;
    }
    polinominalElement *tempPolinominalElement=malloc(sizeof(polinominalElement));
    baseElement *coefficentElement=malloc(sizeof(baseElement)),*expElement=malloc(sizeof(baseElement));//make a base element for coefficent and exp values (this isnt usefull for poliniminal elements since both exp and coefficent is gonna be constant but will come in usefull when dealing with exponantial functions)
    coefficentElement->type=constantElementType;
    expElement->type=constantElementType;

    double *coefficentPtr=malloc(sizeof(double)),*expPtr=malloc(sizeof(double));//make pointers for coeffient and exp to be stored int their baseElement
    *coefficentPtr=coefficent;
    *expPtr=exp;
    coefficentElement->ptr=coefficentPtr;
    expElement->ptr=expPtr;
    

    tempPolinominalElement->coefficent=coefficentElement;
    tempPolinominalElement->exp=expElement;
    polinom->ptr=tempPolinominalElement;
    polinom->type=polinominalElementType;

    for(i=0;i<elementCount;i++){
        free(elementsString[i]);
    }
    free(elementsString);
    return true;
}
bool checkAndConvertToMultipclationElement(char *input,baseElement *multipclationElementBase){
    char **elementsString=malloc(sizeof(char*));
    baseElement *elements=malloc(sizeof(baseElement));
    elementsString[0]=malloc(sizeof(char));
    int elementStringCount=0,i,elementCount=0;
    splitIntoMultipclationElements(&elementsString,input,&elementStringCount);//split the input into parts
    if(elementStringCount==1)return 0; //if its one factor there is no need for it to be MultipclationElement
    bool hasSign=true,isADivider=false;

    
    for(i=0;i<elementStringCount;i++){
        hasSign=true;
        isADivider=false;
        if(elementsString[i][0]=='/')isADivider=true;
        else if (elementsString[i][0]!='*')hasSign=false;
        convertElementIntoTypes(elementsString[i]+hasSign,&elements,&elementCount,true);//try and convert it into a type
        if(isADivider){//change this element with a exponential element that has a exp of -1
            baseElement *expConstantBase=malloc(sizeof(baseElement));//make a base element for exp and put -1 in it
            double *expValuePtr=malloc(sizeof(double));
            *expValuePtr=-1;
            expConstantBase->type=constantElementType;//set up exp base and coefficent base
            expConstantBase->ptr=expValuePtr;
            exponentialElement *divider=malloc(sizeof(exponentialElement));//set the exponentialElement

            baseElement *temp=malloc(sizeof(baseElement));//make a copy of the element since its gonna get replaced
            memcpy(temp,&elements[elementCount-1],sizeof(baseElement));
            divider->exp=expConstantBase;
            divider->element=temp;
            (&elements[elementCount-1])->type=exponentialElementType;
            (&elements[elementCount-1])->ptr=divider;
            
            baseElement *testa=&elements[elementCount-1];
            double *test=(double*)(((baseElement*)(((exponentialElement*)elements[elementCount-1].ptr)->exp))->ptr);
        }
    }
    multipclationElement *temp=malloc(sizeof(multipclationElement));
    temp->elementArray=elements;
    temp->elementCount=elementStringCount;
    multipclationElementBase->type=multipclationElementType;
    multipclationElementBase->ptr=temp;

    for(i=0;i<elementStringCount;i++){
        free(elementsString[i]);
    }
    free(elementsString);
    return true;

}
bool checkAndConvertToExponentialElement(char *input,baseElement *expElementBase){
    char **elementsString=malloc(sizeof(char*));
    baseElement *base=malloc(sizeof(baseElement)),*exp=malloc(sizeof(baseElement));
    exponentialElement *expElement=malloc(sizeof(exponentialElement));
    elementsString[0]=malloc(sizeof(char));
    int elementStringCount=0,i=1,elementCount=0;
    splitIntoExponentialElements(&elementsString,input,&elementStringCount);//split it up
    if(elementStringCount==1)return 0;
    convertElementIntoTypes(elementsString[0],&base,&elementCount,false);//maybe i should make it work for more then one exponential but then syntax gets unpredictable imo
    convertElementIntoTypes(elementsString[1]+1,&exp,&elementCount,false);
    expElementBase->type=exponentialElementType;
    expElementBase->ptr=expElement;
    expElement->element=base;
    expElement->exp=exp;

    for(i=0;i<elementStringCount;i++){
        free(elementsString[i]);
    }
    free(elementsString);
    return true;
}
bool checkAndConvertToSingleParameterFunctionElement(char *input,baseElement *singleParameterFunctionElementBase){
    char **elementsString=malloc(sizeof(char*));
    elementsString[0]=malloc(sizeof(char));
    int elementCount=0,elementStringCount=0;
    splitIntoMultipclationElements(&elementsString,input,&elementStringCount);//check if there is any other factor than the function return if there is cause we want elements with more than one factor to be stored in the MultipclationElement
    if(elementStringCount!=1)return false;
    baseElement *parameter=malloc(sizeof(baseElement)),*coefficent=malloc(sizeof(baseElement));
    singleParameterFunctionElement *singleParameterFuncElement=malloc(sizeof(singleParameterFunctionElement));
    char functions[][10]={"sin","cos","tan","cot","sec","cosec","arcsec","arccosec","arcsin","arccos","arctan","arccot","ln"};
    int lenghts[]={3,3,3,3,3,5,6,6,6,6,6,8,2};
    bool hasSign=true,isNegative=false;//this could actually be made into a func but idk if its worth it
    if(input[0]=='-')isNegative=true;
    else if (input[0]!='+')hasSign=false;
    int i=0;
    while(strncmp(input+hasSign,functions[i],lenghts[i])&&i<13)i++;//find which function this is
    if(i>=13)return false;
    
    singleParameterFuncElement->functionType=i;
    convertElementIntoTypes(input+lenghts[i]+hasSign,&parameter,&elementCount,false);//convert the parameter into types
    singleParameterFuncElement->parameter=parameter;
    singleParameterFuncElement->isNegative=isNegative;
    singleParameterFunctionElementBase->ptr=singleParameterFuncElement;
    singleParameterFunctionElementBase->type=singleParameterFunctionElementType;//set the element
    for(i=0;i<elementStringCount;i++){//free the string array
        free(elementsString[i]);
    }
    free(elementsString);
    return true;

}
bool checkAndConvertToDualParameterFunctionElement(char *input,baseElement *dualParameterFunctionElementBase){
    char **elementsString=malloc(sizeof(char*));
    elementsString[0]=malloc(sizeof(char));
    int elementCount=0,elementStringCount=0;
    splitIntoMultipclationElements(&elementsString,input,&elementStringCount);//check if there is any other factor than the function return if there is cause we want elements with more than one factor to be stored in the MultipclationElement
    if(elementStringCount!=1)return false;
    baseElement *parameter1=malloc(sizeof(baseElement)),*parameter2=malloc(sizeof(baseElement));
    dualParameterFunctionElement *dualParameterFuncElement=malloc(sizeof(dualParameterFunctionElement));
    char functions[][10]={"log"};
    int lenghts[]={3};
    bool hasSign=true,isNegative=false;//this could actually be made into a func but idk if its worth it
    if(input[0]=='-')isNegative=true;
    else if (input[0]!='+')hasSign=false;
    dualParameterFuncElement->isNegative=isNegative;

    int i=0,j=0;
    while(strncmp(input+hasSign,functions[i],lenghts[i])&&i<1)i++;//find which function this is
    if(i>=1)return false;
    if(input[lenghts[i]+hasSign]!='_')return false;
    while(input[j]!='\0'&&input[j]!='(')j++;//either find where the first parameter ends and the second one starts or find if there is no second parameter
    if(input[j]=='\0')return false;//means there is no second parameter

    dualParameterFuncElement->functionType=i;
    char* parameter1String=substring(input,lenghts[i]+hasSign+1,j-1);
    convertElementIntoTypes(parameter1String,&parameter1,&elementCount,false);//convert the first parameter into types
    convertElementIntoTypes(input+j,&parameter2,&elementCount,false);//convert the second parameter into types
    dualParameterFuncElement->firstParameter=parameter1;
    dualParameterFuncElement->secondParameter=parameter2;
    dualParameterFuncElement->isNegative=isNegative;
    dualParameterFunctionElementBase->ptr=dualParameterFuncElement;
    dualParameterFunctionElementBase->type=dualParameterFunctionElementType;//set the element
    for(i=0;i<elementStringCount;i++){//free the string array
        free(elementsString[i]);
    }
    free(elementsString);
    free(parameter1String);
    return true;

}

double getValueOfElement(baseElement *element,double x){
    if(element->type==constantElementType)return *((double*)(element->ptr));//this part isnt switch case cause i thought it was easier to see
    else if(element->type==polinominalElementType)
    {
        return (*((double*)(((polinominalElement*)(element->ptr))->coefficent->ptr)))*pow(x,*((double*)(((polinominalElement*)(element->ptr))->exp->ptr)));
    }
    else if(element->type==paranthesesElementType){
        double sum=0;
        int i=0;
        paranthesesElement *temp= (paranthesesElement*)element->ptr;//get the parantheses element as pointer
        for(i=0;i<temp->elementCount;i++){
            baseElement *temp3=temp->elementArray+i;
            sum=sum+getValueOfElement(temp->elementArray+i,x);
        }
        return sum;
    }
    else if (element->type==multipclationElementType){
        double val=1;
        int i=0;
        multipclationElement *temp= (multipclationElement*)element->ptr;//get the parantheses element as pointer
        for(i=0;i<temp->elementCount;i++){
            val=val*getValueOfElement(temp->elementArray+i,x);
        }
        return val;
    }
    else if(element->type==exponentialElementType){
        exponentialElement *temp =(exponentialElement*) element->ptr;
        return pow(getValueOfElement(temp->element,x),getValueOfElement(temp->exp,x)); 
    }
    else if(element->type==singleParameterFunctionElementType){
        singleParameterFunctionElement *temp =element->ptr;
        double value;
        switch (temp->functionType)
        {
        case sinT:
            value=sin(getValueOfElement(temp->parameter,x));
            break;
        case cosT:
            value=cos(getValueOfElement(temp->parameter,x));
            break;
        case tanT:
            value=tan(getValueOfElement(temp->parameter,x));
            break;
        case cotT:
            value=1/tan(getValueOfElement(temp->parameter,x));
            break;
        case secT:
            value=1/cos(getValueOfElement(temp->parameter,x));
            break;
        case cosecT:
            value=1/sin(getValueOfElement(temp->parameter,x));
            break;
        case arcsinT:
            value=asin(getValueOfElement(temp->parameter,x));
            break;
        case arccosT:
            value=acos(getValueOfElement(temp->parameter,x));
            break;
        case arctanT:
            value=atan(getValueOfElement(temp->parameter,x));
            break;
        case arcsecT:
            value=acos(1/getValueOfElement(temp->parameter,x));
            break;
        case arccosecT:
            value=asin(1/getValueOfElement(temp->parameter,x));
            break;      
        case lnT:
            value=log(getValueOfElement(temp->parameter,x))/log(e);
            break;
        
        default:
            break;
        }
        if(temp->isNegative)value=value*-1;
        return value;
    }
    else if(element->type==dualParameterFunctionElementType){
        dualParameterFunctionElement *temp =element->ptr;
        double value;
        switch (temp->functionType)
        {
        case logT:
        double temp1=getValueOfElement(temp->secondParameter,x),temp2=getValueOfElement(temp->firstParameter,x);
        if(temp1<0||temp2<0||temp2==1)printf("Error log parameters are out of scope");
                value= (temp->isNegative ? -1:1)*log(getValueOfElement(temp->secondParameter,x))/log(getValueOfElement(temp->firstParameter,x));
            break;
        }
        return value;
    }
    return -1;
}
bool convertElementIntoTypes(char *elementsString,baseElement **elements,int *elementCount,bool increaseTheSizeIfSuccesfull)
{
    
    if(checkAndConvertToConstantElement(elementsString,*elements+*elementCount));
    else if(checkAndConvertToPolinominal(elementsString,*elements+*elementCount));
    else if(checkAndConvertToExponentialElement(elementsString,*elements+*elementCount));
    else if(checkAndConvertToSingleParameterFunctionElement(elementsString,*elements+*elementCount));
    else if(checkAndConvertToDualParameterFunctionElement(elementsString,*elements+*elementCount));
    else if(checkAndConvertToMultipclationElement(elementsString,*elements+*elementCount));
    else if(checkAndConvertToParanthesesElements(elementsString,*elements+*elementCount));
    else return false;
    if(increaseTheSizeIfSuccesfull){
        *elementCount+=1;//if it didnt return it means it was succesfull and that we need to make room for more elements
        *elements=(baseElement*)realloc(*elements,(*elementCount+1)*sizeof(baseElement));  
    }
    return true;
}
void convertAllElementsIntoTypes(char ***elementsString,int elementStringCount,baseElement **elements,int *elementCount){
    int i;
    for(i=0;i<elementStringCount;i++){
        if(convertElementIntoTypes((*elementsString)[i],elements,elementCount,true));
    }
}
void freeMemoryOfElement(baseElement *element){
    if(element->type==constantElementType)
    {
        free(element->ptr);
        free(element);
        return;
    }
    else if(element->type==polinominalElementType)
    {
        polinominalElement *temp =(polinominalElement*) element->ptr;
        freeMemoryOfElement(temp->coefficent);
        freeMemoryOfElement(temp->exp);
        free(element->ptr);
        free(element);
    }
    else if(element->type==paranthesesElementType){
        
        int i=0;
        paranthesesElement *temp= (paranthesesElement*)element->ptr;//get the parantheses element as pointer
        for(i=0;i<temp->elementCount;i++){
            baseElement *temp2=temp->elementArray+i;
            freeMemoryOfElement(temp->elementArray+i);
        }
        free(temp);
        free(element);
        return;
    }
    else if (element->type==multipclationElementType){
        double val=1;
        int i=0;
        multipclationElement *temp= (multipclationElement*)element->ptr;//get the parantheses element as pointer
        for(i=0;i<temp->elementCount;i++){
            freeMemoryOfElement(temp->elementArray+i);
        }
        free(temp);
        free(element);
        return;
    }
    else if(element->type==exponentialElementType){
        exponentialElement *temp =(exponentialElement*) element->ptr;
        freeMemoryOfElement(temp->element);
        freeMemoryOfElement(temp->exp);
        free(temp);
        free(element);
        return;
    }
    else if(element->type==singleParameterFunctionElementType){
        singleParameterFunctionElement *temp =(singleParameterFunctionElement*) element->ptr;
        freeMemoryOfElement(temp->parameter);
        free(temp);
        free(element);
        return;
    }
    else if(element->type==dualParameterFunctionElementType){
        dualParameterFunctionElement *temp =(dualParameterFunctionElement*) element->ptr;
        freeMemoryOfElement(temp->firstParameter);
        freeMemoryOfElement(temp->secondParameter);
        free(temp);
        free(element);
        return;
    }
}
char* substring(char* string,int start,int end){
    char* temp=calloc(end-start+2,sizeof(char));//+2 for \0 at the end
    memcpy(temp,string+start,end-start+1);
    temp[end-start+1]='\0';
    return temp;
}
