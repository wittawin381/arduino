#define A 3
#define B 2
#define C 1
#define D 0

struct state
  {
    char out;
    int wait;
    int next[4];
    
  };
  typedef struct state sType;
  sType FSM[4] = {
    {'D',1000,{C,B,A,D}},
    {'C',1000,{B,A,D,C}},
    {'B',1000,{A,D,C,B}},
    {'A',1000,{D,C,B,A}}
  };
  int cState = 3;
  int input,input1,input2;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(FSM[cState].out);
  Serial.println(digitalRead(4));
  delay(FSM[cState].wait);
  input1 = digitalRead(3);
  input2 = digitalRead(4);
  input = (input1 * 2) + input2;
  cState = FSM[cState].next[input];
}
