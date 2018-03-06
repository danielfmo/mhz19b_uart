/*
  MHZ19B_uart.h - Library for MH-Z19B CO2 and temperature sensor
  version 1.0
  
  License MIT
*/

#ifndef MHZ19B_uart_h_
#define MHZ19B_uart_h_

#include "Arduino.h"
#ifdef ARDUINO_ARCH_ESP32
	#include "HardwareSerial.h"
#else
	#include "SoftwareSerial.h"
#endif

class MHZ19B_uart
{
public:
	MHZ19B_uart();
	virtual ~MHZ19B_uart();

#ifdef ARDUINO_ARCH_ESP32
	void begin(int rx=-1, int tx=-1, int s=1);
#else
	void begin(int rx=-1, int tx=-1);
#endif

	void setAutoCalibration(boolean autocalib);
	void calibrateZero();
	void calibrateSpan(int ppm);
	
  int readSensor();
	int getPPM();
	float getTemp();
	int getTempF();

protected:
	void writeCommand(uint8_t com[]);
	void writeCommand(uint8_t com[], uint8_t response[]);

private:
	uint8_t mhz19b_checksum( uint8_t com[] );

	static const int REQUEST_CNT  = 8;
	static const int RESPONSE_CNT = 9;
  
  int _co2_ppm;
  float _tempC;
  int _tempF;
  
	int _rx_pin = -1;
	int _tx_pin = -1;
#ifdef ARDUINO_ARCH_ESP32
	int _serialno = 1;
#endif

	// serial commands 
	uint8_t getppm[REQUEST_CNT]			    = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t zerocalib[REQUEST_CNT]	 	  = {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t spancalib[REQUEST_CNT]	 	  = {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t autocalib_on[REQUEST_CNT] 	= {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00};
	uint8_t autocalib_off[REQUEST_CNT]	= {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};

};

#endif
