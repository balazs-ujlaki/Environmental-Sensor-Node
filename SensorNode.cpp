#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

//defines pressure at sea level.
#define SEALEVELPRESSURE_HPA (1013.25)

//Creates and object called "bme" based on the Adafruit_BME680.
Adafruit_BME680 bme;

//Stores network SSID and password. For privacy reasons the contents of these were removed.
const char* ssid = "EC77 Hyperoptic 1Gb Fibre 2.4Ghz";
const char* pass = "k9qpGbPczAAG";

//Stores the mqttServer adress and port.
const char* mqttServer = "test.mosquitto.org";
int port = 1883;

//Stores the topics for temperature, pressure, humidity, air quality, and motion.
const char* tempTopic = "789hb23rtemp";
const char* presTopic = "789hb23rpres";
const char* humTopic = "789hb23rhum";
const char* airTopic = "789hb23rair";
const char* motTopic = "789hb23rmot";


//Creates an object called "wifiClient" from the "WiFiClient" library.
WiFiClient wifiClient;

//Creates an object called "client" based on the PubSub library and also passing the WiFiClient object to it.
PubSubClient client(wifiClient);

//Initialises various variables that will be used in the code
int mtn =0;
long currentTime;
long lastTime; 
char messages[50];


void setupWifi() {
  
  //WiFi connection is initiated.
  WiFi.begin(ssid, pass);

  //The orgram will be stuck here until WiFi connection is made.
  while(WiFi.status()!= WL_CONNECTED){}
}

//These are messages that would be printed in the terminal if the MCU were connected to a computer. The actual reconnecting occurs is handled by the microcontroller.
//But the program would need to wait for the while loop's condition to cleared before it can move on. The client must be connected.
void reconnect(){
  while(!client.connected()){
    Serial.print("\nConnecting to ");
    Serial.print(mqttServer);
    if(client.connect("esp32")){
      Serial.print("\nConnected to ");
      Serial.print(mqttServer);
    } else {
      Serial.println("Retrying connection");
    }
  }
}

//The callback function is used for receiving messages from the topics that the MCU is connected to.
//a client callback is required to be set for the PubSubClient library to work.
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Received messages: ");
  Serial.println(topic);
  for(int i=0; i<length; i++){
    Serial.print((char) payload[i]);
  }
  Serial.println();
}

void setup(){

  //Pin 13 of the MCU is set to be an input.
  pinMode(13, INPUT);

  Serial.begin(115200);

//Checks if the BME680 can be found at the 0x76 address.
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  //Oversampline for the temperature, pressure, and humidity sensors are set. 
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);

  //The IIR filter is configured.
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);

  //The gas sensor's heater plate is configured.
  bme.setGasHeater(320, 150);

  //setupWifi() on line 45 is called.
  setupWifi();

  //The MQTT server, port and callback function are set on the PubSubClient object.
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

//MCU repeatedly runs this function.
void loop() {
  
  //Checks if the MQTT client is connected.
  if(!client.connected()){

    //If not it attempts to reconnect.
    reconnect();
  }

  //This function tell the MQTT client object to loop infinitely.
  client.loop();

  //Current time of the system is stored.
  currentTime = millis();

  //Checks if 1 second has passed since the last message.
  if(currentTime - lastTime >1000){

    //The pin value (high/low) is stored to the "mtn" variable.
    mtn = digitalRead(13);

    float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

    //The "bme" object reads the temperature sensor. Then this value is stored in a variable.
    float temp = bme.temperature;

    //This function converts the temp variable to a string and stores it in the messages array.
    dtostrf(temp,2,2,messages);

    //The contents of the messages array is published to tempTopic.
    client.publish(tempTopic, messages);
    
    //The "bme" object reads the pressure sensor. Then this value is stored in a variable.
    uint32_t pres = bme.pressure/100;

    //This function converts the pres variable to a string and stores it in the messages array.
    dtostrf(pres,2,0, messages);

    //The contents of the messages array is published to presTopic.
    client.publish(presTopic, messages);

    //The "bme" object reads the humidity sensor. Then this value is stored in a variable.
    float hum = bme.humidity;

    //This function converts the hum variable to a string and stores it in the messages array.
    dtostrf(hum,2,2, messages);

    //The contents of the messages array is published to humTopic.
    client.publish(humTopic, messages);

    //The "bme" object reads the air quality sensor. Then this value is stored in a variable.
    uint32_t airQ = bme.gas_resistance/1000;

    //This function converts the qirQ variable to a string and stores it in the messages array.
    dtostrf(airQ,2,0,messages);
    
    //The contents of the messages array is published to airTopic.
    client.publish(airTopic, messages);

    //This function converts the mtn variable to a string and stores it in the messages array.
    dtostrf(mtn,2,2,messages);

    //The contents of the messages array is published to motTopic.
    client.publish(motTopic,messages);

    //The time is saved to the "lastTime" variable.
    lastTime = millis();
  }

}

