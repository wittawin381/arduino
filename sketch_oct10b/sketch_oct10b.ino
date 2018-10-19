//give each value a name to represent each state
#define goN 0
#define wN 1
#define goE 2
#define wE 3
#define Walk 4
#define wWalk 5

//create structure for state
struct state{
  unsigned long out;
  int wait;
  int next[8];
};

//define a value for each state (light output, delaytime, next state)
typedef struct state sType;
sType FSM[6] = {
  {0x9E,3000,{goN,wN,goN,wN,wN,wN,wN,wN}}, 
  {0xAE,500,{goE,goE,goE,goE,Walk,goE,Walk,goE}},  
  {0xB3,3000,{goE,goE,wE,wE,wE,wE,wE,wE}}, 
  {0xB5,500,{goN,goN,goN,goN,Walk,Walk,Walk,Walk}}, 
  {0x76,3000,{Walk,wWalk,wWalk,wWalk,Walk,wWalk,wWalk,wWalk,}}, 
  {0xF6,350,{goN,goE,goN,goN,goN,goE,goN,goN}}
};

int cState = 0; //variable to hold the current state
int input,input1,input2,input3; //variable to hold the input value (1 or 0).but, input variable is for containing summation of input1,2 and 3

void setup() {
  // define a pin mode for each pin
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  pinMode(19,INPUT);
  Serial.begin(9600);
}

void loop() {
  //display a light according to the out variable value in state structure
  digitalWrite(6,FSM[cState].out & B00000001);
  digitalWrite(7,FSM[cState].out & B00000010);
  digitalWrite(8,FSM[cState].out & B00000100);
  digitalWrite(9,FSM[cState].out & B00001000);
  digitalWrite(10,FSM[cState].out & B00010000);
  digitalWrite(11,FSM[cState].out & B00100000);
  digitalWrite(15,FSM[cState].out & B01000000);
  digitalWrite(17,FSM[cState].out & B10000000);
  
  //make people crossing light blink before turn to red
  if(cState == 5)
  {
    for(int i = 0; i < 3; i++)
    {
      digitalWrite(17,FSM[cState].out & B10000000);
      delay(250);
      digitalWrite(17,!(FSM[cState].out & B10000000));
      delay(250);
    }
  }

  //delay to make light visible for an amount of time before go to next state
  delay(FSM[cState].wait);

  //read the input
  input1=digitalRead(12);
  input2=digitalRead(13);
  input3=digitalRead(19);
  
  //calculate the input value to determine the next state
  input = input1*4 + input2*2 + input3;
  cState = FSM[cState].next[input];
  
}
