
/*  PulseSensor Starter Project and Signal Tester
 *  The Best Way to Get Started  With, or See the Raw Signal of, your PulseSensor.com™ & Arduino.
 *
 *  Here is a link to the tutorial
 *  https://pulsesensor.com/pages/code-and-guide
 *
 *  WATCH ME (Tutorial Video):
 *  https://www.youtube.com/watch?v=RbB8NSRa5X4
 *
 *
-------------------------------------------------------------
1) This shows a live human Heartbeat Pulse.
2) Live visualization in Arduino's Cool "Serial Plotter".
3) Blink an LED on each Heartbeat.
4) This is the direct Pulse Sensor's Signal.
5) A great first-step in troubleshooting your circuit and connections.
6) "Human-readable" code that is newbie friendly."

*/
int x = 0;
int LastTime = 0;
bool BPMTiming = false;
bool BeatComplete = false;
int BPM = 0;    
#define UpperThreshold 518
#define LowerThreshold 490    
int Signal; // holds the incoming raw data. Signal value can range from 0-1024

//  Variables
int PulseSensorPurplePin = 0;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int LED = 2;   //  The on-board Arduion LED


// The SetUp Function:
void setup() {
  pinMode(LED,OUTPUT);         // pin that will blink to your heartbeat!
   Serial.begin(9600);         // Set's up Serial Communication at certain speed.

}

// The Main Loop Function
void loop() {

  int value = analogRead(0);
  if (value > UpperThreshold) {
    if (BeatComplete) {
      BPM = millis() - LastTime;
      BPM = int(60 / (float(BPM) / 1000));
      BPMTiming = false;
      BeatComplete = false;
    }
    if (BPMTiming == false) {
      LastTime = millis();
      BPMTiming = true;
    }
  }
  if ((value < LowerThreshold) & (BPMTiming))
    BeatComplete = true;
    // display bpm
  Serial.print(BPM);
  Serial.println(" BPM");
  x++;
  Signal = analogRead(0); // Read the PulseSensor's value.
  // Assign this value to the "Signal" variable.
  if (Signal > UpperThreshold) {
    // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH); //  Else, the signal must be below "550", so "turn-off" this LED.
  }

  delay(100);

}
