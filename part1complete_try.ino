#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include "LiquidCrystal_I2C.h"
#include "PCF8574.h"
#include <Wire.h>

#include "incubatorWebserver.h"

const char* ssid = "INCUBATOR-Access-Point";
const char* password = "123456789";

#define eggTurner_pin D6
#define heater_pin D5
#define DHTPIN D3    // Digital pin connected to the DHT sensor
#define waterLevel_pin A0

#define DHTTYPE DHT11 // Uncomment the type of sensor in use: DHT11, DHT22, or DHT21

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Set i2c HEX address
PCF8574 pcf8574(0x20);

unsigned char buzLoop;
int menu = 1;
int eggTurn_Stat = 0;
String turnerStat_in_string = "";
int lightState = 0;
int motion_state = 0; 
String btn_turner = "";
String waterLevel = "";

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
int level = 0; // variable to store the sensor value
int setTemp = 37; // Default set temperature

// Time start Settings:
int startingHour = 0; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time
int seconds = 0;
int minutes = 58;
int hours = startingHour;
int days = 0;

// Blink settings
int blinkIntervalHour = 6; // set the hour interval for LED blinking

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned long previousMillisDHT = 0; // will store last time DHT was updated
unsigned long previousMillisTime = 0; // will store last time time was updated

// Updates DHT readings every 10 seconds
const long intervalDHT = 10000;
const long intervalTime = 1000;


// Replaces placeholder with DHT values
String processor(const String& var){
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  else if(var == "DAYS"){
    return String(days);
  }
  else if(var == "HOURS"){
    return String(hours);
  }
  else if(var == "MINUTES"){
    return String(minutes);
  }
  else if(var == "SECONDS"){
    return String(seconds);
  }
  else if(var == "SETTEMP"){
    return String(setTemp);
  }
  else if(var == "WATERLEVEL"){
    return String(waterLevel);
  }
  else if(var == "BTN_TURNER"){
    return String(btn_turner);
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Initialize I2C communication
  dht.begin();
  pinMode(heater_pin, OUTPUT);
  pinMode(eggTurner_pin, OUTPUT);
  lcd.init();
  lcd.backlight();


  pcf8574.pinMode(P0, INPUT_PULLUP);
  pcf8574.pinMode(P1, INPUT_PULLUP);
  pcf8574.pinMode(P2, INPUT_PULLUP);
  pcf8574.pinMode(P3, INPUT_PULLUP);
  pcf8574.pinMode(P4, INPUT_PULLUP);

  pcf8574.pinMode(P5, INPUT); //LDR_PIN
  pcf8574.pinMode(P6, INPUT); //MOTION_SENSOR_PIN
  pcf8574.pinMode(P7, OUTPUT); //BUZZER - the pin of the active buzzer

  pcf8574.begin();
  menu = 1;

  Serial.print("Setting AP (Access Point)...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/days", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(days).c_str());
  });
  server.on("/hours", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(hours).c_str());
  });
  server.on("/minutes", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(minutes).c_str());
  });
  server.on("/seconds", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(seconds).c_str());
  });
   server.on("/waterLevel", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(waterLevel).c_str());
  });
   server.on("/btn_turner", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(btn_turner).c_str());
  });

   // Endpoint to update setTemp
  server.on("/setTemp", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      setTemp = request->getParam("value")->value().toInt();
      request->send(200, "text/plain", "OK");
    } else {
      request->send(200, "text/plain", String(setTemp).c_str());
    }
  });

    server.on("/setTurnerStat", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("value")) {
        eggTurn_Stat = request->getParam("value")->value().toInt();
        request->send(200, "text/plain", "OK");
      } else {
        request->send(400, "text/plain", "Bad Request");
      }
    });
  server.begin();
  
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisDHT >= intervalDHT) {
    previousMillisDHT = currentMillis;
    float newT = dht.readTemperature();
    if (!isnan(newT)) {
      t = newT;
    }
    float newH = dht.readHumidity();
    if (!isnan(newH)) {
      h = newH;
    }
  }
                  
  level = analogRead(waterLevel_pin); // read the analog value from sensor

  Serial.println("Sensor value: ");
  Serial.println(level);

      if (level > 400 && level < 500){
        waterLevel = "HIGH";
        Serial.println("Water level is HIGH");
      } 
      else if (level > 300 && level < 400) {
        waterLevel = "NORMAL";
        Serial.println("Water level is NORMAL");
      } 
      else if (level > 1 && level < 300) {
        waterLevel = "LOW";
        Serial.println("Water level is LOW");
      } else {
        waterLevel = "NO WATER";
        Serial.println("No Water");
      }

  if (currentMillis - previousMillisTime >= intervalTime) {
    previousMillisTime = currentMillis;
    seconds++;
    if (seconds == 60) {
      seconds = 0;
      minutes++;
      if (minutes == 60) {
        minutes = 0;
        hours++;
        if (hours == 24) {
          hours = 0;
          days++;
        }
      }
    }
  }
  Serial.print("The time is:           ");
  Serial.print(days);
  Serial.print(":");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(":"); 
  Serial.println(seconds);


  Serial.println(" "); 
  Serial.print("Menu: "); 
  Serial.println(menu); 
  Serial.println(" "); 
  Serial.print("Set temperature: "); 
  Serial.println(setTemp); 


  // For temperature control
  if (t < setTemp) {
    digitalWrite(heater_pin, HIGH);
  } else if (t >= setTemp + 0.60) {
    digitalWrite(heater_pin, LOW);
  }

 // For automatic rotation control
    if(!pcf8574.digitalRead(P4)){
    eggTurn_Stat++;
    delay(100);
    while(!pcf8574.digitalRead(P4));
  }
  
    switch (eggTurn_Stat) {
      case 0:
      btn_turner = "Turner is OFF";
      turnerStat_in_string = "OFF";
      Serial.println("Egg tilt status is OFF!");
      break;

      case 1:
            if (hours % blinkIntervalHour == 0 && minutes == 0 && seconds == 0) {
              digitalWrite(eggTurner_pin, HIGH);
              delay(2000);
              digitalWrite(eggTurner_pin, LOW);
            }
      btn_turner = "Turner is ON";
      turnerStat_in_string = "ON";
      Serial.println("Egg tilt status is ON!");
      break;

      case 2:
      eggTurn_Stat = 0;
      break;
    }
   
  //For menu navigation buttons
    if(!pcf8574.digitalRead(P0)){
    menu++;
    delay(100);
    while(!pcf8574.digitalRead(P0));
  }

  if(!pcf8574.digitalRead(P1)){
    menu--;
    delay(100);
    while(!pcf8574.digitalRead(P1));
  }

  //For set temperture buttons
    if(!pcf8574.digitalRead(P2)){
    setTemp++;
    delay(100);
    while(!pcf8574.digitalRead(P2));
  }

    if(!pcf8574.digitalRead(P3)){
    setTemp--;
    delay(100);
    while(!pcf8574.digitalRead(P3));
  }

      //For menu display to the lcd
      switch (menu) {
        case 0:
        menu = 1;
        break;

        case 1: 
        clearData();
        sensorReadings();
        lcd.setCursor(6, 2);
        lcd.print((String)t + " " + (char)223 + "C");
        lcd.setCursor(6, 3);
        lcd.print((String)h + " %");
        delay(500);

        Serial.println("  SENSOR READINGS  >");
        break;

        case 2:
        clearData();
        setTemperature();
        lcd.setCursor(6, 2);
        lcd.print((String)setTemp);

        Serial.println("< SET TEMPERATURE  >");
        delay(500);
        break;

        case 3:
        clearData();
        eggTilt();
        lcd.setCursor(6, 2);
        lcd.print((String)turnerStat_in_string);

        Serial.println("< EGG TILT STATUS  >");
        delay(500);
        break;

        case 4:
        clearData();
        water_level();
        lcd.setCursor(6, 2);
        lcd.print((String)waterLevel);

        Serial.println("<   WATER LEVEL    >");
        delay(500);

        break;

        case 5:
        clearData();
        upTime();
        lcd.setCursor(6, 1);
        lcd.print((String)days);
        lcd.setCursor(6, 2);
        lcd.print((String)hours);
        lcd.setCursor(6, 3);
        lcd.print((String)minutes);

        Serial.println("<      UP-TIME      ");
        delay(500);
        break;

        case 6:
        menu = 5;
        break;
      }
      
  Serial.println(" ");
  lightState = pcf8574.digitalRead(P5);
  motion_state = pcf8574.digitalRead(P6);  
       if (lightState == HIGH) {
          Serial.println("It is dark");
      } else {
          Serial.println("It is light");
      }

      if (motion_state == HIGH) {   // pin state change: LOW -> HIGH
        Serial.println("Motion detected!");
     } else {
        Serial.println("No Motion detected!");
    }

     delay(1000);
}

