#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<math.h>
#include"sayisalAnaliz.h"

int main(){
    baseElement *derivitive,*function;//=malloc(sizeof(baseElement)),*derivitive;
    int c=0,mode;
    double x=0;
    //gregoryNewtonEnterpolation();
    while(true){
        printf("Istediginiz yontemi secin\n1: Bisection yontemi\n2: Regula-falsi\n3: Newton-Rapshon yontemi\n4: NxNlik bir matrisin tersi\n5: Gauus Eleminasyon\n6: Gauss Seidal yontemleri\n7: Sayisal Turev (merkezi, ileri ve geri farklar opsiyonlu)\n8: Simpson yontemi\n9: Trapez yontemi\n10: Degisken dnusumsuz Gregory newton Enterpolasyonu\n11: Analitik turev\n");
        scanf("%d",&mode);
        if(mode==1){
            
            double aStart, bStart,ep;
            baseElement *element=readFunction();
            printf("a ve b noktalarini girin\n");
            scanf("%lf %lf", &aStart, &bStart);
            printf("epsilon degerini girin\n");
            scanf("%lf",&ep);
            printf("Kok degeri=%lf\n",BisectionSearch(element,aStart,bStart,ep));
            freeMemoryOfElement(element);
        }
        else if(mode==2){
            
            double xcStart, xpStart,ep;
            baseElement *element=readFunction();
            printf("Ilk iki x degerini girin\n");
            scanf("%lf %lf", &xcStart, &xpStart);
            printf("epsilon degerini girin\n");
            scanf("%lf", &ep);
            printf("Kok degeri=%lf\n",RegulaFalsi(element,xcStart,xpStart,ep));
            freeMemoryOfElement(element);
        }
        else if(mode==3){
            
            double xStart,ep;
            baseElement *element=readFunction();
            printf("Ilk x degerini girin\n");
            scanf("%lf", &xStart);
            printf("epsilon degerini girin\n");
            scanf("%lf", &ep);
            printf("Kok degeri=%lf\n",NewtonRaphson(element,xStart,ep));
            freeMemoryOfElement(element);
        }
        else if(mode==4){
            int n;
            printf("Matrisin n degerini girin\n");
            scanf("%d",&n);
            printf("Matrisi girin\n");
            baseElement *matrix=readAndCreateMatrix(n,n);
            baseElement *inverseMatrix=getInverseOFAMatrix(matrix);
            if(inverseMatrix==NULL)
            {
                printf("Matrisin tersi yoktur\n");
                freeMemoryOfElement(matrix);
            }
            else{
                printf("Matrisin tersi: \n");
                printElement(inverseMatrix);
                freeMemoryOfElement(matrix);
                freeMemoryOfElement(inverseMatrix);
            }
        }
        else if(mode==5){
            int n;
            printf("Matrisin n degerini girin\n");
            scanf("%d",&n);
            printf("Matrisi girin\n");
            baseElement *matrix=readAndCreateMatrix(n,n);
            baseElement *inverseMatrix=gaussEliminationMethod(matrix);
            printf("Matrisin tersi: \n");
            printElement(inverseMatrix);
            freeMemoryOfElement(matrix);
            freeMemoryOfElement(inverseMatrix);
            
        }
        else if(mode==6){
            int n,m,i;
            double epsilon;
            printf("Epsilon degerini girin\n");
            scanf("%lf",&epsilon);
            printf("Katsayi matrisinin n ve m degerlerini girin\n");
            scanf("%d %d",&n,&m);
            printf("Matrisi girin\n");
            baseElement *coefficentMatrixBase=readAndCreateMatrix(n,m);
            printf("Sabit sayilar matrisini girin\n");
            baseElement *constantsMatrixBase=readAndCreateMatrix(m,1);
            double *xValues=calloc(m,sizeof(double));
            printf("Degiskenlerin baslangic degerlerini girin\n");
            for(i=0;i<n;i++)scanf("%lf",xValues+i);
            GaussSeidelIterationMethod(coefficentMatrixBase,constantsMatrixBase,xValues,epsilon);
        }
        else if(mode==7){
            
            baseElement *element=readFunction();
            int derivativeMode;
            double h, x, numericalDerivativeValue, analiticalDerivativeValue;
            printf("Kullanmak istediginiz metodu secin\n-1: Geri Farklar\n0 Merkezi Farklar\n1 Ileri farklar\n");
            scanf("%d", &derivativeMode);
            printf("X degerini girin\n");
            scanf("%lf", &x);
            printf("H degerini girin\n");
            scanf("%lf", &h);
            baseElement *derivative = derivate(element); // there is no need for it i am doing this just cause i made the derivation method already
            printf("Fonksiyonun analitik turevi=");
            printElement(derivative);
            analiticalDerivativeValue = getValueOfElement(derivative, x);
            numericalDerivativeValue=numericalDerivative(element,x,h,derivativeMode);
            printf("\nFonksiyonun analitik turevinin x = %lf deki degeri = %lf\n", x, analiticalDerivativeValue);
            if(derivativeMode==-1) printf("Geri farklar ile sayisal turevin x=%lf deki degeri = %lf\n",x,numericalDerivativeValue);
            else if(derivativeMode==0) printf("Merkezi farklar ile sayisal turevin x=%lf deki degeri = %lf\n",x,numericalDerivativeValue);
            else if(derivativeMode==1) printf("Ileri farklar ile sayisal turevin x=%lf deki degeri = %lf\n",x,numericalDerivativeValue);
            printf("Sayisal turevin mutlak hatasi = %lf\n",fabs(numericalDerivativeValue-analiticalDerivativeValue));
        }
        else if(mode==8){
            
            double xSStart,xEStart,h;
            int simpsonMode;
            baseElement *element=readFunction();
            printf("Kullanmak istediginiz metodu secin simpson 1/3 icin 0 simpson 3/8 icin 1 yazin\n");
            scanf("%d", &simpsonMode);
            printf("Ilk ve son x degerlerini girin\n");
            scanf("%lf %lf", &xSStart, &xEStart);
            printf("H degerini girin\n");
            scanf("%lf", &h);
            printf("Integralin yaklasik degeri=%lf\n",Simpson(element,xSStart,xEStart,h,simpsonMode));
            freeMemoryOfElement(element);
        }
        else if(mode==9){
            
            double xSStart,xEStart,h;
            baseElement *element=readFunction();
            printf("Ilk ve son x degerlerini girin\n");
            scanf("%lf %lf", &xSStart, &xEStart);
            printf("H degerini girin\n");
            scanf("%lf", &h);
            printf("Integralin yaklasik degeri=%lf\n",Trapes(element,xSStart,xEStart,h));
            freeMemoryOfElement(element);
        }
        else if(mode ==10){
            int n,i,j;
            printf("Nokta sayisini giriniz\n");
            scanf("%d",&n);
            baseElement *differencesTableMatrixBaseElement=createMatrixElementWithAllElementsSetToZero(n,n);
            matrixElement *differencesTableMatrix=differencesTableMatrixBaseElement->ptr;
            double xArray[n],h,input;
            printf("x degerini ve karsilik gelen f(x) degerini girin\n");
            for(i=0;i<n;i++){
                scanf("%lf %lf",xArray+i,(double*)differencesTableMatrix->elementMatrix[i][0]->ptr);
            }
            baseElement *enterpoletedFunction=gregoryNewtonEnterpolation(differencesTableMatrixBaseElement,xArray);
        }
        else if(mode==11){
            
            baseElement *element=readFunction(),*derivative;
            derivative=derivate(element);
            double x=0;
            
            printf("Fonksiyonun analitik turevi =");
            printElement(derivative);
            while(x!=-1){
                printf("\nX degerini girin cikmak icin -1 girin\n");
                scanf("%lf",&x);
                printf("\nAnalitik turevinin x=%lfdeki degeri = %lf\n",x,getValueOfElement(derivative,x));
            }
            freeMemoryOfElement(derivative);
            freeMemoryOfElement(element);
        }
        printf("Press any button to continue");
        getchar();
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return 0;
}

