#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "sayisalAnaliz.h"
baseElement *readFunction(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Lutfen fonksiyonu girin\n");
    char input[1000];
    fgets(input+1,1000,stdin);
    input[0]='(';
    c=0;
    while(input[c]!='\n'&&input[c]!='\0')c++;
    input[c]=')';
    input[c+1]='\0';
    baseElement *elements=(baseElement*)malloc(sizeof(baseElement));
    checkAndConvertToParanthesesElements(input,elements);
    return elements;

}
void addElementToTheList(char ***elementsString, char *input, int *startIndex, int endIndex, int *elementCount)
{
    *elementsString = (char **)realloc(*elementsString, (*elementCount + 1) * sizeof(char *));
    (*elementsString)[*elementCount] = (char*)calloc(endIndex - *startIndex + 1, (sizeof(char)));
    memcpy((*elementsString)[*elementCount], input + *startIndex, endIndex - *startIndex);
    (*elementsString)[*elementCount][endIndex - *startIndex] = '\0';
    *elementCount += 1;
    *startIndex = endIndex;
}
void splitIntoAdditionElements(char ***elementsString, char *input, int *elementCount)
{
    int lastElementIndex = 0, i = 0, paranthesesDeepnes = 0;
    while (input[i] != 0 && input[i] != '\n')
    {
        if ((input[i] == '+' || input[i] == '-') && paranthesesDeepnes == 0)
        {
            addElementToTheList(elementsString, input, &lastElementIndex, i, elementCount);
        }
        else if (input[i] == '(')
            paranthesesDeepnes += 1;
        else if (input[i] == ')')
            paranthesesDeepnes -= 1;
        i++;
    }
    addElementToTheList(elementsString, input, &lastElementIndex, i, elementCount);
}
void splitIntoMultipclationElements(char ***elementsString, char *input, int *elementCount)
{
    int lastElementIndex = 0, i = 0, paranthesesDeepnes = 0;
    while (input[i] != 0 && input[i] != '\n')
    {
        if ((input[i] == '*' || input[i] == '/') && paranthesesDeepnes == 0)
        {
            addElementToTheList(elementsString, input, &lastElementIndex, i, elementCount);
        }
        else if (input[i] == '(')
            paranthesesDeepnes += 1;
        else if (input[i] == ')')
            paranthesesDeepnes -= 1;
        i++;
    }
    addElementToTheList(elementsString, input, &lastElementIndex, i, elementCount);
}
void splitIntoExponentialElements(char ***elementsString, char *input, int *elementCount)
{
    int lastElementIndex = 0, i = 0, paranthesesDeepnes = 0; // represents how many parantheses deep the thing is in so it dont split a exponential inside a parantheses
    while (input[i] != 0 && input[i] != '\n')
    {
        if ((input[i] == '^') && paranthesesDeepnes == 0)
        { // idk if smaller than 0 could happenbut it might be a problem
            addElementToTheList(elementsString, input, &lastElementIndex, i, elementCount);
        }
        else if (input[i] == '(')
            paranthesesDeepnes += 1;
        else if (input[i] == ')')
            paranthesesDeepnes -= 1;

        i++;
    }
    addElementToTheList(elementsString, input, &lastElementIndex, i, elementCount);
}

