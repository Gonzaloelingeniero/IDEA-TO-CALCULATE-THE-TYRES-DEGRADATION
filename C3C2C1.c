//PROGRAMA QUE CALCULA LA FUNCIÓN QUE PIDIÓ PIRELLI PARA LOS NEUMÁTICOS C2 Y C1. NO VALE PARA EL C5, C4 y C3
#include <stdio.h>
#include <stdlib.h>
#define CARRERAS 2
#define LEN 2
#define NEUMATICOS 89 // CAMBIAR EN FUNCIÓN DEL TIEMPO POR VUELTA
#include <math.h>
void calcularInversa2x2(float m[CARRERAS][LEN], float mAux[CARRERAS][LEN], float mAux2[CARRERAS][LEN]);
float calcularDeterminante2x2(float m[CARRERAS][LEN]);
int main(){
    int j, vueltas;
    float m[CARRERAS][LEN], mAux[CARRERAS][LEN], mAux2[CARRERAS][LEN];
    float *velocidad, *velocidaddef, *tiempo, *vDef, *tiempodef;
    float *posicion, *posiciondef, *posDef;
    float masa, deformacion, tiempofinal, constante, steerangle, radio, vinicial;
    float sumatorio = 0, sumatoriocurva = 0, sumatoriolateral = 0, cte1, cte2;
    int tvuelta;
    //Pido el número de vueltas:
    printf("Escribe el numero de vueltas que lleva el neumatico: ");
    scanf("%d", &vueltas);
    while(vueltas < 1){
        printf("Vueltas >=1\n");
        printf("Escribe el numero de vueltas que lleva el neumatico: ");
        scanf("%d", &vueltas);
    }
    //Pido el tiempo por vuelta:
    printf("Escribe el tiempo por vuelta: ");
    scanf("%d", &tvuelta);
    velocidad = (float*)malloc(tvuelta*sizeof(float));
    velocidaddef = (float*)malloc(tvuelta*sizeof(float));
    tiempo = (float*)malloc(tvuelta*sizeof(float));
    vDef = (float*)malloc(tvuelta*sizeof(float));
    tiempodef = (float*)malloc(tvuelta*sizeof(float));
    posicion = (float*)malloc(tvuelta*sizeof(float));
    posiciondef = (float*)malloc(tvuelta*sizeof(float));
    posDef = (float*)malloc(tvuelta*sizeof(float));
    //Pido el tiempo final:
    printf("Escribe el tiempo que llevas en el stint: ");
    scanf("%f", &tiempofinal);
    printf("Escribe la velocidad en t=%d: ", 0);
    scanf("%f", &velocidad[0]);
            for(j=1; j<=tvuelta; j++){
            //Pido el ángulo del volante:
            printf("Escribe el angulo del volante: ");
            scanf("%f", &steerangle);
            if(steerangle > -3.5 && steerangle < 3.5){//Si estoy en recta
                //Pido la velocidad en cada tiempo:
                printf("Escribe la velocidad en t=%d: ", j);
                scanf("%f", &velocidad[j]);
                //Relleno la matriz con el tiempo:
                m[0][0] = 0.001;
                m[0][1] = 1/0.001;
                m[1][0] = 1;
                m[1][1] = 1/(1);
                //Calculo el término independiente:
                velocidaddef[j-1] = velocidad[j-1] + velocidad[j-1]*0.001;
                velocidaddef[j] = velocidad[j] + velocidad[j-1]*(1);
                //Calculo la solución:
                calcularInversa2x2(m, mAux, mAux2);
                vDef[j-1] = mAux2[j-1][j-1]*velocidaddef[j-1]+mAux2[j-1][j]*velocidaddef[j];
                vDef[j] = mAux2[j][j-1]*velocidaddef[j-1]+mAux2[j][j]*velocidaddef[j];
                sumatorio = sumatorio + vDef[j-1]-vDef[j] + velocidad[j];
                //Me aseguro de sumar todas las aceleraciones, independientemente del sentido en el que vayan.
                if (sumatorio < 0){
                    sumatorio = sumatorio*(-1);
                }
                //Calculo la aceleración:
                printf("Sumatorio de la aceleracion longitudinal acumulada: %f\n", sumatorio);
                //Pido la masa:
                printf("Escribe la masa del coche en el momento actual: ");
                scanf("%f", &masa);
                //Pido la deformación:
                printf("Escribe la deformacion del neumatico que lleva (de 0 a 1): ");
                scanf("%f", &deformacion);
                //Calculo la constante:
                constante = masa*sumatorio/(tiempofinal*deformacion);
                //Muestro la fórmula
                printf("La formula es deformacion = %f*t\n", 1/constante);
            }
            else {//Si estoy en curva:
                //Pido la posición en el tiempo:
                printf("Escribe la posicion en t=%d: ", j);
                scanf("%f", &posicion[j]);
                printf("Escribe la velocidad en t=%d: ", j);
                scanf("%f", &vinicial);
                //Relleno la matriz con el tiempo:
                m[0][0] = 0.001;
                m[0][1] = 1;
                m[1][0] = 1;
                m[1][1] = 1;
                //Calculo el término independiente:
                posiciondef[j-1] = posicion[j-1] + vinicial*(0.001);
                posiciondef[j] = posicion[j] + vinicial*(1);
                //Calculo la solución:
                calcularInversa2x2(m, mAux, mAux2);
                posDef[j-1]=mAux2[j-1][j-1]*posiciondef[j-1]+mAux2[j-1][j]*posiciondef[j];
                posDef[j]=mAux2[j][j-1]*posiciondef[j-1]+mAux2[j][j]*posiciondef[j];
                sumatoriocurva = sumatoriocurva + 2*posDef[j-1];
                if (sumatoriocurva < 0){
                    sumatoriocurva = sumatoriocurva*(-1);
                }
                //Calculo la aceleración: .
                printf("Aceleracion longiudinal acumulada:%f\n", sumatoriocurva);
                //Calculo el radio de la curva por el teorema del seno para un triangulo isosceles, donde el radio de la curva es la base:
                radio = sin(steerangle)*(posicion[j]- posicion[j-1])/sin(180-2*steerangle);
                sumatoriolateral = sumatoriolateral + (2*posDef[j-1])*(2*posDef[j-1])/radio;
                if (sumatoriolateral < 0){
                    sumatoriolateral = sumatoriolateral*(-1);
                }
                //Muestro la aceleración lateral:
                printf("Sumatorio aceleracion lateral acumulada:%f\n", sumatoriolateral);
                //Pido la masa:
                printf("Escribe la masa del coche: ");
                scanf("%f", &masa);
                //Pido la deformación:
                printf("Escribe la deformacion del neumatico que lleva (de 0 a 1): ");
                scanf("%f", &deformacion);
                //Pido la constante:
                cte1 = sumatoriocurva/(radio*deformacion);
                cte2 = sumatoriolateral/(radio*deformacion);
                //Pido la deformación:
                printf("La formula es deformacion = %f*t + %f*t^2\n", 1/cte1, 1/cte2);
            }
        }
}
void calcularInversa2x2(float m[CARRERAS][LEN], float mAux[CARRERAS][
LEN], float mAux2[CARRERAS][LEN]){
        int i, j;
        mAux[0][0]=m[1][1];
        mAux[0][1]=-m[0][1];
        mAux[1][0]=-m[1][0];
        mAux[1][1]=m[0][0];
        for(i=0; i<CARRERAS; i++){
            for(j=0; j<LEN; j++){
             mAux2[i][j]=mAux[i][j]/calcularDeterminante2x2(m);
             }
        }
 }
 //Para calcular el determinante:
 float calcularDeterminante2x2(float m[CARRERAS][LEN]){
     float determinante;
     determinante=m[0][0]*m[1][1]-m[1][0]*m[0][1];
     return determinante;
 }

