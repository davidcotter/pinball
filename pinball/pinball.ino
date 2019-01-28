#include<Wire.h>
#include <math.h>

#define SAMPLE_LENGTH 100

const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


float stddev(float data[], int len);
float mean(float data[], int len);
void thresholding(float y[], float signals[], int lag, float threshold, float influence);


void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
  int lag = 30;
  float threshold = 5;
  float influence = 0;
  float y[] = {1,1,1.1,1,0.9,1,1,1.1,1,0.9,1,1.1,1,1,0.9,1,1,1.1,1,1,1,1,1.1,0.9,1,1.1,1,1,0.9,
       1,1.1,1,1,1.1,1,0.8,0.9,1,1.2,0.9,1,1,1.1,1.2,1,1.5,1,3,2,5,3,2,1,1,1,0.9,1,1,3,
       2.6,4,3,3.2,2,1,1,0.8,4,4,2,2.5,1,1,1 };
       
  float signal[SAMPLE_LENGTH];
  
  thresholding(y, signal,  lag, threshold, influence);
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


void thresholding(float y[], float signals[], int lag, float threshold, float influence) {
    memset(signals, 0, sizeof(signals));
    float filteredY[SAMPLE_LENGTH];
    memcpy(filteredY, y, SAMPLE_LENGTH);
    float avgFilter[SAMPLE_LENGTH];
    float stdFilter[SAMPLE_LENGTH];
    memset(stdFilter, 0, sizeof(stdFilter));

    avgFilter[lag - 1] = mean(y, lag);
    stdFilter[lag - 1] = stddev(y, lag);
    
    for (int i = lag; i < SAMPLE_LENGTH; i++) {
        if (abs(y[i] - avgFilter[i-1]) > threshold * stdFilter[i-1]) {
            if (y[i] > avgFilter[i-1]) {
                signals[i] = 1;
            } else { 
                signals[i] = -1;
            }

            filteredY[i] = influence * y[i] + (1 - influence) * filteredY[i-1];
            avgFilter[i] = mean(filteredY + i-lag+1, i+1);
            stdFilter[i] = stddev(filteredY + i-lag+1, i+1);            
        } else {
            signals[i] = 0;
            filteredY[i] = y[i];
            avgFilter[i] = mean(filteredY + i-lag+1, i+1);
            stdFilter[i] = stddev(filteredY + i-lag+1, i+1);
        }
    }
}
                
float mean(float data[], int len) {
    float sum = 0.0, mean = 0.0;

    int i;
    for(i=0; i<len; ++i) {
        sum += data[i];
    }

    mean = sum/len;
    return mean;


}

float stddev(float data[], int len) {
    float the_mean = mean(data, len);
    float standardDeviation = 0.0;
     
    int i;
    for(i=0; i<len; ++i) {
        standardDeviation += pow(data[i] - the_mean, 2);
    }
    
    return sqrt(standardDeviation/len);
}
