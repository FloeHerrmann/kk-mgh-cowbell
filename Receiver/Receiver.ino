#define RFCHANNEL       0       // Let's use channel 0
#define SYNCWORD1        0x64    // Synchronization word, high byte
#define SYNCWORD0        0x32    // Synchronization word, low byte
#define SOURCE_ADDR      2       // Sender address

//#define USE_SERIAL

#define SERVO_PIN 9
#define MIN_ANGLE 28
#define MAX_ANGLE 140

#ifdef USE_SERIAL
	#include <HardwareSerial.h>
#endif

CCPACKET packet;

void rfPacketReceived(CCPACKET *packet) {   
	if (packet->length > 1) {   
		digitalWrite( LED , HIGH );
		#ifdef USE_SERIAL
			Serial.print( "RECEIVED -> " );
			Serial.println( packet->data[1] );
			Serial.flush();
		#endif
		int pos = packet->data[1];
		if( pos > MAX_ANGLE ) pos = MAX_ANGLE;
		else if( pos < MIN_ANGLE ) pos = MIN_ANGLE;
		servoPulse( pos );
		delay( 250 );
		digitalWrite( LED , LOW );
	}
}

void setup() {
	
	#ifdef USE_SERIAL
		Serial.begin( 9600 );
		Serial.print( "START" );
		Serial.flush();
	#endif
	
	pinMode(LED, OUTPUT);
	pinMode( SERVO_PIN , OUTPUT );
	digitalWrite( LED , HIGH );
	delay( 1000 );
	digitalWrite( LED , LOW );
	delay( 1000 );
	digitalWrite( LED , HIGH );
	delay( 1000 );
	digitalWrite( LED , LOW );
	
	panstamp.radio.setChannel(RFCHANNEL);
	panstamp.radio.setSyncWord(SYNCWORD1, SYNCWORD0);
	panstamp.radio.setDevAddress(SOURCE_ADDR);
	panstamp.radio.setCCregs();
	panstamp.radio.disableAddressCheck();
	panstamp.setPacketRxCallback(rfPacketReceived);
	
	#ifdef USE_SERIAL
		Serial.print( "READY" );
		Serial.flush();
	#endif
}

void loop() {	
}

void servoPulse( int angle) {
	int pwm = (angle*11) + 500;
	digitalWrite( 9 , HIGH );
	delayMicroseconds( pwm );
	digitalWrite( 9 , LOW );
	delay( 20 );
}