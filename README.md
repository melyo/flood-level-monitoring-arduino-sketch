This project is a rapid prototype designed to manage water level sensors and visualize their data in real time. It includes a web-based user interface that displays the current water level and sends warning notifications to clients when critical thresholds are reached.

This part of the project contains the Arduino sketch responsible for reading water level sensor data and transmitting it to the WebSocket server.

## Getting Started

First, open the Arduino IDE and ensure that the ESP8266 board package is properly installed. You can follow the detailed instructions here:

- [Arduino on ESP8266](https://github.com/esp8266/Arduino)

Next, create a new project and copy the provided code into the Arduino IDE. Be sure to modify all constant values to match your specific setup. Once done, upload the code to the ESP8266 microcontroller and run it with the connected sensor.
