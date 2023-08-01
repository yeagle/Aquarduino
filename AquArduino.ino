#include <FastLED.h>
#include <RTClib.h>

#define LED_PIN     6
#define LED_PIN2    9 
#define NUM_LEDS  144 
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B

#define pass (void)0

RTC_DS3231 rtc;
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];

struct Color {
  int r,g,b,w;
};

//int rnum=0; // variable for random function

const unsigned long SECOND = 1000;
const unsigned long MINUTE = SECOND * 60;
const unsigned long HOUR = SECOND * 3600;
unsigned long ctime = 0;

struct Color colorFn(int r, int g, int b, int w) {
  struct Color returnColor = {r,g,b,w};
  return returnColor;
}

void setLED(unsigned int dot, struct Color color) {
  if (color.w > 0){
    color.r = min(255, color.r+color.w/3);
    color.g = min(255, color.g+color.w/3);
    color.b = min(255, color.b+color.w/3);
  }
  leds[dot] = CRGB(color.r,color.g,color.b);
  leds2[dot] = CRGB(color.r,color.g,color.b);
}

void showLEDs() {
  FastLED.show();
}

void setColor(struct Color color) {
  for(int i=0; i<NUM_LEDS; i++) {
    setLED(i, color);
  }
}

void setColoredStripe(int pos, int len, struct Color color) {
  int s1 = 70-pos;
  int s2 = 70+pos;
  for(int i=s1; i<s1+len; i++) {
    setLED(i, color);
  }
  for(int i=s2-len; i<s2; i++) {
    setLED(i, color);
  }
}

void setColoredStripeOnly(int pos, int len, struct Color color) {
  int s1 = 70-pos;
  int s2 = 70+pos;
  for(int i=0; i<NUM_LEDS; i++) {
    if(i>=s1 && i<s1+len) {
      setLED(i, color);
    }
    else if(i>=s2-len && i<s2) {
      setLED(i, color);
    }
    else {
      setLED(i, colorFn(0,0,0,0));
    }
  }
}

void lightOff() {
  setColor(colorFn(0,0,0,0)); 
  showLEDs();
}

void daylight() {
  setColor(colorFn(130, 70, 0, 150)); 
  showLEDs();
}

void brightlight() {
  setColor(colorFn(150,150,150,150)); 
  showLEDs();
}

void daylightWithStripe() {
  for(int i=0; i<NUM_LEDS; i++) {
    setLED(i, colorFn(130, 70, 0, 150));
  }
  setColoredStripe(30, 10, colorFn(0,200,0,0));
  setColoredStripe(53, 10, colorFn(0,0,200,0));
  showLEDs();
}

void moonlight() {
  setColoredStripeOnly(34, 2, colorFn(3, 1, 70, 0));
  showLEDs();
}

void moonlightWithStripe() {
  setColoredStripeOnly(34, 2, colorFn(3, 1, 70, 0));
  setColoredStripe(10, 1, colorFn(0,5,0,0));
  //setColoredStripe(19, 1, colorFn(0,20,0,0));
  //setColoredStripe(44, 1, colorFn(0,20,0,0));
  showLEDs();
}

