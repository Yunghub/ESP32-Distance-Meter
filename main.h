#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "index.h"  //Web page header file

#include "Free_Fonts.h" // Include the header file attached to this sketch

#include "SPI.h"
#include "TFT_eSPI.h"

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

unsigned long drawTime = 0;

WebServer server(80);

long duration;
float distanceCm = 5;
float distanceInch = 5;

int measureState = 0;

//Enter your SSID and PASSWORD
const char* ssid = "SSID";
const char* password = "Password";

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {
 double a = distanceCm;
 String adcValue = String(a);
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void handleADC1() {
 double a = distanceInch;
 String adcValue = String(a);
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

const int trigPin = 5;
const int echoPin = 17;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

int measured = 0;

float displayCm = 0.00;
float displayIn = 0.00;

float potValue = 0;
int nSample = 1;

char sampleStr[6];

void setup(void) {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Serial.println();
  Serial.println("Booting Sketch...");

/*
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/
//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);


  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
  if (WiFi.status() == WL_CONNECTED){
    server.on("/", handleRoot);      //This is display page
    server.on("/readADC", handleADC);//To get update of ADC Value only
    server.on("/readADC1", handleADC1);//To get update of ADC Value only
 
    server.begin();                  //Start server
    Serial.println("HTTP server started");
  }

  tft.begin();

  tft.setRotation(1);

    // Set text datum to middle centre
  tft.setTextDatum(MC_DATUM);

  // Set text colour to orange with black background
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillScreen(TFT_BLACK); 
  tft.setFreeFont(FF22);
  tft.drawString("DISTANCE METER", 160, 20, GFXFF);// Print the string name of the font

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FF23);
  tft.drawString("CM", 233, 95, GFXFF);// Print the string name of the font
  tft.drawString("IN", 233, 145, GFXFF);// Print the string name of the font

  tft.setFreeFont(FF21);

  char connectedStatus[20];
  
  if (WiFi.status() == WL_CONNECTED){
    sprintf(connectedStatus, "Web Server: ON", displayCm);
  } else{
    sprintf(connectedStatus, "Web Server: OFF", displayCm);
  }
  tft.drawString(connectedStatus, 160, 220, GFXFF);// Print the string name of the font

  tft.drawString("Readings: ", 140, 200, GFXFF);// Print the string name of the font

}

void loop() {
  int currentStateReset;
  int currentStateMeasure;
  int lastStateReset = LOW;
  int lastStateMeasure = LOW;
  int potValue;
  
  currentStateReset = digitalRead(21);
  Serial.println(currentStateReset);
  if(currentStateReset == HIGH) {
    measureState = 0;
    measured = 0;
  }
  lastStateReset = currentStateReset;

  currentStateMeasure = digitalRead(22);
  Serial.println(currentStateMeasure);
  if(currentStateMeasure == HIGH) {
    measureState = 1;
    measured = 0;
  }
  lastStateMeasure = currentStateMeasure;

  potValue = analogRead(34);
  nSample = map(potValue, 0, 4095, 10, 300);

  Serial.print ("POTTTT: ");
  Serial.println(nSample);
  
  
  if (nSample <10){
    sprintf(sampleStr, "00%d", nSample);
  }
  else if(nSample <100){
    sprintf(sampleStr, "0%d", nSample);
  }
  else if(nSample <350){
    sprintf(sampleStr, "%d", nSample);
  } else{
    sprintf(sampleStr, "   N/A   ", nSample);
  }

  tft.setFreeFont(FF21 );
  
  tft.drawString(sampleStr, 205, 200, GFXFF);// Print the string name of the font
 
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

  char str[10];
  char str2[10];

  if (measureState == 0 && measured == 0) {
    displayCm = distanceCm;
    displayIn = distanceInch;

    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setFreeFont(FF22);
    tft.drawString("       LIVE       ", 160, 50, GFXFF);// Print the string name of the font

  } else if (measureState == 1 && measured == 0) {
        delay(12);
        int i = 0;
        int error = 0;
        float measure, sum = 0.00, average, initial = 0.00;

          // Clears the trigPin
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          // Sets the trigPin on HIGH state for 10 micro seconds
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          
          // Reads the echoPin, returns the sound wave travel time in microseconds
          duration = pulseIn(echoPin, HIGH);
          
          // Calculate the distance
          distanceCm = duration * SOUND_SPEED/2;

          initial = distanceCm;
    
        while (i < nSample) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.setFreeFont(FF22);
            tft.drawString("MEASURING", 160, 50, GFXFF);// Print the string name of the font

          // Clears the trigPin
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          // Sets the trigPin on HIGH state for 10 micro seconds
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          
          // Reads the echoPin, returns the sound wave travel time in microseconds
          duration = pulseIn(echoPin, HIGH);
          
          // Calculate the distance
          distanceCm = duration * SOUND_SPEED/2;

          Serial.println("IN LOOP");
          Serial.println(initial);

          if (distanceCm < initial + 2 && distanceCm > initial - 2){

            sum += distanceCm;
            i++;
            
          } else {
            error++;
          }
          delay(12);
        if (error >= nSample/10){
          break;
        }
        }
    
        

        if (error >= nSample/10){
          displayCm = 1000;
          displayIn = 1000;
          tft.setTextColor(TFT_ORANGE, TFT_BLACK);
          tft.drawString("      MOVED      ", 160, 50, GFXFF);// Print the string name of the font
        } else {
          average = sum / nSample;
          printf("Average: %.2f\n", average);
          displayCm = average;
          displayIn = average * CM_TO_INCH;
          tft.setTextColor(TFT_BLUE, TFT_BLACK);
          tft.drawString("  MEASURED  ", 160, 50, GFXFF);// Print the string name of the font
        }
        

        measured = 1;
  }


  if (displayCm <10){

    sprintf(str, "00%1.2f", displayCm);

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(displayCm);
  }
  else if(displayCm <100){
    sprintf(str, "0%2.2f", displayCm);

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(displayCm);
  }
  else if(displayCm <350){
    sprintf(str, "%3.2f", displayCm);

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(displayCm);
  } else{
    sprintf(str, "   N/A   ", distanceCm);
  }

  if (displayIn <10){

    sprintf(str2, "00%1.2f", displayIn);

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(displayIn);
  }
  else if(displayIn <100){
    sprintf(str2, "0%2.2f", displayIn);

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(displayIn);
  }
  else if(displayCm <350){
    sprintf(str2, "%3.2f", displayIn);

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(displayIn);
  } else {
    sprintf(str2, "   N/A   ", displayIn);
  }

  

  Serial.println(str);
  Serial.println(str2);
  tft.setFreeFont(FF24);                 // Select the font
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(str, 128, 90, GFXFF);// Print the string name of the font
  tft.drawString(str2, 128, 140, GFXFF);// Print the string name of the font

  if (WiFi.status() != WL_CONNECTED){
    server.handleClient();
    
  }
  delay(12);
  
}

#ifndef LOAD_GLCD
//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif
