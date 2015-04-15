#include "EEPROM.h"
#include "cc1101.h"

#define RFCHANNEL 0
#define SYNCWORD1 0x64
#define SYNCWORD0 0x32
#define SOURCE_ADDR 1
#define DEST_ADDR 2

#define PIN04 4
#define PIN07 7

#define STATE_NEUTRAL 0
#define STATE_PIN04 1
#define STATE_PIN07 2

//#define USE_SERIAL

CC1101 cc1101;
byte currentState;

void setup() {

	pinMode( PIN04 , INPUT_PULLUP );
	pinMode( PIN07 , INPUT_PULLUP );

	#ifdef USE_SERIAL
		Serial.begin( 9600 );
		Serial.println( "LETS GO" );
		Serial.flush();
	#endif

	cc1101.init();
	cc1101.setChannel( RFCHANNEL , false );
	cc1101.setSyncWord( SYNCWORD1 , SYNCWORD0 , false );
	cc1101.setDevAddress( SOURCE_ADDR , false );
	cc1101.setTxPowerAmp( PA_LongDistance );

	currentState = STATE_NEUTRAL;
}

void loop() {

	byte state04 = digitalRead( PIN04 );
	byte state07 = digitalRead( PIN07 );

	if( state04 == LOW && currentState != STATE_PIN04 ) {
		delay( 100 );
		if( digitalRead( PIN04 ) == LOW ) {
			currentState = STATE_PIN04;
			sendPacket( 0 );
		}
	} else if( state07 == LOW && currentState != STATE_PIN07 ) {
		delay( 100 );
		if( digitalRead( PIN07 ) == LOW ) {
			currentState = STATE_PIN07;
			sendPacket( 180 );
		}
	} else if( state04 == HIGH && state07 == HIGH ) {
		delay( 100 );
		state04 = digitalRead( PIN04 );
		state07 = digitalRead( PIN07 );
		if( state04 == HIGH && state07 == HIGH ) {
			if( currentState == STATE_PIN04 || currentState == STATE_PIN07 ) {
				currentState = STATE_NEUTRAL;
			}
		}
	}
}

void sendPacket( int value ) {

	#ifdef USE_SERIAL
		Serial.print( "SEND " );
		Serial.println( value );
	#endif

	CCPACKET packet;
	packet.length = 2;
	packet.data[0] = DEST_ADDR;
	packet.data[1] = value;
	cc1101.sendData( packet );
}
