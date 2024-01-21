#include <Bridge.h>
#include <Console.h>
#include <BridgeClient.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ArduinoJson.h>

/***** Assetwolf Portal Setup *********/

//Connection details found on the asset page
#define SERVER "example.assetwolfportal.com"
#define PORT 1883
#define USERNAME "username"
#define PASSWORD "password"
#define DEVICE_ID "Xxs2h2"

/** Global State (you don't need to change this!) ****/

// Create a BridgeClient instance to communicate using the Yun's bridge & Linux OS.
BridgeClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, SERVER, PORT, USERNAME, PASSWORD);

/****** Feeds ***********/

// Setup a feed called 'assetPub' for publishing.
// Notice MQTT paths for Assetwolf follow the form: up/client/<username>/asset/<device_id>
Adafruit_MQTT_Publish assetPub = Adafruit_MQTT_Publish(&mqtt, "up/client/" USERNAME "/asset/" DEVICE_ID);

/***** Sketch Code **********/

void setup() {
Bridge.begin();
Console.begin();
Console.println(F("Assetwolf MQTT demo"));
}

int sensorPin = A0;
String sPayload;
char* cPayload;

void loop() {
// Ensure the connection to the MQTT server is alive (this will make the first
// connection and automatically reconnect when disconnected). See the MQTT_connect
// function definition further below.
MQTT_connect();

analogReference(EXTERNAL);
pinMode(13, OUTPUT);

StaticJsonBuffer<200> jsonBuffer;
JsonObject& payload = jsonBuffer.createObject();
payload["device_id"] = DEVICE_ID;
payload["light_level"] = analogRead(sensorPin);

sPayload = "";
payload.printTo(sPayload);
cPayload = &sPayload[0u];

// Now we can publish stuff!
Console.print(F("\nPublishing "));
Console.print(cPayload);
Console.print("...");

if (! assetPub.publish(cPayload)) {
Console.println(F("Failed"));
} else {
Console.println(F("OK!"));
}

// ping the server to keep the mqtt connection alive
if(! mqtt.ping()) {
Console.println(F("MQTT Ping failed."));
}

delay(5000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
int8_t ret;

// Stop if already connected.
if (mqtt.connected()) {
return;
}

Console.print("Connecting to MQTT... ");

while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
Console.println(mqtt.connectErrorString(ret));
Console.println("Retrying MQTT connection in 5 seconds...");
mqtt.disconnect();
delay(5000); // wait 5 seconds
}
Console.println("MQTT Connected!");
}





DC Motor speed control and measurement using Arduino and LCD

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 10, 11, 12,  13);
#define motor_RPS_pulse_ip 7
#define motor_PWM_pulse_ip 4
int motor_RPS_Ton,motor_RPS_Toff,PWM_Ton,PWM_Toff,PWM_T;
int  PWM_duty,pulse_width=255,RPS,motor_RPS_period,RPM;
float voltage;
   
void  setup()
{
  pinMode(motor_RPS_pulse_ip,INPUT);
  pinMode(motor_PWM_pulse_ip,INPUT);
  lcd.begin(16, 4);
  lcd.clear();
  lcd.print("Pulse Width:");
  lcd.setCursor(0,1);
  lcd.print("PWM Volt:");
  lcd.setCursor(0,2);
  lcd.print("Speed(RPS):");   
  lcd.setCursor(0,3);
  lcd.print("Speed(RPM):");   
}
void loop()
{
   analogWrite(3,pulse_width);  
   PWM_Ton= pulseIn(motor_PWM_pulse_ip,HIGH);
   PWM_Toff=pulseIn(motor_PWM_pulse_ip,LOW); 
   delay(2000);    
   PWM_T  = PWM_Ton+PWM_Toff;
   PWM_duty = (PWM_Ton/PWM_T)*100;  
   voltage = 0.12*PWM_duty;  
   lcd.setCursor(12,0);
   lcd.print(PWM_duty); 
   lcd.print('%');   
   lcd.setCursor(9,1);
   lcd.print(voltage); 
   lcd.print('V');   
   pulse_width-=15;
   if(pulse_width==90) pulse_width = 255;
   delay(3000);
}