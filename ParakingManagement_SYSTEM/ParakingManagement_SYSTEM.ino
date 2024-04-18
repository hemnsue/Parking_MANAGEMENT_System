#include <Servo.h>
#include <LiquidCrystal.h>
#include <string.h>
Servo myservo1;
Servo myservo2;
int Contrast=100;
LiquidCrystal lcd(12,11,5,4,3,2);
int ir[5]={A0,A1,A2,A4,A5};
int irs[5]={0,0,0,0,0};
char update[]="";
int start=0;
int slots_info;
int deku=0;

void setup() {
  Serial.begin(9600);
  for(int i=0;i<5;i++){
    pinMode(ir[i],INPUT);
  }
  myservo2.attach(10);
  myservo1.attach(9);
  analogWrite(6,Contrast);
  lcd.begin(16,2);
}

void loop(){
  initials();
  slot_printing();
  gate_entry();
  gate_exit();
}

void initials(){
  if (start==0){
    myservo1.write(0);
    myservo2.write(0);
    start=start+1;
  }
}

void slot_printing(){
  slots_info=checking();
  if (slots_info==0){
    deku=0;
  }
  else{
    sprintf(update,"%d",slots_info);
    deku=1;
  }
}

int checking(){
  int slots_available=0;
  for(int i=0;i<=2;i++){
    irs[i]=digitalRead(ir[i]);
    if (irs[i]==1){
      slots_available=slots_available*10+(i+1);
    }
  }
  return slots_available;
}

void gate_entry(){
  irs[4]=digitalRead(ir[4]);
  int count1=0;
  if (irs[4]==0){
    if (slots_info!=0){
      myservo1.write(110);
      count1++;
      delay(3000);
    }
  }
  else if(irs[4]==1 && count1>0){
    delay(3000);
    myservo1.write(0);
    count1=0;
  }
  else if(irs[4]==1 && count1==0){
    myservo1.write(0);
  }
}

void gate_exit(){
  irs[3]=digitalRead(ir[3]);
  int count2=0;
    if (irs[3]==0){
    if (strlen(update)!=3){
      myservo2.write(110);
      count2++;
      delay(3000);
    }
  }
  else if(irs[3]==1 && count2>0){
    delay(3000);
    myservo2.write(0);
    count2=0;
  }
  else if(irs[3]==1 && count2==0){
    myservo2.write(0);
  }
}

void printing(){
  if (deku==0){
    lcd.setCursor(0,0);
    lcd.print("SLOTS AVAILABLE");
    lcd.setCursor(0 ,1);
    lcd.print("0");
  }
  else if(deku==1){
    lcd.setCursor(0,0);
    lcd.print("SLOTS AVAILABLE");
    lcd.setCursor(0 ,1);
    lcd.print(update);
  }
}