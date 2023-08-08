#include <Arduino.h>

int lsa_pins[8]={A3, 2, A4, 3, A5, 4, A6, 5},lsa_vals[8]={-7, -3, -1, 0, 0, 1, 3, 7},right_mot[2]={6,8},left_mot[2]={9,10},prev_offset=0;
#define led 7
#define button 11

int get_offset() {
float flag=0,offset=0,flag_line_over=0,flag_white=0;

  for(int i=0;i<6;i++) {
    if(digitalRead(lsa_pins[i])) {
      offset+=lsa_vals[i];
      flag++;
      prev_offset=offset;
      flag_line_over=1;
    } 
    else{
      if(flag_line_over==1){
        flag_white=1;
        break;
      }
    }
  }
  if(analogRead(lsa_pins[6])>1000 && flag_white==0){
      offset+=lsa_vals[6];
      flag++;
      prev_offset=offset;
  }
  else{
    flag_white=1;
  }
  if(digitalRead(lsa_pins[7]) && flag_white==0) {
      offset+=lsa_vals[7];
      flag++;
      prev_offset=offset;
  } 

  if(flag!=0){
    if(flag<3){
    return offset/flag;
    }
    else{
      return offset*2;
    }
  }
  else {
    return prev_offset;
  }


}
int integral_prior=0,integral,derivative,offset_prior=0,pwm_offset;
void move(float offset) {
  float kp=7,kd=0,ki=0,set_speed=70;
  integral = integral_prior + offset * 0.010;
  derivative= (offset-offset_prior)/0.010;
  pwm_offset=kp*offset+ki*integral+kd*derivative;
  digitalWrite(right_mot[1], 0);
  digitalWrite(left_mot[1], 0);
  analogWrite(left_mot[0],(set_speed+pwm_offset)<0?0:(set_speed+pwm_offset));
  analogWrite(right_mot[0],(set_speed-pwm_offset)<0?0:(set_speed-pwm_offset));
  Serial.print("pwm:");Serial.print(set_speed+pwm_offset);Serial.print(",");Serial.println(set_speed-pwm_offset);


  offset_prior=offset;
  integral_prior = integral;
}

void setup() 
{
  Serial.begin(9600);
  for(int i=0; i<8; i++){
    pinMode(lsa_pins[i], INPUT);
  }
for(int i=0; i<2; i++){
    pinMode(right_mot[i], OUTPUT);
    pinMode(left_mot[i], OUTPUT);
  }
  digitalWrite(right_mot[1], 0);
  digitalWrite(left_mot[1], 0);
  analogWrite(right_mot[0],0);
  analogWrite(left_mot[0],0);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(led,1);
  while(!digitalRead(button));
  digitalWrite(led,0);
}

void loop() {
  float t=get_offset(); 
  move(t); 
  Serial.println(t);
  delay(10);
}
