// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//i2c pins
LiquidCrystal_I2C lcd(0x27, 20, 4); 

#define DHTPIN A0    // connect to pin A0
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

const String myphone = "+8489350025";       // Replace your number here
const int PWR_KEY =  9;                     // PIN 9 controll turn on/off Module Sim900A

void Gsm_Power_On();                        // Function turn on module Sim 900A
void Gsm_Init();                            // Function configure Module Sim 900A
void Gsm_MakeSMS();                         // Function to send sms


//int sensorPin = A0; // select the input pin for the LDR

//int sensorValue = 0; // variable to store the value coming from the sensor

int led_buzzer = A3; // Output pin for LED and Buzzer

void setup() 
{
    Serial.begin(9600);
    Serial2.begin(9600);
    dht.begin();
    //We define our LCD 16 columns and 2 rows
    lcd.init();
    lcd.backlight();//Power on the back light
    pinMode(led_buzzer, OUTPUT); 
    digitalWrite(led_buzzer,HIGH); 
    Gsm_Power_On();                               
    Gsm_Init();                                                         
}

void loop() 
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    int h = int(dht.readHumidity());
    int t = int(dht.readTemperature());

    // check if returns are valid, if they are NaN (not a number)
    // then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else 
    {
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
        //Print the result to LCD
        lcd.clear();//Clean the screen
        lcd.setCursor(0,0); 
        lcd.print("H = ");
        lcd.print(h);
        lcd.print("% ");
        lcd.print("T = ");
        lcd.print(t);
        lcd.print("*C");
        digitalWrite(led_buzzer,HIGH); 
        delay(1000); 

        if (t>50) {
            lcd.setCursor(0,1);
            lcd.print("Heat Warning!");
            digitalWrite(led_buzzer,LOW); 
            delay(1000);
            Gsm_MakeSMS();
            delay(20000);
        }
        delay(2000);
    }
}

void Gsm_Power_On()
{
  digitalWrite(PWR_KEY, HIGH);                
  delay(1500);                                
  digitalWrite(PWR_KEY, LOW);                 
  delay(10000);                               
}
 
void Gsm_Init()
{
  Serial2.println("ATE0");                     
  delay(2000);
  Serial2.println("AT+IPR=9600");              
  delay(2000);
  Serial2.println("AT+CMGF=1");                // Select TEXT Mode
  delay(2000);
  Serial2.println("AT+CLIP=1");                // Show the caller's information
  delay(2000);
  Serial2.println("AT+CNMI=2,2");              // Show the sms
  delay(2000);
}
 
void Gsm_MakeSMS()
{
  Serial2.println("AT+CMGS=\"" + myphone + "\"");     
  delay(5000);
  Serial2.print("Fire Warning!")
  Serial2.print("The temperature is over: ");            
  Serial2.print(t);
  Serial2.print(" *C!");
  Serial2.print("Please check your house");
  Serial2.print((char)26);
  delay(5000);
}