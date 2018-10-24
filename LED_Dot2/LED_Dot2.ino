  #include "LedControl.h"
#include "FontLEDClock.h" // Font library

LedControl lc=LedControl(11,13,10,4);  // DIN,CLK,CS,Number of LED Module
uint8_t intensity = 1;

unsigned long delaytime=50;  // time to updates of the display

int timer1_counter;

#define left 5
#define right 6
#define up 7
#define down 8
#define menu 9

void setup() {
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(3,OUTPUT);
  int devices=lc.getDeviceCount();  // find no of LED modules
  //we have to init all devices in a loop
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  timer1_counter = 3036;   // preload timer 65536-16MHz/256/1Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);
  for(int address=0;address<devices;address++) {  // set up each device 
    lc.shutdown(address,false);
    lc.setIntensity(address,8);
    lc.clearDisplay(address);
  }
  Serial.begin(9600);
}

void test() {
  int devices=lc.getDeviceCount();  // find no of LED modules
  
  for(int row=32;row>=0;row--) {
    for(int col=0;col<8;col++) {
        delay(delaytime);
        plot(row,col,true);
        delay(delaytime);
        plot(row,col,false);
    }
  }  
}

//plot a point on the display
void plot (uint8_t x, uint8_t y, uint8_t value) {

  //select which matrix depending on the x coord
  uint8_t address;
  if (x >= 0 && x <= 7)   { address = 3; }
  if (x >= 8 && x <= 15)  { address = 2; x = x - 8; }
  if (x >= 16 && x <= 23) { address = 1; x = x - 16;}
  if (x >= 24 && x <= 31) { address = 0; x = x - 24;}

  if (value == 1) {
    lc.setLed(address, y, x, true);
  } else {
    lc.setLed(address, y, x, false);
  }
}

//clear screen
void clear_display() {
  for (uint8_t address = 0; address < 4; address++) {
    lc.clearDisplay(address);
  }
}

//fade screen down
void fade_down() {

  //fade from global intensity to 1
  for (uint8_t i = intensity; i > 0; i--) {
    for (uint8_t address = 0; address < 4; address++) {
      lc.setIntensity(address, i);
    }
    delay(30); //change this to change fade down speed
  }

  clear_display(); //clear display completely (off)

  //reset intentsity to global value
  for (uint8_t address = 0; address < 4; address++) {
    lc.setIntensity(address, intensity);
  }
}

void test_all_led(){
    for (uint8_t x = 0; x <= 31; x++) {
      for (uint8_t y = 0; y <= 7; y++) {
        plot(x, y, 1);
        delay(10);
    }
  }
}

void print_tiny_char(uint8_t x, uint8_t y, char c)
{
  uint8_t dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') ) { c &= 0x1F; }  // A-Z maps to 1-26 
  else if (c >= '0' && c <= '9') { c = (c - '0') + 32; }
  else if (c == ' ') { c = 0;  } // space 
  else if (c == '.') { c = 27; } // full stop 
  else if (c == ':') { c = 28; } // colon 
  else if (c == '\''){ c = 29; } // single quote mark 
  else if (c == '!') { c = 30; } // single quote mark 
  else if (c == '?') { c = 31; } // single quote mark 

  for (uint8_t col = 0; col < 3; col++) {
    dots = pgm_read_byte_near(&mytinyfont[c][col]);
    for (uint8_t row = 0; row < 5; row++) {
      if (dots & (16 >> row))
        plot(x + col, y + row, 1);
      else
        plot(x + col, y + row, 0);
    }
  }
}

void print_normal_char(uint8_t x, uint8_t y, char c)
{

  uint8_t dots;
  if (c >= 'A' && c <= 'Z' ) { c &= 0x1F; }  // A-Z maps to 1-26
  else if (c >= 'a' && c <= 'z') { c = (c - 'a') + 41; }  // A-Z maps to 41-67
  else if (c >= '0' && c <= '9') { c = (c - '0') + 31; }
  else if (c == ' ') { c = 0; } // space
  else if (c == '.') { c = 27; } // full stop
  else if (c == '\'') { c = 28; } // single quote mark
  else if (c == ':') { c = 29; } // clock_mode selector arrow
  else if (c == '>') { c = 30; } // clock_mode selector arrow
  else if (c >= -80 && c <= -67) { c *= -1; }

  for (uint8_t col = 0; col < 5; col++) {
    dots = pgm_read_byte_near(&myfont[c][col]);
    for (uint8_t row = 0; row < 7; row++) {
      if (dots & (64 >> row)) {   // only 7 rows.
        plot(x + col, y + row, 1);
      } else {
        plot(x + col, y + row, 0);
      }
    }
  }
}
int seconds = 0 , minutes = 48, hours = 21;
char times[6];

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
  //times[4] = times[4] + 1;
  seconds ++ ;
  if(seconds == 60){
    minutes ++ ;
    seconds = 0;
  }
  if(minutes == 60){
    hours ++;
    minutes = 0;
  }
  if(hours == 25){
    hours = 0;
  }

  if(cd == true)
  {
    secondsC --;
    if(secondsC == 0)
    {
      minutesC --;
      secondsC = 59;
    }
    if(minutesC == 0)
    {
      cd = false;
      cAlarm = true;
    }
  }
  
}
int k = 4;
int menuEntry = 0;
//int lbs9 = HIGH, lbs5 = HIGH,lbs6 = HIGH,bs5,bs6,bs9;
int secondsA = 0 , minutesA = 49, hoursA = 21;
int secondsC = 0 , minutesC = 0;
unsigned long int ldt5 = 0,ldt6 = 0, ldt9 = 0;
unsigned long int prev = 0;
void setTime();
void setAlarm();
void countDown();
bool pressed = false, alarm = false, cd = false, cAlarm = false;
bool printBasic = true, printAlarm = true, printSet = true, printCD;

