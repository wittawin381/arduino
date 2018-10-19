#include <SoftwareSerial.h> 
String inputString = ""; // a string to hold incoming data 
SoftwareSerial mySerial(10,11); // SoftwareSerial(rxPin, txPin) 
 void setup() {     
  // Open serial communications and wait for port to open:     
  Serial.begin(4800);     
  //Serial.println("Hello World");     // set the data rate for the SoftwareSerialport     
  mySerial.begin(4800); // recommentlow speed     
  mySerial.println("Software Serial->Hello, world?"); 
  } 
  
  void loop() // run over and over 
  {     
//   while(Serial.available()) // recheck serial is available            
//  {   
//    char inChar=(char)Serial.read(); // get the new byte:  
//    inputString+=inChar; // add it to the inputString:
//    mySerial.write(inChar);
//    if (inChar=='\r') // check received 'enter' (0x0D)   
//    {        
//      //mySerial.print("TX from Software serial -> ");        
//        
//      //Serial.println("KUAY");      
//      inputString="";  
//    }             
//  }

  if(mySerial.available()){
    Serial.print((char)mySerial.read());
  }
  while(Serial.available()) // test this condition by connecting pin rxsoftware with pin'0'(Rx)               
  {  
    char CharKuay = (char)Serial.read();
    mySerial.print(CharKuay);  
    Serial.print(CharKuay); 
    //mySerial.write(CharKuay);            
  }  
} 
  
