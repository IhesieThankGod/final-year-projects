#include <SoftwareSerial.h>
SoftwareSerial SIM900A(10,11);

const int TRIG_PIN = 12;
const int ECHO_PIN = 13;
 
 long duration, distanceCm, distanceIn;
 
void loop()
{
  long duration, distanceCm, distanceIn;
 
  
}


void setup()
{
  SIM900A.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println ("SIM900A Ready");
  delay(100);
  Serial.println ("Type s to send message or r to receive message");
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
}
void loop()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
 
  // convert the time into a distance
  distanceCm = duration / 29.1 / 2 ;
  distanceIn = duration / 74 / 2;
 
  if (distanceCm <= 7)
  {
    Serial.println("TOO CLOSE");
    SendMessage();
  }
  else 
  {
    Serial.print(distanceIn);
    Serial.print("in: ");
    Serial.print(distanceCm);
    Serial.print("cm");
    Serial.println();
  }
  delay(1000);
  
}
 
 void SendMessage()
{
  Serial.println ("Sending Message");
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+9067120120\"\r"); //Mobile phone number to send message
  delay(1000);
  Serial.println ("Set SMS Content");
  SIM900A.println("Good morning, how are you doing?");// Messsage content
  delay(100);
  Serial.println ("Finish");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println ("Message has been sent ->SMS Selesai dikirim");
}
 
