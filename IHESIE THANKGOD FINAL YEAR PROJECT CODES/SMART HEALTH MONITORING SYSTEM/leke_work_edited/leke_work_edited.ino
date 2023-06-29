#include <PulseSensorPlayground.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into Nodemcu pin D5
#define OneWireBus D5

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(OneWireBus);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float temperatureC;
float temperatureF;

String sendtemperatureC, sendtemperatureF, sendmyBPM, postData;

// Variables
#define PulseSensorPurplePin A0 // PulseSensor data wire is connected to ANALOG PIN 0
#define RLED D6 // Assigning the red LED to D6
int rawBPM;
int myBPM;
int Threshold = 60 ; 

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define HOST "youngancient.000webhostapp.com"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "MTN_4G_2C6CA7"            // WIFI SSID here                                   
#define WIFI_PASSWORD "B9F285E7"        // WIFI password here


void setup(){
    Serial.begin(115200); 
    Serial.println("Communication Started \n\n");  
    sensors.begin();
    
    Serial.begin(115200);
    Serial.println("DS18B20 Output!");
    sensors.begin();

    pinMode(LED_BUILTIN, OUTPUT);     // initialize built in led on the Nodemcu board
    pinMode(RLED, OUTPUT);     // initialize built in led on the Nodemcu board

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
    delay(500);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5, 5);
    // Display static text
    display.println("WELCOME !");
    display.display(); 
 
    WiFi.mode(WIFI_STA);           
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    display.clearDisplay();
    display.setCursor(5, 20);
    display.println("Connecting to");
    display.setCursor(10, 30);
    display.println(WIFI_SSID);
    display.display(); 
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
    
    // Double-check the "pulseSensor" object was created and "began" seeing a signal.
    Serial.println("We created a pulseSensor Object !"); //This prints one time at Arduino power-up, or on Arduino reset.        
}

void loop(){

  int rawBPM = analogRead(PulseSensorPurplePin);
  myBPM = rawBPM/9 ;
  delay(20);

  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);

   if(isnan(temperatureC) || isnan(temperatureF)){
    Serial.println("Failed to read DS18B20");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Failed to read DS18B20");
    display.display();
    display.clearDisplay();
  }
  else{
    Serial.print("Temperature in Celcius is ");
    Serial.print(temperatureC);
    Serial.print(" *C ");
    Serial.print(" Temperature in Fahrenheit is ");
    Serial.print(temperatureF);
    Serial.println(" *F");
    Serial.println(myBPM);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5,2);
    display.println("Temp in C is");
    display.setCursor(85,2);
    display.println(temperatureC);

   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(5,20);
   display.println("Your BPM is ");
   display.setCursor(85,20);
   display.println(myBPM);
   display.display();

   while(myBPM > Threshold || temperatureC > 37 )
   {            // If pulse is below "60", then "turn-on" Arduino's on-Board LED.
     digitalWrite(RLED,LOW); 
   }

   delay(1000);
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
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(2,40);
      display.println("Values uploaded successfully.");
      display.display();

      String webpage = http.getString();    // Get html webpage output and store it in a string
      Serial.println(webpage + "\n");
      delay(2000);
      }

   // if failed to connect then return and restart

   else {
     Serial.println(httpCode); 
     Serial.println("Failed to upload values. \n");
     display.setTextSize(1);
     display.setTextColor(WHITE);
     display.setCursor(2,40);
     display.println("Failed to upload values.");
     display.display();
     delay(2000);
     http.end(); 
     return; 

     }


   delay(1000); 
   digitalWrite(LED_BUILTIN, LOW);
   delay(1000);
   digitalWrite(LED_BUILTIN, HIGH);

   temperatureC+=1; temperatureF+=10;  // Incrementing value of variables 
   delay(500);
}
    
    
