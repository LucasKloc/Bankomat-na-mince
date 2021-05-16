#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Declare LCD object for software SPI
// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(12, 11, 10, 9, 8);

void setup()   {
  Serial.begin(9600);

  //Initialize Display
  display.begin();

  // you can change the contrast around to adapt the display for the best viewing!
  display.setContrast(65);

  // Clear the buffer.
  display.clearDisplay();

  // Display Text
  display.setTextSize(2);             //nastaví velikost textu (nejlepší je 1 nebo 2) - 3 moc velká
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello world!");    //VYPÍŠE TEXT
  display.display();
  delay(2000);
  display.clearDisplay();


  // Display Numbers
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(123456789);     //vypisuje čísla
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display ASCII Characters   
  display.setCursor(0,0);
  display.setTextSize(2);
  display.write(3);        //SRDÍČKO
  display.display();
  delay(2000);
  display.clearDisplay();

}

void loop() {
  }
