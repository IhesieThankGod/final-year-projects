#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

// sensor pins
#define sensorPower 7
#define sensorPin A0

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,2);
  display.println("Hello,");
  display.setCursor(0,25);
  display.println("Horticulturist");
  display.display();
  display.clearDisplay();
  delay(2000);

  pinMode(sensorPower, OUTPUT);

  // Initially keep the sensor off
  digitalWrite(sensorPower, LOW);

}

void loop() {

    // get the reading from the function below and print it
    Serial.print("Analog output: ");
    Serial.println(readSensor());
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(2,3);
    display.println("Moisture content is ");
    display.println(readSensor());
    display.display();

    delay(1000);
}

// this function returns the analog soil moistur measurement
int readSensor() {
  digitalWrite(sensorPower, HIGH);    // Turn the sensnor ON
  delay(10);                          // Allow power to settle
  int val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);     // Turn the sensor OFF

  if (val < 500){
      display.setCursor(5,15);
      display.println(val);
      display.setCursor(5,40);
      display.println("soil is too wet ");
      display.display();
    }
    else if(val>= 500 && val < 751){
      display.setCursor(5,15);
      display.println(val);
      display.setCursor(5,40);
      display.println("soil moisture is     okay and within        acceptable range");
      display.display();
      }
    else {
      display.setCursor(5,15);
      display.println(val);
      display.setCursor(5,40);
      display.println("Soil is dry enough  to be watered ");
      display.display();
    }
  return 0 ;                         // Return the analog moisture value
}
