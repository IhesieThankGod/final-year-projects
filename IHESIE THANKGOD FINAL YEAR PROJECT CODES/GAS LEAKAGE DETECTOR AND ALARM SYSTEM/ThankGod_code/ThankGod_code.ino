#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <AltSoftSerial.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Choose two Arduino pins to use for software serial
SoftwareSerial mySerial(10, 11);


// Create a TinyGPS++ object
TinyGPSPlus gps;

AltSoftSerial altSerial;

int redLed = 6;
int greenLed = 5;
int smokeA0 = A0;
int sensorThres = 550; // the threshold value that indicates a gas leak
int buzzer = 7;

void setup() {

  lcd.begin(16, 2);

  // Turn on the blacklight and print a message.
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Hello, world!");

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(smokeA0, INPUT);
  pinMode(buzzer, OUTPUT);
   
  // Start the software serial port at the GPS's default baud
  altSerial.begin(9600);

  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  
  // Start the software serial port for the gsm module
  mySerial.begin(9600);
  // Start the Arduino hardware serial port at 9600 baud
  Serial.println ("SIM900A Ready");
  delay(100);

}

void loop() {
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gas conc is ");
  lcd.setCursor(12,0);
  lcd.print(analogSensor);


  if(analogSensor > sensorThres)
  {
    while (altSerial.available() > 0)
    if (gps.encode(altSerial.read()))
    SendMessage2();
    SendMessage();
    displayInfo();
     
  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
  
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    Serial.print("Smoke detected");
    tone(buzzer, 5000); 
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    noTone(buzzer);
  }

  delay(2000);
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    lcd.setCursor(0,1);
    lcd.print("Danger! gas leakage");

    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  
  }
  else
  {
    Serial.println("Location: Not Available");
  }
}

void SendMessage()
{
  Serial.println ("Sending Message");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);  // Delay of 1000 milli seconds or 1 second
  Serial.println ("Set SMS Number");
  mySerial.println("AT+CMGS=\"ZZZ\"\r"); // Replace x with mobile number
  delay(200);
  mySerial.println("Danger! gas leak detected, latitude and longitude of gas leak location");// The SMS text you want to send
  mySerial.println(gps.location.lat(), 6);
  mySerial.println(gps.location.lng(), 6);
  delay(200);
  Serial.println ("Finish");
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(500);
  Serial.println ("Message has been sent ->SMS young ancient");
  delay(1000);
}


void SendMessage2()
{
  Serial.println ("Sending Message");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);  // Delay of 1000 milli seconds or 1 second
  Serial.println ("Set SMS Number");
  mySerial.println("AT+CMGS=\"ZZZ\"\r"); // Replace ZZZ with mobile number
  delay(200);
  mySerial.println("Danger! Gas leak detected");// The SMS text you want to send
  delay(200);
  Serial.println ("Finish");
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(500);
  Serial.println ("Message has been sent ->SMS young ancient");
  delay(1000);
}
  
