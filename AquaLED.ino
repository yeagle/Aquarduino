#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6 // Which pin on the Arduino is connected to the NeoPixels
#define LED_COUNT  140 // How many NeoPixels are attached to the Arduino
#define BRIGHTNESS 255 // NeoPixel brightness, 0 (min) to 255 (max)

#define pass (void)0

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800); // Declare NeoPixel strip object

const unsigned long SECOND = 1000;
const unsigned long MINUTE = SECOND * 60;
const unsigned long HOUR = SECOND * 3600;
unsigned long ctime = 0;

void setColor(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setColorLow(uint32_t color) {
  int rnum = random(-5,5);
  for(int i=10; i<strip.numPixels(); i++) {
    if(i==(30+rnum) || i==(50+rnum) || i==(102-rnum) || i==(122-rnum)) { 
      strip.setPixelColor(i, color);
    }
    else {
      strip.setPixelColor(i, strip.Color(0,0,0,0));
    }
  }
  strip.show();
}

void daylight() {
  setColor(strip.Color(130, 70, 0, 150)); 
}

void moonlight() {
  setColorLow(strip.Color(3, 1, 70, 0));
}

void sundown(unsigned int sec_speed) {
  int r = 130;
  int g = 70;
  int b = 0;
  int w = 150;
  setColor(strip.Color(r,g,b,w)); 
  for(int i=120; i>0; i--) {
    g = g-i;
    g = max(g,0);
    b = b-i;
    b = max(b,0);
    w = w-i;
    setColor(strip.Color(r,g,b,w)); 
    strip.show();
    delay(sec_speed*SECOND);
  }
  g=0;
  b=0;
  for(int i=130; i>0; i--) {
    r = r-i;
    max(r,0);
    if (i % 2 != 0) {
      w = w-i;
      max(w,0);
    }
    setColor(strip.Color(r,g,b,w)); 
    strip.show();
    delay(sec_speed*SECOND);
  }
  setColor(strip.Color(0,0,0,0)); 
}

void colorTest() {
  setColor(strip.Color(255, 0, 0, 0)); // Red
  delay(500);
  setColor(strip.Color(0, 255, 0, 0)); // Green
  delay(500);
  setColor(strip.Color(0, 0, 255, 0)); // Blue
  delay(500);
  setColor(strip.Color(0, 0, 0, 255)); // White
  delay(500);
}

void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  // Serial communication
  Serial.begin(115200);

  // for random function
  randomSeed(analogRead(0));
}

void loop() {
  Serial.println("Loop start");
  ctime = millis();
  // 17:00
  Serial.println("17:00");
  colorTest();
  daylight();
  while(millis() < ctime+(3*HOUR+30*MINUTE)) pass;
  ctime = millis();
  // 20:30
  Serial.println("20:30");
  sundown(4);
  moonlight();
  while(millis() < ctime+(30*MINUTE)) pass;
  ctime = millis();
  while(millis() < ctime+(5*HOUR)) pass;
  ctime = millis();
  // 02:00
  Serial.println("02:00");
  setColor(strip.Color(0, 0, 0, 0)); // Off
  while(millis() < ctime+(3*HOUR)) pass;
  ctime = millis();
  // 05:00
  Serial.println("05:00");
  moonlight();
  while(millis() < ctime+(2*HOUR)) pass;
  ctime = millis();
  // 07:00
  Serial.println("07:00");
  daylight();
  while(millis() < ctime+(2*HOUR+30*MINUTE)) pass;
  ctime = millis();
  // 09:30
  Serial.println("09:30");
  setColor(strip.Color(0, 0, 0, 0)); // Off
  while(millis() < ctime+(2*HOUR)) pass;
  ctime = millis();
  // 11:30
  Serial.println("11:30");
  daylight();
  while(millis() < ctime+(2*HOUR)) pass;
  ctime = millis();
  // 13:30
  Serial.println("13:30");
  setColor(strip.Color(0, 0, 0, 0)); // Off
  while(millis() < ctime+(3*HOUR+30*MINUTE)) pass;
  ctime = millis();
}

