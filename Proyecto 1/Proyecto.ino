#include <Wire.h>
#include <SoftwareSerial.h>

#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "heartRate.h"

#include <Adafruit_MLX90614.h>

SoftwareSerial miBT(10,11);
MAX30105 particleSensor;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

float temp;

int x,y,z;
int count =0;
double distance;//En cm
int Buzzer = 47;
int Ventilador = 37;

unsigned long tiempo;
int repeticiones=0;
double velocidad = 0;

void setup() {
  Serial.begin(9600);
  miBT.begin(38400);//Módulo Bluetooth
  pinMode(Buzzer, OUTPUT);
  pinMode(Ventilador, OUTPUT);
  tiempo = millis();

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  calibrar();
}

void calibrar()
{
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
    particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sample

    Serial.print(F("red="));
    Serial.print(redBuffer[i], DEC);
    Serial.print(F(", ir="));
    Serial.println(irBuffer[i], DEC);
  }

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
}

void loop() {
  if(millis() - tiempo >= 60000) {
    repeticiones++;
    digitalWrite(Buzzer, HIGH);
    tiempo = millis();
    while(millis() - tiempo < 1200){
      //Tiempo donde el buzzer estará sonando.
    }
    digitalWrite(Buzzer, LOW);
    tiempo = millis();
  } else {
    //SECCION DE RITMO CARDIACO
    particleSensor.begin(Wire, I2C_SPEED_FAST);

    for (byte i = 10; i < 100; i++)
    {
      redBuffer[i - 10] = redBuffer[i];
      irBuffer[i - 10] = irBuffer[i];
    }
  
    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 90; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data
  
      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample
  
      //send samples and calculation result to terminal program through UART
    }

    Serial.print(F(", HR="));
    Serial.print(heartRate, DEC);
  
    Serial.print(F(", HRvalid="));
    Serial.print(validHeartRate, DEC);

    Serial.println();

    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    
    //SECCION DE TEMPERATURA
    mlx.begin(); 
    Serial.print("Object = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C ");
    temp = mlx.readObjectTempC();
    Serial.println("----------");
    //Serial.println(temp);
    //Serial.println("========");

    //SECCION DE DISTANCIA
    x = analogRead(A0);
    y = analogRead(A1);
    z = analogRead(A2);
    if ( x <= 450 && y >= 385)
    { 
      count++;
      //delay(300);
    }
    if(x < 450 && y <= 325){
      count++;
    }
    distance = (count * 62.25); // My step it is on average 62.25cm. 
    Serial.print("x");
    Serial.print(x);
    Serial.print(",y");
    Serial.print(y);
    Serial.print(",z");
    Serial.print(z);
    Serial.print(",cd");
    Serial.print(count);
    Serial.print(",d");
    Serial.print(distance);
    Serial.print(";");
    Serial.println("");
    Serial.println("---------------");
    Serial.print("Rep:");
    Serial.println(repeticiones);

    velocidad = distance / (millis() / 1000);
    Serial.print("vel:");
    Serial.println(velocidad);
    Serial.println("=============");

    //VALIDACION DE DATOS
    if(heartRate >= 275 ){
      Serial.println("*DEBE PARAR*");
      digitalWrite(Ventilador, HIGH);
      while(true){
        //Perdio entonces la app se bloquea para no recibir mas datos.
      }
    }

    //SECCION DEL MODULO BLUETOOTH
    //miBT.begin(38400);//Módulo Bluetooth
    //Lee Arduino y envia a BT
    char valores[25];
    int a = temp;
    
    char Tmp[5];
    itoa(a,Tmp,10);
    strcpy(valores,Tmp);
  
    char RitC[5];
    if(validHeartRate == 1){
      itoa(heartRate, RitC, 10);
      strcat(valores,",");
      strcat(valores,RitC);
    } else {
      strcat(valores,",0");
    }

    char Dis[5];
    itoa(distance, Dis, 10);
    strcat(valores,",");
    strcat(valores,Dis);

    char Vel[5];
    itoa(velocidad,Vel,10);
    strcat(valores,",");
    strcat(valores,Vel);

    char Rep[5];
    itoa(repeticiones,Rep,10);
    strcat(valores,",");
    strcat(valores,Rep);
      
    miBT.print(valores);
    //---//
    //delay(500);
  } 
}
