//__author__ <tinkrmind>
//23 Aug 2015
//Creative Commons 2.0 Share Alike
//
//
//Hardware Connections:
//Accelerometer and Barometer connected to I2C lines (A4, A5)
//DHT sensor connected to D4
//UV sensor connected to A0
//Noise sensor to A1
//Dust sensor connected to D5
//
//
//======================================INITIAL DECLARATIONS==============================
#include <Suli.h>
#include <Wire.h>

//Accelerometer Stuff
#include "acc_adxl345_Arduino.h"
Acc_Adxl345 acc;
float a, ax, ay, az;
unsigned long accel_time;

//DHT stuff
#include "DHT.h"
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
unsigned long dht_time, dht_interval = 2000;

//UV Sensor stuff
unsigned long uv_time;
float max_uv = 100000000, exposed_uv=0;

//Barometer Stuff
#include "Barometer.h"
float temperature;
float pressure;
float atm;
float altitude;
Barometer myBarometer;
unsigned long baro_time, baro_interval = 1000;

//dust sensor stuff
int dust_pin = 5;
unsigned long duration;
unsigned long starttime;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
unsigned long dust_time, dust_interval = 30000;//sampe 30s ;

//GPS Stuff
#include <LGPS.h>
gpsSentenceInfoStruct info;
char buff[256];
unsigned long gps_time, gps_interval = 1000;
int led = 13, num_sat;

//SD Card Stuff
#include <LTask.h>
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>
#define Drv LFlash          // use Internal 10M Flash

//Time stuff
#include <LDateTime.h>
unsigned int rtc;
unsigned long now_time;

// Sending data stuff
struct data_struct{
  int token;
  long id;
  unsigned long time;
  float ax;
  float ay;
  float az;
  float bump;
  float temp;
  float humid;
  int noise;
  float lati;
  float longi;  
  float alt;
  float uv;
  float dust;
  float sped;
} data;

int buzzer = 8;
//Internat logic stuff
int start_pin = 3;
int stop_pin = 2;
bool ride_started = FALSE;

//====================================SETUP===============================================

void setup()
{  
  data.token = 1;
  
  LDateTime.getRtc(&rtc);
  data.id = (unsigned long)rtc;
  
  data.humid = 0;
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  acc.begin();
  accel_time = millis();
  
  dht.begin();
  dht_time = millis();
  
  myBarometer.init();
  baro_time = millis();
  
  pinMode(dust_pin, INPUT);
  dust_time = millis();
  
  LGPS.powerOn();
//  Serial.println("LGPS Power on, and waiting ..."); 
  gps_time = millis();
  
  pinMode(start_pin, INPUT);
  pinMode(stop_pin, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
    
  pinMode(10, OUTPUT);
  LTask.begin();
  Drv.begin();
//  Serial.println("card initialized.");
  
  uv_time = millis();
  
  starttime = millis();   
  
  Serial.println("hello world"); 
    
  while(digitalRead(start_pin));
  
  start_ride();
  
//  LGPS.getData(&info);
//  num_sat = parseGPGGA((const char*)info.GPGGA);
//  while(num_sat < 3)
//  {
//    Serial.println("Waiting on GPS fix");
//    LGPS.getData(&info);
//    num_sat = parseGPGGA((const char*)info.GPGGA);
//    delay(1000);
//  }
}

//========================================LOOPING============================================
// Each loop cycle prints all sensor values and writes to the datafile. 
void loop()
{
    now_time= millis();
    
    if(!digitalRead(stop_pin))
      stop_ride();

    accel_sense();
    accel_time = now_time;
    
    
    now_time= millis(); 
    if((now_time - dht_time) > dht_interval)
    {
      dht_read();
      dht_time = now_time;
    }
    
    now_time= millis();
    uv_sense();
    uv_time = now_time;
    
    now_time= millis();
    if(now_time - baro_time > baro_interval)
    {
      baro_sense();
      baro_time = now_time;
    }
    
    duration = pulseIn(dust_pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;
    
    now_time= millis();
    if((now_time - dust_time) > dust_interval)// && sensor_num == 5)
    {
      dust_sense();
      dust_time = now_time;
    }
    
    now_time= millis();      
    sound_sense();    
    
    now_time= millis();      
    if((now_time - gps_time) > gps_interval)// && sensor_num == 5)
    {
       LGPS.getData(&info);
       num_sat = parseGPGGA((const char*)info.GPGGA);
       gps_time = now_time;
    }      
    
    send_data();
    
    for(int o=0; o<100; o++)
    {
      digitalWrite(buzzer, HIGH);
      delayMicroseconds(200);
      digitalWrite(buzzer, LOW);
      delayMicroseconds(200);
    }
}


