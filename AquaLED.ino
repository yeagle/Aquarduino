#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN1    6 // Which pin on the Arduino is connected to the NeoPixels
#define LED_PIN2    9 // Which pin on the Arduino is connected to the NeoPixels
#define LED_COUNT  120 // How many NeoPixels are attached to the Arduino
#define BRIGHTNESS 255 // NeoPixel brightness, 0 (min) to 255 (max)

#define pass (void)0

Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN1, NEO_GRBW + NEO_KHZ800); // Declare NeoPixel strip object
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRBW + NEO_KHZ800); // Declare NeoPixel strip object

const unsigned long SECOND = 1000;
const unsigned long MINUTE = SECOND * 60;
const unsigned long HOUR = SECOND * 3600;
unsigned long ctime = 0;

void setColor(uint32_t color) {
  for(int i=0; i<LED_COUNT; i++) {
    strip1.setPixelColor(i, color);
    strip2.setPixelColor(i, color);
  }
  strip1.show();
  strip2.show();
}

void setColorLow(uint32_t color) {
  int rnum = random(-25,25);
  for(int i=50; i<=70; i++) {
    if(i==(50+rnum) || i==(55+rnum) || i==(60+rnum) || i==(65+rnum) || i==(70+rnum)) { 
      strip1.setPixelColor(i, color);
      strip2.setPixelColor(i, color);
    }
    else {
      strip1.setPixelColor(i, strip2.Color(0,0,0,0));
      strip2.setPixelColor(i, strip2.Color(0,0,0,0));
    }
  }
  strip1.show();
  strip2.show();
}

void daylight() {
  setColor(strip1.Color(130, 70, 0, 150)); 
  setColor(strip2.Color(130, 70, 0, 150)); 
}

void lightBright() {
  setColor(strip1.Color(20,20,20,200)); 
  setColor(strip2.Color(20,20,20,200)); 
}

void moonlight() {
  setColorLow(strip1.Color(3, 1, 70, 0));
  setColorLow(strip2.Color(3, 1, 70, 0));
}

void sundown(unsigned int sec_speed) {
  int r = 130;
  int g = 70;
  int b = 0;
  int w = 150;
  //setColor(strip1.Color(r,g,b,w)); 
  for(int i=120; i>0; i--) {
    if (g>0) g = g-1;
    if (b>0) b = b-1;
    if (w>0) w = w-1;
    setColor(strip1.Color(r,g,b,w)); 
    setColor(strip2.Color(r,g,b,w)); 
    strip1.show();
    strip2.show();
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
      for(int j=i/2; j<(LED_COUNT-i/2); j++) {
        strip1.setPixelColor(j, strip1.Color(r,g,b,w));
        strip2.setPixelColor(j, strip2.Color(r,g,b,w));
      }
      for(int j=0; j<i/2; j++) {
        strip1.setPixelColor(j, strip1.Color(0,0,0,0));
        strip2.setPixelColor(j, strip2.Color(0,0,0,0));
        strip1.setPixelColor((LED_COUNT-j), strip1.Color(0,0,0,0));
        strip2.setPixelColor((LED_COUNT-j), strip2.Color(0,0,0,0));
      }
    }
    else {
      for(int j=60/2; j<=(LED_COUNT-60/2); j++) {
        strip1.setPixelColor(j, strip1.Color(r,g,b,w));
        strip2.setPixelColor(j, strip2.Color(r,g,b,w));
      }
    }
    strip1.show();
    strip2.show();
    delay(sec_speed*SECOND);
  }
  setColor(strip1.Color(0,0,0,0)); 
  setColor(strip2.Color(0,0,0,0)); 
  strip1.show();
  strip2.show();
}

