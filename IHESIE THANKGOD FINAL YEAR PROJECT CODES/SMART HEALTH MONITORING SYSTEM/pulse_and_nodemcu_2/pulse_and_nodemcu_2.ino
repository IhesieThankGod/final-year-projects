int UpperThreshold = 518;
int LowerThreshold = 490;
int reading = 0;
float myBPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;
const unsigned long delayTime = 10;
const unsigned long delayTime2 = 1000;
const unsigned long baudRate = 115200;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into pin 2 on the Arduino
const int OneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(OneWireBus);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float temperatureC;
float temperatureF;

String sendtemperatureC, sendtemperatureF, sendmyBPM, postData;

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define HOST "youngancient.000webhostapp.com"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "Galaxy XCover 4sCA50"            // WIFI SSID here                                   
#define WIFI_PASSWORD "gggggggg"        // WIFI password here

int LED13 = 2;// The on-board Arduino LED, close to PIN 13.

void setup(){
  Serial.begin(baudRate);
  pinMode(LED13, OUTPUT);
  Serial.println("Communication Started \n\n");  
  sensors.begin();
  Serial.println("DS18B20 Output!");
  digitalWrite(LED13, LOW);

  WiFi.mode(WIFI_STA);           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(500);
   }

   Serial.println();
   Serial.print("Connected to ");
   Serial.println(WIFI_SSID);
   Serial.print("IP Address is : ");
   Serial.println(WiFi.localIP());    //print local IP address

   delay(30);
   
}

void loop(){

  // Get current time
  unsigned long currentMillis = millis();

  // First event
  if(myTimer1(delayTime, currentMillis) == 1){

    reading = analogRead(0); 

    // Heart beat leading edge detected.
    if(reading > UpperThreshold && IgnoreReading == false){
      if(FirstPulseDetected == false){
        FirstPulseTime = millis();
        FirstPulseDetected = true;
      }
      else{
        SecondPulseTime = millis();
        PulseInterval = SecondPulseTime - FirstPulseTime;
        FirstPulseTime = SecondPulseTime;
      }
      IgnoreReading = true;
      digitalWrite(LED13, LOW);
    }

    // Heart beat trailing edge detected.
    if(reading < LowerThreshold && IgnoreReading == true){
      IgnoreReading = false;
      digitalWrite(LED13, HIGH);
    }  

    // Calculate Beats Per Minute.
    myBPM = (1.0/PulseInterval) * 60.0 * 1000;
  }

  // Second event
  if(myTimer2(delayTime2, currentMillis) == 1){
    Serial.print(reading);
    Serial.print("\t");
    Serial.print(PulseInterval);
    Serial.print("\t");
    Serial.print(myBPM);
    Serial.println(" BPM");
    Serial.flush();
  }

  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);  
  
  if(isnan(temperatureC) || isnan(temperatureF)){
    Serial.println("Failed to read DS18B20");
  }
  else{
    Serial.print("Temperature in Celcius is ");
    Serial.print(temperatureC);
    Serial.print(" *C ");
    Serial.print(" Temperature in Fahrenheit is ");
    Serial.print(temperatureF);
    Serial.println(" *F");
  }

   // Convert integer variables to string
   sendtemperatureC = String(temperatureC);  
   sendtemperatureF = String(temperatureF);
   sendmyBPM = String(myBPM); 

 
   postData = "sendtemperatureC=" + sendtemperatureC + "&sendtemperatureF=" + sendtemperatureF + "&sendmyBPM=" + sendmyBPM ;

   // We can post values to PHP files as  example.com/dbwrite.php?name1=val1&name2=val2&name3=val3
   // Hence created variable postDAta and stored our variables in it in desired format
   // For more detials, refer:- https://www.tutorialspoint.com/php/php_get_post.htm

   // Update Host URL here:- 

   HTTPClient http;    // http object of clas HTTPClient
   WiFiClient wclient; // wclient object of clas HTTPClient 
  
   http.begin(wclient, "http://youngancient.000webhostapp.com/dbwrite.php");              // Connect to host where MySQL databse is hosted
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header

  
 
   int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
   Serial.println("Temperature Values are in Celcius = " + sendtemperatureC + " *C" + " and in Fahrenheit = "+sendtemperatureF+ " *K" + " Pulse rate is "+ sendmyBPM + " BPM");


   // if connection eatablished then do this
   if (httpCode == 200){
      Serial.println("Values uploaded successfully."); Serial.println(httpCode); 
      String webpage = http.getString();    // Get html webpage output and store it in a string
      Serial.println(webpage + "\n");
      }

   // if failed to connect then return and restart

   else {
     Serial.println(httpCode); 
     Serial.println("Failed to upload values. \n"); 
     http.end(); 
     return;
     }


   delay(4000); 

   temperatureC+=1; temperatureF+=10; // Incrementing value of variables 
   delay(500);
}

// First event timer
int myTimer1(long delayTime, long currentMillis){
  if(currentMillis - previousMillis >= delayTime){previousMillis = currentMillis;return 1;}
  else{return 0;}
}

// Second event timer
int myTimer2(long delayTime2, long currentMillis){
  if(currentMillis - previousMillis2 >= delayTime2){previousMillis2 = currentMillis;return 1;}
  else{return 0;}
}