bool checkAndConvertToConstantElement(char *elementString, baseElement *constant)
{
    int i = 0, j = 0, dotsIndex = -1;
    double *temp = (double*)calloc(1, sizeof(double)); // i was using malloc here and it was not working randomly and ive spent a lot of time trying to fix it :(
    bool isNegative = false, hasSign = true;
    if (elementString[0] == '-')
        isNegative = true;
    else if (elementString[0] != '+')
        hasSign = false;
    i = hasSign;
    if(!strncmp(elementString+hasSign,"e\0",2)||!strncmp(elementString+hasSign,"e\n",2)){
        *temp=(isNegative ? -1:1)*eConstant;
        constant->type = constantElementType;
        constant->ptr = temp;
        return true;
    }
    if(!strncmp(elementString+hasSign,"pi\0",3)||!strncmp(elementString+hasSign,"pi\n",3)){
        *temp=(isNegative ? -1:1)*piConstant;
        constant->type = constantElementType;
        constant->ptr = temp;
        return true;
    }
    while (elementString[i] != '\0')
    {
        if (elementString[i] == '.')
            dotsIndex = i; // we need to know where the dot is
        else if (!(elementString[i] >= '0' && elementString[i] <= '9'))
            return false; // check if its a number if not return
        // printf("constant char count=%d %d\n",i,elementString[i]-'0');
        i++;
    }
    if (dotsIndex == -1)
        dotsIndex = i;
    for (j = dotsIndex + 1; j < i; j++)
    { // add whats after the dot
        *temp += pow(10, dotsIndex - j) * (elementString[j] - '0');
    }
    for (j = dotsIndex - 1; j >= hasSign; j--)
    { // add whats before the dot
        *temp += pow(10, dotsIndex - 1 - j) * (elementString[j] - '0');
    }
    if (isNegative)
        *temp = (*temp) * -1;
    // printf("%lf",*temp);
    constant->type = constantElementType;
    constant->ptr = temp;
    return true;
}
bool checkAndConvertToParanthesesElements(char *input, baseElement *parantheses)
{
    char **elementStrings = (char **)calloc(1, sizeof(char *));
    bool hasSign = true, isNegative = false;
    int elementStringCount = 0, i = 0, elementCount = 0;
    baseElement **elements = (baseElement**)calloc(1, sizeof(baseElement *)); // element array to be stored in the parantheseselement
    
    if (input[0] == '-')
    {
        isNegative = true;
    }
    else if (input[0] != '+')
        hasSign = false;
    while (input[i] != '\n' && input[i] != '\0')
        i++;
    char *substr = substring(input, 1 + hasSign, i - 2);
	splitIntoAdditionElements(&elementStrings, substr, &elementStringCount);
    convertAllElementsIntoTypes(&elementStrings, elementStringCount, &elements, &elementCount);
    paranthesesElement *paranthesesPtr = (paranthesesElement*)calloc(1, sizeof(paranthesesElement)); // create the parantheses element
    parantheses->ptr = paranthesesPtr;                                          // set the pointer for the base element
    parantheses->type = paranthesesElementType;                                 // set the type
    paranthesesPtr->elementArray = elements;                                    // set the array on the parantheses element
    paranthesesPtr->elementCount = elementCount;                                // set the element count
    paranthesesPtr->isNegative = isNegative;
    for (i = 0; i < elementStringCount; i++)
    { // free the string array
        free(elementStrings[i]);
    }
    free(substr);
    free(elementStrings);
    return true;
}
bool checkAndConvertToPolinominal(char *input, baseElement *polinom)
{
    char **elementsString = (char**)calloc(1, sizeof(char *));
    elementsString[0] = (char*)calloc(1, sizeof(char));
    int elementCount = 0, i = 0;
    double coefficent = 1, exp = 0;
    baseElement tempConstant;
    bool isADivider = false, hasSign = true;

    splitIntoMultipclationElements(&elementsString, input, &elementCount); // split the element up into its factors

    for (i = 0; i < elementCount; i++)
    {
        if (elementsString[i][0] == '/')
            isADivider = true; // check for the operator
        else if (elementsString[i][0] != '*')
            hasSign = false;

        if (checkAndConvertToConstantElement(elementsString[i] + hasSign, &tempConstant)) // check if its a constant and multiply or divide coefficent with it
        {
            if (isADivider)
            {
                if (*((double *)tempConstant.ptr) == 0)
                    return false;
                else
                    coefficent = coefficent / (*((double *)tempConstant.ptr));
            }
            else
                coefficent = coefficent * (*((double *)tempConstant.ptr));
        }
        else
        { // if its not a constant check if it
            if (elementsString[i][0] == '-')
            {
                coefficent = coefficent * -1; // check if x is the first element in which case there will be + or - before it
                hasSign = true;
            }
            if (elementsString[0][0] == '+')
                hasSign = true;

            if (elementsString[i][0 + hasSign] == 'x')
            {
                if (elementsString[i][1 + hasSign] == '^')
                {
                    if (checkAndConvertToConstantElement(elementsString[i] + 2 + hasSign, &tempConstant))
                    {
                        exp = exp + (*((double *)tempConstant.ptr)) * (isADivider ? -1 : 1);
                    }
                    else
                    {
                        for (i = 0; i < elementCount; i++)
                        {
                            free(elementsString[i]);
                        }
                        free(elementsString);
                        return false;
                    }
                }
                else
                    exp = exp + (isADivider ? -1 : 1);
            }
            else
                return false;
        }
        isADivider = false;
        hasSign = true;
    }
    if (exp == 0)
    {
        for (i = 0; i < elementCount; i++)
        {
            free(elementsString[i]);
        }
        free(elementsString);
        return false; // return if there is no x
    }
    polinominalElement *tempPolinominalElement = (polinominalElement*)calloc(1, sizeof(polinominalElement));
    baseElement *coefficentElement = (baseElement*)calloc(1, sizeof(baseElement)), *expElement = (baseElement*)calloc(1, sizeof(baseElement)); // make a base element for coefficent and exp values (this isnt usefull for poliniminal elements since both exp and coefficent is gonna be constant but will come in usefull when dealing with exponantial functions)
    coefficentElement->type = constantElementType;
    expElement->type = constantElementType;

    double *coefficentPtr = (double*)calloc(1, sizeof(double)), *expPtr = (double*)calloc(1, sizeof(double)); // make pointers for coeffient and exp to be stored int their baseElement
    *coefficentPtr = coefficent;
    *expPtr = exp;
    coefficentElement->ptr = coefficentPtr;
    expElement->ptr = expPtr;
    tempPolinominalElement->coefficent = coefficentElement;
    tempPolinominalElement->exp = expElement;
    polinom->ptr = tempPolinominalElement;
    polinom->type = polinominalElementType;

    for (i = 0; i < elementCount; i++)
    {
        free(elementsString[i]);
    }
    free(elementsString);
    return true;
}
bool checkAndConvertToMultipclationElement(char *input, baseElement *multipclationElementBase)
{
    char **elementsString = (char**)calloc(1, sizeof(char *));

    elementsString[0] = (char*)calloc(1, sizeof(char));
    int elementStringCount = 0, i;
    splitIntoMultipclationElements(&elementsString, input, &elementStringCount); // split the input into parts
    if (elementStringCount == 1)
    {
        for (i = 0; i < elementStringCount; i++)
        {
            free(elementsString[i]);
        }
        free(elementsString);
        return false;
    } // if its one factor there is no need for it to be MultipclationElement
    baseElement **elements = (baseElement**)calloc(elementStringCount, sizeof(baseElement *));
    bool hasSign = true, isADivider = false;

    for (i = 0; i < elementStringCount; i++)
    {
        hasSign = true;
        isADivider = false;
        if (elementsString[i][0] == '/')
            isADivider = true;
        else if (elementsString[i][0] != '*')
            hasSign = false;
        elements[i] = (baseElement*)calloc(1, sizeof(baseElement));
        convertElementIntoTypes(elementsString[i] + hasSign, *(elements + i)); // try and convert it into a type
        if (isADivider)
        {                                                                  // change this element with a exponential element that has a exp of -1
            baseElement *expConstantBase = (baseElement*)calloc(1, sizeof(baseElement)); // make a base element for exp and put -1 in it
            double *expValuePtr = (double*)calloc(1, sizeof(double));
            *expValuePtr = -1;
            expConstantBase->type = constantElementType; // set up exp base and coefficent base
            expConstantBase->ptr = expValuePtr;
            exponentialElement *divider = (exponentialElement*)calloc(1, sizeof(exponentialElement)); // set the exponentialElement
            divider->isNegative = false;                                         // if it was something like 1/-2 it should be written as 1/(-2)
            baseElement *temp = (baseElement*)calloc(1, sizeof(baseElement));                  // make a copy of the element since its gonna get replaced
            memcpy(temp, *(elements + i), sizeof(baseElement));
            divider->exp = expConstantBase;
            divider->element = temp;
            (*(elements + i))->type = exponentialElementType;
            (*(elements + i))->ptr = divider;
        }
    }
    multipclationElement *temp = (multipclationElement*)calloc(1, sizeof(multipclationElement));
    temp->elementArray = elements;
    temp->elementCount = elementStringCount;
    multipclationElementBase->type = multipclationElementType;
    multipclationElementBase->ptr = temp;

    for (i = 0; i < elementStringCount; i++)
    {
        free(elementsString[i]);
    }
    free(elementsString);
    return true;
}
bool checkAndConvertToExponentialElement(char *input, baseElement *expElementBase)
{
    char **elementsString = (char**)calloc(1, sizeof(char *));
    baseElement *base = (baseElement*)calloc(1, sizeof(baseElement)), *exp = (baseElement*)calloc(1, sizeof(baseElement));
    exponentialElement *expElement = (exponentialElement*)calloc(1, sizeof(exponentialElement));
    elementsString[0] = (char*)calloc(1, sizeof(char));
    int elementStringCount = 0, i = 1, elementCount = 0;
    splitIntoExponentialElements(&elementsString, input, &elementStringCount); // split it up
    if (elementStringCount == 1)
    {
        for (i = 0; i < elementStringCount; i++)
        {
            free(elementsString[i]);
        }
        free(elementsString);
        return false;
    }
    if (elementsString[0][0] == '-')
    {
        expElement->isNegative = true;
        convertElementIntoTypes(elementsString[0] + 1, base);
    }
    else convertElementIntoTypes(elementsString[0], base); // maybe i should make it work for more then one exponential but then syntax gets unpredictable imo
    convertElementIntoTypes(elementsString[1] + 1, exp);
    expElementBase->type = exponentialElementType;
    expElementBase->ptr = expElement;
    expElement->element = base;
    expElement->exp = exp;

    for (i = 0; i < elementStringCount; i++)
    {
        free(elementsString[i]);
    }
    free(elementsString);
    return true;
}
bool checkAndConvertToSingleParameterFunctionElement(char *input, baseElement *singleParameterFunctionElementBase)
{
    char **elementsString = (char**)calloc(1, sizeof(char *));
    elementsString[0] = (char*)calloc(1, sizeof(char));
    int elementCount = 0, elementStringCount = 0;
    splitIntoMultipclationElements(&elementsString, input, &elementStringCount); // check if there is any other factor than the function return if there is cause we want elements with more than one factor to be stored in the MultipclationElement
    if (elementStringCount != 1)
        return false;
    baseElement *parameter = (baseElement*)calloc(1, sizeof(baseElement));
    singleParameterFunctionElement *singleParameterFuncElement = (singleParameterFunctionElement*)calloc(1, sizeof(singleParameterFunctionElement));
    char functions[][10] = {"sin", "cos", "tan", "cot", "sec", "cosec", "arcsin", "arccos", "arcsec", "arccosec", "arctan", "arccot", "ln"};
    int lenghts[] = {3, 3, 3, 3, 3, 5, 6, 6, 6, 8, 6, 6, 2};
    bool hasSign = true, isNegative = false; // this could actually be made into a func but idk if its worth it
    if (input[0] == '-')
        isNegative = true;
    else if (input[0] != '+')
        hasSign = false;
    int i = 0, inputLenght = 0;
    while (input[inputLenght + hasSign] != '(')
        inputLenght++; // check if it goes to \0
    while ((inputLenght != lenghts[i] || strncmp(input + hasSign, functions[i], lenghts[i])) && i < 13)
        i++; // find which function this is
    if (i >= 13)
    {
        for (i = 0; i < elementStringCount; i++)
        {
            free(elementsString[i]);
        }
        free(elementsString);
        return false;
    }

    singleParameterFuncElement->functionType = (singleParameterFunctionTypes)i;
    convertElementIntoTypes(input + lenghts[i] + hasSign, parameter); // convert the parameter into types
    singleParameterFuncElement->parameter = parameter;
    singleParameterFuncElement->isNegative = isNegative;
    singleParameterFunctionElementBase->ptr = singleParameterFuncElement;
    singleParameterFunctionElementBase->type = singleParameterFunctionElementType; // set the element
    for (i = 0; i < elementStringCount; i++)
    { // free the string array
        free(elementsString[i]);
    }
    free(elementsString);
    return true;
}
bool checkAndConvertToDualParameterFunctionElement(char *input, baseElement *dualParameterFunctionElementBase)
{
    char **elementsString = (char**)calloc(1, sizeof(char *));
    elementsString[0] = (char*)calloc(1, sizeof(char));
    int elementStringCount = 0;
    splitIntoMultipclationElements(&elementsString, input, &elementStringCount); // check if there is any other factor than the function return if there is cause we want elements with more than one factor to be stored in the MultipclationElement
    if (elementStringCount != 1)
        return false;
    baseElement *parameter1 = (baseElement*)calloc(1, sizeof(baseElement)), *parameter2 = (baseElement*)calloc(1, sizeof(baseElement));
    dualParameterFunctionElement *dualParameterFuncElement = (dualParameterFunctionElement*)calloc(1, sizeof(dualParameterFunctionElement));
    char functions[][10] = {"log"};
    int lenghts[] = {3};
    bool hasSign = true, isNegative = false; // this could actually be made into a func but idk if its worth it
    if (input[0] == '-')
        isNegative = true;
    else if (input[0] != '+')
        hasSign = false;
    dualParameterFuncElement->isNegative = isNegative;

    int i = 0, j = 0, inputLenght = 0;
    while (input[inputLenght + hasSign] != '_' && input[inputLenght + hasSign] != '\0' && input[inputLenght + hasSign] != '\n')
        inputLenght++;
    if (input[inputLenght + hasSign] != '_')
        return false;
    while ((inputLenght != lenghts[i] || strncmp(input + hasSign, functions[i], lenghts[i])) && i < 1)
        i++; // find out which function it is
    if (i >= 1)
        return false;
    if (input[lenghts[i] + hasSign] != '_')
    {
        for (i = 0; i < elementStringCount; i++)
        {
            free(elementsString[i]);
        }
        free(elementsString);
        return false;
    }
    while (input[j] != '\0' && input[j] != '(')
        j++; // either find where the first parameter ends and the second one starts or find if there is no second parameter
    if (input[j] == '\0')
    {
        for (i = 0; i < elementStringCount; i++)
        {
            free(elementsString[i]);
        }
        free(elementsString);
        return false;
    } // means there is no second parameter

    dualParameterFuncElement->functionType = (dualParameterFunctionTypes)i;
    char *parameter1String = substring(input, lenghts[i] + hasSign + 1, j - 1);
    convertElementIntoTypes(parameter1String, parameter1); // convert the first parameter into types
    convertElementIntoTypes(input + j, parameter2);        // convert the second parameter into types
    dualParameterFuncElement->firstParameter = parameter1;
    dualParameterFuncElement->secondParameter = parameter2;
    dualParameterFuncElement->isNegative = isNegative;
    dualParameterFunctionElementBase->ptr = dualParameterFuncElement;
    dualParameterFunctionElementBase->type = dualParameterFunctionElementType; // set the element
    for (i = 0; i < elementStringCount; i++)
    { // free the string array
        free(elementsString[i]);
    }
    free(elementsString);
    free(parameter1String);
    return true;
}

