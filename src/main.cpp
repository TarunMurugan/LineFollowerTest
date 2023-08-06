#include <Arduino.h>

int lsa_pins[8]={21, 22, 23, 25, 26, 27, 32, 33},lsa_vals[8]={-6, -2, -1, 0, 0, 1, 2, 6},left_mot[2]={13,14},right_mot[2]={18,19},prev_offset=0;

int get_offset() {
int flag=0,offset=0;

  for(int i=0;i<8;i++) {
    if(digitalRead(lsa_pins[i])) {
      offset+=lsa_vals[i];
      flag=1;
      prev_offset=offset;
    }
  
  }
  
  if(flag==1){
  return offset;
  }
  else {
    if(prev_offset>0){
      return 10;
    }
    return -10;
  }


}
int integral_prior=0,integral,derivative,offset_prior=0,pwm_offset;
void move(int offset) {
  float kp=10,kd=0,ki=0,set_speed=70;
  integral = integral_prior + offset * 0.010;
  derivative= (offset-offset_prior)/0.010;
  pwm_offset=kp*offset+ki*integral+kd*derivative;
  analogWrite(left_mot[0],set_speed+pwm_offset);
  analogWrite(right_mot[0],set_speed-pwm_offset);


  offset_prior=offset;
  integral_prior = integral;
}

void setup() {
  for(int i=0; i<8; i++){
    pinMode(lsa_pins[i], INPUT);
  }
for(int i=0; i<2; i++){
    pinMode(right_mot[i], OUTPUT);
    pinMode(left_mot[i], OUTPUT);
    analogWrite(right_mot[i],0);
    analogWrite(left_mot[i],0);
  }

}

void loop() {
  move(get_offset());
  delay(10);
}
