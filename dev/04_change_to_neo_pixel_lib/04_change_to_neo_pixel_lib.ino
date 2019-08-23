#include<Servo.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define startBtn A3
#define senser A2

#define PIN        5 
#define NUMPIXELS 10
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

bool isStart = 0;
unsigned long timer = millis();
unsigned long blinkTimer = millis();
int counter = 0;

Servo servo_2;
Servo servo_3;

int count=0;

void setup()
{
  //Serial.begin(9600);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  pinMode(startBtn, INPUT_PULLUP);
  
  servo_2.attach(2); 
  servo_3.attach(3); 
  
  servo_2.write(84);
  servo_3.write(81);
  delay(500);
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
      if(counter<100){
        counter++;
      }else{
        counter=0;
        gameOver();
      }
      showLed();
    }
    
  }else{
    for(int i=0;i<10;i++){
      pixels.setPixelColor(i, pixels.Color(150,150,150));
    }
    pixels.show();
    if(!digitalRead(startBtn)){
      isStart = 1;
      counter = 0;
      gameBegin();
      timer = millis();  
    }
  }
}

void gameBegin(){
  servo_2.write(84);
  servo_3.write(81);
  int colorVal[3];
  for(int k=0;k<3;k++){
    switch(k){
      case 0:
        colorVal[0]=150;
        colorVal[1]=0;
        colorVal[2]=0;
        break;
      case 1:
        colorVal[0]=150;
        colorVal[1]=150;
        colorVal[2]=0;
        break;
      case 2:
        colorVal[0]=0;
        colorVal[1]=150;
        colorVal[2]=0;
        break;  
    }
    for(int i=0;i<10;i++){
      pixels.setPixelColor(i, pixels.Color(colorVal[0], colorVal[1], colorVal[2]));
    }
    pixels.show();
    delay(500);
    pixels.clear();
    delay(200);
  }
  
}

bool isWin(){
  if(analogRead(senser)>50){
    return true;
  }else{
    return false;  
  }
}

void wining(){
  blink3();
  servo_2.write(84);
  servo_3.write(81); 
  showLed();
  while(digitalRead(startBtn)){






    
  }
  isStart = 0;
  delay(300);
}

void blink3(){
  for(int i=0;i<3;i++){
    showLed();
    delay(500);
    pixels.clear();
    pixels.show();
    delay(200);
  }
}

void gameOver(){
  blink3();
  isStart = 0;
  counter=0;
  servo_2.write(84);
  servo_3.write(81);
}

void showLed(){
  pixels.clear();
  for(int i=0;i<counter/10;i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));
  }
  pixels.setPixelColor(counter%10, pixels.Color(0, 150, 0));
  pixels.show();
  delay(10);
}
