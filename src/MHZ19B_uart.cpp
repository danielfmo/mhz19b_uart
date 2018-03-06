/*
  MHZ19B_uart.h - Library for MH-Z19B CO2 and temperature sensor
  version 1.0
  
  License MIT
*/

#include "MHZ19B_uart.h"
#include "Arduino.h"

#define WAIT_READ_TIMES	100
#define WAIT_READ_DELAY	10

//#define DEBUG_WRITE
//#define DEBUG_READ

MHZ19B_uart::MHZ19B_uart(){
}

MHZ19B_uart::~MHZ19B_uart(){
}

#ifdef ARDUINO_ARCH_ESP32
void MHZ19B_uart::begin(int rx, int tx, int s){
	_rx_pin = rx;
	_tx_pin = tx;
	_serialno = s;
  _co2_ppm = -1;
  _tempC = -1;
  _tempF = -1;  
}
#else
void MHZ19B_uart::begin(int rx, int tx){
	_rx_pin = rx;
	_tx_pin = tx;
  _co2_ppm = -1;
  _tempC = -1;
  _tempF = -1;    
}
#endif

void MHZ19B_uart::setAutoCalibration(boolean autocalib){
	writeCommand( autocalib ? autocalib_on : autocalib_off );
}

void MHZ19B_uart::calibrateZero() {
	writeCommand( zerocalib );
}

void MHZ19B_uart::calibrateSpan(int ppm) {
	if( ppm < 1000 )	return;

	uint8_t com[MHZ19B_uart::REQUEST_CNT];
	for(int i=0; i<MHZ19B_uart::REQUEST_CNT; i++) {
		com[i] = spancalib[i];
	}
	com[3] = (uint8_t)(ppm/256);
	com[4] = (uint8_t)(ppm%256);
	writeCommand( com );
}

int MHZ19B_uart::getPPM() {
	return _co2_ppm;
}

float MHZ19B_uart::getTemp() {
	return _tempC;
}

int MHZ19B_uart::getTempF() {
	return _tempF;
}

// protected
void MHZ19B_uart::writeCommand(uint8_t cmd[]) {
	writeCommand(cmd,NULL);
}

void MHZ19B_uart::writeCommand(uint8_t cmd[], uint8_t* response) {
#ifdef ARDUINO_ARCH_ESP32
	HardwareSerial hserial(_serialno);
	hserial.begin(9600, SERIAL_8N1, _rx_pin, _tx_pin);
#else
	SoftwareSerial hserial(_rx_pin, _tx_pin);
	hserial.begin(9600);
#endif
  
#ifdef DEBUG_WRITE 
  Serial.print("WRITE : ");
  char tmp[16];
  for (int i=0; i<REQUEST_CNT; i++) { 
   sprintf(tmp, "0x%.2X",cmd[i]); 
   Serial.print(tmp); Serial.print(" ");
  }
  sprintf(tmp, "0x%.2X", mhz19b_checksum(cmd)); 
  Serial.println(tmp); 
#endif  

  hserial.write(cmd, REQUEST_CNT);
	hserial.write(mhz19b_checksum(cmd));
	hserial.flush();
	
	if (response != NULL) {
		int i = 0;
		while(hserial.available() <= 0) {
			if( ++i > WAIT_READ_TIMES ) {
				Serial.println("ERROR: can't get MH-Z19B response.");
				return;
			}
			yield();
			delay(WAIT_READ_DELAY);
		}
		hserial.readBytes(response, MHZ19B_uart::RESPONSE_CNT);
	}
}

int MHZ19B_uart::readSensor() {
	uint8_t buf[MHZ19B_uart::RESPONSE_CNT];
	for( int i=0; i<MHZ19B_uart::RESPONSE_CNT; i++){
		buf[i]=0x0;
	}

	writeCommand(getppm, buf);

#ifdef DEBUG_READ 
  Serial.print("READ  : ");
  char tmp[16];
  for (int i=0; i<RESPONSE_CNT; i++) { 
   sprintf(tmp, "0x%.2X",buf[i]); 
   Serial.print(tmp); Serial.print(" ");
  }
  Serial.println();
#endif  
  
	// parse
	if (buf[0] == 0xff && buf[1] == 0x86 && mhz19b_checksum(buf) == buf[MHZ19B_uart::RESPONSE_CNT-1]) {
		_co2_ppm = buf[2] * 256 + buf[3];
		_tempF = buf[4];
    _tempC = (_tempF - 32.0 ) / 1.8;
    return 1;
	}
  return 0;
}	

// private
uint8_t MHZ19B_uart::mhz19b_checksum( uint8_t com[] ) {
	uint8_t sum = 0x00;
	for ( int i = 1; i < MHZ19B_uart::REQUEST_CNT; i++) {
		sum += com[i];
	}
	sum = 0xff - sum + 0x01;
	return sum;
}
