


#define ON_Board_LED 2  //--> Defining an On Board LED (GPIO2 = D4), used for indicators when the process of connecting to a wifi router and when there is a client request.

unsigned long previousMillisGetHR = 0; //--> will store the last time Millis (to get Heartbeat) was updated.
unsigned long previousMillisHR = 0; //--> will store the last time Millis (to get BPM) was updated.

const long intervalGetHR = 10; //--> Interval for reading heart rate (Heartbeat) = 10ms.
const long intervalHR = 10000; //--> Interval for obtaining the BPM value based on the sample is 10 seconds.

const int PulseSensorHRWire = A0; //--> PulseSensor connected to ANALOG PIN 0 (A0 / ADC 0).
const int LED_D1 = D1; //--> LED to detect when the heart is beating. The LED is connected to PIN D1 (GPIO5) on the NodeMCU ESP12E.
int Threshold = 600; //--> Determine which Signal to "count as a beat" and which to ignore.

int cntHB = 0; //--> Variable for counting the number of heartbeats.
boolean ThresholdStat = true; //--> Variable for triggers in calculating heartbeats.
int BPMval = 0; //--> Variable to hold the result of heartbeats calculation.


//--------------------------------------------------------------------------------void GetHeartRate()
// This subroutine is for reading the heart rate and calculating it to get the BPM value.
// To get a BPM value based on a heart rate reading for 10 seconds.
void GetHeartRate() {
  //----------------------------------------Process of reading heart rate.
  unsigned long currentMillisGetHR = millis();

  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;

    int PulseSensorHRVal = analogRead(PulseSensorHRWire);

    if (PulseSensorHRVal > Threshold && ThresholdStat == true) {
      cntHB++;
      ThresholdStat = false;
      digitalWrite(ON_Board_LED,HIGH);
    }

    if (PulseSensorHRVal < Threshold) {
      ThresholdStat = true;
      digitalWrite(ON_Board_LED,LOW);
    }
  }
  //----------------------------------------

  //----------------------------------------The process for getting the BPM value.
  unsigned long currentMillisHR = millis();

  if (currentMillisHR - previousMillisHR >= intervalHR) {
    previousMillisHR = currentMillisHR;

    BPMval = cntHB * 6; //--> The taken heart rate is for 10 seconds. So to get the BPM value, the total heart rate in 10 seconds x 6.
    Serial.print("BPM : ");
    Serial.println(BPMval);
    
    cntHB = 0;
  }
  //----------------------------------------
}
//--------------------------------------------------------------------------------


// The SetUp Function:
void setup() {
  Serial.begin(115200);         // Set's up Serial Communication at certain speed.
  delay(500);
  pinMode(ON_Board_LED,OUTPUT);         // pin that will blink to your heartbeat!
  digitalWrite(ON_Board_LED, HIGH);
}

// The Main Loop Function
void loop() {
  GetHeartRate(); //--> Calling the GetHeartRate() subroutine
  
delay(10);


}
