#include <SoftwareSerial.h> 
String inputString = ""; // a string to hold incoming data 
char me[5];
SoftwareSerial mySerial(3,11); // SoftwareSerial(rxPin, txPin)

void SoftwareSerialEvent();
void setup() {     
  // Open serial communications and wait for port to open: 
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);    
  Serial.begin(4800);     
  Serial.println("Hello World");     // set the data rate for the SoftwareSerialport     
  mySerial.begin(4800); // recommentlow speed     
  mySerial.println("Software Serial->Hello, world?"); 
     // attachInterrupt(interrupt, ISR, mode) interrupt-> 1(pin3)     
     attachInterrupt(1,SoftwareSerialEvent,FALLING); 
     } 
void loop(){ 
  if(strcmp(mySerial.read(),me) == 0){
    digitalWrite(12,HIGH);
  }
  else if(mySerial.read() == "paper" && me == "rock"){
    digitalWrite(12,HIGH);
  }
  else if(mySerial.read() == "paper" && me == "scisor"){
    digitalWrite(12,HIGH);
  }
  else if(mySerial.read() == "rock" && me == "paper"){
    digitalWrite(12,HIGH);
  }
  else if(mySerial.read() == "rock" && me == "scisor"){
    digitalWrite(12,HIGH);
  }
  else if(mySerial.read() == "scisor" && me == "paper"){
    digitalWrite(12,HIGH);
  }
  else if(mySerial.read() == "scisor" && me == "rock"){
    digitalWrite(12,HIGH);
  }
} 

void serialEvent() {     
  while(Serial.available()) // recheck serial is available            
  {   
   
    char inChar=(char)Serial.read(); // get the new byte:  
    inputString+=inChar; // add it to the inputString:  
    if (digitalRead(5) == LOW) // check received 'enter' (0x0D)   
    {                
      mySerial.print("rock");
      strcpy(me,"rock");        
    }
    if(digitalRead(6) == LOW)
    {
      mySerial.print("paper");
      strcpy(me,"paper");        
    }
    if(digitalRead(7) == LOW){
      mySerial.print("scisor");
      strcpy(me,"scisor");  
    }
   } 
  } 

 
 
void SoftwareSerialEvent() {      
  if(mySerial.available()) // test this condition by connecting pin rxsoftware with pin'0'(Rx)               
  {  
    Serial.print((char)mySerial.read());               
    } 
  } 
