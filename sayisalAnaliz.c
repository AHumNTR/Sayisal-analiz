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
    double temp=0;//i was using malloc here and it was not working randomly and ive spent a lot of time trying to fix it :(
    bool isNegative=false,hasSign=true;
    if(elementString[0]=='-')isNegative=true;
    else if (elementString[0]!='+')hasSign=false;
    i=hasSign;
    while(elementString[i]!='\0'){
        if(elementString[i]=='.')dotsIndex=i; //we need to know where the dot is
        else if(!(elementString[i]>='0'&&elementString[i]<='9'))return false; //check if its a number if not return
        //printf("constant char count=%d %d\n",i,elementString[i]-'0');
        i++;
    }
    if(dotsIndex==-1)dotsIndex=i;
    for(j=dotsIndex+1;j<i;j++){//add whats after the dot
        temp+= pow(10,dotsIndex-j)*(elementString[j]-'0');
    }
    for(j=dotsIndex-1;j>=hasSign;j--){//add whats before the dot
        temp+= pow(10,dotsIndex-1-j)*(elementString[j]-'0');
    }
    if(isNegative)temp=temp*-1;
    //printf("%lf",*temp);
    constant->type=constantElementType;
    constant->ptr=temp;
    return true;
}
bool checkAndConvertToParanthesesElements(char *input, baseElement *parantheses){
    char **elementStrings=(char**)calloc(1,sizeof(char*));
    bool hasSign=true,isNegative=false;
    int elementStringCount=0,i=0,elementCount=0;
    baseElement **elements=calloc(1,sizeof(baseElement*));//element array to be stored in the parantheseselement
    if(input[0]=='-'){
        isNegative=true;
    }
    else if (input[0]!='+')hasSign=false;
    while(input[i]!='\n'&&input[i]!='\0')i++;
    char* substr= substring(input,1+hasSign,i-2);
    splitIntoAdditionElements(&elementStrings,substr,&elementStringCount);
    convertAllElementsIntoTypes(&elementStrings,elementStringCount,&elements,&elementCount);
    
    paranthesesElement *paranthesesPtr=calloc(1,sizeof(paranthesesElement)); //create the parantheses element
    parantheses->ptr=paranthesesPtr;//set the pointer for the base element
    parantheses->type= paranthesesElementType;//set the type
    paranthesesPtr->elementArray=elements;//set the array on the parantheses element
    paranthesesPtr->elementCount=elementCount;//set the element count
    paranthesesPtr->isNegative=isNegative;
    for(i=0;i<elementStringCount;i++){//free the string array
        free(elementStrings[i]);
    }
    free(substr);
    free(elementStrings);
    return true;
}
bool checkAndConvertToPolinominal(char *input, baseElement *polinom){
    char **elementsString=calloc(1,sizeof(char*));
    elementsString[0]=calloc(1,sizeof(char));
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
                    else{
                        for(i=0;i<elementCount;i++){
                            free(elementsString[i]);
                        }
                        free(elementsString);
                        return false;
                    } 
                }
                else exp= exp+(isADivider ? -1:1);
            }
            else return false;
        }
        isADivider=false;
        hasSign=true;
    }
    if(exp==0){
        for(i=0;i<elementCount;i++){
            free(elementsString[i]);
        }
        free(elementsString);
        return false;//return if there is no x
    }
    polinominalElement *tempPolinominalElement=calloc(1,sizeof(polinominalElement));
    baseElement *coefficentElement=calloc(1,sizeof(baseElement)),*expElement=calloc(1,sizeof(baseElement));//make a base element for coefficent and exp values (this isnt usefull for poliniminal elements since both exp and coefficent is gonna be constant but will come in usefull when dealing with exponantial functions)
    coefficentElement->type=constantElementType;
    expElement->type=constantElementType;

    double *coefficentPtr=calloc(1,sizeof(double)),*expPtr=calloc(1,sizeof(double));//make pointers for coeffient and exp to be stored int their baseElement
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
    char **elementsString=calloc(1,sizeof(char*));
    
    elementsString[0]=calloc(1,sizeof(char));
    int elementStringCount=0,i;
    splitIntoMultipclationElements(&elementsString,input,&elementStringCount);//split the input into parts
    if(elementStringCount==1){
            for(i=0;i<elementStringCount;i++){
            free(elementsString[i]);
        }
        free(elementsString);
        return false;
    } //if its one factor there is no need for it to be MultipclationElement
    baseElement **elements=calloc(elementStringCount,sizeof(baseElement*));
    bool hasSign=true,isADivider=false;

    
    for(i=0;i<elementStringCount;i++){
        hasSign=true;
        isADivider=false;
        if(elementsString[i][0]=='/')isADivider=true;
        else if (elementsString[i][0]!='*')hasSign=false;
        elements[i]=calloc(1,sizeof(baseElement));
        convertElementIntoTypes(elementsString[i]+hasSign,*(elements+i));//try and convert it into a type
        if(isADivider){//change this element with a exponential element that has a exp of -1
            baseElement *expConstantBase=calloc(1,sizeof(baseElement));//make a base element for exp and put -1 in it
            double *expValuePtr=calloc(1,sizeof(double));
            *expValuePtr=-1;
            expConstantBase->type=constantElementType;//set up exp base and coefficent base
            expConstantBase->ptr=expValuePtr;
            exponentialElement *divider=calloc(1,sizeof(exponentialElement));//set the exponentialElement
            divider->isNegative=false;//if it was something like 1/-2 it should be written as 1/(-2)
            baseElement *temp=calloc(1,sizeof(baseElement));//make a copy of the element since its gonna get replaced
            memcpy(temp,*(elements+i),sizeof(baseElement));
            divider->exp=expConstantBase;
            divider->element=temp;
            (*(elements+i))->type=exponentialElementType;
            (*(elements+i))->ptr=divider;
        }
    }
    multipclationElement *temp=calloc(1,sizeof(multipclationElement));
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
    char **elementsString=calloc(1,sizeof(char*));
    baseElement *base=calloc(1,sizeof(baseElement)),*exp=calloc(1,sizeof(baseElement));
    exponentialElement *expElement=calloc(1,sizeof(exponentialElement));
    elementsString[0]=calloc(1,sizeof(char));
    int elementStringCount=0,i=1,elementCount=0;
    splitIntoExponentialElements(&elementsString,input,&elementStringCount);//split it up
    if(elementStringCount==1){
        for(i=0;i<elementStringCount;i++){
            free(elementsString[i]);
        }
        free(elementsString);
        return false;
    }
    if(elementsString[0][0]=='-'){
        expElement->isNegative=true;
        convertElementIntoTypes(elementsString[0]+1,base);
    }
    else convertElementIntoTypes(elementsString[0],base);//maybe i should make it work for more then one exponential but then syntax gets unpredictable imo
    convertElementIntoTypes(elementsString[1]+1,exp);
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
    char **elementsString=calloc(1,sizeof(char*));
    elementsString[0]=calloc(1,sizeof(char));
    int elementCount=0,elementStringCount=0;
    splitIntoMultipclationElements(&elementsString,input,&elementStringCount);//check if there is any other factor than the function return if there is cause we want elements with more than one factor to be stored in the MultipclationElement
    if(elementStringCount!=1)return false;
    baseElement *parameter=calloc(1,sizeof(baseElement));
    singleParameterFunctionElement *singleParameterFuncElement=calloc(1,sizeof(singleParameterFunctionElement));
    char functions[][10]={"sin","cos","tan","cot","sec","cosec","arcsin","arccos","arcsec","arccosec","arctan","arccot","ln"};
    int lenghts[]={3,3,3,3,3,5,6,6,6,8,6,6,2};
    bool hasSign=true,isNegative=false;//this could actually be made into a func but idk if its worth it
    if(input[0]=='-')isNegative=true;
    else if (input[0]!='+')hasSign=false;
    int i=0,inputLenght=0;
    while(input[inputLenght+hasSign]!='(')inputLenght++;//check if it goes to \0
    while((inputLenght!=lenghts[i]||strncmp(input+hasSign,functions[i],lenghts[i]))&&i<13)i++;//find which function this is
    if(i>=13){
        for(i=0;i<elementStringCount;i++){
            free(elementsString[i]);
        }
        free(elementsString);
        return false;
    }
    
    singleParameterFuncElement->functionType=i;
    convertElementIntoTypes(input+lenghts[i]+hasSign,parameter);//convert the parameter into types
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
    char **elementsString=calloc(1,sizeof(char*));
    elementsString[0]=calloc(1,sizeof(char));
    int elementStringCount=0;
    splitIntoMultipclationElements(&elementsString,input,&elementStringCount);//check if there is any other factor than the function return if there is cause we want elements with more than one factor to be stored in the MultipclationElement
    if(elementStringCount!=1)return false;
    baseElement *parameter1=calloc(1,sizeof(baseElement)),*parameter2=calloc(1,sizeof(baseElement));
    dualParameterFunctionElement *dualParameterFuncElement=calloc(1,sizeof(dualParameterFunctionElement));
    char functions[][10]={"log"};
    int lenghts[]={3};
    bool hasSign=true,isNegative=false;//this could actually be made into a func but idk if its worth it
    if(input[0]=='-')isNegative=true;
    else if (input[0]!='+')hasSign=false;
    dualParameterFuncElement->isNegative=isNegative;

    int i=0,j=0 ,inputLenght=0;
    while(input[inputLenght+hasSign]!='_' && input[inputLenght+hasSign]!='\0'&& input[inputLenght+hasSign]!='\n')inputLenght++;
    if(input[inputLenght+hasSign]!='_')return false;
    while((inputLenght!=lenghts[i]||strncmp(input+hasSign,functions[i],lenghts[i]))&&i<1)i++;//find out which function it is
    if(i>=1)return false;
    if(input[lenghts[i]+hasSign]!='_'){
    for(i=0;i<elementStringCount;i++){
        free(elementsString[i]);
    }
    free(elementsString);
    return false;
    }
    while(input[j]!='\0'&&input[j]!='(')j++;//either find where the first parameter ends and the second one starts or find if there is no second parameter
    if(input[j]=='\0'){
    for(i=0;i<elementStringCount;i++){
        free(elementsString[i]);
    }
    free(elementsString);
    return false;
    }//means there is no second parameter

    dualParameterFuncElement->functionType=i;
    char* parameter1String=substring(input,lenghts[i]+hasSign+1,j-1);
    convertElementIntoTypes(parameter1String,parameter1);//convert the first parameter into types
    convertElementIntoTypes(input+j,parameter2);//convert the second parameter into types
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
    if(element->type==constantElementType){
        //printf("constant %lf\n",*((double*)(element->ptr)));
        return *((double*)(element->ptr));//this part isnt switch case cause i thought it was easier to see
    }
    else if(element->type==polinominalElementType)
    {
        //printf("polinominal %lf\n",(*((double*)(((polinominalElement*)(element->ptr))->coefficent->ptr)))*pow(x,*((double*)(((polinominalElement*)(element->ptr))->exp->ptr))));
        return (*((double*)(((polinominalElement*)(element->ptr))->coefficent->ptr)))*pow(x,*((double*)(((polinominalElement*)(element->ptr))->exp->ptr)));
    }
    else if(element->type==paranthesesElementType){
        double sum=0;
        int i=0;
        //printf("entering paratez\n");
        paranthesesElement *temp= (paranthesesElement*)element->ptr;//get the parantheses element as pointer
        for(i=0;i<temp->elementCount;i++){
            sum=sum+getValueOfElement(*(temp->elementArray+i),x);
        }
         //printf("parantheses %lf\n",sum);
         sum=sum*(temp->isNegative? -1:1);
        return sum;
    }
    else if (element->type==multipclationElementType){
        double val=1,tempDouble=1;
        int i=0;
        //printf("entering carpma\n");
        multipclationElement *temp= (multipclationElement*)element->ptr;//get the multipclation element as pointer
        while(i<temp->elementCount&&tempDouble!=0){//to still get zero if there were values like nan or inf 
            tempDouble=getValueOfElement(*(temp->elementArray+i),x);
            val=val*tempDouble;
            i++;
        }
        if(tempDouble==0)val=0;
        //printf("multipclation %lf\n",val);
        val=val*(temp->isNegative? -1:1);
        return val;
    }
    else if(element->type==exponentialElementType){
        exponentialElement *temp =(exponentialElement*) element->ptr;
        //printf("exp %lf\n",pow(getValueOfElement(temp->element,x),getValueOfElement(temp->exp,x)));
        return (temp->isNegative? -1:1)*pow(getValueOfElement(temp->element,x),getValueOfElement(temp->exp,x)); 
    }
    else if(element->type==singleParameterFunctionElementType){
        singleParameterFunctionElement *temp =element->ptr;
        double value;
        //printf("entering func\n");
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
        case arccotT:
            value=atan(1/getValueOfElement(temp->parameter,x));//idk if this works
        case arcsecT:
            value=acos(1/getValueOfElement(temp->parameter,x));
            break;
        case arccosecT:
            value=asin(1/getValueOfElement(temp->parameter,x));
            break;      
        case lnT:
            double temp1=getValueOfElement(temp->parameter,x);
            if(temp1<0)printf("ln parameter out of range using absolute instead\n");
            value=log(abs(getValueOfElement(temp->parameter,x)))/log(e);
            break;
        
        default:
            break;
        }
        if(temp->isNegative)value=value*-1;
        //printf("funct %lf\n",value);
        return value;
    }
    else if(element->type==dualParameterFunctionElementType){
        dualParameterFunctionElement *temp =element->ptr;
        double value;
        //printf("entering log\n");
        switch (temp->functionType)
        {
        case logT:
        double temp1=getValueOfElement(temp->secondParameter,x),temp2=getValueOfElement(temp->firstParameter,x);
        if(temp1<0||temp2<0||temp2==1)printf("Error log parameters are out of scope");
                value= (temp->isNegative ? -1:1)*log(temp1)/log(temp2);
            break;
        }
        //printf("func 2 %lf\n",value);
        return value;
    }
    return -1;
}
bool convertElementIntoTypes(char *elementsString,baseElement *elements)
{
    //printf("%s\n",elementsString);
    if(checkAndConvertToConstantElement(elementsString,elements));
    else if(checkAndConvertToPolinominal(elementsString,elements));
    else if(checkAndConvertToExponentialElement(elementsString,elements));
    else if(checkAndConvertToSingleParameterFunctionElement(elementsString,elements));
    else if(checkAndConvertToDualParameterFunctionElement(elementsString,elements));
    else if(checkAndConvertToMultipclationElement(elementsString,elements));
    else if(checkAndConvertToParanthesesElements(elementsString,elements));
    else return false;
    return true;
}
void convertAllElementsIntoTypes(char ***elementsString,int elementStringCount,baseElement ***elements,int *elementCount){
    int i;
    for(i=0;i<elementStringCount;i++){
        *elements=(baseElement**)realloc(*elements,(*elementCount+1)*sizeof(baseElement*));  
        *elementCount+=1;//if it didnt return it means it was succesfull and that we need to make room for more elements
        *(*elements+i)=calloc(1,sizeof(baseElement));
        if(convertElementIntoTypes((*elementsString)[i],*(*elements+i)));
    }
}

