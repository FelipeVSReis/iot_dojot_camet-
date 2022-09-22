#include <SPI.h>
#include <RH_RF95.h>
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11


//Variáveis

int VIBRPIN = 0;
int DHTPIN = 3;
int LIGHTPIN = A0;
int FLAMEPIN = A1;
int SOLOHPIN = A2;

int output_value =0;
String deviceId = "A1";
float temp,humid,luz,chama;
char tem_1[8]={"\0"},hum_1[8]={"\0"},luz_1[8]={"\0"},dis_1[8]={"\0"},cha_1[4]={"\0"},deviceId_1[6] = {"\0"};
char *node_id = "<16a>"; //From LG01 via web Local Channel settings on MQTT.Please refer <> dataformat in here.
uint8_t datasend[70];
unsigned int count = 1;

DHT dht(DHTPIN, DHTTYPE); //Inicia Biblioteca

/** Variáveis LoRa**/
//RH_RF95 rf95;

//The parameter are pre-set for 868Mhz used. If user want to use lower frenqucy 433Mhz.Better to set 
//rf95.setSignalBandwidth(31250);
//rf95.setCodingRate4(8);
float frequency = 915.0;

void setup() {
  Serial.begin(9600);
  //while (!Serial) ; // Wait for serial port to be available
  pinMode(LIGHTPIN, INPUT);
  pinMode(FLAMEPIN, INPUT);
  pinMode(SOLOHPIN, INPUT);
  pinMode(VIBRPIN, INPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  Serial.println("Start Setup LoRa Client");
  //if (!rf95.init())
  // Serial.println("init failed");
  // Setup ISM frequency
  //rf95.setFrequency(frequency);
  // Setup Power,dBm
  //rf95.setTxPower(13);
  // Setup Spreading Factor (6 ~ 12)
  // rf95.setSpreadingFactor(7);
  // Setup BandWidth, option: 7800,10400,15600,20800,31250,41700,62500,125000,250000,500000
  //Lower BandWidth for longer distance.
  //rf95.setSignalBandwidth(125000);
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  //rf95.setCodingRate4(5);
  //rf95.setSyncWord(0x34);
  }

void configPin(){
  }
  void configLoRa(){
    }

void loop() {
  Serial.print("###########    ");
  Serial.print("COUNT=");
  Serial.print(count);
  Serial.println("    ###########");
  count++;
  dhtTempHumid();
  valor_chama();
  isVibration();
  soloHSensor();
  valor_luz();
}

  
void dhtTempHumid()
{
  temp = dht.readTemperature();
  humid = dht.readHumidity();
  Serial.println(F("Temperatura e Humidade:"));
  Serial.print("[");
  Serial.print(temp);
  Serial.print("℃");
  Serial.print(",");
  Serial.print(humid);
  Serial.print("%");
  Serial.print("]");
  Serial.println("");
  delay(3000);
}

void soloHSensor() {
  output_value= analogRead(SOLOHPIN);
  output_value = map(output_value,550,0,0,100);
  Serial.print("Humidade do Solo : ");
  Serial.print(output_value);
  Serial.println("%");
  delay(1000);
  }

  void valor_luz(){
  luz = analogRead(LIGHTPIN);
  Serial.print("valor luz: ");
  Serial.println(luz);
  delay(1000);
}

void valor_chama(){
  chama = analogRead(FLAMEPIN);
  Serial.print("valor chama: ");
  Serial.println(chama);
  delay(1000);
  }
void isVibration(){
  bool value = digitalRead(VIBRPIN);
  if (value == 1) {
    Serial.println("Com Vibração: SIM ");
  }else if(value == 0){
    Serial.println("Com Vibração: NÃO ");
}
}
