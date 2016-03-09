#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include "DHT.h"
#include <VirtualWire.h>  // you must download and install the VirtualWire.h to your hardware/libraries folder

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 5   // set these to be whatever pins you like!
#define TFT_MOSI 6   // set these to be whatever pins you like!
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
//char sensorPrintout[4];
int numbers[3];
void setup() {
  Serial.begin(9600);
  Serial.println("DHT test!");
  vw_set_ptt_inverted(true);    // Required for RX Link Module
 vw_setup(2000);                   // Bits per sec
 vw_set_rx_pin(3);           // We will be receiving on pin 23 (Mega) ie the RX pin from the module connects to this pin.
 vw_rx_start();                      // Start the receiver

 Serial.println("Receiver ready!");
  dht.begin();
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
}

void loop() {
 float h = dht.readHumidity();
  // Citire temperatura
  float t = dht.readTemperature();
 

  if (isnan(h) || isnan(t))  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.println();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(" Punctul de roua: ");
  Serial.print(dewPointFast(t, h));
  Serial.print(" *C ");

 uint8_t buf[VW_MAX_MESSAGE_LEN];
 uint8_t buflen = VW_MAX_MESSAGE_LEN;

 if (vw_get_message(buf, &buflen)) // check to see if anything has been received
    Serial.println(" Pachete primite!");
 for (int i = 0; i < 3; i++)
     numbers[i] = word(buf[i*2+1], buf[i*2]);
     Serial.println();
     Serial.print("numbers[0]=");
     Serial.print(numbers[0]);
     Serial.println();
     Serial.print("numbers[1]=");
     Serial.print(numbers[1]);
     Serial.println();
     Serial.print("numbers[2]=");
     Serial.print(numbers[2]);
     Serial.println();
     
    tft.fillScreen(ST7735_BLACK);
    testfastlines(ST7735_RED, ST7735_BLUE);    
  tft.setCursor(70, 10);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.print("T ext:");
  //temperatura interior
  tft.setCursor(70, 20);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.print(numbers[0]);
  /////////// simbol
   tft.setCursor(105, 15);
  tft.setTextSize(2);
  tft.print("o");
  tft.setCursor(115, 20);
  tft.setTextSize(2);
  tft.print("C");
     
    
      // Print out the measurement:
// tft.setRotation(tft.getRotation()+1);
  //text
  tft.setCursor(5, 10);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.print("T int:");
  //temperatura interior
  tft.setCursor(10, 20);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.print(t, 0);
  /////////// simbol
   tft.setCursor(40, 15);
  tft.setTextSize(2);
  tft.print("o");
  tft.setCursor(50, 20);
  tft.setTextSize(2);
  tft.print("C");


  tft.setCursor(5, 40);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.print("RH int:");
  //umiditate interior
  tft.setCursor(10, 50);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.print(h, 0);
  /////////// simbol
  tft.setCursor(45, 50);
  tft.setTextSize(2);
  tft.print("%");

tft.setCursor(70, 40);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.print("Presiune:");
  //temperatura interior
  tft.setCursor(70, 50);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.print(numbers[1]);
  /////////// simbol
   tft.setCursor(105, 50);
  tft.setTextSize(1);
  tft.print("mm");
  tft.setCursor(105, 58);
  tft.print("HG");

  tft.setCursor(5, 65);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.print("Pct roua:");
  //PUNCT roua
  tft.setCursor(10, 75);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.print(dewPointFast(t, h), 0);
   /////////// simbol
   tft.setCursor(35, 70);
  tft.setTextSize(2);
  tft.print("o");
  tft.setCursor(45, 75);
  tft.setTextSize(2);
  tft.print("C");

if(numbers[2]<12)
{tft.fillScreen(ST7735_WHITE);
  tft.setCursor (30,50);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(4);
   tft.print("LOW:");
}

else{
   tft.setCursor(70, 65);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.print("Baterie:");
  //PUNCT roua
  tft.setCursor(70, 75);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.print(numbers[2]);
};
  
 delay(10000);
 
}







void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

double dewPointFast(double celsius, double humidity)
{
 double a = 17.271;
 double b = 237.7;
 double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
 double Td = (b * temp) / (a - temp);
 return Td;
}
