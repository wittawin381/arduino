
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float a = 0;
  for(a = 0; a <= 90; a+=0.1){
    float sina = sin(a);
    float cosa = cos(a);
    Serial.print(sina);
    Serial.print(" ");
    Serial.print(cosa);
    delay(1);
  }
}
