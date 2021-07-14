//Programa: End Node com dispositivos IoT
//Autor: LABEX

//variaveis sensor distancia
 #define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
 #define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
//fim variaveis sensor distancia

//variaveis sensor chama

int pino_D0 = 7;
int pino_A0 = A1;

int valor_a = 0;
int valor_d = 0;
//fim variaveis sensor chama

//variaveis temperatura e umidade
#include "DHT.h"
#define DHTPIN  A0 //pino utilizado no esp
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
//fim variaveis temperatura e umidade

void setup()
{
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed
  pinMode(pino_A0, INPUT);
  pinMode(pino_D0, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  //Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  //Serial.println("with Arduino UNO R3");
  dht.begin();
  //Tem esses delays, nao sei se podemos tirar trata do sistema de temp e umidade
 // delay(500);//Delay to let system boot
 // Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
}
 
void loop()
{
  //Sensor de chama
  int valor_a = analogRead(pino_A0);
  int valor_d = digitalRead(pino_D0);
 
  //Editar para enviar com ou sem fogo 
  if (valor_d != 1)
  {
    Serial.println("Fogo detectado !!!");
  }else{
    Serial.println("Nada identificado!!!");
  }
  //Fim Sensor de chama
  //Decidir o que fazer com o delay que parece útil para o sensor de chama
//  delay(500); //Acho que esse pode sair e fica o delay la no final que é o maior
  //Fim Decidir o que fazer com o delay que parece útil para o sensor de chama

  //Sensor de distancia
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); //delay entre digitalWrite, nao iremos poder retirar por conta do algoritmo
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //delay entre digitalWrite, nao iremos poder retirar por conta do algoritmo
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  //Serial.print("Distance: ");
  Serial.print(distance); //Valor em centimetros que iremos enviar para a Dojot
  //Serial.println(" cm");
  //Fim sensor distancia

  //sensor de temperatura e umidade
    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();
    
    Serial.print("Current humidity = ");
    Serial.print(umidade); //Resultado umidade
    //Serial.print("%  "); 
    Serial.print("temperature = ");
    Serial.print(temperatura); //Resultado temperatura
//    Serial.println("C  ");

    //Permanece esse delay para todos, sendo baixado para 3 ou 2 segundos
    delay(3000);
  
  //fim sensor de temperatura e umidade
}
