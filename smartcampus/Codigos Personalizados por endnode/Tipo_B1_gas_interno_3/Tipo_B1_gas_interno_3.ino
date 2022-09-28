#include <SPI.h>
#include <RH_RF95.h>
#include "DHT.h"
#define DHTTYPE DHT11   


int DHTPIN = A1;
int SOUNDPIN = A2;
int MQ5PIN = A0;
int MQ3PIN = A3;

int deviceId = 3;
boolean vibracao;
float sound,mq5,mq3,temp,umi;
char tem_1[8]={"\0"},umi_1[8]={"\0"},mq5_1[8]={"\0"},mq3_1[8]={"\0"},sound_1[8]={"\0"},deviceId_1[6] = {"\0"};
char *node_id = "<16a>"; 
uint8_t datasend[80];
unsigned int count = 1;

DHT dht(DHTPIN, DHTTYPE); 

RH_RF95 rf95;
float frequency = 915.0;

void setup() {
  Serial.begin(9600);
  Serial.println("MQ3 and MQ5 is warming up");
  delay(120000);  
  configPin();
  configLoRa();
  }

void configPin(){
  pinMode(MQ5PIN, INPUT);
  pinMode(MQ3PIN, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(SOUNDPIN, INPUT);
  dht.begin();
  }
  
  void configLoRa(){
    Serial.println("Start Setup LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  rf95.setFrequency(frequency);
  rf95.setTxPower(13);
   rf95.setSpreadingFactor(7);
  rf95.setSignalBandwidth(125000);
  rf95.setCodingRate4(5);
  rf95.setSyncWord(0x34);
}

void loop() {
  dhtTempHumid();
  valor_som();
  valor_glp();
  valor_alcool();
  printSerial();
  sensorWrite();
  SendData();
}

  
void dhtTempHumid()
{
  temp = dht.readTemperature();
  umi = dht.readHumidity();
  delay(3000);
}

void valor_som() 
{
  sound= analogRead(SOUNDPIN);
  delay(1000);
  }

void valor_glp()
{
  mq5 = analogRead(MQ5PIN);  
  delay(1000);
}

void valor_alcool()
{
  mq3 = analogRead(MQ3PIN);
  delay(1000);
  }

void printSerial()
{
  Serial.print("###########    ");
  Serial.print("COUNT=");
  Serial.print(count);
  Serial.println("    ###########");
  count++;
  Serial.println(F("Temperatura e Umidade:"));
  Serial.print("[");
  Serial.print(temp);
  Serial.print("℃");
  Serial.print(",");
  Serial.print(umi);
  Serial.print("%");
  Serial.print("]");
  Serial.println("");
  Serial.print("Som: ");
  Serial.print(sound);
  Serial.println("%");
  Serial.print("valor alcool: ");
  Serial.println(mq3);
  Serial.print("valor glp: ");
  Serial.println(mq5);
}

void sensorWrite()
{
    Serial.println("Inicio sensor write:");
    char data[80] = {};
    for(int i = 0; i < 80;i++)
    {
       data[i] = node_id[i];
    }
    dtostrf(temp,0,1,tem_1);
    dtostrf(umi,0,1,umi_1);
    dtostrf(sound,0,1,sound_1);
    dtostrf(mq3,0,1,mq3_1);
    dtostrf(mq5,0,1,mq5_1);
    ltoa(deviceId,deviceId_1,10);
    Serial.println("debugInicial:");
    Serial.println(tem_1);
    Serial.println(umi_1);
    Serial.println(sound_1);
    Serial.println(mq3_1);
    Serial.println(mq5_1);
     strcat(data,"{\"pld\":");
     strcat(data,"{\"i\":");
     strcat(data,deviceId_1);
     strcat(data,",\"t\":");
     strcat(data,tem_1);
     strcat(data,",\"u\":");
     strcat(data,umi_1);
     strcat(data,",\"s\":");
     strcat(data,sound_1);
     strcat(data,",\"a\":");
     strcat(data,mq3_1);
     strcat(data,",\"g\":");
     strcat(data,mq5_1);
     strcat(data,"}}");
     strcpy((char *)datasend,data);
     
   Serial.println("debug pacote: ");
   Serial.println((char *)datasend);
   Serial.println(sizeof datasend);     
}

void SendData()
{
  Serial.println(F("Sending data to LG01"));
      rf95.send((char *)datasend,sizeof(datasend));  
      rf95.waitPacketSent(); 
    
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

     if (rf95.waitAvailableTimeout(3000))
  { 
    if (rf95.recv(buf, &len))
   {
     
      Serial.print("got reply from LG01: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }
  delay(5000);
}