void loop() {
  
  if(menuEntry == 0  && printBasic == true){
    printBasic = true; printAlarm = true; printSet = true; printCD = true;
    char setT[9] = "Basic";
  while(printBasic == true){
    clear_display();
    for(int i = 0; i < 9; i++){
      print_tiny_char(1+(i * 4),2,setT[i]);
    }
    delay(750);
    clear_display();
    printBasic = false;  
  }
  }
  if(menuEntry == 0);
  Serial.println(seconds);
  if(menuEntry == 0 || menuEntry == 1){
  times[0] = {'0' + (hours / 10)};
  times[1] = {'0' + (hours % 10)};
  times[2] = ':';
  times[3] = {'0' + (minutes / 10)}; 
  times[4] = {'0' + (minutes % 10)};
  }
  else if(menuEntry == 2){
    times[0] = {'0' + (hoursA / 10)};
    times[1] = {'0' + (hoursA % 10)};
    times[2] = ':';
    times[3] = {'0' + (minutesA / 10)}; 
    times[4] = {'0' + (minutesA % 10)};
  }
  char hello[6] = "Hello";  
  char world[6] = "EiEi";
  interrupts();
  uint8_t i = 0;
  for(i = 0; i < 5; i++){
    print_tiny_char(3 + (i * 6), 2, times[i]);
  }
  
  while(digitalRead(9) == LOW){
    pressed = true;
  }
  if(pressed == true){
    menuEntry ++;
    pressed =false;
  }
  if(menuEntry > 3){
    menuEntry  = 0;
  }
  switch(menuEntry){
    case 1:
      setTime();
      break;
    case 2:
      Serial.println("ENTER");
      setAlarm();
      break;
    case 3:
      countDown();
      break;
  }
  if(alarm == true){
    Serial.println("Alarm Set");
  }
  else{
    Serial.println("Alarm not set");
  }
  if(hoursA == hours && minutesA == minutes && alarm == true && menuEntry == 0){
    Serial.println("EIEI");
    tone(3,440,1000);
    delay(500);
    tone(3,261,1000);
    while(digitalRead(5) == LOW){
      alarm = false;
    }
  }
  
}

void setTime(){
  
  unsigned long int current = millis();
  if(menuEntry == 1 && printSet == true){
    printBasic = true; printAlarm = true; printSet = true; printCD = true;
  char setT[9] = "set time";
  while(printSet == true){
    clear_display();
    for(int i = 0; i < 9; i++){
      print_tiny_char(1+(i * 4),2,setT[i]);
    }
    delay(750);
    clear_display();
    printSet = false; 
  }
  }
  if(menuEntry == 1){
    uint8_t i = 0;
      if(digitalRead(7) == LOW){
        k-= 3;
        if(k <= 1){
          k = 1;
        }
      }
      if(digitalRead(8) == LOW){
        k+=3;
        if(k >= 4){
          k = 4;
        }
      }
//      if(digitalRead(5) != lbs5){
//        ldt5 = millis();
//      }
      if(digitalRead(5) == LOW){
            if(k == 4){
              minutes ++;
              if(minutes >= 60){
                minutes = 0;
              }
            }
            if(k == 1){
              hours ++;
              if(hours > 24){
                hours = 0;
              }
            }
          }
      //lbs5 = digitalRead(5);
      if(digitalRead(6) == LOW){

        if(k == 4){
          minutes --;
          if(minutes < 0){
            minutes = 59;
          }
        }

        if(k == 1){
          hours --;
          if(hours < 0){
            hours = 24;
          }
        }
      }
      if((current - prev) >= 75){
        prev = current;
        if(times[k] != ' '){
          times[k] = ' ';
          times[k-1] = ' ';
        }
        else{
          times[0] = {'0' + (hours / 10)};
          times[1] = {'0' + (hours % 10)};
          times[2] = ':';
          times[3] = {'0' + (minutes / 10)}; 
          times[4] = {'0' + (minutes % 10)};
        }
      }
   for(i = 0; i < 5; i++){
     print_tiny_char(3 + (i * 6), 2, times[i]);
  }
  while(digitalRead(5) == LOW && digitalRead(6) == LOW){
    seconds = secondsA;
  }
//  if(digitalRead(9) != lbs9){
//    ldt9 = millis();  
//  }

  while(digitalRead(9) == LOW){
    pressed = true;
  }
  if(pressed == true){
    menuEntry ++;
    pressed =false;
  }
  }
 } 

