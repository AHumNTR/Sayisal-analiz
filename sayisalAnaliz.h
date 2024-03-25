
#include <stdbool.h>

#define pi 3.14159265359
#define e 2.718281828459045
typedef enum elementType{
constantElementType,multipclationElementType,polinominalElementType,paranthesesElementType,exponentialElementType,singleParameterFunctionElementType,dualParameterFunctionElementType
} elementType;
typedef enum singleParameterFunctionTypes{
sinT,cosT,tanT,cotT,secT,cosecT,arcsecT,arccosecT,arcsinT,arccosT,arctanT,arccotT,lnT //T for type
} singleParameterFunctionTypes;
typedef enum dualParameterFunctionTypes{
logT //T for type
} dualParameterFunctionTypes;
typedef struct baseElement{
    elementType type;
    void *ptr;
}baseElement;

typedef struct polinominalElement
{
    baseElement* exp,*coefficent;
} polinominalElement;

typedef struct singleParameterFunctionElement
{
    singleParameterFunctionTypes functionType;
    bool isNegative;
    baseElement* parameter;
} singleParameterFunctionElement;

typedef struct dualParameterFunctionElement
{
    dualParameterFunctionTypes functionType;
    bool isNegative;
    baseElement* firstParameter,*secondParameter;
} dualParameterFunctionElement;

typedef struct paranthesesElement
{
    int elementCount;
    bool isNegative;
    baseElement** elementArray;
} paranthesesElement;

typedef struct multipclationElement{
    int elementCount;
    bool isNegative;
    baseElement** elementArray;
}multipclationElement;//stores each factor as a baseElement

typedef struct exponentialElement{
    baseElement *exp;
    baseElement *element;
}exponentialElement;
void addElementToTheList(char ***elementsString, char *input, int *startIndex, int endIndex, int *elementCount);
bool convertElementIntoTypes(char *elementsString,baseElement *elements);
void convertAllElementsIntoTypes(char ***elementsString,int elementStringCount,baseElement ***elements,int *elementCount);

void splitIntoAdditionElements(char ***elementsString,char *input,int *elementCount);
void splitIntoMultipclationElements(char ***elementsString,char *input,int *elementCount);
void splitIntoExponentialElements(char ***elementsString,char *input,int *elementCount);

bool checkAndConvertToMultipclationElement(char *input,baseElement *multipclationElement);
bool checkAndConvertToConstantElement(char *elementString,baseElement *constant);
bool checkAndConvertToPolinominal(char *input, baseElement *polinom);
bool checkAndConvertToParanthesesElements(char *input, baseElement *polinom);
bool checkAndConvertToSingleParameterFunctionElement(char *input,baseElement *trigonometricElementBase);
bool checkAndConvertToDualParameterFunctionElement(char *input, baseElement *dualParameterFunctionElementBase);
bool checkAndConvertToExponentialElement(char *input,baseElement *expElementBase);

double getValueOfElement(baseElement *element,double x);
void freeMemoryOfElement(baseElement *element);

char* substring(char* string,int start,int end);
