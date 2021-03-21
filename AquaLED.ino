#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6 // Which pin on the Arduino is connected to the NeoPixels
#define LED_COUNT  239 //Benny // How many NeoPixels are attached to the Arduino
//#define LED_COUNT  140 // How many NeoPixels are attached to the Arduino
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
  int rnum = random(-3,3);
  for(int i=10; i<strip.numPixels(); i++) {
    if(i==(30+rnum) || i==(45+rnum) || i==(92-rnum) || i==(107-rnum) || i==(122-rnum)) { 
      strip.setPixelColor(i, color);
    }
    else {
      strip.setPixelColor(i, strip.Color(0,0,0,0));
    }
  }
  strip.show();
}

void setColorLowBenny(uint32_t color) {
  int rnum = random(-3,3);
  for(int i=0; i<strip.numPixels(); i++) {
    if(i % 5 == 0) { 
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

void lightBenny() {
  setColor(strip.Color(20,20,20,200)); 
}

void moonlight() {
  setColorLow(strip.Color(3, 1, 70, 0));
}
void moonlightBenny() {
  setColorLowBenny(strip.Color(3, 1, 70, 0));
}

void sundown(unsigned int sec_speed) {
  int r = 130;
  int g = 70;
  int b = 0;
  int w = 150;
  //setColor(strip.Color(r,g,b,w)); 
  for(int i=120; i>0; i--) {
    if (g>0) g = g-1;
    if (b>0) b = b-1;
    if (w>0) w = w-1;
    setColor(strip.Color(r,g,b,w)); 
    strip.show();
    delay(sec_speed*SECOND);
  }
  g=0;
  b=0;
  for(int i=0; i<130; i++) {
    if (r>0) r = r-1;
    if (i % 3 == 0) {
      if (w>0) w = w-1;
    }
    if (i <= 60) {
      for(int j=i/2; j<(strip.numPixels()/2-i/2); j++) {
        strip.setPixelColor(j, strip.Color(r,g,b,w));
      }
      for(int j=strip.numPixels()/2+i/2; j<(strip.numPixels()-i/2); j++) {
        strip.setPixelColor(j, strip.Color(r,g,b,w));
      }
      for(int j=0; j<i/2; j++) {
        strip.setPixelColor(j, strip.Color(0,0,0,0));
        strip.setPixelColor((strip.numPixels()/2-j), strip.Color(0,0,0,0));
        strip.setPixelColor((strip.numPixels()/2+j), strip.Color(0,0,0,0));
        strip.setPixelColor((strip.numPixels()-j), strip.Color(0,0,0,0));
      }
    }
    else {
      for(int j=60/2; j<=(strip.numPixels()/2-60/2); j++) {
        strip.setPixelColor(j, strip.Color(r,g,b,w));
        strip.setPixelColor((strip.numPixels()/2+j), strip.Color(r,g,b,w));
      }
    }
    strip.show();
    delay(sec_speed*SECOND);
  }
  setColor(strip.Color(0,0,0,0)); 
  strip.show();
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
  Serial.begin(9600);

  // for random function
  randomSeed(analogRead(0));
}

void timedRoutine () {
  Serial.println("Start of timed routine");
  ctime = millis();
  // 17:00
  Serial.println("17:00");
  colorTest();
  daylight();
  while(millis() < ctime+(3*HOUR+30*MINUTE)) pass;
  ctime = millis();
  // 20:30
  Serial.println("20:30");
  sundown(5);
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

void timedRoutineBenny () {
  Serial.println("Start of timed routine");
  ctime = millis();
  // 17:00
  Serial.println("15:00");
  colorTest();
  lightBenny();
  while(millis() < ctime+(10*HOUR)) pass;
  ctime = millis();
  // 01:00
  Serial.println("01:00");
  sundown(5);
  moonlightBenny();
  while(millis() < ctime+(1*HOUR)) pass;
  ctime = millis();
  // 02:00
  Serial.println("02:00");
  setColor(strip.Color(0, 0, 0, 0)); // Off
  while(millis() < ctime+(13*HOUR)) pass;
  ctime = millis();
  // 15:00
}

void loop() {
  Serial.println("Loop start");
  //timedRoutine();
  timedRoutineBenny();
  //sundown(0);
}

