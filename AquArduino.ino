#include <Adafruit_NeoPixel.h>
#include <RTClib.h>

#ifdef __AVR__
  #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN1    6 // Which pin on the Arduino is connected to the NeoPixels
#define LED_PIN2    9 // Which pin on the Arduino is connected to the NeoPixels
#define LED_COUNT  120 // How many NeoPixels are attached per strip
#define BRIGHTNESS 255 // NeoPixel brightness, 0 (min) to 255 (max)

#define pass (void)0

RTC_DS3231 rtc;

Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN1, NEO_GRBW + NEO_KHZ800); 
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRBW + NEO_KHZ800); 

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

void show() {
  strip1.show();
  strip2.show();
}

void setColor(uint32_t color) {
  for(int i=0; i<LED_COUNT; i++) {
    strip1.setPixelColor(i, color);
    strip2.setPixelColor(i, color);
  }
}

void setColoredStripe(int pos, int len, uint32_t color) {
  for(int i=pos; i<pos+len; i++) {
    strip1.setPixelColor(i, color);
    strip2.setPixelColor(i, color);
  }
}

void setColoredStripeOnly(int pos, int len, uint32_t color) {
  for(int i=0; i<LED_COUNT; i++) {
    if(i>=pos && i<pos+len) {
      strip1.setPixelColor(i, color);
      strip2.setPixelColor(i, color);
    }
    else {
      strip1.setPixelColor(i, strip1.Color(0, 0, 0, 0));
      strip2.setPixelColor(i, strip2.Color(0, 0, 0, 0));
    }
  }
}

void lightOff() {
  setColor(strip1.Color(0,0,0,0)); 
  show();
}

void daylight() {
  setColor(strip1.Color(130, 70, 0, 150)); 
  show();
}

void lightBright() {
  setColor(strip1.Color(200,200,200,255)); 
  show();
}

void moonlight() {
  setColoredStripeOnly(55,10,strip1.Color(3, 1, 70, 0));
  show();
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
    show();
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
    show();
    delay(sec_speed*SECOND);
  }
  setColor(strip1.Color(0,0,0,0)); 
  show();
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

  lightOff(); // make sure to start with lights off

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
    show();
    delay(sec_speed*SECOND);
  }

  for(int i=0; i<=115; i++) {
    if (r<=r_target) r = r+2;
    if (g<=g_target) g = g+1;
    if (b<=b_target) b = b+1;
    if (w<=w_target) w = w+1;
    setColor(strip1.Color(r,g,b,w)); 
    show();
    delay(sec_speed*SECOND);
  }
}

// plant light
void plantlight() {
  for(int i=0; i<LED_COUNT; i++) {
    strip1.setPixelColor(i, strip1.Color(200,0,50,0));
    strip2.setPixelColor(i, strip2.Color(200,0,50,0));
  }
  show();
}

void colorTest() {
  setColor(strip1.Color(255, 0, 0, 0)); // Red
  show();
  delay(500);
  setColor(strip1.Color(0, 255, 0, 0)); // Green
  show();
  delay(500);
  setColor(strip1.Color(0, 0, 255, 0)); // Blue
  show();
  delay(500);
  setColor(strip1.Color(0, 0, 0, 255)); // White
  show();
  delay(500);
  setColor(strip1.Color(255, 255, 255, 255)); // White+RGB
  show();
  delay(500);
}

void setup() {
  strip1.begin();           // INITIALIZE NeoPixel strip1 object (REQUIRED)
  strip2.begin();           // INITIALIZE NeoPixel strip2 object (REQUIRED)
  strip1.show();            // Turn OFF all pixels ASAP
  strip2.show();            // Turn OFF all pixels ASAP
  strip1.setBrightness(BRIGHTNESS);
  strip2.setBrightness(BRIGHTNESS);

  // Serial communication
  Serial.begin(9600);

  // for random function
  //randomSeed(analogRead(0));
  //rnum = random(-45,45)

  // start rtc
  rtc.begin();

  // test colors
  colorTest();
}

void rtcRoutine () {
  Serial.println("Start of rtc routine");

  DateTime now = rtc.now();
  printTimeNow(now);

  if (now.hour() >= 15 && now.hour() <16) 
  {
  Serial.println("Time 15 - 16");
  ctime = millis();
  sunrise(10);
  daylight();
  while(millis() < ctime+(1*HOUR)) pass;
  }
  else if (now.hour() >= 16 && now.hour() <22) 
  {
  Serial.println("Time 16 - 22");
  //daylight();
  lightBright();
  }
  else if (now.hour() >= 22 && now.hour() <23) 
  {
  Serial.println("Time 22 - 23");
  ctime = millis();
  sundown(15);
  moonlight();
  while(millis() < ctime+(1*HOUR)) pass;
  }
  else if (now.hour() >= 23 && now.hour() <24) 
  {
  Serial.println("Time 23 - 0");
  moonlight();
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
  setColoredStripeOnly(20,1,strip1.Color(0, 0, 0, 100));
  setColoredStripe(60,1,strip1.Color(0, 0, 0, 100));
  }
  else if (now.hour() >= 10 && now.hour() <15) 
  {
  Serial.println("Time 10 - 15");
  plantlight();
  }

  delay(1*MINUTE);
}

void loop() {
  Serial.println("Loop start");
  rtcRoutine();
  //moonlight();
  //plantlight();
  //sunrise(0);
  //sundown(0);
  //delay(1*SECOND);
}