double getValueOfElement(baseElement *element, double x)
{
    if (element->type == constantElementType)
    {
        // printf("constant %lf\n",*((double*)(element->ptr)));
        return *((double *)(element->ptr)); // this part isnt switch case cause i thought it was easier to see
    }
    else if (element->type == polinominalElementType)
    {
        // printf("polinominal %lf\n",(*((double*)(((polinominalElement*)(element->ptr))->coefficent->ptr)))*pow(x,*((double*)(((polinominalElement*)(element->ptr))->exp->ptr))));
        return (*((double *)(((polinominalElement *)(element->ptr))->coefficent->ptr))) * pow(x, *((double *)(((polinominalElement *)(element->ptr))->exp->ptr)));
    }
    else if (element->type == paranthesesElementType)
    {
        double sum = 0;
        int i = 0;
        // printf("entering paratez\n");
        paranthesesElement *temp = (paranthesesElement *)element->ptr; // get the parantheses element as pointer
        for (i = 0; i < temp->elementCount; i++)
        {
            sum = sum + getValueOfElement(*(temp->elementArray + i), x);
        }
        // printf("parantheses %lf\n",sum);
        sum = sum * (temp->isNegative ? -1 : 1);
        return sum;
    }
    else if (element->type == multipclationElementType)
    {
        double val = 1, tempDouble = 1;
        int i = 0;
        // printf("entering carpma\n");
        multipclationElement *temp = (multipclationElement *)element->ptr; // get the multipclation element as pointer
        while (i < temp->elementCount && tempDouble != 0)
        { // to still get zero if there were values like nan or inf
            tempDouble = getValueOfElement(*(temp->elementArray + i), x);
            val = val * tempDouble;
            i++;
        }
        if (tempDouble == 0)
            val = 0;
        // printf("multipclation %lf\n",val);
        val = val * (temp->isNegative ? -1 : 1);
        return val;
    }
    else if (element->type == exponentialElementType)
    {
        exponentialElement *temp = (exponentialElement *)element->ptr;
        //printf("element value=%lf\nexp value=%lf\n",getValueOfElement(temp->element,x),getValueOfElement(temp->exp,x));
         //printf("exp %lf\n",pow(getValueOfElement(temp->element,x),getValueOfElement(temp->exp,x)));
        return (temp->isNegative ? -1 : 1) * pow(getValueOfElement(temp->element, x), getValueOfElement(temp->exp, x));
    }
    else if (element->type == singleParameterFunctionElementType)
    {
        singleParameterFunctionElement *temp = (singleParameterFunctionElement*)element->ptr;
        double value, temp1;
        // printf("entering func\n");
        switch (temp->functionType)
        {
        case sinT:
            value = sin(getValueOfElement(temp->parameter, x));
            break;
        case cosT:
            value = cos(getValueOfElement(temp->parameter, x));
            break;
        case tanT:
            value = tan(getValueOfElement(temp->parameter, x));
            break;
        case cotT:
            value = 1 / tan(getValueOfElement(temp->parameter, x));
            break;
        case secT:
            value = 1 / cos(getValueOfElement(temp->parameter, x));
            break;
        case cosecT:
            value = 1 / sin(getValueOfElement(temp->parameter, x));
            break;
        case arcsinT:
            value = asin(getValueOfElement(temp->parameter, x));
            break;
        case arccosT:
            value = acos(getValueOfElement(temp->parameter, x));
            break;
        case arctanT:
            value = atan(getValueOfElement(temp->parameter, x));
            break;
        case arccotT:
            value = atan(1 / getValueOfElement(temp->parameter, x)); // idk if this works
        case arcsecT:
            value = acos(1 / getValueOfElement(temp->parameter, x));
            break;
        case arccosecT:
            value = asin(1 / getValueOfElement(temp->parameter, x));
            break;
        case lnT:
            temp1 = getValueOfElement(temp->parameter, x);
            if (temp1 < 0)
                printf("ln parametresi tanim araligi disinda degiskenin mutlak degeri alinip islem yapilacak (buyuk ihtimalle hatali olucak)\n");
            value = log(fabs(getValueOfElement(temp->parameter, x))) / log(eConstant);
            break;

        default:
            break;
        }
        if (temp->isNegative)
            value = value * -1;
        // printf("funct %lf\n",value);
        return value;
    }
    else if (element->type == dualParameterFunctionElementType)
    {
        dualParameterFunctionElement *temp = (dualParameterFunctionElement*)element->ptr;
        double value;
        // printf("entering log\n");
        if(temp->functionType==logT){
            double temp1 = getValueOfElement(temp->secondParameter, x), temp2 = getValueOfElement(temp->firstParameter, x);
            if (temp1 < 0 || temp2 < 0 || temp2 == 1)
                printf("log parametresi tanim araligi disinda degiskenin mutlak degeri alinip islem yapilacak (buyuk ihtimalle hatali olucak)\n");
            value = (temp->isNegative ? -1 : 1) * log(temp1) / log(temp2);
        }
        // printf("func 2 %lf\n",value);
        return value;
    }
    return -1;
}
bool convertElementIntoTypes(char *elementsString, baseElement *elements)
{
    // printf("%s\n",elementsString);
    if (checkAndConvertToConstantElement(elementsString, elements))
        ;
    else if (checkAndConvertToPolinominal(elementsString, elements))
        ;
    else if (checkAndConvertToExponentialElement(elementsString, elements))
        ;
    else if (checkAndConvertToSingleParameterFunctionElement(elementsString, elements))
        ;
    else if (checkAndConvertToDualParameterFunctionElement(elementsString, elements))
        ;
    else if (checkAndConvertToMultipclationElement(elementsString, elements))
        ;
    else if (checkAndConvertToParanthesesElements(elementsString, elements))
        ;
    else
        return false;
    return true;
}
void convertAllElementsIntoTypes(char ***elementsString, int elementStringCount, baseElement ***elements, int *elementCount)
{
    int i;
    for (i = 0; i < elementStringCount; i++)
    {
        *elements = (baseElement **)realloc(*elements, (*elementCount + 1) * sizeof(baseElement *));
        *elementCount += 1; // if it didnt return it means it was succesfull and that we need to make room for more elements
        *(*elements + i) = (baseElement*)calloc(1, sizeof(baseElement));
        if (convertElementIntoTypes((*elementsString)[i], *(*elements + i)));//should probably do some stuff here if it returns false. Eh maybe later
    }
}

