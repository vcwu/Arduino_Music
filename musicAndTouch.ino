/**
----------------------------------------------------------------
Play Melody by Touch
written by victoria wu
summer 2012

Cycle through a melody using a touch sensor. 
Uses CapSense libarry - http://www.arduino.cc/playground/Main/CapSense
----------------------------------------------------------------
*/

/*
--------------------------------------------
Note Frequencies taken from
http://www.phy.mtu.edu/~suits/notefreqs.html
*/

#define c4 261
#define d4 293
#define e4 329
#define f4 349
#define g4 392
#define a4 440
#define b4 493
#define c5 523

#include <CapSense.h>

/**
--------------------------------------------
pin definitions

Speaker-   speakerOut -> speaker -> ground
Capsense-  sendPin -> 10 megohm resistor -> foil -> receive pin 
*/

int speakerOut = 9;
int sendPin = 4;      
int receivePin = 2; 

/**
--------------------------------------------
init
*/

CapSense   cs_4_2 = CapSense(sendPin,receivePin);        
//int melody[] = {  c4, d4, e4, f4, g4, a4, b4, c5};  //c scale
int melody[] = {e4, d4, c4, d4, e4, e4, e4, d4, d4, d4, e4, g4, g4, 
                e4, d4, c4, d4, e4, e4, e4, e4, d4, d4, e4, d4, c4};  //mary had a little lamb
/*
int melody[] = {c4, c4, g4, g4, a4, a4, g4, f4, f4, e4, e4, d4, d4, c4, 
                g4, g4, f4, f4, e4, e4, d4, g4, g4, f4, f4, e4, e4, d4, 
                c4, c4, g4, g4, a4, a4, g4, f4, f4, e4, e4, d4, d4, c4};  //twinkle twinkle little star
*/

int melodySize =  sizeof(melody)/sizeof(int);
int counter = 0;  				//keep track of which note of melody
enum {NONE, INCR, TOUCH};		//State - no touch, letting go, touch
int state = NONE;				//Current state


//----------------------------------------------------------------

void setup()                    
{

   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   pinMode(speakerOut, OUTPUT);                  //set speakerOut pin to OUTPUT
   Serial.begin(9600);                           //use serial for debugging

}

void loop()                    
{
	
	long start = millis();	//get time since Arduino began running current program
  
	//Set the next state to Touch if a touch is sensed
	//200 is an arbitrary number, will depend on the CapSense sensor
    if(cs_4_2.capSense(30) > 200)	
        state = TOUCH;
  

    switch(state)
    {
		
		//TOUCH state - while you are touching sensor. 
		case (TOUCH):
		{
			int freq = melody[counter];	//Set freq to note in melody array
			tone(speakerOut, freq);		//Play note from speaker
			Serial.print("Freq ");
			Serial.print(freq);
			state = INCR; 				//Set next state to INCR
			break;
		}
		case(INCR):
		{
			if(counter < melodySize - 1)	//Set to play next note
				counter++;
			else
				counter = 0;
			Serial.print(" \t COUNTER: ");
			Serial.print(counter);
			state = NONE;
			break;
		}
		case (NONE):
		{
			noTone(speakerOut);			//Stop playing note from speaker
			Serial.print("Freq ");
			Serial.print(0); 
			Serial.print("\t ");
			break;
		}
    }

	Serial.print("\n");

    delay(10);                         	//arbitrary delay    	
}


/**
------------------------------------------------------
More Ideas
-Have pitch dependent on time of sensor contact
-Play random tones 
-Have CapSense sensor control pitch
------------------------------------------------------
*/


/*
Buzz function 
Buzzer example function for the CEM-1203 buzzer (Sparkfun's part #COM-07950).
by Rob Faludi
http://www.faludi.com
*/
void buzz(int targetPin, long frequency, long length) {
    long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
    
	//// 1 second's worth of microseconds, divided by the frequency, then split in half since
    //// there are two phases to each cycle
    long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
    
	//// multiply frequency, which is really cycles per second, by the number of seconds to 
    //// get the total number of cycles to produce
    for (long i=0; i < numCycles; i++){ 	// for the calculated length of time...
        digitalWrite(targetPin,HIGH); 		// write the buzzer pin high to push out the diaphram
        delayMicroseconds(delayValue); 		// wait for the calculated delay value
        digitalWrite(targetPin,LOW); 		// write the buzzer pin low to pull back the diaphram
        delayMicroseconds(delayValue); 		// wait again for the calculated delay value
    }
}