void sundown(unsigned int sec_speed) {
  int r = 130;
  int g = 70;
  int b = 0;
  int w = 150;
  for(int i=120; i>0; i--) {
    if (g>0) g = g-1;
    if (b>0) b = b-1;
    if (w>0) w = w-1;
    setColor(colorFn(r,g,b,w)); 
    showLEDs();
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
      for(int j=i/2; j<(NUM_LEDS/2-i/2); j++) {
        setLED(j, colorFn(r,g,b,w));
      }
      for(int j=NUM_LEDS/2+i/2; j<(NUM_LEDS-i/2); j++) {
        setLED(j, colorFn(r,g,b,w));
      }
      for(int j=0; j<i/2; j++) {
        setLED(j, colorFn(0,0,0,0));
        setLED((NUM_LEDS/2-j), colorFn(0,0,0,0));
        setLED((NUM_LEDS/2+j), colorFn(0,0,0,0));
        setLED((NUM_LEDS-j), colorFn(0,0,0,0));
      }
    }
    else {
      for(int j=60/2; j<=(NUM_LEDS/2-60/2); j++) {
        setLED(j, colorFn(r,g,b,w));
        setLED((NUM_LEDS/2+j), colorFn(r,g,b,w));
      }
    }
    showLEDs();
    delay(sec_speed*SECOND);
  }
  setColor(colorFn(0,0,0,0)); 
  showLEDs();
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

  setColor(colorFn(0,0,0,0)); 
  showLEDs();

  for(int i=0; i<35; i++) {
    if (r<=r_target) r = r+2;
    if (i != 0 && i % 5 == 0) {
      if (w<=w_target) w = w+1;
    }
    for(int j=0; j<i; j++) {
      setLED(NUM_LEDS/2+NUM_LEDS/4+j, colorFn(r,g,b,w));
      setLED(NUM_LEDS/2+NUM_LEDS/4-j, colorFn(r,g,b,w));
      setLED(NUM_LEDS/2-NUM_LEDS/4+j, colorFn(r,g,b,w));
      setLED(NUM_LEDS/2-NUM_LEDS/4-j, colorFn(r,g,b,w));
    }
    showLEDs();
    delay(sec_speed*SECOND);
  }

  for(int i=0; i<=115; i++) {
    if (r<=r_target) r = r+2;
    if (g<=g_target) g = g+1;
    if (b<=b_target) b = b+1;
    if (w<=w_target) w = w+1;
    setColor(colorFn(r,g,b,w)); 
    showLEDs();
    delay(sec_speed*SECOND);
  }
}

// plant light
// every 5th led blue, others red (rrrrbrrrrbrrrrb...)
void plantlight2() {
  for(int i=0; i<NUM_LEDS; i++) {
    if(i % 5 == 0) {
      setLED(i, colorFn(0,0,255,0)); // blue
    }
    else {
      setLED(i, colorFn(255,0,0,0)); // red
    }
  }
  showLEDs();
}

void plantlight() {
  for(int i=0; i<NUM_LEDS; i++) {
    setLED(i, colorFn(200,0,50,0)); // blue
  }
  showLEDs();
}

void colorTest(int wait) {
  for (int i=0; i-30<NUM_LEDS; i++) {
    if (i >=  0 && i- 0 < NUM_LEDS) setLED(i,colorFn(255, 0, 0, 0));
    if (i >= 10 && i-10 < NUM_LEDS) setLED(i-10,colorFn(0, 255, 0, 0));
    if (i >= 20 && i-20 < NUM_LEDS) setLED(i-20,colorFn(0, 0, 255, 0));
    if (i >= 30 && i-30 < NUM_LEDS) setLED(i-30,colorFn(0, 0, 0, 255));
    showLEDs();
    delay(wait);
    setColor(colorFn(0,0,0,0));
  }
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
  plantlight();
  }
  else if (now.hour() >= 10 && now.hour() <12) 
  {
  Serial.println("Time 10 - 12");
  plantlight();
  }

  delay(1*MINUTE);
}

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

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS);
  
  // Turn off all leds
  setColor(colorFn(0,0,0,0));
  showLEDs();

  // Serial communication
  Serial.begin(9600);

  // for random function
  //randomSeed(analogRead(0));
  //rnum = random(0,100);

  // start rtc
  rtc.begin();

  // first start test colors
  colorTest(10);
}

void loop() {
  Serial.println("Loop");

  //rtcRoutine();
  //brightlight();
  //daylight();
  moonlight();
  //moonlightWithStripe();
  //plantlight();
  //sunrise(0);
  //sundown(0);
  //daylightWithStripe();
}

