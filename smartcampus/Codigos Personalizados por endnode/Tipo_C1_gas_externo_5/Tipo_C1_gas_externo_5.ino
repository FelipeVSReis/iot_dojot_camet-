#include <SPI.h>
#include <RH_RF95.h>
#include "DHT.h"
#define DHTTYPE DHT11

int MQ135PIN = A0;
int DHTPIN = A1;
int MQ9PIN = A2;
int MQ4PIN = A3;

int deviceId = 5;

float mq4, mq9, mq135, temp, umi;
char tem_1[8] = {"\0"}, umi_1[8] = {"\0"}, mq4_1[8] = {"\0"}, mq9_1[8] = {"\0"}, mq135_1[8] = {"\0"}, deviceId_1[6] = {"\0"};
char *node_id = "<16a>";
uint8_t datasend[80];
unsigned int count = 1;

DHT dht(DHTPIN, DHTTYPE);

RH_RF95 rf95;
float frequency = 915.0;

void setup()
{
  Serial.begin(9600);
  Serial.println("MQ4, MQ9 and MQ135 are warming up");
  delay(120000);
  configPin();
  configLoRa();
}

void configPin()
{
  pinMode(MQ4PIN, INPUT);
  pinMode(MQ9PIN, INPUT);
  pinMode(MQ135PIN, INPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
}

void configLoRa()
{
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

void loop()
{
  dhtTempHumid();
  valor_metano();
  valor_CO();
  valor_toxico();
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

void valor_metano()
{
  mq4 = analogRead(MQ4PIN);
  delay(1000);
}

void valor_CO()
{
  mq9 = analogRead(MQ9PIN);
  delay(1000);
}

void valor_toxico()
{
  mq135 = analogRead(MQ135PIN);
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
  Serial.print("Metano: ");
  Serial.print(mq4);
  Serial.print("CO: ");
  Serial.println(mq9);
  Serial.print("Gases Tóxicos: ");
  Serial.println(mq135);
}

void sensorWrite()
{
  Serial.println("Inicio sensor write:");
  char data[80] = {};
  for (int i = 0; i < 80; i++)
  {
    data[i] = node_id[i];
  }
  dtostrf(temp, 0, 1, tem_1);
  dtostrf(umi, 0, 1, umi_1);
  dtostrf(mq4, 0, 1, mq4_1);
  dtostrf(mq9, 0, 1, mq9_1);
  dtostrf(mq135, 0, 1, mq135_1);
  ltoa(deviceId, deviceId_1, 10);
  //Serial.println("debugInicial:");
  //Serial.println(tem_1);
  //Serial.println(umi_1);
  //Serial.println(mq4_1);
  //Serial.println(mq9_1);
  //Serial.println(mq135_1);
  strcat(data, "{\"pld\":");
  strcat(data, "{\"i\":");
  strcat(data, deviceId_1);
  strcat(data, ",\"t\":");
  strcat(data, tem_1);
  strcat(data, ",\"u\":");
  strcat(data, umi_1);
  strcat(data, ",\"m\":");
  strcat(data, mq4_1);
  strcat(data, ",\"c\":");
  strcat(data, mq9_1);
  strcat(data, ",\"x\":");
  strcat(data, mq135_1);
  strcat(data, "}}");
  strcpy((char *)datasend, data);

  Serial.println("debug pacote: ");
  Serial.println((char *)datasend);
  Serial.println(sizeof datasend);
}

void SendData()
{
  Serial.println(F("Sending data to LG01"));
  rf95.send((char *)datasend, sizeof(datasend));
  rf95.waitPacketSent();

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000))
  {
    if (rf95.recv(buf, &len))
    {

      Serial.print("got reply from LG01: ");
      Serial.println((char *)buf);
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
