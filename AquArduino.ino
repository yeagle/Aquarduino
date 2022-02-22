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

//int rnum=0; // variable for random function

const unsigned long SECOND = 1000;
const unsigned long MINUTE = SECOND * 60;
const unsigned long HOUR = SECOND * 3600;
unsigned long ctime = 0;

void printTimeNow(DateTime now) {
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

void setColor(uint32_t color) {
  for(int i=0; i<LED_COUNT; i++) {
    strip.setPixelColor(i, color);
  }
}

void setColoredStripe(int pos, int len, uint32_t color) {
  int s1 = 70-pos;
  int s2 = 70+pos;
  for(int i=s1; i<s1+len; i++) {
    strip.setPixelColor(i, color);
  }
  for(int i=s2-len; i<s2; i++) {
    strip.setPixelColor(i, color);
  }
}

void setColoredStripeOnly(int pos, int len, uint32_t color) {
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
}

void lightOff() {
  setColor(strip.Color(0,0,0,0)); 
  strip.show();
}

void daylight() {
  setColor(strip.Color(130, 70, 0, 150)); 
  strip.show();
}

void brightlight() {
  setColor(strip.Color(150,150,150,150)); 
  strip.show();
}

void daylightWithStripe() {
  for(int i=0; i<LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(130, 70, 0, 150));
  }
  setColoredStripe(23, 10, strip.Color(200,0,50,0));
  setColoredStripe(49, 10, strip.Color(200,0,50,0));
  strip.show();
}

void moonlight() {
  setColoredStripeOnly(34, 2, strip.Color(3, 1, 70, 0));
  strip.show();
}

void moonlightWithStripe() {
  setColoredStripeOnly(34, 2, strip.Color(3, 1, 70, 0));
  setColoredStripe(10, 1, strip.Color(0,5,0,0));
  //setColoredStripe(19, 1, strip.Color(0,20,0,0));
  //setColoredStripe(44, 1, strip.Color(0,20,0,0));
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

  setColor(strip.Color(0,0,0,0)); 
  strip.show();

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
void plantlight2() {
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

void plantlight() {
  for(int i=0; i<LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(200,0,50,0)); // blue
  }
  strip.show();
}

void colorTest() {
  setColor(strip.Color(255, 0, 0, 0)); // Red
  strip.show();
  delay(500);
  setColor(strip.Color(0, 255, 0, 0)); // Green
  strip.show();
  delay(500);
  setColor(strip.Color(0, 0, 255, 0)); // Blue
  strip.show();
  delay(500);
  setColor(strip.Color(0, 0, 0, 255)); // White
  strip.show();
  delay(500);
  setColor(strip.Color(0, 0, 0, 0)); // Off
  strip.show();
  delay(500);
  setColor(strip.Color(255, 255, 255, 0)); // RGB
  strip.show();
  delay(500);
  setColor(strip.Color(255, 255, 255, 255)); // White+RGB
  strip.show();
  delay(500);
}

void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  // Serial communication
  Serial.begin(9600);

  // for random function
  //randomSeed(analogRead(0));
  //rnum = random(0,100);

  // start rtc
  rtc.begin();

  // first start test colors
  colorTest();
}

void rtcRoutine () {
  Serial.println("Start of rtc routine");
  DateTime now = rtc.now();
  printTimeNow(now);

  if (now.hour() >= 12 && now.hour() <13) 
  {
  Serial.println("Time 12 - 13");
  //rnum = random(0,100);
  ctime = millis();
  sunrise(10);
  daylight();
  while(millis() < ctime+(1*HOUR)) pass;
  }
  else if (now.hour() >= 13 && now.hour() <17) 
  {
  Serial.println("Time 13 - 17");
  brightlight();
  }
  else if (now.hour() >= 17 && now.hour() <20) 
  {
  Serial.println("Time 17 - 20");
  daylightWithStripe();
  }
  else if (now.hour() >= 20 && now.hour() <21) 
  {
  Serial.println("Time 20 - 21");
  daylight();
  }
  else if (now.hour() >= 21 && now.hour() <22) 
  {
  Serial.println("Time 21 - 22");
  ctime = millis();
  sundown(15);
  moonlightWithStripe();
  while(millis() < ctime+(1*HOUR)) pass;
  }
  else if (now.hour() >= 22 && now.hour() <24) 
  {
  Serial.println("Time 22 - 0");
  moonlightWithStripe();
  }
  else if (now.hour() >= 0 && now.hour() <6) 
  {
  Serial.println("Time 0 - 6");
  lightOff();
  }
  else if (now.hour() >= 6 && now.hour() <8) 
  {
  Serial.println("Time 6 - 8");
  moonlight();
  }
  else if (now.hour() >= 8 && now.hour() <10) 
  {
  Serial.println("Time 8 - 10");
  plantlight2();
  }
  else if (now.hour() >= 10 && now.hour() <12) 
  {
  Serial.println("Time 10 - 12");
  plantlight();
  }

  delay(1*MINUTE);
}

void loop() {
  Serial.println("Loop");
  rtcRoutine();
  //brightlight();
  //daylight();
  //moonlight();
  //moonlightWithStripe();
  //plantlight();
  //sunrise(0);
  //sundown(0);
  //delay(1*SECOND);
}