void sensorReadings() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  SENSOR READINGS  >");
  lcd.setCursor(0, 2);
  lcd.print("Temp:");
  lcd.setCursor(0, 3);
  lcd.print("Humi:");
}

void setTemperature() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("< SET TEMPERATURE  >");
  lcd.setCursor(0, 2);
  lcd.print("Set: ");
}

void eggTilt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("< EGG TILT STATUS  >");
  lcd.setCursor(0, 2);
  lcd.print("Stat: ");
}

void water_level() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("<    WATER LEVEL    >");
  lcd.setCursor(0, 2);
  lcd.print(" Lvl: ");
}

void upTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("<      UP-TIME      ");
  lcd.setCursor(0, 1);
  lcd.print("Days :");
  lcd.setCursor(0, 2);
  lcd.print("Hours:");
  lcd.setCursor(0, 3);
  lcd.print("Mins: ");
}


void clearData() {
  lcd.setCursor(6, 1);
  lcd.print("         ");
  lcd.setCursor(6, 2);
  lcd.print("         ");
  lcd.setCursor(6, 3);
  lcd.print("         ");
}


void buzzerSound() {
    while(1) {
    for(buzLoop = 0; buzLoop < 100; buzLoop++) {
      pcf8574.digitalWrite(P7,HIGH);
      delay(500);
      pcf8574.digitalWrite(P7,LOW);
      delay(500);
    }

  }
}
