#include <Arduino.h>
#include <math.h>
#include <SoftwareSerial.h>

SoftwareSerial miBT(10,11);

int Sensor = A1;
int vs = 5;
double rho = 1.1225; //air density in kg/m^3
double dt = 0.001;
/*
double area_1 = 4.90874;//d = 2.5cm, r = 1.25cm, 0.0125 m
double area_2 = 0.19635;  //d = 0.5cm, r = 0.25cm, 0.0025 m
*/
double area_1 = 0.00490874;//d = 2.5cm, r = 1.25cm, 0.0125 m
double area_2 = 0.000019635;  //d = 0.5cm, r = 0.25cm, 0.0025 m

double Vint = 0;
double Vex = 0;

int inputVolt;
double volt;
double pressure_psi;
double pressure_pa;
double massFlow;
double volFlow;
double volume;
double dp;

void setup() {
  // put your setup code here, to run once:
  volume = 0;
  pinMode(Sensor, INPUT);
  Serial.begin(9600);
  miBT.begin(38400);
}

void loop() {
  // put your main code here, to run repeatedly:
  volume = 0;
  Serial.println("Inicio Medida");
  inputVolt = analogRead(Sensor);
  
  volt = inputVolt*(vs/1023.0);
  //pressure_psi = (15/2)*(volt-2.5);
  //pressure_pa = pressure_psi * 6894.75729;
  pressure_pa = (volt-0.2) / 0.009;
  
  massFlow = 1000*sqrt((abs(pressure_pa)*2*rho)/((1/(pow(area_2,2)))-(1/(pow(area_1,2)))));  
  volFlow = massFlow/rho;
  
  //Integrar 1 seg.
  for(int i = 0; i <=400; i++) {
    volume = volFlow * dt + volume;
    delay(2);
  }
  //Fin integrar
  
  //Mandar a BT
  Serial.print("A: ");
  Serial.print(inputVolt);
  char valores[6];
  //ml/min
  if(inputVolt < 34) {
    Serial.print(", VEx: "); //Soplo (mayor) - 0
    //Vex = 1412.64 - volume*1000*60;
    Vex = volume*1000*60;
    Serial.println(Vex);

    int e = Vex;
    char Ex[5];
    itoa(e, Ex,10);
    strcpy(valores,Ex);
    strcat(valores,",0");
    miBT.print(valores);
    
  } else if(inputVolt > 36) {
    Serial.print(", VInt: "); //Jalo (menor) - 1
    //Vint = volume*1000*60 - 1412.64;
    Vint = volume*1000*60;
    Serial.println(Vint);

    //Serial.print("VT: ");
    //Serial.print(Vex - Vint);
    //Serial.print(", vo2: ");
    //Serial.println( ((Vex - Vint)+1412.64)/49 );

    int i = Vint;
    char Int[5];
    itoa(i, Int,10);
    strcpy(valores,Int);
    strcat(valores,",1");
    miBT.print(valores);
    
  } else {
    Serial.print(", V0: ");
    Serial.println(volume*1000*60);

    miBT.print("0,3");
  }
  Serial.println("Fin Medida\n");
}