baseElement* derivate(baseElement *element){
    if(element->type==constantElementType)return createConstantElement(0);
    
    else if(element->type==polinominalElementType){
        polinominalElement *temp =element->ptr;
        return   createPolinominalElementAndFill(createConstantElement(getValueOfElement(temp->coefficent,0)*getValueOfElement(temp->exp,0)),createConstantElement(getValueOfElement(temp->exp,0)-1));
    }
    else if(element->type==exponentialElementType){//d(f(x)^g(x))= ( gt(x)*ln(f(x))+ft(x)*g(x)/f(x) )*f(x)^g(x)
        exponentialElement *temp=element->ptr;
        
        baseElement *derivitive=createMultipclationElement(2,false);
        baseElement* ftGdivFxBaseElement=createMultipclationElement(3,false),*gtLnFBaseElement=createMultipclationElement(2,false);

        multipclationElement *mainMultipclationElement=derivitive->ptr,*gtLnF=gtLnFBaseElement->ptr,*ftGDivFx=ftGdivFxBaseElement->ptr;
        baseElement *mainParanthesesBaseElement=createParanthesesElement(2,false);
        paranthesesElement *mainParanthesesElement=mainParanthesesBaseElement->ptr;

        mainMultipclationElement->elementArray[0]=element;//*f(x)^g(x)this part
        mainMultipclationElement->elementArray[1]=mainParanthesesBaseElement;

        gtLnF->elementArray[0]=derivate(temp->exp);
        gtLnF->elementArray[1]=createSingleParameterFunctionElementAndFill(lnT,false,temp->element);
        

        ftGDivFx->elementArray[0]=derivate(temp->element);
        ftGDivFx->elementArray[1]=temp->exp;
        ftGDivFx->elementArray[2]=createExponentialElementAndFill(false,temp->element,createConstantElement(-1));
        
        mainParanthesesElement->elementArray[0]=gtLnFBaseElement;
        mainParanthesesElement->elementArray[1]=ftGdivFxBaseElement;

        derivitive->type=multipclationElementType;
        derivitive->ptr=mainMultipclationElement;
        return derivitive;
    }
    else if(element->type==singleParameterFunctionElementType){
        singleParameterFunctionElement *temp=element->ptr;
        baseElement *derivitive;
        if(temp->functionType==sinT){
            derivitive=createMultipclationElement(2,false);
            multipclationElement *tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createSingleParameterFunctionElementAndFill(cosT,false,temp->parameter);
            return derivitive;
        }
        else if(temp->functionType==cosT){
            derivitive=createMultipclationElement(2,false);
            multipclationElement *tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createSingleParameterFunctionElementAndFill(sinT,true,temp->parameter);
            return derivitive;
        }
        else if(temp->functionType==tanT){
            derivitive=createMultipclationElement(2,false);
            multipclationElement *tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,createSingleParameterFunctionElementAndFill(secT,false,temp->parameter),createConstantElement(2));
            return derivitive;
        }
        else if(temp->functionType==cotT){
            derivitive=createMultipclationElement(2,false);
            multipclationElement *tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,createSingleParameterFunctionElementAndFill(cosecT,true,temp->parameter),createConstantElement(-2));
            return derivitive;
        }
        else if(temp->functionType==secT){
            derivitive=createMultipclationElement(3,false);
            multipclationElement *tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createSingleParameterFunctionElementAndFill(tanT,false,temp->parameter);
            tempMultipclationElement->elementArray[2]=element;//this is pretty scary ngl
            return derivitive;
        }
        else if(temp->functionType==cosecT){
            derivitive=createMultipclationElement(3,false);
            multipclationElement *tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createSingleParameterFunctionElementAndFill(cotT,true,temp->parameter);
            tempMultipclationElement->elementArray[2]=element;//this is pretty scary ngl
            return derivitive;
        }
        else if(temp->functionType==arcsinT){
            derivitive=createMultipclationElement(2,false);
            multipclationElement* tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            
            baseElement *tempParanthesesElementBase=createParanthesesElement(2,false);
            paranthesesElement* tempParanthesesElement=tempParanthesesElementBase->ptr;
            baseElement *parameterSquared=createExponentialElementAndFill(true,temp->parameter,createConstantElement(2));
            tempParanthesesElement->elementArray[0]=createConstantElement(1);
            tempParanthesesElement->elementArray[1]=parameterSquared;

            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,tempParanthesesElementBase,createConstantElement(-0.5));
            return derivitive;
        }
        else if(temp->functionType==arccosT){
            derivitive=createMultipclationElement(2,true);
            multipclationElement* tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            
            baseElement *tempParanthesesElementBase=createParanthesesElement(2,false);
            paranthesesElement* tempParanthesesElement=tempParanthesesElementBase->ptr;
            baseElement *parameterSquared=createExponentialElementAndFill(true,temp->parameter,createConstantElement(2));
            tempParanthesesElement->elementArray[0]=createConstantElement(1);
            tempParanthesesElement->elementArray[1]=parameterSquared;

            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,tempParanthesesElementBase,createConstantElement(-0.5));
            return derivitive;
        }
        else if(temp->functionType==arctanT){
           derivitive=createMultipclationElement(2,false);
            multipclationElement* tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            
            baseElement *tempParanthesesElementBase=createParanthesesElement(2,false);
            paranthesesElement* tempParanthesesElement=tempParanthesesElementBase->ptr;
            baseElement *parameterSquared=createExponentialElementAndFill(false,temp->parameter,createConstantElement(2));
            tempParanthesesElement->elementArray[0]=createConstantElement(1);
            tempParanthesesElement->elementArray[1]=parameterSquared;

            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,tempParanthesesElementBase,createConstantElement(-1));
            return derivitive;
        }
        else if(temp->functionType==arccotT){
           derivitive=createMultipclationElement(2,true);
            multipclationElement* tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            
            baseElement *tempParanthesesElementBase=createParanthesesElement(2,false);
            paranthesesElement* tempParanthesesElement=tempParanthesesElementBase->ptr;
            baseElement *parameterSquared=createExponentialElementAndFill(false,temp->parameter,createConstantElement(2));
            tempParanthesesElement->elementArray[0]=createConstantElement(1);
            tempParanthesesElement->elementArray[1]=parameterSquared;

            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,tempParanthesesElementBase,createConstantElement(-1));
            return derivitive;
        }
        else if(temp->functionType==arcsecT){
            derivitive=createMultipclationElement(3,false);
            multipclationElement* tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,temp->parameter,createConstantElement(-1));

            baseElement *tempParanthesesElementBase=createParanthesesElement(2,false);
            paranthesesElement* tempParanthesesElement=tempParanthesesElementBase->ptr;
            baseElement *parameterSquared=createExponentialElementAndFill(false,temp->parameter,createConstantElement(2));
            tempParanthesesElement->elementArray[0]=createConstantElement(-1);
            tempParanthesesElement->elementArray[1]=parameterSquared;

            tempMultipclationElement->elementArray[2]=createExponentialElementAndFill(false,tempParanthesesElementBase,createConstantElement(-0.5));
            return derivitive;
        }
        else if(temp->functionType==arccosecT){
            derivitive=createMultipclationElement(3,true);
            multipclationElement* tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,temp->parameter,createConstantElement(-1));

            baseElement *tempParanthesesElementBase=createParanthesesElement(2,false);
            paranthesesElement* tempParanthesesElement=tempParanthesesElementBase->ptr;
            baseElement *parameterSquared=createExponentialElementAndFill(false,temp->parameter,createConstantElement(2));
            tempParanthesesElement->elementArray[0]=createConstantElement(-1);
            tempParanthesesElement->elementArray[1]=parameterSquared;

            tempMultipclationElement->elementArray[2]=createExponentialElementAndFill(false,tempParanthesesElementBase,createConstantElement(-0.5));
            return derivitive;
        }
        else if(temp->functionType==lnT) {//d/dx(ln(f(x))=ft/f
            derivitive=createMultipclationElement(2,false);
            multipclationElement *tempMultipclationElement=derivitive->ptr;
            tempMultipclationElement->elementArray[0]=derivate(temp->parameter);
            tempMultipclationElement->elementArray[1]=createExponentialElementAndFill(false,temp->parameter,createConstantElement(-1));
            return derivitive;
        
        }
    }
    else if(element->type==dualParameterFunctionElementType){
        dualParameterFunctionElement *temp=element->ptr;
        if(temp->functionType==logT){//d/dx(log_fx(g(x))=(gt*lnf/g-fd*lng/f)/((lnf)^2)
            baseElement* derivitive=createMultipclationElement(2,false);
            baseElement *mainParanthesesBaseElement=createParanthesesElement(2,false),*gtLnFDivGBaseElement=createMultipclationElement(3,false),*fdLnGdivFBaseElement=createMultipclationElement(3,false);
            multipclationElement *tempMultipclationElement=derivitive->ptr,*gtLnFDivG=gtLnFDivGBaseElement->ptr,*fdLnGdivF=fdLnGdivFBaseElement->ptr;
            tempMultipclationElement->elementArray[0]=createExponentialElementAndFill(false,createSingleParameterFunctionElementAndFill(lnT,false,temp->firstParameter),createConstantElement(-2));// =/((lnf)^2)
            tempMultipclationElement->elementArray[1]=mainParanthesesBaseElement;
            paranthesesElement *mainParanthesesElement=mainParanthesesBaseElement->ptr;//set the main parantheses
            mainParanthesesElement->elementArray[0]=gtLnFDivGBaseElement;
            mainParanthesesElement->elementArray[1]=fdLnGdivFBaseElement;

            gtLnFDivG->elementArray[0]=derivate(temp->secondParameter);
            gtLnFDivG->elementArray[1]=createSingleParameterFunctionElementAndFill(lnT,false,temp->firstParameter);
            gtLnFDivG->elementArray[2]=createExponentialElementAndFill(false,temp->secondParameter,createConstantElement(-1));

            fdLnGdivF->elementArray[0]=derivate(temp->firstParameter);
            fdLnGdivF->elementArray[1]=createSingleParameterFunctionElementAndFill(lnT,false,temp->secondParameter);
            fdLnGdivF->elementArray[2]=createExponentialElementAndFill(false,temp->firstParameter,createConstantElement(-1));

            return derivitive;
        }
    }
    else if(element->type==multipclationElementType){
        int i=0,j=0;
        multipclationElement *temp=element->ptr;
        baseElement *derivitive=createParanthesesElement(temp->elementCount,temp->isNegative);
        paranthesesElement *newMultipclationElement=derivitive->ptr;
        for(i=0;i<newMultipclationElement->elementCount;i++){
            baseElement *tempSubMultipclationBaseElement=createMultipclationElement(newMultipclationElement->elementCount,false);
            multipclationElement *tempSubMultipclationElement=tempSubMultipclationBaseElement->ptr;
            for(j=0;j<tempSubMultipclationElement->elementCount;j++){
                if(i!=j)tempSubMultipclationElement->elementArray[j]=temp->elementArray[j];
                else tempSubMultipclationElement->elementArray[j]=derivate(temp->elementArray[j]);
            }
            newMultipclationElement->elementArray[i]=tempSubMultipclationBaseElement;
        }
        return derivitive;
    }
    else if(element->type==paranthesesElementType){
        int i=0;
        paranthesesElement *temp=element->ptr;
        baseElement *derivitive=createParanthesesElement(temp->elementCount,temp->isNegative);
        paranthesesElement *newParanthesesElement=derivitive->ptr;
        for(i=0;i<newParanthesesElement->elementCount;i++){
            newParanthesesElement->elementArray[i]=derivate(temp->elementArray[i]);
        }
        return derivitive;
    }
    return NULL;
}
baseElement* createConstantElement(double value){
    baseElement *tempConstantBaseElement=calloc(1,sizeof(baseElement));
    double *tempDouble=calloc(1,sizeof(double));
    *tempDouble=value;
    tempConstantBaseElement->ptr=tempDouble;
    return tempConstantBaseElement;
}
baseElement* createPolinominalElementAndFill(baseElement* coefficent,baseElement* exp){
    baseElement *tempPolinominalBaseElement=calloc(1,sizeof(baseElement));
    polinominalElement *tempPolinominalElement=calloc(1,sizeof(polinominalElement));
    tempPolinominalBaseElement->type=polinominalElementType;
    tempPolinominalBaseElement->ptr=tempPolinominalElement;
    tempPolinominalElement->coefficent=coefficent;
    tempPolinominalElement->exp= exp;
    return tempPolinominalBaseElement;
}
baseElement* createExponentialElement(bool isNegative){
    baseElement *tempExponentailBaseElement=calloc(1,sizeof(baseElement));
    exponentialElement *tempExponentialElement=calloc(1,sizeof(exponentialElement));
    tempExponentailBaseElement->type=exponentialElementType;
    tempExponentailBaseElement->ptr=tempExponentialElement;
    tempExponentialElement->isNegative=isNegative;
    return tempExponentailBaseElement;
}
baseElement* createExponentialElementAndFill(bool isNegative,baseElement *element,baseElement *exp){
    baseElement *tempExponentailBaseElement=calloc(1,sizeof(baseElement));
    exponentialElement *tempExponentialElement=calloc(1,sizeof(exponentialElement));
    tempExponentailBaseElement->type=exponentialElementType;
    tempExponentailBaseElement->ptr=tempExponentialElement;
    tempExponentialElement->element=element;
    tempExponentialElement->exp=exp;
    tempExponentialElement->isNegative=isNegative;
    return tempExponentailBaseElement;
}
baseElement* createSingleParameterFunctionElement(singleParameterFunctionTypes type,bool isNegative){
    baseElement *tempSingleParameterFunctionBaseElement= calloc(1,sizeof(baseElement));
    singleParameterFunctionElement *tempSingleParameterFunctionElement=calloc(1,sizeof(singleParameterFunctionElement));
    tempSingleParameterFunctionBaseElement->ptr=tempSingleParameterFunctionElement;
    tempSingleParameterFunctionBaseElement->type=singleParameterFunctionElementType;
    tempSingleParameterFunctionElement->functionType=type;
    tempSingleParameterFunctionElement->isNegative=isNegative;
    return tempSingleParameterFunctionBaseElement;
}
baseElement* createSingleParameterFunctionElementAndFill(singleParameterFunctionTypes type,bool isNegative,baseElement* parameter){
    baseElement *tempSingleParameterFunctionBaseElement= calloc(1,sizeof(baseElement));
    singleParameterFunctionElement *tempSingleParameterFunctionElement=calloc(1,sizeof(singleParameterFunctionElement));
    tempSingleParameterFunctionBaseElement->ptr=tempSingleParameterFunctionElement;
    tempSingleParameterFunctionBaseElement->type=singleParameterFunctionElementType;
    tempSingleParameterFunctionElement->functionType=type;
    tempSingleParameterFunctionElement->isNegative=isNegative;
    tempSingleParameterFunctionElement->parameter=parameter;
    return tempSingleParameterFunctionBaseElement;
}
baseElement* createDualParameterFunctionElement(dualParameterFunctionTypes type,bool isNegative){
    baseElement *tempDualParameterFunctionBaseElement= calloc(1,sizeof(baseElement));
    dualParameterFunctionElement *tempDualParameterFunctionElement=calloc(1,sizeof(dualParameterFunctionElement));
    tempDualParameterFunctionBaseElement->ptr=tempDualParameterFunctionElement;
    tempDualParameterFunctionBaseElement->type=dualParameterFunctionElementType;
    tempDualParameterFunctionElement->functionType=type;
    tempDualParameterFunctionElement->isNegative=isNegative;
    return tempDualParameterFunctionBaseElement;
}
baseElement* createDualParameterFunctionElementAndFill(dualParameterFunctionTypes type,bool isNegative,baseElement* firstParameter,baseElement *secondParameter){
    baseElement *tempDualParameterFunctionBaseElement= calloc(1,sizeof(baseElement));
    dualParameterFunctionElement *tempDualParameterFunctionElement=calloc(1,sizeof(dualParameterFunctionElement));
    tempDualParameterFunctionBaseElement->ptr=tempDualParameterFunctionElement;
    tempDualParameterFunctionBaseElement->type=dualParameterFunctionElementType;
    tempDualParameterFunctionElement->functionType=type;
    tempDualParameterFunctionElement->isNegative=isNegative;
    tempDualParameterFunctionElement->firstParameter=firstParameter;
    tempDualParameterFunctionElement->secondParameter=secondParameter;
    return tempDualParameterFunctionBaseElement;
}
baseElement* createMultipclationElement(int elementCount,bool isNegative){
    baseElement *tempMultipclationBaseElement=calloc(1,sizeof(baseElement));
    multipclationElement *tempMultipclationElement=calloc(1,sizeof(multipclationElement));
    tempMultipclationBaseElement->ptr=tempMultipclationElement;
    tempMultipclationBaseElement->type=multipclationElementType;
    tempMultipclationElement->elementCount=elementCount;
    tempMultipclationElement->isNegative=isNegative;
    tempMultipclationElement->elementArray=calloc(elementCount,sizeof(baseElement*));
    return tempMultipclationBaseElement;
}
baseElement* createParanthesesElement(int elementCount,bool isNegative){
    baseElement *tempParanthesesBaseElement=calloc(1,sizeof(baseElement));
    paranthesesElement *tempParanthesesElement=calloc(1,sizeof(paranthesesElement));
    tempParanthesesBaseElement->ptr=tempParanthesesElement;
    tempParanthesesBaseElement->type=paranthesesElementType;
    tempParanthesesElement->elementCount=elementCount;
    tempParanthesesElement->isNegative=isNegative;
    tempParanthesesElement->elementArray=calloc(elementCount,sizeof(baseElement*));
    return tempParanthesesBaseElement;
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
            freeMemoryOfElement(*(temp->elementArray+i));
        }
        free(temp->elementArray);
        free(temp);
        free(element);
        return;
    }
    else if (element->type==multipclationElementType){
        double val=1;
        int i=0;
        multipclationElement *temp= (multipclationElement*)element->ptr;//get the parantheses element as pointer
        baseElement *test1=temp->elementArray[0];
        baseElement *test2=temp->elementArray[1];
        for(i=0;i<temp->elementCount;i++){
            freeMemoryOfElement(*(temp->elementArray+i));
        }
        
        free(temp->elementArray);
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
void printElement(baseElement *element){
    if(element->type==constantElementType)
    {
        printf("%.2f",(getValueOfElement(element,0)));
        return;
    }
    else if(element->type==polinominalElementType)
    {
        polinominalElement *temp =(polinominalElement*) element->ptr;
        printf("%.2fx^%.2f",getValueOfElement(temp->coefficent,0),getValueOfElement(temp->exp,0));
    }
    else if(element->type==paranthesesElementType){
        
        int i=0;
        paranthesesElement *temp= (paranthesesElement*)element->ptr;//get the parantheses element as pointer
        if(temp->isNegative)printf("-");
        printf("(");
        printElement(temp->elementArray[0]);
        for(i=1;i<temp->elementCount;i++){
            printf("+");
            printElement(temp->elementArray[i]);
        }
        printf(")");
        return;
    }
    else if (element->type==multipclationElementType){
        int i=0;
        multipclationElement *temp= (multipclationElement*)element->ptr;//get the parantheses element as pointer
        if(temp->isNegative)printf("-");
        printElement(temp->elementArray[0]);
        for(i=1;i<temp->elementCount;i++){
            printf("*");
            printElement(temp->elementArray[i]);
        }
        return;
    }
    else if(element->type==exponentialElementType){
        exponentialElement *temp =(exponentialElement*) element->ptr;
        if(temp->isNegative)printf("-");
        printElement(temp->element);
        printf("^");
        printElement(temp->exp);
        
        return;
    }
    else if(element->type==singleParameterFunctionElementType){
        singleParameterFunctionElement *temp =(singleParameterFunctionElement*) element->ptr;
        char functions[][10]={"sin","cos","tan","cot","sec","cosec","arcsin","arccos","arcsec","arccosec","arctan","arccot","ln"};
        if(temp->isNegative)printf("-");
        printf("%s(",functions[temp->functionType]);
        printElement(temp->parameter);
        printf(")");
        return;
    }
    else if(element->type==dualParameterFunctionElementType){
        dualParameterFunctionElement *temp =(dualParameterFunctionElement*) element->ptr;
        char functions[][10]={"log"};
        if(temp->isNegative)printf("-");
        printf("%s_",functions[temp->functionType]);
        printElement(temp->firstParameter);
        printf("(");
        printElement(temp->secondParameter);
        printf(")");
        return;
    }
}


void BisectionSearch(baseElement* element){
    int a,b,ep;
    printf("a ve b noktalarini girin\n");
    scanf("%d %d",&a,&b);
    printf("epsilon degerini girin\n");
    scanf("%d",&ep);
    if(getValueOfElement(element,a)<0&&getValueOfElement(element,b)>0){
        while(abs(getValueOfElement(element,a))-ep>0&&abs(getValueOfElement(element,b))-ep>0){
            
        }
    }
}