baseElement *derivate(baseElement *element)
{
    if (element->type == constantElementType)
        return createConstantElement(0);

    else if (element->type == polinominalElementType)
    {
        polinominalElement *temp = (polinominalElement*)element->ptr;
        return createPolinominalElementAndFill(createConstantElement(getValueOfElement(temp->coefficent, 0) * getValueOfElement(temp->exp, 0)), createConstantElement(getValueOfElement(temp->exp, 0) - 1));
    }
    else if (element->type == exponentialElementType)
    {                                                       // d(f(x)^g(x))= ( gt(x)*ln(f(x))+ft(x)*g(x)/f(x) )*f(x)^g(x)
        baseElement *tempBase = makeCopyOfElement(element); // make a copy of element so that when i free element i can still free the derivative of element
        exponentialElement *temp = (exponentialElement*)tempBase->ptr;

        baseElement *derivitive = createMultipclationElement(2, false);
        baseElement *ftGdivFxBaseElement = createMultipclationElement(3, false), *gtLnFBaseElement = createMultipclationElement(2, false);

        multipclationElement *mainMultipclationElement = (multipclationElement*)derivitive->ptr, *gtLnF = (multipclationElement*)gtLnFBaseElement->ptr, *ftGDivFx = (multipclationElement*)ftGdivFxBaseElement->ptr;
        baseElement *mainParanthesesBaseElement = createParanthesesElement(2, false);
        paranthesesElement *mainParanthesesElement = (paranthesesElement*)mainParanthesesBaseElement->ptr;

        mainMultipclationElement->elementArray[0] = element; //*f(x)^g(x)this part
        mainMultipclationElement->elementArray[1] = mainParanthesesBaseElement;

        gtLnF->elementArray[0] = derivate(temp->exp);
        gtLnF->elementArray[1] = createSingleParameterFunctionElementAndFill(lnT, false, temp->element);

        ftGDivFx->elementArray[0] = derivate(temp->element);
        ftGDivFx->elementArray[1] = temp->exp;
        ftGDivFx->elementArray[2] = createExponentialElementAndFill(false, temp->element, createConstantElement(-1));

        mainParanthesesElement->elementArray[0] = gtLnFBaseElement;
        mainParanthesesElement->elementArray[1] = ftGdivFxBaseElement;

        free(tempBase);
        return derivitive;
    }
    else if (element->type == singleParameterFunctionElementType)
    {
        baseElement *tempBase = makeCopyOfElement(element);
        singleParameterFunctionElement *temp = (singleParameterFunctionElement*)tempBase->ptr;
        baseElement *derivitive;
        if (temp->functionType == sinT)
        {
            derivitive = createMultipclationElement(2, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createSingleParameterFunctionElementAndFill(cosT, false, temp->parameter);
            return derivitive;
        }
        else if (temp->functionType == cosT)
        {
            derivitive = createMultipclationElement(2, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createSingleParameterFunctionElementAndFill(sinT, true, temp->parameter);
            return derivitive;
        }
        else if (temp->functionType == tanT)
        {
            derivitive = createMultipclationElement(2, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, createSingleParameterFunctionElementAndFill(secT, false, temp->parameter), createConstantElement(2));
            return derivitive;
        }
        else if (temp->functionType == cotT)
        {
            derivitive = createMultipclationElement(2, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, createSingleParameterFunctionElementAndFill(cosecT, true, temp->parameter), createConstantElement(-2));
            return derivitive;
        }
        else if (temp->functionType == secT)
        {
            derivitive = createMultipclationElement(3, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createSingleParameterFunctionElementAndFill(tanT, false, temp->parameter);
            tempMultipclationElement->elementArray[2] = element; // this is pretty scary ngl
            return derivitive;
        }
        else if (temp->functionType == cosecT)
        {
            derivitive = createMultipclationElement(3, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createSingleParameterFunctionElementAndFill(cotT, true, temp->parameter);
            tempMultipclationElement->elementArray[2] = element; // this is pretty scary ngl
            return derivitive;
        }
        else if (temp->functionType == arcsinT)
        {
            derivitive = createMultipclationElement(2, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);

            baseElement *tempParanthesesElementBase = createParanthesesElement(2, false);
            paranthesesElement *tempParanthesesElement = (paranthesesElement*)tempParanthesesElementBase->ptr;
            baseElement *parameterSquared = createExponentialElementAndFill(true, temp->parameter, createConstantElement(2));
            tempParanthesesElement->elementArray[0] = createConstantElement(1);
            tempParanthesesElement->elementArray[1] = parameterSquared;

            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, tempParanthesesElementBase, createConstantElement(-0.5));
            return derivitive;
        }
        else if (temp->functionType == arccosT)
        {
            derivitive = createMultipclationElement(2, !temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);

            baseElement *tempParanthesesElementBase = createParanthesesElement(2, false);
            paranthesesElement *tempParanthesesElement = (paranthesesElement*)tempParanthesesElementBase->ptr;
            baseElement *parameterSquared = createExponentialElementAndFill(true, temp->parameter, createConstantElement(2));
            tempParanthesesElement->elementArray[0] = createConstantElement(1);
            tempParanthesesElement->elementArray[1] = parameterSquared;

            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, tempParanthesesElementBase, createConstantElement(-0.5));
            return derivitive;
        }
        else if (temp->functionType == arctanT)
        {
            derivitive = createMultipclationElement(2, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);

            baseElement *tempParanthesesElementBase = createParanthesesElement(2, false);
            paranthesesElement *tempParanthesesElement = (paranthesesElement*)tempParanthesesElementBase->ptr;
            baseElement *parameterSquared = createExponentialElementAndFill(false, temp->parameter, createConstantElement(2));
            tempParanthesesElement->elementArray[0] = createConstantElement(1);
            tempParanthesesElement->elementArray[1] = parameterSquared;

            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, tempParanthesesElementBase, createConstantElement(-1));
            return derivitive;
        }
        else if (temp->functionType == arccotT)
        {
            derivitive = createMultipclationElement(2, !temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);

            baseElement *tempParanthesesElementBase = createParanthesesElement(2, false);
            paranthesesElement *tempParanthesesElement = (paranthesesElement*)tempParanthesesElementBase->ptr;
            baseElement *parameterSquared = createExponentialElementAndFill(false, temp->parameter, createConstantElement(2));
            tempParanthesesElement->elementArray[0] = createConstantElement(1);
            tempParanthesesElement->elementArray[1] = parameterSquared;

            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, tempParanthesesElementBase, createConstantElement(-1));
            return derivitive;
        }
        else if (temp->functionType == arcsecT)
        {
            derivitive = createMultipclationElement(3, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, temp->parameter, createConstantElement(-1));

            baseElement *tempParanthesesElementBase = createParanthesesElement(2, false);
            paranthesesElement *tempParanthesesElement = (paranthesesElement*)tempParanthesesElementBase->ptr;
            baseElement *parameterSquared = createExponentialElementAndFill(false, temp->parameter, createConstantElement(2));
            tempParanthesesElement->elementArray[0] = createConstantElement(-1);
            tempParanthesesElement->elementArray[1] = parameterSquared;

            tempMultipclationElement->elementArray[2] = createExponentialElementAndFill(false, tempParanthesesElementBase, createConstantElement(-0.5));
            return derivitive;
        }
        else if (temp->functionType == arccosecT)
        {
            derivitive = createMultipclationElement(3, !temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, temp->parameter, createConstantElement(-1));

            baseElement *tempParanthesesElementBase = createParanthesesElement(2, false);
            paranthesesElement *tempParanthesesElement = (paranthesesElement*)tempParanthesesElementBase->ptr;
            baseElement *parameterSquared = createExponentialElementAndFill(false, temp->parameter, createConstantElement(2));
            tempParanthesesElement->elementArray[0] = createConstantElement(-1);
            tempParanthesesElement->elementArray[1] = parameterSquared;

            tempMultipclationElement->elementArray[2] = createExponentialElementAndFill(false, tempParanthesesElementBase, createConstantElement(-0.5));
            return derivitive;
        }
        else if (temp->functionType == lnT)
        { // d/dx(ln(f(x))=ft/f
            derivitive = createMultipclationElement(2, temp->isNegative);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr;
            tempMultipclationElement->elementArray[0] = derivate(temp->parameter);
            tempMultipclationElement->elementArray[1] = createExponentialElementAndFill(false, temp->parameter, createConstantElement(-1));
            return derivitive;
        }
        free(tempBase);
    }
    else if (element->type == dualParameterFunctionElementType)
    {
        baseElement *tempBase = makeCopyOfElement(element);
        dualParameterFunctionElement *temp = (dualParameterFunctionElement*)tempBase->ptr;
        if (temp->functionType == logT)
        { // d/dx(log_fx(g(x))=(gt*lnf/g-fd*lng/f)/((lnf)^2)
            baseElement *derivitive = createMultipclationElement(2, temp->isNegative);
            baseElement *mainParanthesesBaseElement = createParanthesesElement(2, false), *gtLnFDivGBaseElement = createMultipclationElement(3, false), *fdLnGdivFBaseElement = createMultipclationElement(3, true);
            multipclationElement *tempMultipclationElement = (multipclationElement*)derivitive->ptr, *gtLnFDivG = (multipclationElement*)gtLnFDivGBaseElement->ptr, *fdLnGdivF = (multipclationElement*)fdLnGdivFBaseElement->ptr;
            tempMultipclationElement->elementArray[0] = createExponentialElementAndFill(false, createSingleParameterFunctionElementAndFill(lnT, false, makeCopyOfElement(temp->firstParameter)), createConstantElement(-2)); // =/((lnf)^2)
            tempMultipclationElement->elementArray[1] = mainParanthesesBaseElement;
            paranthesesElement *mainParanthesesElement = (paranthesesElement*)mainParanthesesBaseElement->ptr; // set the main parantheses
            mainParanthesesElement->elementArray[0] = gtLnFDivGBaseElement;
            mainParanthesesElement->elementArray[1] = fdLnGdivFBaseElement;

            gtLnFDivG->elementArray[0] = derivate(temp->secondParameter);
            gtLnFDivG->elementArray[1] = createSingleParameterFunctionElementAndFill(lnT, false, makeCopyOfElement(temp->firstParameter));
            gtLnFDivG->elementArray[2] = createExponentialElementAndFill(false, makeCopyOfElement(temp->secondParameter), createConstantElement(-1));

            fdLnGdivF->elementArray[0] = derivate(temp->firstParameter);
            fdLnGdivF->elementArray[1] = createSingleParameterFunctionElementAndFill(lnT, false, temp->secondParameter);
            fdLnGdivF->elementArray[2] = createExponentialElementAndFill(false, temp->firstParameter, createConstantElement(-1));
            free(tempBase);
            return derivitive;
        }
    }
    else if (element->type == multipclationElementType)
    {
        int i = 0, j = 0;
        baseElement *tempBase = makeCopyOfElement(element);
        multipclationElement *temp = (multipclationElement*)tempBase->ptr;
        baseElement *derivitive = createParanthesesElement(temp->elementCount, temp->isNegative);
        paranthesesElement *newMultipclationElement = (paranthesesElement*)derivitive->ptr;
        for (i = 0; i < newMultipclationElement->elementCount; i++)
        {
            baseElement *tempSubMultipclationBaseElement = createMultipclationElement(newMultipclationElement->elementCount, false);
            multipclationElement *tempSubMultipclationElement = (multipclationElement*)tempSubMultipclationBaseElement->ptr;
            for (j = 0; j < tempSubMultipclationElement->elementCount; j++)
            {
                if (i != j)
                    tempSubMultipclationElement->elementArray[j] = temp->elementArray[j];
                else
                    tempSubMultipclationElement->elementArray[j] = derivate(temp->elementArray[j]);
            }
            newMultipclationElement->elementArray[i] = tempSubMultipclationBaseElement;
        }
        free(tempBase);
        return derivitive;
    }
    else if (element->type == paranthesesElementType)
    {
        int i = 0;
        baseElement *tempBase = makeCopyOfElement(element);
        paranthesesElement *temp = (paranthesesElement*)tempBase->ptr;
        baseElement *derivitive = createParanthesesElement(temp->elementCount, temp->isNegative);
        paranthesesElement *newParanthesesElement = (paranthesesElement*)derivitive->ptr;
        for (i = 0; i < newParanthesesElement->elementCount; i++)
        {
            newParanthesesElement->elementArray[i] = derivate(temp->elementArray[i]);
        }
        free(tempBase);
        return derivitive;
    }
    return NULL;
}
baseElement *createConstantElement(double value)
{
    baseElement *tempConstantBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    double *tempDouble = (double*)calloc(1, sizeof(double));
    *tempDouble = value;
    tempConstantBaseElement->ptr = tempDouble;
    return tempConstantBaseElement;
}
baseElement *createPolinominalElementAndFill(baseElement *coefficent, baseElement *exp)
{
    baseElement *tempPolinominalBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    polinominalElement *tempPolinominalElement = (polinominalElement*)calloc(1, sizeof(polinominalElement));
    tempPolinominalBaseElement->type = polinominalElementType;
    tempPolinominalBaseElement->ptr = tempPolinominalElement;
    tempPolinominalElement->coefficent = coefficent;
    tempPolinominalElement->exp = exp;
    return tempPolinominalBaseElement;
}
baseElement *createExponentialElement(bool isNegative)
{
    baseElement *tempExponentailBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    exponentialElement *tempExponentialElement = (exponentialElement*)calloc(1, sizeof(exponentialElement));
    tempExponentailBaseElement->type = exponentialElementType;
    tempExponentailBaseElement->ptr = tempExponentialElement;
    tempExponentialElement->isNegative = isNegative;
    return tempExponentailBaseElement;
}
baseElement *createExponentialElementAndFill(bool isNegative, baseElement *element, baseElement *exp)
{
    baseElement *tempExponentailBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    exponentialElement *tempExponentialElement = (exponentialElement*)calloc(1, sizeof(exponentialElement));
    tempExponentailBaseElement->type = exponentialElementType;
    tempExponentailBaseElement->ptr = tempExponentialElement;
    tempExponentialElement->element = element;
    tempExponentialElement->exp = exp;
    tempExponentialElement->isNegative = isNegative;
    return tempExponentailBaseElement;
}
baseElement *createSingleParameterFunctionElement(singleParameterFunctionTypes type, bool isNegative)
{
    baseElement *tempSingleParameterFunctionBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    singleParameterFunctionElement *tempSingleParameterFunctionElement = (singleParameterFunctionElement*)calloc(1, sizeof(singleParameterFunctionElement));
    tempSingleParameterFunctionBaseElement->ptr = tempSingleParameterFunctionElement;
    tempSingleParameterFunctionBaseElement->type = singleParameterFunctionElementType;
    tempSingleParameterFunctionElement->functionType = type;
    tempSingleParameterFunctionElement->isNegative = isNegative;
    return tempSingleParameterFunctionBaseElement;
}
baseElement *createSingleParameterFunctionElementAndFill(singleParameterFunctionTypes type, bool isNegative, baseElement *parameter)
{
    baseElement *tempSingleParameterFunctionBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    singleParameterFunctionElement *tempSingleParameterFunctionElement = (singleParameterFunctionElement*)calloc(1, sizeof(singleParameterFunctionElement));
    tempSingleParameterFunctionBaseElement->ptr = tempSingleParameterFunctionElement;
    tempSingleParameterFunctionBaseElement->type = singleParameterFunctionElementType;
    tempSingleParameterFunctionElement->functionType = type;
    tempSingleParameterFunctionElement->isNegative = isNegative;
    tempSingleParameterFunctionElement->parameter = parameter;
    return tempSingleParameterFunctionBaseElement;
}
baseElement *createDualParameterFunctionElement(dualParameterFunctionTypes type, bool isNegative)
{
    baseElement *tempDualParameterFunctionBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    dualParameterFunctionElement *tempDualParameterFunctionElement = (dualParameterFunctionElement*)calloc(1, sizeof(dualParameterFunctionElement));
    tempDualParameterFunctionBaseElement->ptr = tempDualParameterFunctionElement;
    tempDualParameterFunctionBaseElement->type = dualParameterFunctionElementType;
    tempDualParameterFunctionElement->functionType = type;
    tempDualParameterFunctionElement->isNegative = isNegative;
    return tempDualParameterFunctionBaseElement;
}
baseElement *createDualParameterFunctionElementAndFill(dualParameterFunctionTypes type, bool isNegative, baseElement *firstParameter, baseElement *secondParameter)
{
    baseElement *tempDualParameterFunctionBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    dualParameterFunctionElement *tempDualParameterFunctionElement = (dualParameterFunctionElement*)calloc(1, sizeof(dualParameterFunctionElement));
    tempDualParameterFunctionBaseElement->ptr = tempDualParameterFunctionElement;
    tempDualParameterFunctionBaseElement->type = dualParameterFunctionElementType;
    tempDualParameterFunctionElement->functionType = type;
    tempDualParameterFunctionElement->isNegative = isNegative;
    tempDualParameterFunctionElement->firstParameter = firstParameter;
    tempDualParameterFunctionElement->secondParameter = secondParameter;
    return tempDualParameterFunctionBaseElement;
}
baseElement *createMultipclationElement(int elementCount, bool isNegative)
{
    baseElement *tempMultipclationBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    multipclationElement *tempMultipclationElement = (multipclationElement*)calloc(1, sizeof(multipclationElement));
    tempMultipclationBaseElement->ptr = tempMultipclationElement;
    tempMultipclationBaseElement->type = multipclationElementType;
    tempMultipclationElement->elementCount = elementCount;
    tempMultipclationElement->isNegative = isNegative;
    tempMultipclationElement->elementArray = (baseElement**)calloc(elementCount, sizeof(baseElement *));
    return tempMultipclationBaseElement;
}
baseElement *createParanthesesElement(int elementCount, bool isNegative)
{
    baseElement *tempParanthesesBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    paranthesesElement *tempParanthesesElement = (paranthesesElement*)calloc(1, sizeof(paranthesesElement));
    tempParanthesesBaseElement->ptr = tempParanthesesElement;
    tempParanthesesBaseElement->type = paranthesesElementType;
    tempParanthesesElement->elementCount = elementCount;
    tempParanthesesElement->isNegative = isNegative;
    tempParanthesesElement->elementArray = (baseElement**)calloc(elementCount, sizeof(baseElement *));
    return tempParanthesesBaseElement;
}
baseElement *createMatrixElementWithAllElementsSetToZero(int n, int m)
{
    int i,j;
    baseElement *tempMatrixBaseElement = (baseElement*)calloc(1, sizeof(baseElement));
    matrixElement *tempMatrixElement = (matrixElement*)calloc(1, sizeof(matrixElement));
    tempMatrixBaseElement->ptr = tempMatrixElement;
    tempMatrixBaseElement->type = matrixElementType;
    tempMatrixElement->n = n;
    tempMatrixElement->m = m;
    tempMatrixElement->elementMatrix = (baseElement***)calloc(n, sizeof(baseElement **));
    for(i=0;i<n;i++){
        tempMatrixElement->elementMatrix[i] = (baseElement**)calloc(m, sizeof(baseElement *));
        for(j=0;j<m;j++){
            tempMatrixElement->elementMatrix[i][j]=createConstantElement(0);
        }
    }
    return tempMatrixBaseElement;
}
baseElement *makeCopyOfElement(baseElement *element)
{
    baseElement *copy;
    if (element->type == constantElementType)
    {
        copy = createConstantElement(getValueOfElement(element, 0));
        return copy;
    }
    else if (element->type == polinominalElementType)
    {
        polinominalElement *temp = (polinominalElement *)element->ptr;
        copy = createPolinominalElementAndFill(createConstantElement(getValueOfElement(temp->coefficent, 0)), createConstantElement(getValueOfElement(temp->exp, 0)));
        return copy;
    }
    else if (element->type == paranthesesElementType)
    {

        int i = 0;
        paranthesesElement *temp = (paranthesesElement *)element->ptr; // get the parantheses element as pointer
        copy = createParanthesesElement(temp->elementCount, temp->isNegative);
        paranthesesElement *temp2 = (paranthesesElement *)copy->ptr;
        for (i = 0; i < temp->elementCount; i++)
        {
            temp2->elementArray[i] = makeCopyOfElement(temp->elementArray[i]);
        }
        return copy;
    }
    else if (element->type == multipclationElementType)
    {
        int i = 0;
        multipclationElement *temp = (multipclationElement *)element->ptr; // get the parantheses element as pointer
        copy = createMultipclationElement(temp->elementCount, temp->isNegative);
        multipclationElement *temp2 = (multipclationElement *)copy->ptr;
        for (i = 0; i < temp->elementCount; i++)
        {
            temp2->elementArray[i] = makeCopyOfElement(temp->elementArray[i]);
        }
        return copy;
    }
    else if (element->type == exponentialElementType)
    {
        exponentialElement *temp = (exponentialElement *)element->ptr;
        copy = createExponentialElementAndFill(temp->isNegative, makeCopyOfElement(temp->element), makeCopyOfElement(temp->exp));
        return copy;
    }
    else if (element->type == singleParameterFunctionElementType)
    {
        singleParameterFunctionElement *temp = (singleParameterFunctionElement *)element->ptr;
        copy = createSingleParameterFunctionElementAndFill(temp->functionType, temp->isNegative, makeCopyOfElement(temp->parameter));
        return copy;
    }
    else if (element->type == dualParameterFunctionElementType)
    {
        dualParameterFunctionElement *temp = (dualParameterFunctionElement *)element->ptr;
        copy = createDualParameterFunctionElementAndFill(temp->functionType, temp->isNegative, makeCopyOfElement(temp->firstParameter), makeCopyOfElement(temp->secondParameter));
        return copy;
    }
}
void setValueOfConstantElement(baseElement *constant,double value){
    if(constant->type==constantElementType){
        *((double*)constant->ptr)=value;
    }
}
void freeMemoryOfElement(baseElement *element)
{
    if (element->type == constantElementType)
    {
        free(element->ptr);
        free(element);
        return;
    }
    else if (element->type == polinominalElementType)
    {
        polinominalElement *temp = (polinominalElement *)element->ptr;
        freeMemoryOfElement(temp->coefficent);
        freeMemoryOfElement(temp->exp);
        free(element->ptr);
        free(element);
    }
    else if (element->type == paranthesesElementType)
    {

        int i = 0;
        paranthesesElement *temp = (paranthesesElement *)element->ptr; // get the parantheses element as pointer
        for (i = 0; i < temp->elementCount; i++)
        {
            freeMemoryOfElement(*(temp->elementArray + i));
        }
        free(temp->elementArray);
        free(temp);
        free(element);
        return;
    }
    else if (element->type == multipclationElementType)
    {
        int i = 0;
        multipclationElement *temp = (multipclationElement *)element->ptr; // get the parantheses element as pointer
        for (i = 0; i < temp->elementCount; i++)
        {
            freeMemoryOfElement(*(temp->elementArray + i));
        }

        free(temp->elementArray);
        free(temp);
        free(element);
        return;
    }
    else if (element->type == exponentialElementType)
    {
        exponentialElement *temp = (exponentialElement *)element->ptr;
        freeMemoryOfElement(temp->element);
        freeMemoryOfElement(temp->exp);
        free(temp);
        free(element);
        return;
    }
    else if (element->type == singleParameterFunctionElementType)
    {
        singleParameterFunctionElement *temp = (singleParameterFunctionElement *)element->ptr;
        freeMemoryOfElement(temp->parameter);
        free(temp);
        free(element);
        return;
    }
    else if (element->type == dualParameterFunctionElementType)
    {
        dualParameterFunctionElement *temp = (dualParameterFunctionElement *)element->ptr;
        freeMemoryOfElement(temp->firstParameter);
        freeMemoryOfElement(temp->secondParameter);
        free(temp);
        free(element);
        return;
    }
    else if(element->type== matrixElementType){
        int i = 0,j=0;
        matrixElement *temp = (matrixElement *)element->ptr; // get the parantheses element as pointer
        for (i = 0; i < temp->n; i++)
        {
            for(j=0;j<temp->m;j++){
                freeMemoryOfElement(temp->elementMatrix[i][j]);
            }
            free(temp->elementMatrix[i]);
        }

        free(temp->elementMatrix);
        free(temp);
        free(element);
    }
}
char *substring(char *string, int start, int end)
{
    char *temp = (char*)calloc(end - start + 2, sizeof(char)); //+2 for \0 at the end
    memcpy(temp, string + start, end - start + 1);
    temp[end - start + 1] = '\0';
    return temp;
}
void printElement(baseElement *element)
{
    if (element->type == constantElementType)
    {
        printf("%.2f", (getValueOfElement(element, 0)));
        return;
    }
    else if (element->type == polinominalElementType)
    {
        polinominalElement *temp = (polinominalElement *)element->ptr;
        printf("%.2f*x^%.2f", getValueOfElement(temp->coefficent, 0), getValueOfElement(temp->exp, 0));
    }
    else if (element->type == paranthesesElementType)
    {

        int i = 0;
        paranthesesElement *temp = (paranthesesElement *)element->ptr; // get the parantheses element as pointer
        if (temp->isNegative)
            printf("-");
        printf("(");
        printElement(temp->elementArray[0]);
        for (i = 1; i < temp->elementCount; i++)
        {
            printf("+");
            printElement(temp->elementArray[i]);
        }
        printf(")");
        return;
    }
    else if (element->type == multipclationElementType)
    {
        int i = 0;
        multipclationElement *temp = (multipclationElement *)element->ptr; // get the parantheses element as pointer
        if (temp->isNegative)
            printf("-");
        printElement(temp->elementArray[0]);
        for (i = 1; i < temp->elementCount; i++)
        {
            printf("*");
            printElement(temp->elementArray[i]);
        }
        return;
    }
    else if (element->type == exponentialElementType)
    {
        exponentialElement *temp = (exponentialElement *)element->ptr;
        if (temp->isNegative)
            printf("-");
        printElement(temp->element);
        printf("^");
        printElement(temp->exp);

        return;
    }
    else if (element->type == singleParameterFunctionElementType)
    {
        singleParameterFunctionElement *temp = (singleParameterFunctionElement *)element->ptr;
        char functions[][10] = {"sin", "cos", "tan", "cot", "sec", "cosec", "arcsin", "arccos", "arcsec", "arccosec", "arctan", "arccot", "ln"};
        if (temp->isNegative)
            printf("-");
        printf("%s(", functions[temp->functionType]);
        printElement(temp->parameter);
        printf(")");
        return;
    }
    else if (element->type == dualParameterFunctionElementType)
    {
        dualParameterFunctionElement *temp = (dualParameterFunctionElement *)element->ptr;
        char functions[][10] = {"log"};
        if (temp->isNegative)
            printf("-");
        printf("%s_", functions[temp->functionType]);
        printElement(temp->firstParameter);
        printf("(");
        printElement(temp->secondParameter);
        printf(")");
        return;
    }
    else if(element->type==matrixElementType)
    {
        int i,j;
        matrixElement *temp =(matrixElement*)element->ptr;
        for(i=0;i<temp->n;i++)
        {
            for(j=0;j<temp->m;j++)
            {
                printf("%lf ",getValueOfElement(temp->elementMatrix[i][j],0));
            }
            printf("\n");
        }
    }
}
int factorial(int value){
    int factorial=1;
    while(value){
        factorial=factorial*value;
        value--;
    }
    return factorial;
}

