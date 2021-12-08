//PROGRAMA QUE CALCULA LA FUNCIÓN QUE PIDIÓ PIRELLI PARA LOS NEUMÁTICOS C5, C4 Y C3. NO VALE PARA EL C2 Y EL C1
#include <stdio.h>
#define CARRERAS 2
#define LEN 2
#include <math.h>
#include <stdlib.h>
void calcularInversa2x2(float m[CARRERAS][LEN], float mAux[CARRERAS][LEN], float mAux2[CARRERAS][LEN]);
float calcularDeterminante2x2(float m[CARRERAS][LEN]);
int main(){
    int j, vueltas;
    float m[CARRERAS][LEN], mAux[CARRERAS][LEN], mAux2[CARRERAS][LEN];
    float *velocidad, *velocidaddef, *tiempo, *vDef, *tiempodef;
    float *posicion, *posiciondef, *posDef;
    float masa, deformacion, tiempofinal, constante, steerangle, radio, vinicial;
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
    //Pido la velocidad en cada tiempo:
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
                m[1][0] = vueltas*tvuelta+(int)tiempofinal%(int)tvuelta+1;
                m[1][1] = 1/(vueltas*tvuelta+(int)tiempofinal%(int)tvuelta);
                //Calculo el término independiente:
                velocidaddef[j-1] = velocidad[j-1] + velocidad[j-1]*(vueltas*tvuelta+(int)tiempofinal%(int)tvuelta-1);
                velocidaddef[j] = velocidad[j] + velocidad[j-1]*(vueltas*tvuelta+(int)tiempofinal%(int)tvuelta);
                //Calculo la solución:
                calcularInversa2x2(m, mAux, mAux2);
                vDef[j-1]=mAux2[j-1][j-1]*velocidaddef[j-1]+mAux2[j-1][j]*velocidaddef[j];
                vDef[j]=mAux2[j][j-1]*velocidaddef[j-1]+mAux2[j][j]*velocidaddef[j];
                //Calculo la aceleración:
                printf("Integral de la aceleracion longitudinal acumulada en valor absoluto: %f*t+%f/t^2+%f\n",
                       vDef[j-1], vDef[j], velocidad[j-1]);
                //Pido la masa:
                printf("Escribe la masa del coche en este momento: ");
                scanf("%f", &masa);
                //Pido la deformación:
                printf("Escribe la deformacion que lleva el neumatico (de 0 a 1): ");
                scanf("%f", &deformacion);
                //Calculo la constante:
                constante = masa*(vDef[j-1]*(tiempofinal)+vDef[j]/((tiempofinal)*(tiempofinal)))/(deformacion);
                //Muestro la fórmula
                printf("La formula es deformacion = %f*t+%f/t^2+%f\n",
                       vDef[j-1]/constante, vDef[j]/constante, velocidad[j-1]/constante);
                }
            else {//Si estoy en curva:
                //Pido la posición en el tiempo:
                printf("Escribe la posicion en t=%d: ", j);
                scanf("%f", &posicion[j]);
                printf("Escribe la v.inicial: ");
                scanf("%f", &vinicial);
                //Relleno la matriz con el tiempo:
                m[0][0] = 0.001;
                m[0][1] = 1;
                m[1][0] = (int)tiempofinal%(int)tvuelta+vinicial*tvuelta;
                m[1][1] = 1;
                //Calculo el término independiente:
                posiciondef[j-1] = posicion[j-1] + vinicial*(vueltas*tvuelta+(int)tiempofinal%(int)tvuelta);
                posiciondef[j] = posicion[j] + vinicial*(vueltas*tvuelta+(int)tiempofinal%(int)tvuelta);
                //Calculo la solución:
                calcularInversa2x2(m, mAux, mAux2);
                posDef[j-1]=mAux2[j-1][j-1]*posiciondef[j-1]+mAux2[j-1][j]*posiciondef[j];
                posDef[j]=mAux2[j][j-1]*posiciondef[j-1]+mAux2[j][j]*posiciondef[j];
                //Calculo la aceleración: .
                printf("Aceleracion longiudinal acumulada:%f*t+%f\n", 2*posDef[j-1], vinicial);
                //Calculo el radio de la curva por el teorema del seno para un triangulo isosceles, donde el radio de la curva es la base:
                radio = sin(steerangle)*(posicion[j]- posicion[j-1])/sin(180-2*steerangle);
                //Muestro la aceleración lateral:
                printf("Sumatorio aceleracion lateral acumulada:%f*t^4+%f*t^2+%f*t^3+%f+%f*t^2+%f*t^2+%f*t+%f*t^2\n",
                       posDef[j-1]*posDef[j-1]/radio ,
                       posDef[j-1]*2*posDef[j]/radio, posDef[j-1]*vinicial, posDef[j]*posDef[j],
                       posDef[j]*vinicial, posDef[j-1]*vinicial,
                       posDef[j]*vinicial, vinicial*vinicial);
               //Pido la masa:
                printf("Escribe la masa del coche en este momento: ");
                scanf("%f", &masa);
                //Pido la deformación:
                printf("Escribe la deformacion del neumatico que lleva (de 0 a 1): ");
                scanf("%f", &deformacion);
                //Pido la constante:
                constante = pow((masa*(posDef[j-1]*pow((vueltas*tvuelta+(int)tiempofinal%(int)tvuelta), 2))
                                 +posDef[j]-vinicial*(vueltas*tvuelta+(int)tiempofinal%(int)tvuelta)), 2)/(radio*deformacion);
                //Pido la deformación:
                printf("La formula es deformacion = %f*t^4+%f*t^2+%f*t^3+%f+%f*t^2+%f*t^2+%f*t+%f*t^2\n+%f*t+%f\n",
                       (posDef[j-1]*posDef[j-1]/radio ,posDef[j-1]*2*posDef[j]/radio, posDef[j-1]*vinicial, posDef[j]*posDef[j],
                        posDef[j]*vinicial, posDef[j-1]*vinicial, posDef[j]*vinicial, vinicial*vinicial, posDef[j-1]+2*posDef[j-1])/constante,
                       vinicial/constante);
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
