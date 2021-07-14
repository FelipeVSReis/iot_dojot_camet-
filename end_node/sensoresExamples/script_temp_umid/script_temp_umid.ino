#include "DHT.h"
#define DHTPIN  A0 //pino utilizado no esp
#define DHTTYPE DHT11   // DHT 11
 
DHT dht(DHTPIN, DHTTYPE);
 
void setup(){
 
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
  dht.begin();
 
}//end "setup()"
 
void loop(){
  //Start of Program 
    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();
    
    Serial.print("Current humidity = ");
    Serial.print(umidade);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(temperatura); 
    Serial.println("C  ");
    
    delay(5000);//Wait 5 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.
 
}// end loop(