double BisectionSearch(baseElement *element,double aStart,double bStart,double ep)
{
    double a=aStart, b=bStart;
    int i = 0;
    if (getValueOfElement(element, a) < 0 && getValueOfElement(element, b) > 0)
    {
        while (fabs(getValueOfElement(element, a)) - ep > 0 && fabs(getValueOfElement(element, b)) - ep > 0)
        {
            printf("iterasyon %d\na=%lf f(a)=%lf\nb=%lf f(b)=%lf\n", i, a, getValueOfElement(element, a), b, getValueOfElement(element, b));
            if (getValueOfElement(element, (a + b) / 2) > 0)
                b = (a + b) / 2;
            else
                a = (a + b) / 2;
            i++;
        }
    }
    else if (getValueOfElement(element, a) > 0 && getValueOfElement(element, b) < 0)
    {
        while (fabs(getValueOfElement(element, a)) - ep > 0 && fabs(getValueOfElement(element, b)) - ep > 0)
        {
            printf("iterasyon %d\na=%lf f(a)=%lf\nb=%lf f(b)=%lf\n", i, a, getValueOfElement(element, a), b, getValueOfElement(element, b));
            if (getValueOfElement(element, (a + b) / 2) > 0)
                a = (a + b) / 2;
            else
                b = (a + b) / 2;
            i++;
        }
    }
    else printf("Arada bulunabilecek bir kok yoktur\n");
    printf("iterasyon %d\na=%lf f(a)=%lf\nb=%lf f(b)=%lf\n", i, a, getValueOfElement(element, a), b, getValueOfElement(element, b));
    return a;
}

