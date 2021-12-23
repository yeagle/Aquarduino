#include <Adafruit_NeoPixel.h>
#include <RTClib.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6 // Which pin on the Arduino is connected to the NeoPixels
#define LED_COUNT  140 // How many NeoPixels are attached to the Arduino
#define BRIGHTNESS 255 // NeoPixel brightness, 0 (min) to 255 (max)

#define pass (void)0

RTC_DS3231 rtc;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800); // Declare NeoPixel strip object

const unsigned long SECOND = 1000;
const unsigned long MINUTE = SECOND * 60;
const unsigned long HOUR = SECOND * 3600;
unsigned long ctime = 0;

void setColor(uint32_t color) {
  for(int i=0; i<LED_COUNT; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setColorLow(uint32_t color) {
  for(int i=0; i<LED_COUNT; i++) {
    if(i==(70-2) || i==(70-32) || i==(70-62) ||
      i==(70+2) || i==(70+32) || i==(70+62)) { 
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

void coloredStripe(int pos, int len, uint32_t color) {
  int s1 = 70-pos;
  int s2 = 70+pos;
  for(int i=s1; i<s1+len; i++) {
    strip.setPixelColor(i, color);
  }
  for(int i=s2-len; i<s2; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void coloredStripeOnly(int pos, int len, uint32_t color) {
  int s1 = 70-pos;
  int s2 = 70+pos;
  for(int i=0; i<LED_COUNT; i++) {
    if(i>=s1 && i<s1+len) {
      strip.setPixelColor(i, color);
    }
    else if(i>=s2-len && i<s2) {
      strip.setPixelColor(i, color);
    }
    else {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }
  strip.show();
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
      for(int j=i/2; j<(LED_COUNT/2-i/2); j++) {
        strip.setPixelColor(j, strip.Color(r,g,b,w));
      }
      for(int j=LED_COUNT/2+i/2; j<(LED_COUNT-i/2); j++) {
        strip.setPixelColor(j, strip.Color(r,g,b,w));
      }
      for(int j=0; j<i/2; j++) {
        strip.setPixelColor(j, strip.Color(0,0,0,0));
        strip.setPixelColor((LED_COUNT/2-j), strip.Color(0,0,0,0));
        strip.setPixelColor((LED_COUNT/2+j), strip.Color(0,0,0,0));
        strip.setPixelColor((LED_COUNT-j), strip.Color(0,0,0,0));
      }
    }
    else {
      for(int j=60/2; j<=(LED_COUNT/2-60/2); j++) {
        strip.setPixelColor(j, strip.Color(r,g,b,w));
        strip.setPixelColor((LED_COUNT/2+j), strip.Color(r,g,b,w));
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
      strip.setPixelColor(LED_COUNT/2+LED_COUNT/4+j, strip.Color(r,g,b,w));
      strip.setPixelColor(LED_COUNT/2+LED_COUNT/4-j, strip.Color(r,g,b,w));
      strip.setPixelColor(LED_COUNT/2-LED_COUNT/4+j, strip.Color(r,g,b,w));
      strip.setPixelColor(LED_COUNT/2-LED_COUNT/4-j, strip.Color(r,g,b,w));
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
  for(int i=0; i<LED_COUNT; i++) {
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

  // start rtc
  rtc.begin();

  // first start test colors
  colorTest();
}

void rtcRoutine () {
  Serial.println("Start of rtc routine");
  DateTime now = rtc.now();
  Serial.println("Hour now:");
  Serial.print(now.hour(), DEC);

  // 12-13 Uhr
  if (now.hour() >= 12 && now.hour() <13) 
  {
  Serial.println("\nTime 12 - 13");
  ctime = millis();
  sunrise(10);
  daylight();
  while(millis() < ctime+(1*HOUR)) pass;
  }

  // 13-21 Uhr
  else if (now.hour() >= 13 && now.hour() <21) 
  {
  Serial.println("\nTime 13 - 21");
  daylight();
  coloredStripe(23, 10, strip.Color(255,0,0,0));
  coloredStripe(49, 10, strip.Color(0,0,255,0));
  }

  // 21-22 Uhr
  else if (now.hour() >= 21 && now.hour() <22) 
  {
  Serial.println("\nTime 21 - 22");
  ctime = millis();
  sundown(30);
  moonlight();
  coloredStripe(19, 1, strip.Color(0,20,0,0));
  coloredStripe(44, 1, strip.Color(20,0,0,0));
  while(millis() < ctime+(1*HOUR)) pass;
  }

  // 22-0 Uhr
  else if (now.hour() >= 22 && now.hour() <24) 
  {
  Serial.println("\nTime 22 - 0");
  moonlight();
  coloredStripe(19, 1, strip.Color(0,20,0,0));
  coloredStripe(44, 1, strip.Color(20,0,0,0));
  }

  // 0-6
  else if (now.hour() >= 0 && now.hour() <6) 
  {
  Serial.println("\nTime 0 - 6");
  setColor(strip.Color(0, 0, 0, 0)); // Off
  }

  // 6-8 Uhr
  else if (now.hour() >= 6 && now.hour() <8) 
  {
  Serial.println("\nTime 6 - 8");
  moonlight();
  coloredStripe(19, 1, strip.Color(0,20,0,0));
  coloredStripe(44, 1, strip.Color(20,0,0,0));
  }

  // 8-10 Uhr
  else if (now.hour() >= 8 && now.hour() <10) 
  {
  Serial.println("\nTime 8 - 10");
  coloredStripeOnly(35, 1, strip.Color(0,0,0,100));
  }

  // 8-12 Uhr
  else if (now.hour() >= 8 && now.hour() <12) 
  {
  Serial.println("\nTime 10 - 12");
  plantlight();
  }

  delay(1*MINUTE);
}

void timedRoutine () {
  Serial.println("Start of timed routine");
  ctime = millis();
  // 
  Serial.println("17:00");
  colorTest();
  daylight();
  coloredStripe(23, 10, strip.Color(255,0,0,0));
  coloredStripe(49, 10, strip.Color(0,0,255,0));
  while(millis() < ctime+(4*HOUR+0*MINUTE)) pass;
  ctime = millis();
  // 
  Serial.println("21:00");
  sundown(30);
  moonlight();
  coloredStripe(19, 1, strip.Color(0,20,0,0));
  coloredStripe(44, 1, strip.Color(20,0,0,0));
  while(millis() < ctime+(3*HOUR)) pass;
  ctime = millis();
  // 
  Serial.println("00:00");
  setColor(strip.Color(0, 0, 0, 0)); // Off
  while(millis() < ctime+(5*HOUR+30*MINUTE)) pass;
  ctime = millis();
  //
  Serial.println("05:30");
  moonlight();
  coloredStripe(19, 1, strip.Color(0,20,0,0));
  coloredStripe(44, 1, strip.Color(20,0,0,0));
  while(millis() < ctime+(1*HOUR+30*MINUTE)) pass;
  ctime = millis();
  // 
  Serial.println("07:00");
  setColor(strip.Color(0, 0, 0, 0)); // Off
  coloredStripe(35, 1, strip.Color(0,0,0,50));
  while(millis() < ctime+(2*HOUR)) pass;
  ctime = millis();
  // 
  Serial.println("09:00");
  plantlight();
  while(millis() < ctime+(3*HOUR)) pass;
  ctime = millis();
  // 
  Serial.println("12:00");
  sunrise(10);
  daylight();
  while(millis() < ctime+(5*HOUR)) pass;
  ctime = millis();
}

void loop() {
  Serial.println("Loop start");
  rtcRoutine();
  //timedRoutine();
  //moonlight();
  //plantlight();
  //sunrise(0);
  //sundown(0);
  //delay(1*SECOND);
}

