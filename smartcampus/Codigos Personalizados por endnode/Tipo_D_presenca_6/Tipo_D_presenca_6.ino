#include <SPI.h>
#include <RH_RF95.h>
#include "DHT.h"
#define DHTTYPE DHT22

int PIRPIN = 4;
int SOUNDPIN = A0;
int DHTPIN = A1;
int LIGHTPIN = A2;

int deviceId = 6;

float temp, umi, pir, som, luz;
char tem_1[8] = {"\0"}, umi_1[8] = {"\0"}, luz_1[8] = {"\0"}, pir_1[8] = {"\0"}, som_1[8] = {"\0"}, deviceId_1[6] = {"\0"};
char *node_id = "<16a>";
uint8_t datasend[80];
unsigned int count = 1;

DHT dht(DHTPIN, DHTTYPE);

RH_RF95 rf95;
float frequency = 915.0;

void setup()
{
  Serial.begin(9600);
  configPin();
  configLoRa();
}

void configPin()
{
  pinMode(PIRPIN, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(SOUNDPIN, INPUT);
  pinMode(LIGHTPIN, INPUT);
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
  valor_luz();
  valor_som();
  PIRSensor();
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
  som = analogRead(SOUNDPIN);
  delay(1000);
}

void valor_luz()
{
  luz = analogRead(LIGHTPIN);
  delay(1000);
}

void PIRSensor()
{
  pir = digitalRead(PIRPIN);
  delay(50);
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
  Serial.print("valor som: ");
  Serial.print(som);
  Serial.print("valor luz: ");
  Serial.println(luz);
  Serial.print("valor PIR (Presenca): ");
  Serial.println(pir);
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
  dtostrf(som, 0, 1, som_1);
  dtostrf(luz, 0, 1, luz_1);
  dtostrf(pir, 0, 1, pir_1);
  ltoa(deviceId, deviceId_1, 10);
  // Serial.println("debugInicial:");
  // Serial.println(tem_1);
  // Serial.println(umi_1);
  // Serial.println(som_1);
  // Serial.println(presenca_1);
  // Serial.println(luz_1);
  strcat(data, "{\"pld\":");
  strcat(data, "{\"i\":");
  strcat(data, deviceId_1);
  strcat(data, ",\"t\":");
  strcat(data, tem_1);
  strcat(data, ",\"u\":");
  strcat(data, umi_1);
  strcat(data, ",\"s\":");
  strcat(data, som_1);
  strcat(data, ",\"p\":");
  strcat(data, pir_1);
  strcat(data, ",\"l\":");
  strcat(data, luz_1);
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
