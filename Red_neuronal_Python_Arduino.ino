#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

double accelX = 0, accelY =0, accelZ;

unsigned long lastTime = 0, sampleTime = 100;


///////////////////////////////// Variables Red Neuronal /////////////////////////////////
double a0[3];
double W1[2][3] = {{2.728,0.18,0.906},{-2.217,-0.471,-0.489}};
double a1[2];
double W2[1][2] = {{1.655,-2.267}};
double a2[1]; 
double b1[2]= {2.443,0.191};
double b2[1]= {1.074};
double aux = 0.0;
//////////////////////////////////////////////////////////////////



///////////////////////////////// Preprocesamiento Red Neuronal /////////////////////////////////
double mean[3]={3.582,-1.058,3.444};
double dstd[3]={7.321,0.893,5.074};
///////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  
  Serial.begin(9600);
  
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    delay(500);
  }
}

void loop() {
  
  if(millis()-lastTime>=sampleTime)
  {
    lastTime = millis();  
    
    Vector normAccel = mpu.readNormalizeAccel();
    accelX = normAccel.XAxis;
    accelY = normAccel.YAxis;
    accelZ = normAccel.ZAxis;

    a0[0] = dataNormalized(accelX,mean[0],dstd[0]);
    a0[1] = dataNormalized(accelY,mean[1],dstd[1]);
    a0[2] = dataNormalized(accelZ,mean[2],dstd[2]);


    ///////////////////////////////// Estructura Red Neuronal /////////////////////////////////
    for(int i = 0 ; i<2; i++ ) {aux=0.0;for(int j = 0 ; j <3 ; j++ ) { aux=aux+W1[i][j]*a0[j];} a1[i]=relu(aux+b1[i]);}
    for(int i = 0 ; i<1; i++ ) {aux=0.0;for(int j = 0 ; j <2 ; j++ ) { aux=aux+W2[i][j]*a1[j];} a2[i]=sigmoid(aux+b2[i]);}
    //////////////////////////////////////////////////////////////////////////////////////////
    Serial.println(round(a2[0])); 
  }
}

double relu(double n)
{
  if(n>=0) return n; else if (n<0) return 0;
}
double sigmoid(double n)
{
  return 1.0/(1.0 + exp(-n));
}

double dataNormalized(double inputData,double mean,double desvStandar) 
{
  double valueNorm;
  valueNorm = (inputData-mean)/desvStandar;
  return valueNorm;
}
