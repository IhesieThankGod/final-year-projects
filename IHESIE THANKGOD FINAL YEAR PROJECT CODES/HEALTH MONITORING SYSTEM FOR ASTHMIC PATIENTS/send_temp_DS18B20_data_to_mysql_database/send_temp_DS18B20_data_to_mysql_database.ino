#include <PulseSensorPlayground.h>

#include <LiquidCrystal_I2C.h>

#include <LiquidCrystal_I2C.h>


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

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);




// Variables
const int PulseSensorPurplePin = 0; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int LED13 = 2;// The on-board Arduino LED, close to PIN 13.
int myBPM;
int Threshold = 550; // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.
 

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define HOST "youngancient.000webhostapp.com"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "Galaxy XCover 4sCA50"            // WIFI SSID here                                   
#define WIFI_PASSWORD "gggggggg"        // WIFI password here


void setup(){
    Serial.begin(115200); 
    Serial.println("Communication Started \n\n");  
    sensors.begin();
    
    Serial.begin(115200);
    Serial.println("DS18B20 Output!");
    sensors.begin();

    pinMode(LED13, OUTPUT);     // initialize built in led on the board

    
    Wire.begin(2,0);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("welcome");
 
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

    // Configure the PulseSensor object, by assigning our variables to it.
    pinMode(LED13,OUTPUT); 
    
    // Double-check the "pulseSensor" object was created and "began" seeing a signal.
    Serial.println("We created a pulseSensor Object !"); //This prints one time at Arduino power-up, or on Arduino reset.
    lcd.setCursor(0,0);
    lcd.print("Heart Rate Monitor");
        
}

void loop(){
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
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("temp is");
    lcd.setCursor(2,1);
    lcd.print(temperatureC);   
  }
    
   myBPM = analogRead(PulseSensorPurplePin);
   Serial.println(myBPM);

   if(myBPM > Threshold){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
     digitalWrite(LED13,LOW);
   } else {
     digitalWrite(LED13,HIGH);                //  Else, the sigal must be below "550", so "turn-off" this LED.
   }
   
   delay(10);
   
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


   delay(3000); 
   digitalWrite(LED_BUILTIN, LOW);
   delay(3000);
   digitalWrite(LED_BUILTIN, HIGH);

   temperatureC+=1; temperatureF+=10; // Incrementing value of variables 
   delay(500);
}
    
    