void sunrise(unsigned int sec_speed) {
  int r_target = 130;
  int g_target = 70;
  int b_target = 0;
  int w_target = 150;

  int r = 0;
  int g = 0;
  int b = 0;
  int w = 0;

  for(int i=0; i<35; i++) {
    if (r<=r_target) r = r+2;
    if (i != 0 && i % 5 == 0) {
      if (w<=w_target) w = w+1;
    }
    for(int j=0; j<i; j++) {
      strip1.setPixelColor(LED_COUNT/2+j, strip1.Color(r,g,b,w));
      strip2.setPixelColor(LED_COUNT/2+j, strip2.Color(r,g,b,w));
      strip1.setPixelColor(LED_COUNT/2-j, strip1.Color(r,g,b,w));
      strip2.setPixelColor(LED_COUNT/2-j, strip2.Color(r,g,b,w));
    }
    strip1.show();
    strip2.show();
    delay(sec_speed*SECOND);
  }

  for(int i=0; i<=115; i++) {
    if (r<=r_target) r = r+2;
    if (g<=g_target) g = g+1;
    if (b<=b_target) b = b+1;
    if (w<=w_target) w = w+1;
    setColor(strip1.Color(r,g,b,w)); 
    setColor(strip2.Color(r,g,b,w)); 
    strip1.show();
    strip2.show();
    delay(sec_speed*SECOND);
  }
}

// plant light
// every 5th led blue, others red (rrrrbrrrrbrrrrb...)
void plantlight() {
  for(int i=0; i<LED_COUNT; i++) {
    if(i % 5 == 0) {
      strip1.setPixelColor(i, strip1.Color(0,0,255,0)); // blue
      strip2.setPixelColor(i, strip2.Color(0,0,255,0)); // blue
    }
    else {
      strip1.setPixelColor(i, strip1.Color(255,0,0,0)); // red
      strip2.setPixelColor(i, strip2.Color(255,0,0,0)); // red
    }
  }
  strip1.show();
  strip2.show();
}

void colorTest() {
  setColor(strip1.Color(255, 0, 0, 0)); // Red
  setColor(strip2.Color(255, 0, 0, 0)); // Red
  delay(500);
  setColor(strip1.Color(0, 255, 0, 0)); // Green
  setColor(strip2.Color(0, 255, 0, 0)); // Green
  delay(500);
  setColor(strip1.Color(0, 0, 255, 0)); // Blue
  setColor(strip2.Color(0, 0, 255, 0)); // Blue
  delay(500);
  setColor(strip1.Color(0, 0, 0, 255)); // White
  setColor(strip2.Color(0, 0, 0, 255)); // White
  delay(500);
}

void setup() {
  strip1.begin();           // INITIALIZE NeoPixel strip1 object (REQUIRED)
  strip2.begin();           // INITIALIZE NeoPixel strip1 object (REQUIRED)
  strip1.show();            // Turn OFF all pixels ASAP
  strip2.show();            // Turn OFF all pixels ASAP
  strip1.setBrightness(BRIGHTNESS);
  strip2.setBrightness(BRIGHTNESS);

  // Serial communication
  Serial.begin(9600);

  // for random function
  randomSeed(analogRead(0));
}

void timedRoutine () {
  Serial.println("Start of timed routine");
  ctime = millis();
  // 15:00
  Serial.println("15:00");
  colorTest();
  daylight();
  while(millis() < ctime+(7*HOUR+0*MINUTE)) pass;
  ctime = millis();
  // 22:00
  Serial.println("22:00");
  sundown(30);
  moonlight();
  while(millis() < ctime+(4*HOUR)) pass;
  ctime = millis();
  // 02:00
  Serial.println("02:00");
  setColor(strip1.Color(0, 0, 0, 0)); // Off
  setColor(strip2.Color(0, 0, 0, 0)); // Off
  while(millis() < ctime+(4*HOUR)) pass;
  ctime = millis();
  // 06:00
  Serial.println("06:00");
  moonlight();
  while(millis() < ctime+(2*HOUR)) pass;
  ctime = millis();
  // 08:00
  Serial.println("08:00");
  setColor(strip1.Color(0, 0, 0, 0)); // Off
  setColor(strip2.Color(0, 0, 0, 0)); // Off
  while(millis() < ctime+(2*HOUR)) pass;
  ctime = millis();
  // 03:00
  Serial.println("10:00");
  setColor(strip1.Color(0, 0, 0, 0)); // Off
  setColor(strip2.Color(0, 0, 0, 0)); // Off
  sunrise(30);
  plantlight();
  while(millis() < ctime+(5*HOUR)) pass;
  ctime = millis();
  // 12:00
  Serial.println("15:00");
  daylight();
  while(millis() < ctime+(5*HOUR)) pass;
  ctime = millis();
  // 15:00
}

void loop() {
  Serial.println("Loop start");
  timedRoutine();
  //moonlight();
  //plantlight();
  //sunrise(0);
  //sundown(0);
  //delay(1*SECOND);
}

