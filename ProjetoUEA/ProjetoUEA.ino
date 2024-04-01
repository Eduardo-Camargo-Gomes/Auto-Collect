#include <Servo.h>

#define Num_Sen 4

Servo Papel;
Servo Metal;
Servo Plastico;
Servo Vidro;

const int pinoTrig[Num_Sen] = {40, 42, 44, 46};
const int pinoEcho[Num_Sen] = {41, 43, 45, 47};

float mediaDasDistancias[Num_Sen] = {0};
String status= "desocupado";

void setup() {
  Serial.begin(9600); 
  Papel.attach(3);
  Metal.attach(4);
  Plastico.attach(5);
  for (int i = 0; i < Num_Sen; i++) {
    pinMode(pinoTrig[i], OUTPUT);
    pinMode(pinoEcho[i], INPUT);
    mediaDasDistancias[i] = calculaMediaDasDistancias(pinoTrig[i], pinoEcho[i]);
  }
   Serial.println("desocupado");
}

void loop() {
  if (status=="desocupado") { 
    if (Serial.available() > 0) {
      String material = Serial.readStringUntil(';');
      if (material == "vidro") {
        status="ocupado";
        Serial.println("ocupado");
        verificar(Vidro, pinoTrig[0], pinoEcho[0], 0);
      } else if (material == "papel") {
       status="ocupado";
        Serial.println("ocupado");
        abrir(Papel);
        verificar(Papel, pinoTrig[1], pinoEcho[1], 1);
      } else if (material == "metal") {
        status="ocupado";
         Serial.println("ocupado");
        abrir(Metal);
        verificar(Metal, pinoTrig[2], pinoEcho[2], 2);
      } else if (material == "plastico") {
        status="ocupado";
         Serial.println("ocupado");
        abrir(Plastico);
        verificar(Plastico, pinoTrig[3], pinoEcho[3], 3);
      }
    }
  }
  
}

void abrir(Servo servo) {
  servo.write(60);
}

void verificar(Servo servo, int pinoTrig, int pinoEcho, int indice) {
  while (true) {
    float distancia = calculaMediaDasDistancias(pinoTrig, pinoEcho);
    Serial.println(distancia);
    if (abs(mediaDasDistancias[indice]-distancia) >= 2.5) {
      if (indice == 0) {
        status="desocupado";
        Serial.println("desocupado");
        break; 
      } else {
        servo.write(0);
        status="desocupado";
        Serial.println(status);
        break; 
      }
    }
  }
}

float calculaMediaDasDistancias(int pinoTrig, int pinoEcho) {
  float distancia = 0;
  for (int i = 0; i < 10; i++) {
    digitalWrite(pinoTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(pinoTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinoTrig, LOW);
    float duracao = pulseIn(pinoEcho, HIGH);
    distancia += (duracao * 0.034 / 2);
  }
  return distancia / 10.0;
}
