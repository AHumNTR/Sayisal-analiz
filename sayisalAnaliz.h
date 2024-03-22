
#include <stdbool.h>
typedef enum elementType{
constantElementType,multipclationElementType,polinominalElementType,paranthesesElementType,exponentialElementType,functionElementType
} elementType;

typedef struct baseElement{
    elementType type;
    void *ptr;
}baseElement;

typedef struct polinominalElement
{
    baseElement* exp,*coefficent;
} polinominalElement;

typedef struct functionElement
{
    baseElement* inside,*coefficent;
} polinominalElement;

typedef struct paranthesesElement
{
    int elementCount;
    baseElement* elementArray;
} paranthesesElement;

typedef struct multipclationElement{
    int elementCount;
    baseElement* elementArray;
}multipclationElement;//stores each factor as a baseElement

typedef struct exponentialElement{
    baseElement *exp;
    baseElement *element;
}exponentialElement;
void addElementToTheList(char ***elementsString, char *input, int *startIndex, int endIndex, int *elementCount);
bool convertElementIntoTypes(char *elementsString,baseElement **elements,int *elementCount,bool increaseTheSizeIfSuccesfull);
void convertAllElementsIntoTypes(char ***elementsString,int elementStringCount,baseElement **elements,int *elementCount);

void splitIntoAdditionElements(char ***elementsString,char *input,int *elementCount);
void splitIntoMultipclationElements(char ***elementsString,char *input,int *elementCount);
void splitIntoExponentialElements(char ***elementsString,char *input,int *elementCount);

bool checkAndConvertToMultipclationElement(char *input,baseElement *multipclationElement);
bool checkAndConvertToConstantElement(char *elementString,baseElement *constant);
bool checkAndConvertToPolinominal(char *input, baseElement *polinom);
bool checkAndConvertToParanthesesElements(char *input, baseElement *polinom);
bool checkAndConvertToFunctionElements(char *input, baseElement *polinom);


double getValueOfElement(baseElement *element,double x);