double RegulaFalsi(baseElement *element,double xcStart,double xpStart,double ep)
{
    double xc=xcStart, xp=xpStart, temp; // x current and x previous
    double fxc, fxp;
    int i = 0;
    
    fxc = getValueOfElement(element, xc);
    fxp = getValueOfElement(element, xp);
    while (fabs(fxc) - ep > 0)
    {
        printf("iterasyon %d\nsuanki x=%lf suanki f(x)=%lf\nonceki x=%lf onceki f(x)=%lf\n\n", i, xc, fxc, xp, fxp);
        temp = xc;
        xc = xc - ((fxc * (xp - xc)) / (fxp - fxc));
        xp = temp;
        fxc = getValueOfElement(element, xc);
        fxp = getValueOfElement(element, xp);
        i++;
    }
    printf("iterasyon %d\nsuanki x=%lf suanki f(x)=%lf\nonceki x=%lf onceki f(x)=%lf\n", i, xc, fxc, xp, fxp);
    return xc;
}
double NewtonRaphson(baseElement *element,double xStart,double ep)
{
    baseElement *derivative = derivate(element);
    double x=xStart, temp; // x current and x previous
    double fx, fdx;
    int i = 0;
    //printf("Ilk x degerini girin\n");
    //scanf("%lf", &x);
    //printf("epsilon degerini girin\n");
    //scanf("%lf", &ep);
    fx = getValueOfElement(element, x);
    fdx = getValueOfElement(derivative, x);//not checking the first iteration might be an issue but since we only need to get out of the loop if its broken it should be fine
    printf("Fonksiyonun analitik turevi:");
    printElement(derivative);
    printf("\n");
    while (fabs(fx) - ep > 0)
    {
        printf("iterasyon %d\nsuanki x=%lf suanki f(x)=%lf df(x)/dx=%lf\n\n\n", i, x, fx,fdx);
        x = x - fx / fdx;
        fx = getValueOfElement(element, x);
        fdx = getValueOfElement(derivative, x);
        if(fdx==INFINITY||fdx==-INFINITY){
            printf("turevin degeri sonsuz oldugundan algoritmadan cikiliyor");
            return 0;
        }
        else if(fdx==0){
            printf("turevin degeri 0 oldugundan algoritmadan cikiliyor");
            return 0;
        }
        else if(fx==INFINITY||fx==-INFINITY){
            printf("fonksiyonun degeri sonsuz oldugundan algoritmadan cikiliyor");
            return 0;
        }
        i++;
    }
    printf("iterasyon %d\nsuanki x=%lf suanki f(x)=%lf\n\n", i, x, fx);
    freeMemoryOfElement(derivative);
    return x;
}
double Trapes(baseElement *element,double xSStart, double xEStart, double h)
{
    double xS=xSStart, xE=xEStart, sum; // x Start , x End
    int i = 0;
    
    for (i = 0; i < (xE - xS) / h; i++)
    {
        sum = sum + h * getValueOfElement(element, xS + h * i);
    }
    sum = sum - (getValueOfElement(element, xS) + getValueOfElement(element, xE)) * h / 2;
    return sum;
}
double Simpson(baseElement *element,double xSStart,double xEStart,double h,int mode)
{
    double xS=xSStart, xE=xEStart, sum = 0; // x Start , x End
    int i = 0;
    if (mode == 0)
    {
        for (i = 1; i < (xE - xS) / h; i++)
        {
            sum = sum + 2 * (h / 3) * getValueOfElement(element, xS + h * i) * (1 + i % 2); // multiply by 4 if odd and by 2 if even
        }
        sum = sum + (getValueOfElement(element, xS) + getValueOfElement(element, xE)) * h / 3;
    }
    else if (mode == 1)
    {
        for (i = 1; i < (xE - xS) / h; i++)
        {
            if (i % 3 != 0)
                sum = sum + h * getValueOfElement(element, xS + h * i) * 9 / 8;
        }
        for (i = 1; i < ((xE - xS) / h) / 3; i++)
        {
            sum = sum + getValueOfElement(element, xS + 3 * h * i) * h * 6 / 8;
        }
        sum = sum + (getValueOfElement(element, xS) + getValueOfElement(element, xE)) * h * (3 / 8);
    }
    return sum;
}
double  numericalDerivative(baseElement *element,double x,double h,int mode)
{
    double numericalDerivativeValue;
    if (mode == -1)
    {
        numericalDerivativeValue = (getValueOfElement(element, x) - getValueOfElement(element, x - h)) / h;
    }
    if (mode == 0)
    {
        numericalDerivativeValue = (getValueOfElement(element, x + h) - getValueOfElement(element, x - h)) / (2 * h);
    }
    if (mode == 1)
    {
        numericalDerivativeValue = (getValueOfElement(element, x + h) - getValueOfElement(element, x)) / h;
    }
    return numericalDerivativeValue;
}
baseElement* gregoryNewtonEnterpolation(baseElement *differencesTableMatrixBaseElement,double *xArray){
    int n,i,j;
    matrixElement *differencesTableMatrix=(matrixElement*)differencesTableMatrixBaseElement->ptr;
    n=differencesTableMatrix->n;
    double h,input;
    for(j=1;j<n;j++){//how this is set up would allow me to acces nth differensial of xi by using [i][n]
        for(i=0;i<n-j;i++){
           setValueOfConstantElement(differencesTableMatrix->elementMatrix[i][j],getValueOfElement(differencesTableMatrix->elementMatrix[i+1][j-1],0)-getValueOfElement(differencesTableMatrix->elementMatrix[i][j-1],0));
        }
    }
    printElement(differencesTableMatrixBaseElement);
    printf("\n");
    h=xArray[1]-xArray[0];
    baseElement *elementBase=createParanthesesElement(n,false);//make a parantheses element to hold all the things
    paranthesesElement *element=(paranthesesElement*)elementBase->ptr;
    for(i=0;i<n;i++){
        baseElement *mainMultipclationBaseElement=createMultipclationElement(3+i,false);
        multipclationElement *mainMultipclationElement=(multipclationElement*)mainMultipclationBaseElement->ptr;
        mainMultipclationElement->elementArray[0]=createConstantElement(getValueOfElement(differencesTableMatrix->elementMatrix[0][i],0));//delta^i f 0
        mainMultipclationElement->elementArray[1]=createConstantElement(1/pow(h,i));//1/h^i
        mainMultipclationElement->elementArray[2]=createConstantElement(1/((double)factorial(i)));//1/h^i
        for(j=0;j<i;j++){
            baseElement *xMinusXjBaseElement=createParanthesesElement(2,false);//multiply all the (x-xj)s
            paranthesesElement *xMinusX=(paranthesesElement*)xMinusXjBaseElement->ptr;
            xMinusX->elementArray[0]=createPolinominalElementAndFill(createConstantElement(1),createConstantElement(1));
            xMinusX->elementArray[1]=createConstantElement(-1*xArray[j]);
            mainMultipclationElement->elementArray[3+j]=xMinusXjBaseElement;
        }
        element->elementArray[i]=mainMultipclationBaseElement;
    }
    printf("Enterpole edilmis fonksiyon = ");
    printElement(elementBase);
    printf("\nDenemek istediginiz x degerini girin cikmak icin -1 yazin\n");
    while (input!=-1)
    {
        scanf("%lf",&input);
        printf("%lf\n",getValueOfElement(elementBase,input));
    }
    return elementBase;
}

