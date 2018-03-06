# MHZ19B_uart 
Arduino IDE library for operating the MH-Z19B CO2 sensor in ESP-WROOM-02/32(ESP8266/ESP32) or Arduino  
version 0.3

# Credits and license  
License MIT

# How to use

* Include this library to your Arduino IDE.
* Wiring MH-Z19B sensor to your Arduino or ESP-WROOM-02/32(ESP8266/ESP32).

    MH-Z19B Vout to Arduino Vout(5V)  
    MH-Z19B GND  to Arduino GND  
    MH-Z19B Tx   to Arduino Digital Pin (Serial Rx pin)  
    MH-Z19B Rx   to Arduino Digital Pin (Serial Tx pin)  
    other MH-Z19B pins are not used.  

### CAUTION
* This lib does only support tje MH-Z19B uart/serie interface. PWM and Analog outputs are NOT supported.

# MHZ19B_uart library function

## Public functions

* void begin(int rx, int tx)  
  setting rx and tx pin, and initialize Software Serial.
  
* void setAutoCalibration(bool autocalib)  
  MH-Z19B has automatic calibration procedure. the MH-Z19B executing automatic calibration, its do zero point(stable gas environment (400ppm)) judgement.
  The automatic calibration cycle is every 24 hours after powered on.  
  If you use this sensor in door, you execute `setAutoCalibration(false)`.

* void calibrateZero()  
  execute zero point calibration. 
  if you want to execute zero point calibration, the MH-Z19B sensor must work in stable gas environment (400ppm) for over 20 minutes and you execute this function.

* void calibrateSpan(int ppm)  
  execute span point calibration.
  if you want to execute span point calibration, the MH-Z19B sensor must work in between 1000 to 2000ppm level co2 for over 20 minutes and you execute this function.
  
* int getPPM()  
  get co2 ppm.
  
* int getTemperature()  
  get temperature (MH-Z19B hidden function?  this function is not support.)

# link
* MH-Z19B Data sheet  
  http://www.winsen-sensor.com/d/files/PDF/Infrared%20Gas%20Sensor/NDIR%20CO2%20SENSOR/MH-Z19B%20CO2%20Ver1.0.pdf

* MH-Z19BB Data sheet  
  http://www.winsen-sensor.com/d/files/infrared-gas-sensor/mh-z19b-co2-ver1_0.pdf

# history
* ver. 0.1: initial version

