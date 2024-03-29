include <PulseSensorPlayground.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire of temp sensor is plugged into pin 2 on the Nodemcu
#define OneWireBus D7

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(OneWireBus);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float temperatureC;
float temperatureF;

String sendtemperatureC, sendtemperatureF, sendmyBPM, postData;

// Variables
#define PulseSensorPurplePin  A0 // Pulse Sensor data wire is connected to ANALOG PIN 0
#define REDLED D5 // The LED is connected to pin D7 on the board
#define GASSENSOR D6 // value for MQ135 gas sensor
int digitalValue;
int myBPM;
int Threshold = 550; // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define HOST "XXXXXXX"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "XXXXXXX"            // WIFI SSID here                                   
#define WIFI_PASSWORD "XXXXX"        // WIFI password here



void setup(){
    Serial.begin(115200); 
    Serial.println("Communication Started \n\n");  
    sensors.begin();
    
    Serial.begin(115200);
    Serial.println("DS18B20 Output!");
    sensors.begin();

    pinMode(GASSENSOR, INPUT);        // initialize the gas sensor
    pinMode(REDLED, OUTPUT);          // initialize the red LED
    pinMode(LED_BUILTIN, OUTPUT);     // initialize built in led on the board

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
    }
    delay(500);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    // Display static text
    display.println("WELCOME");
    display.display(); 

 
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
     
    // Double-check the "pulseSensor" object was created and "began" seeing a signal.
    Serial.println("We created a pulseSensor Object !"); //This prints one time at Arduino power-up, or on Arduino reset.
}

void loop(){

  digitalValue = digitalRead(GASSENSOR);    // get readings from the gas sensor
  Serial.print("gas reading is: ");
  Serial.print(digitalValue, DEC );
  Serial.println();
  delay(500);
  
  myBPM = analogRead(PulseSensorPurplePin); // get readings from pulse sensor
  myBPM = (myBPM/10);
  Serial.print("BPM is  ");
  Serial.print(myBPM);
  delay(1000);


   if(myBPM > Threshold || digitalValue > 0){                     // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
     digitalWrite(REDLED,LOW);
   } else {
     digitalWrite(REDLED,HIGH);                //  Else, the sigal must be below "550", so "turn-off" this LED.
   }
   delay(100);  // wait for the next 100ms before taking reaading agains
   
  
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
    Serial.println(myBPM);
    display.clearDisplay();
    display.setTextSize(1);             
    display.setTextColor(WHITE);        
    display.setCursor(0,5);             
    display.println("temp is ");
    display.setCursor(50,5);             
    display.println(temperatureC);
    display.setCursor(85,5);             
    display.println("C");
    
    display.setCursor(5,30);             
    display.println("pulse is");
    display.setCursor(70,30);             
    display.println(myBPM);
    display.display();
  }

  if (digitalValue > 0){
    display.setCursor(5,50);             
    display.println("Air not clean");
    display.setCursor(100,50);             
    display.println(digitalValue);
    display.display();
    }
  else{
    display.setCursor(5,50);             
    display.println("Air is clean");
    display.setCursor(100,50);             
    display.println(digitalValue);
    display.display();
  }
       
   // Convert integer variables to string
   sendtemperatureC = String(temperatureC);  
   sendtemperatureF = String(temperatureF);
   sendmyBPM = String(myBPM); 

 
   postData = "sendtemperatureC=" + sendtemperatureC + "&sendtemperatureF=" + sendtemperatureF + "&sendmyBPM=" + sendmyBPM ;

   // Update Host URL here:- 

   HTTPClient http;    // http object of clas HTTPClient
   WiFiClient wclient; // wclient object of clas HTTPClient 
  
   http.begin(wclient, "http://XXXXXXXXXX/dblwrite.php");              // Connect to host where MySQL databse is hosted
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
   delay(1000);
}
    
    
