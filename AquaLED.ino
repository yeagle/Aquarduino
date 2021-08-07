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
  int rnum = random(-10,10);
  for(int i=10; i<strip.numPixels(); i++) {
    if(i==(28+rnum) || i==(43+rnum) || i==(88-rnum) || i==(103-rnum) || i==(118-rnum)) { 
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
      strip.setPixelColor(strip.numPixels()/2+strip.numPixels()/4+j, strip.Color(r,g,b,w));
      strip.setPixelColor(strip.numPixels()/2+strip.numPixels()/4-j, strip.Color(r,g,b,w));
      strip.setPixelColor(strip.numPixels()/2-strip.numPixels()/4+j, strip.Color(r,g,b,w));
      strip.setPixelColor(strip.numPixels()/2-strip.numPixels()/4-j, strip.Color(r,g,b,w));
    }
    strip.show();
    delay(sec_speed*SECOND);
  }

  for(int i=0; i<=115; i++) {
    if (r<=r_target) r = r+2;
    if (g<=g_target) g = g+1;
    if (b<=b_target) b = b+1;
    if (w<=w_target) w = w+1;
    setColor(strip.Color(r,g,b,w)); 
    strip.show();
    delay(sec_speed*SECOND);
  }
}

// plant light
// every 5th led blue, others red (rrrrbrrrrbrrrrb...)
void plantlight() {
  for(int i=0; i<strip.numPixels(); i++) {
    if(i % 5 == 0) {
      strip.setPixelColor(i, strip.Color(0,0,255,0)); // blue
    }
    else {
      strip.setPixelColor(i, strip.Color(255,0,0,0)); // red
    }
  }
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
  while(millis() < ctime+(4*HOUR+0*MINUTE)) pass;
  ctime = millis();
  // 21:00
  Serial.println("21:00");
  sundown(30);
  moonlight();
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
  setColor(strip.Color(0, 0, 0, 0)); // Off
  sunrise(30);
  plantlight();
  while(millis() < ctime+(5*HOUR)) pass;
  ctime = millis();
  // 12:00
  Serial.println("12:00");
  daylight();
  while(millis() < ctime+(5*HOUR)) pass;
  ctime = millis();
  // 17:00
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

