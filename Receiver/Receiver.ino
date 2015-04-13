//#include <Servo.h>
#include "EEPROM.h"
#include "cc1101.h"

#define RFCHANNEL 0
#define SYNCWORD1 0x64
#define SYNCWORD0 0x32
#define SOURCE_ADDR 2

#define LED_PIN 4

#define USE_SERIAL
#define USE_LED

CC1101 cc1101;

//Servo targetServo;

int currentPosition = 0;
int targetPosition = 0;
bool steerServoHelper = false;

void rfPacketReceived( void ) {

	CCPACKET packet;	
	detachInterrupt(0);

	#ifdef USE_LED
		digitalWrite( LED_PIN , HIGH );
	#endif

	if( cc1101.receiveData( &packet ) > 0 ) {
		
		if( packet.length > 1 ) {

			#ifdef USE_SERIAL
				Serial.print( "RECEIVED: " );
				Serial.println( packet.data[1] );
			#endif

			//targetPosition = packet.data[1];
			//steerServoHelper = true;

		}
	}

	//currentPosition = 0;
	//targetPosition = 180;
	//steerServoHelper = true;

	#ifdef USE_LED
		delay( 1000 );
		digitalWrite( LED_PIN , LOW );
	#endif

	attachInterrupt( 0 , rfPacketReceived , FALLING );
	
}

void setup() {

	#ifdef USE_SERIAL
		Serial.begin( 9600 );
		Serial.println( "LETS GO" );
		Serial.flush();
	#endif

	#ifdef USE_LED
		pinMode( LED_PIN , OUTPUT );
		digitalWrite( LED_PIN , HIGH );delay( 1000 );
		digitalWrite( LED_PIN , LOW );delay( 1000 );
		digitalWrite( LED_PIN , HIGH );delay( 1000 );
		digitalWrite( LED_PIN , LOW );
	#endif

	cc1101.init();
	cc1101.setChannel( RFCHANNEL , false );
	cc1101.setSyncWord( SYNCWORD1 , SYNCWORD0 , false );
	cc1101.setDevAddress( SOURCE_ADDR , false );
	cc1101.disableAddressCheck();

	attachInterrupt( 0 , rfPacketReceived , FALLING );
	
}

void loop() {
	if( steerServoHelper == true ) {
		//steerServo();
		attachInterrupt( 0 , rfPacketReceived , FALLING);
	}
}
/*
void steerServo( ) {
	//detachInterrupt(0);
	
	#ifdef USE_LED
		digitalWrite( LED_PIN , HIGH );
	#endif

	targetServo.attach( 9 );
	
	if( targetPosition > currentPosition  ) {
		for( int pos = currentPosition ; pos <= targetPosition ; pos++ ) {
			targetServo.write( pos );
			delay( 15 );
		}
	} else if( targetPosition < currentPosition ){
		for( int pos = currentPosition ; pos >= targetPosition ; pos-- ) {
			targetServo.write( pos );
			delay( 15 );
		}
	}
	currentPosition = targetPosition;
	
	
	targetServo.detach();

	steerServoHelper = false;
	
	#ifdef USE_LED
		delay( 500 );
		digitalWrite( LED_PIN , LOW );
	#endif
	
	//attachInterrupt(0, rfPacketReceived, FALLING);
}
*/