void setAlarm(){
  if(menuEntry == 2 && printAlarm ==true){
    printBasic = true; printAlarm = true; printSet = true; printCD = true;
  char setT[9] = "Alarm";
  while(printAlarm == true){
    clear_display();
    for(int i = 0; i < 9; i++){
      print_normal_char(1+(i * 6),1,setT[i]);
    }
    delay(750);
    clear_display();

    printAlarm = false;  
  }
  }
  unsigned long int current = millis();
  Serial.println(minutesA);
  uint8_t i = 0;
      if(digitalRead(7) == LOW){
        k-= 3;
        if(k <= 1){
          k = 1;
        }
      }
      if(digitalRead(8) == LOW){
        k+=3;
        if(k >= 4){
          k = 4;
        }
      }
//      if(digitalRead(5) != lbs5){
//        ldt5 = millis();
//      }
      if(digitalRead(5) == LOW){
            if(k == 4){
              minutesA ++;
              if(minutesA >= 60){
                minutesA = 0;
              }
            }
            if(k == 1){
              hoursA ++;
              if(hoursA > 24){
                hoursA = 0;
              }
            }
          }
      //lbs5 = digitalRead(5);
      if(digitalRead(6) == LOW){

        if(k == 4){
          minutesA --;
          if(minutesA < 0){
            minutesA = 59;
          }
        }

        if(k == 1){
          hoursA --;
          if(hoursA < 0){
            hoursA = 24;
          }
        }
      }
      if((current - prev) >= 75){
        prev = current;
        if(times[k] != ' '){
          times[k] = ' ';
          times[k-1] = ' ';
        }
        else{
          times[0] = {'0' + (hoursA / 10)};
          times[1] = {'0' + (hoursA % 10)};
          times[2] = ':';
          times[3] = {'0' + (minutesA / 10)}; 
          times[4] = {'0' + (minutesA % 10)};
        }
      }
   for(i = 0; i < 5; i++){
     print_tiny_char(3 + (i * 6), 2, times[i]);
  }
  while(digitalRead(5) == LOW && digitalRead(6) == LOW){
    alarm = true;
    Serial.println("Alarm Set");
  }
//  if(digitalRead(9) != lbs9){
//    ldt9 = millis();  
//  }

  while(digitalRead(9) == LOW){
    pressed = true;
  }
  if(pressed == true){
    menuEntry ++;
    pressed =false;
  }
}

void countDown(){
  if(menuEntry == 3 && printCD ==true){
    printBasic = true; printAlarm = true; printSet = true; printCD = true;
  char setT[9] = "CountDown";
  while(printAlarm == true){
    clear_display();
    for(int i = 0; i < 9; i++){
      print_normal_char(1+(i * 6),1,setT[i]);
    }
    delay(750);
    clear_display();

    printCD = false;  
  }
  }
  unsigned long int current = millis();
  //Serial.println(minutesA);
  uint8_t i = 0;
      if(digitalRead(7) == LOW){
        k-= 3;
        if(k <= 1){
          k = 1;
        }
      }
      if(digitalRead(8) == LOW){
        k+=3;
        if(k >= 4){
          k = 4;
        }
      }
//      if(digitalRead(5) != lbs5){
//        ldt5 = millis();
//      }
      if(digitalRead(5) == LOW){
            if(k == 4){
              minutesC ++;
              if(secondsC >= 60){
                secondsC = 0;
              }
            }
            if(k == 1){
              minutesC ++;
              if(minutesC > 24){
                minutesC = 0;
              }
            }
          }
      //lbs5 = digitalRead(5);
      if(digitalRead(6) == LOW){

        if(k == 4){
          secondsA --;
          if(secondsC < 0){
            secondsC = 59;
          }
        }

        if(k == 1){
          minutesA --;
          if(minutesC < 0){
            minutesC = 59;
          }
        }
      }
      if((current - prev) >= 75){
        prev = current;
        if(times[k] != ' '){
          times[k] = ' ';
          times[k-1] = ' ';
        }
        else{
          times[0] = {'0' + (minutesC / 10)};
          times[1] = {'0' + (minutesC % 10)};
          times[2] = ':';
          times[3] = {'0' + (secondsC / 10)}; 
          times[4] = {'0' + (secondsC % 10)};
        }
      }
   for(i = 0; i < 5; i++){
     print_tiny_char(3 + (i * 6), 2, times[i]);
  }
  while(digitalRead(5) == LOW){
    cd = true;
    Serial.println("Alarm Set");
  }

  if(cAlarm == true)
  {
    tone(3,440,1000);
    delay(500);
    tone(3,261,1000);
    cAlarm = false;
  }
//  if(digitalRead(9) != lbs9){
//    ldt9 = millis();  
//  }

  while(digitalRead(9) == LOW){
    pressed = true;
  }
  if(pressed == true){
    menuEntry ++;
    pressed =false;
  }
}
