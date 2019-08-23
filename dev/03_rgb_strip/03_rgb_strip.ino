#include<Servo.h>
#include <FastLED.h>

#define NUM_LEDS 10
#define DATA_PIN 10
#define BRIGHTNESS  32

#define COM1 4
#define COM2 5
#define startBtn A3
#define senser A2

CRGB leds[NUM_LEDS];

Servo servo_2;     //创建一个舵机，命名为servo_2
Servo servo_3;    //创建另外一个舵机，命名为servo_3


bool isStart = 0;
unsigned char segment[7]={6,7,8,9,10,11,12};//pin number of segment A, segment B ...segment G
unsigned long timer = millis();
unsigned long blinkTimer = millis();
int countDown = 0;
unsigned char table[12][8] =                
{
  {0, 0,  1,  1,  1,  1,  1,  1},     //0
  {0, 0,  0,  0,  0,  1,  1,  0},     //1
  {0, 1,  0,  1,  1,  0,  1,  1},     //2
  {0, 1,  0,  0,  1,  1,  1,  1},     //3
  {0, 1,  1,  0,  0,  1,  1,  0},     //4
  {0, 1,  1,  0,  1,  1,  0,  1},     //5
  {0, 1,  1,  1,  1,  1,  0,  1},     //6
  {0, 0,  0,  0,  0,  1,  1,  1},     //7
  {0, 1,  1,  1,  1,  1,  1,  1},     //8
  {0, 1,  1,  0,  1,  1,  1,  1},     //9
  {0, 0,  0,  0,  0,  0,  0,  0},     //space
  {0, 1,  0,  0,  0,  0,  0,  0}      //-
};

void setup()
{
  for(int i=0; i<7;i++){
    pinMode(segment[i],OUTPUT);
  }
  
  pinMode(COM1,OUTPUT);
  pinMode(COM2,OUTPUT);

  pinMode(startBtn, INPUT_PULLUP);
  servo_2.attach(2); 
  servo_3.attach(3); 
  servo_2.write(84);
  servo_3.write(81);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void loop()
{
  if(isWin() && isStart){
    wining();
  }
  if(isStart){
    servo_2.write(map(analogRead(A1),0,1023,74,94));
    servo_3.write(map(analogRead(A0),0,1023,71,91));
    if(millis()-timer>=1000){
      timer = millis();
      if(countDown<99){
        countDown++;
      }else{
        countDown=0;
        gameOver();
      }
    }
    showLed();
    Display(countDown);
  }else{
    Display(100);//display dash
    if(!digitalRead(startBtn)){
      isStart = 1;
      countDown = 0;
      gameBegin();
      timer = millis();  
    }
  }
}

void showLed(){
  for(int i=0;i<countDown;i++)  {
    leds[i] = CRGB::Blue;
  }
  FastLED.show();
}

void gameBegin(){
  servo_2.write(84);
  servo_3.write(81);
  for(int i=3;i>0;i--){
    Display(i);
    delay(300);
  }
  clearDisplay();
  delay(300);
  
}
void wining(){
  blink3(countDown);
  servo_2.write(84);
  servo_3.write(81); 
  while(digitalRead(startBtn)){
    Display(countDown);
  }
  isStart = 0;
  delay(300);
}

void gameOver(){
  blink3(0);
  isStart = 0;
  servo_2.write(84);
  servo_3.write(81);
}

bool isWin(){
  if(analogRead(senser)>50){
    return true;
  }else{
    return false;  
  }
}

int blink3(int num){

  for(int i=0;i<3;i++){
    blinkTimer = millis();
    while(millis()-blinkTimer<300){
      Display(num);
    }
    clearDisplay();
    delay(300);
    
  }
}

void clearDisplay(){
  digitalWrite(COM1,HIGH);
  digitalWrite(COM2,HIGH);
}

void Display(int num)
{
  int digitA;
  int digitB;
  if(num<10 && num>=0){
    digitB = num;
    digitA = 10;
  }else if(num>=10 && num<=99){
    digitA = (num/10)%10;
    digitB = num%10;
  }else if(num = 100){ //display dash
    digitA=11;
    digitB=11;
  }
  for(int i=0; i<7;i++){
    digitalWrite(segment[i],LOW);
  }

  for(int i=0;i<2;i++){
    if(i==0){
      digitalWrite(COM1,LOW);
      digitalWrite(COM2,HIGH);
      for(int k=0; k<7;k++){
        digitalWrite(segment[k],table[digitA][7-k]);
      }
    }else{
      digitalWrite(COM1,HIGH);
      digitalWrite(COM2,LOW);
      for(int k=0; k<8;k++){
        digitalWrite(segment[k],table[digitB][7-k]);
      }
    }
    delay(5);

  }
}
