#include <SoftwareSerial.h>
SoftwareSerial SIM900A(10,11);

const int TRIG_PIN = 12;
const int ECHO_PIN = 13;
int LED = 6;
int buzzer = 3;
int LDRpin = A0;

int LDRValue = 0;

long duration, distanceCm, distanceIn;
 
void setup()
{
  SIM900A.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println ("SIM900A Ready");
  delay(100);
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(LDRpin, INPUT);
  pinMode(buzzer,OUTPUT);
  Serial.println ("Motion sensor ready");
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

  LDRValue = analogRead(LDRpin);
  Serial.println(LDRValue);
  delay(100);
   
  if (distanceCm <= 10)
  {
    Serial.println("TOO CLOSE");
    Serial.print(distanceCm);
    Serial.print("cm");
    Serial.println();
    tone(buzzer,5000);
    if(LDRValue < 200)
    {
      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      delay(200);
      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      delay(200);
      digitalWrite(LED, HIGH);
      delay(200); 
      digitalWrite(LED, LOW);
      delay(200);
      digitalWrite(LED, HIGH);
      delay(200); 
      } 
    SendMessage();  
  }
  else 
  {
    digitalWrite(LED, LOW);
    noTone(buzzer);
    Serial.print(distanceIn);
    Serial.print(" in: ");
    Serial.print(distanceCm);
    Serial.print("cm");
    Serial.println();
  }
  delay(4000);
  
}
 
 void SendMessage()
{
  Serial.println ("Sending Message");
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+2349018932591\"\r"); //Mobile phone number to send message
  delay(1000);
  Serial.println ("Set SMS Content");
  SIM900A.println("Animal in the farm");// Messsage content
  delay(100);
  Serial.println ("Finish");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println ("Message has been sent ->SMS Selesai dikirim");
}
 
