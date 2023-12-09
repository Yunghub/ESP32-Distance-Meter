#Ultrasonic Distance Sensor using HC-SR04, ESP32, TFT 2.8 inches 320x240 display and Web Server
This project demonstrates how to use an ultrasonic distance sensor with an ESP32 board, a TFT 2.8 inches 320x240 display, and a web server that updates the distance. The ultrasonic sensor uses sonar to determine the distance to an object. We’ll show you how to wire the sensor to the ESP32 and provide several example sketches to determine the distance to an object using the HC-SR04.

Parts Required
To complete this project, you need the following parts:

HC-SR04 Ultrasonic Sensor
ESP32 board
TFT 2.8 inches 320x240 display
Breadboard
Jumper wires
Wiring the Ultrasonic Sensor HC-SR04 to the ESP32
The following table shows the pinout of the HC-SR04 Ultrasonic Sensor:


Pin	Description
VCC	Powers the sensor (5V)
Trig	Trigger Input Pin
Echo	Echo Output Pin
GND	Common GND
The ultrasonic sensor uses sonar to determine the distance to an object. Here’s how it works:

The ultrasound transmitter (trig pin) emits a high-frequency sound (40 kHz).
The sound travels through the air. If it finds an object, it bounces back to the module.
The ultrasound receiver (echo pin) receives the reflected sound (echo).
Taking into account the sound’s velocity in the air and the travel time (time passed since the transmission and reception of the signal) we can calculate the distance to an object.
Here’s the formula: distance to an object = ( (speed of sound in the air)*time)/2 speed of sound in the air at 20ºC (68ºF) = 343m/s

Getting Distance to an Object Using the Ultrasonic Sensor HC-SR04 with the ESP32
This guide shows how to use the HC-SR04 Ultrasonic Sensor with the ESP32 board using the Arduino core. The following example sketches demonstrate how to determine the distance to an object using the HC-SR04:

Ultrasonic Sensor HC-SR04 with Arduino IDE
Arduino library for HC-SR04 ultrasonic distance sensor
Displaying the Distance to an Object on a Display Using the ESP32 and HC-SR04
To display the distance to an object on a TFT 2.8 inches 320x240 display using the ESP32 and HC-SR04, you can use the following example sketch:

Using the ILI9341 TFT & XPT2046 Touch with ESP32 & Arduino
Web Server that Updates the Distance
To create a web server that updates the distance, you can use the following example sketch:

IoT Web Server Based Object Distance Meter using Ultrasonic
