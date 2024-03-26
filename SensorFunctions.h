#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_Sensor.h> 
#include "Protocentral_MAX30205.h"
#include <Adafruit_ADXL345_U.h>
#include <ArduinoJson.h>

MAX30205 tempSensor;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
Adafruit_AHTX0 aht;

sensors_event_t event; 
float temp;
sensors_event_t humidity, ambtemp;

void initSensors() {
  Serial.begin(115200);  
  
  if (!accel.begin()) {
    Serial.println("No valid sensor found");
    while (1);
  }
  
  tempSensor.begin();
  
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  
  Serial.println("AHT10 or AHT20 found");
}

void readSensorData() {

  accel.getEvent(&event);
  // Serial.print("X: "); 
  // Serial.print(event.acceleration.x); 
  // Serial.print("  ");
  // Serial.print("Y: "); 
  // Serial.print(event.acceleration.y); 
  // Serial.print("  ");
  // Serial.print("Z: "); 
  // Serial.print(event.acceleration.z); 
  // Serial.print("  ");
  // Serial.print("m/s^2  ");

  temp = tempSensor.getTemperature(); // read temperature for every 100ms
  // Serial.print(temp, 2);
  // Serial.print("'c  " );

  
  aht.getEvent(&humidity, &ambtemp);
  // Serial.print("Temperature: "); 
  // Serial.print(ambtemp.temperature); 
  // Serial.print(" degrees C  ");
  // Serial.print("Humidity: "); 
  // Serial.print(humidity.relative_humidity); 
  // Serial.println("% rH");  

  delay(500);
}

String serializeSensorData() {
  StaticJsonDocument<200> doc;
  doc["temperature"] = temp;
  doc["xaxis"] = event.acceleration.x;
  doc["yaxis"] = event.acceleration.y;
  doc["zaxis"] = event.acceleration.z;
  doc["ambienttemperature"] = ambtemp.temperature;
  doc["humidity"] = humidity.relative_humidity;
  
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  
  return String(jsonBuffer);
}

#endif
