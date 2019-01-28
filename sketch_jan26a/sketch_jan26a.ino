#include<Wire.h>
#include <math.h>

const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


float std_dev(float data[]);
float mean(float data[]);



void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
//  Serial.print("Accelerometer: ");
  Serial.print(AcX);
  Serial.print(" "); Serial.print(AcY);
  Serial.print(" "); Serial.print(AcZ); 

//  Serial.print("Gyroscope: ");
//Serial.print(" "); Serial.print(GyX);
//Serial.print(" "); Serial.print(GyY);
//Serial.print(" "); Serial.println(GyZ);

  delay(10);
}

float mean(float data[])
{
    float sum = 0.0, mean = 0.0;

    int i;
    for(i=0; i<10; ++i)
    {
        sum += data[i];
    }

    mean = sum/10;
    return mean;


}

float std_dev(float data[])
{
    float the_mean = mean(data);
    float standardDeviation = 0.0;
     
    int i;
    for(i=0; i<10; ++i)
        standardDeviation += pow(data[i] - the_mean, 2);

    return sqrt(standardDeviation/10);
}