baseElement *findTheCofactorMatrix(baseElement *element){
    int i,j;
    matrixElement *tempMainMatrix=(matrixElement*)element->ptr;
    baseElement *cofactorMatrixBaseElement=createMatrixElementWithAllElementsSetToZero(tempMainMatrix->n,tempMainMatrix->m),*tempSubMatrix;
    matrixElement* tempCofactorMatrix=(matrixElement*)cofactorMatrixBaseElement->ptr;
    for(i=0;i<tempCofactorMatrix->n;i++){
        for(j=0;j<tempCofactorMatrix->m;j++){
            tempSubMatrix=getSubMatrix(element,i,j);
            setValueOfConstantElement(tempCofactorMatrix->elementMatrix[i][j],((i+j)%2==0? 1:-1)*findDeterminentOfMatrix(tempSubMatrix));
            freeMemoryOfElement(tempSubMatrix);
        }
    }
    return cofactorMatrixBaseElement;
}
baseElement* getSubMatrix(baseElement* matrix,int iEliminate,int jEliminate){
    int iSub=0,iMain=0,jSub=0,jMain=0;
    matrixElement *temp=(matrixElement*)matrix->ptr;
    baseElement *subMatrixBaseElement= createMatrixElementWithAllElementsSetToZero(temp->n-1,temp->m-1);//make a new sub matrix that is smaller by 1 on each axis
    matrixElement *subMatrix=(matrixElement*)subMatrixBaseElement->ptr;
    for(iMain=0;iMain<temp->n;iMain++){
        if(iMain!=iEliminate){
            for(jMain=0;jMain<temp->m;jMain++){
                if(jMain!=jEliminate){
                    setValueOfConstantElement(subMatrix->elementMatrix[iSub][jSub],getValueOfElement(temp->elementMatrix[iMain][jMain],0));//eliminate the row and column we are at and copy the rest to the new sub matrix
                    jSub++;
                }
            }
            jSub=0;
            iSub++;
        }
    }
    return subMatrixBaseElement;
}
double findDeterminentOfMatrix(baseElement *element){
    if(element->type!=matrixElementType)return getValueOfElement(element,0);//act as if it was a 1x1 matrix
    matrixElement *temp=(matrixElement*)element->ptr;
    if(temp->m==1&&temp->n==1)return getValueOfElement(temp->elementMatrix[0][0],0);
    if(temp->m==2&&temp->n==2)return getValueOfElement(temp->elementMatrix[0][0],0)*getValueOfElement(temp->elementMatrix[1][1],0)-getValueOfElement(temp->elementMatrix[1][0],0)*getValueOfElement(temp->elementMatrix[0][1],0);//return the value if its 2x2
    int iDet;
    double det=0;
    for(iDet=0;iDet<temp->n;iDet++){
        baseElement *subMatrixBaseElement=getSubMatrix(element,iDet,0);//make a sub matrix
        det+=(iDet%2==0 ? 1:-1)*findDeterminentOfMatrix(subMatrixBaseElement)*getValueOfElement(temp->elementMatrix[iDet][0],0);//calculate determinant with ne new sub matrix
        freeMemoryOfElement(subMatrixBaseElement);//free the sub matrix
    }
    return det;
}
baseElement* readAndCreateMatrix(int n,int m){//this could be improved to read variables into the matrix like e^x
    int i,j;
    baseElement *matrixBaseElement=createMatrixElementWithAllElementsSetToZero(n,m);
    matrixElement *element=(matrixElement*)matrixBaseElement->ptr;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            scanf("%lf",(double*)element->elementMatrix[i][j]->ptr);
        }
    }
    return matrixBaseElement;
}
baseElement* getTranspose(baseElement *matrix){
    matrixElement *tempMatrix=(matrixElement*)matrix->ptr;
    baseElement *transposeMatrixBaseElement=createMatrixElementWithAllElementsSetToZero(tempMatrix->m,tempMatrix->n);
    matrixElement *tempTransposeMatrix=(matrixElement*)transposeMatrixBaseElement->ptr;
    int i=0,j=0;
    for(i=0;i<tempTransposeMatrix->n;i++){
        for(j=0;j<tempTransposeMatrix->m;j++){
            setValueOfConstantElement(tempTransposeMatrix->elementMatrix[i][j],getValueOfElement(tempMatrix->elementMatrix[j][i],0));
        }
    }
    return transposeMatrixBaseElement;
}
baseElement* getInverseOFAMatrix(baseElement *matrix)
{
    double det=findDeterminentOfMatrix(matrix);
    if(det==0)return NULL;
    baseElement *cofactor=findTheCofactorMatrix(matrix);
    printf("Cofactor matrisi\n");
    printElement(cofactor);
    baseElement *transposeBaseElement=getTranspose(cofactor);
    matrixElement *transposeTemp=(matrixElement*)transposeBaseElement->ptr;
    
    printf("Cofactor matrisinin transpozu\n");
    printElement(transposeBaseElement);
    printf("Matrisin determinanti= %lf\n",det);
    int i;
    for(i=0;i<transposeTemp->n;i++){
        multiplyARowWithAConstant(transposeBaseElement,i,1/det);
    }
    freeMemoryOfElement(cofactor);
    return transposeBaseElement;
}
void addARowToAnother(baseElement *matrixBase,int source,int dest,double coefficent){
    matrixElement *matrix=(matrixElement*)matrixBase->ptr;
    int j=0;
    for(j=0;j<matrix->m;j++){
        setValueOfConstantElement(matrix->elementMatrix[dest][j],getValueOfElement(matrix->elementMatrix[dest][j],0)+getValueOfElement(matrix->elementMatrix[source][j],0)*coefficent);
    }
}
void multiplyARowWithAConstant(baseElement *matrixBase,int row,double coefficent){
    matrixElement *matrix=(matrixElement*)matrixBase->ptr;
    int j=0;
    for(j=0;j<matrix->m;j++){
        setValueOfConstantElement(matrix->elementMatrix[row][j],getValueOfElement(matrix->elementMatrix[row][j],0)*coefficent);
    }
}
void swapRows(baseElement *matrixBase, int firstRow,int secondRow){
    matrixElement *matrix=(matrixElement*)matrixBase->ptr;
    baseElement **temp=matrix->elementMatrix[firstRow];
    matrix->elementMatrix[firstRow]=matrix->elementMatrix[secondRow];
    matrix->elementMatrix[secondRow]=temp;
}
baseElement* gaussEliminationMethod(baseElement *matrixBase){
    int n,m,i,j;
    matrixElement *matrix=(matrixElement*)matrixBase->ptr;
    n=matrix->n;
    m=matrix->m;
    double tempCofactor,tempAnchorValue;
    baseElement *inverseMatrixBase=createMatrixElementWithAllElementsSetToZero(n,m);//instead of making the matrix bigger make a difference matrix for inverse since it is easier that way
    matrixElement *inverseMatrix=(matrixElement*)inverseMatrixBase->ptr;
    
    for(i=0;i<n;i++)setValueOfConstantElement(inverseMatrix->elementMatrix[i][i],1);
    for(i=0;i<n;i++){
        tempCofactor=1/getValueOfElement(matrix->elementMatrix[i][i],0);//after multiplying the value changes so i have to use some temp values
        multiplyARowWithAConstant(matrixBase,i,tempCofactor);
        multiplyARowWithAConstant(inverseMatrixBase,i,tempCofactor);
        for(j=0;j<m;j++){
            if(j!=i) {
                tempAnchorValue=-1*getValueOfElement(matrix->elementMatrix[j][i],0);//same thing with the anchor value
                addARowToAnother(matrixBase,i,j,tempAnchorValue);
                addARowToAnother(inverseMatrixBase,i,j,tempAnchorValue);
            }
            
        }
        printElement(matrixBase);
        printf("\n");
        printElement(inverseMatrixBase);
        printf("\n");
    }
    return inverseMatrixBase;
}
void gaussEliminationLinearEquationsMethod(baseElement *coefficentMatrixBase,baseElement *constantsMatrixBase){
    int n,m,i,j;
    matrixElement *coefficentMatrix=(matrixElement*)coefficentMatrixBase->ptr;
    n=coefficentMatrix->n;
    m=coefficentMatrix->m;
    double tempCofactor,tempAnchorValue;
    matrixElement *constantMatrix=(matrixElement*)constantsMatrixBase->ptr;
    
    for(i=0;i<n;i++){
        tempCofactor=1/getValueOfElement(coefficentMatrix->elementMatrix[i][i],0);//after multiplying the value changes so i have to use some temp values
        multiplyARowWithAConstant(coefficentMatrixBase,i,tempCofactor);
        multiplyARowWithAConstant(constantsMatrixBase,i,tempCofactor);
        for(j=0;j<m;j++){
            if(j!=i) {
                tempAnchorValue=-1*getValueOfElement(coefficentMatrix->elementMatrix[j][i],0);//same thing with the anchor value
                addARowToAnother(coefficentMatrixBase,i,j,tempAnchorValue);
                addARowToAnother(constantsMatrixBase,i,j,tempAnchorValue);
            }
            
        }
        printElement(coefficentMatrixBase);
        printf("\n");
        printElement(constantsMatrixBase);
        printf("\n");
    }
}
void GaussSeidelIterationMethod(baseElement *coefficentMatrixBase,baseElement *constantMatrixBase,double *xValues,double epsilon){
    int n,m,i,j,iterationNumber=0,k=0;
    double tempCofactor,tempAnchorValue,sum;
    bool isFinished=false;
    matrixElement *constantMatrix=(matrixElement*)constantMatrixBase->ptr,*coefficentMatrix=(matrixElement*)coefficentMatrixBase->ptr;
    n=coefficentMatrix->n;
    m=coefficentMatrix->m;
    double max=0;
    int maxIndex=0;
    int alreadySwapped[n];
    for(i=0;i<n;i++)alreadySwapped[i]=-1;
    for(j=0;j<n;j++){//make it diagonally dominant also its n for both cause you dont want to consider any column past n cause they will act like constants anyways 
        for(i=0;i<n;i++){
            if(fabs(getValueOfElement(coefficentMatrix->elementMatrix[i][j],0))>max&&fabs(getValueOfElement(coefficentMatrix->elementMatrix[i][j],0))>alreadySwapped[i])
            {
                max=fabs(getValueOfElement(coefficentMatrix->elementMatrix[i][j],0));
                maxIndex=i;
            }
        }
        if(alreadySwapped[maxIndex]==-1&&max>=0){
            alreadySwapped[j]=max;
            swapRows(coefficentMatrixBase,j,maxIndex);
            swapRows(constantMatrixBase,j,maxIndex);
        }
        else{
            if(alreadySwapped[maxIndex]<max){
                alreadySwapped[j]=max;
                swapRows(coefficentMatrixBase,j,maxIndex);
                swapRows(constantMatrixBase,j,maxIndex);
            }
            if(k==0){
                printf("Matris kosegence baskin hale getirilemiyor dogru sonuc vermeyebilir menuye donmek icin 0 devam etmek icin 1 yazin\n"); 
                while ((k = getchar()) != '\n' && k != EOF);
                k=getchar();
                if(k=='0')return;
            }
        }
        
        max=-1;
    }
   printElement(coefficentMatrixBase);
    while(!isFinished){
        isFinished=true;
        for(i=0;i<n;i++){
            sum=getValueOfElement(constantMatrix->elementMatrix[i][0],0);
            for(j=0;j<m;j++){
                if(j!=i)sum-=getValueOfElement(coefficentMatrix->elementMatrix[i][j],0)*xValues[j];//get the sum division will be done when setting the variables value
            }
            if(fabs(xValues[i]-sum/getValueOfElement(coefficentMatrix->elementMatrix[i][i],0))>epsilon)isFinished=false;//if it doesnt satisfie for 1 element continue with iterating
            xValues[i]=sum/getValueOfElement(coefficentMatrix->elementMatrix[i][i],0);//set the variables value
            sum =0;
        }
        printf("Iterasyon %d\n",iterationNumber);
        for(k=0;k<m;k++)printf("%lf ",xValues[k]);
        printf("\n\n");
        iterationNumber++;
    }
    printf("Degiskenlerin son degerleri\n");
    for(i=0 ;i<m;i++)printf("x%d = %lf\n",i,xValues[i]);
}