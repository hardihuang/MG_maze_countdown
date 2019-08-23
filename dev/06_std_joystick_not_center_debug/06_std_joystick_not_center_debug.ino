/*
========================================
  摇杆弹珠迷宫 v0.5.2  (2019.07.20)
========================================

【说明】
---------------------------------------
本硬件可通过摇杆控制2轴迷宫台面，进行X和Y轴的倾斜，从而控制迷宫中的小球移动
同时通过2位数码管进行计时，当小球成功走出迷宫击中压电片上时视为通过成功

通过单击按钮开始游戏，游戏中途可通过单击按钮退出游戏



【数码管】数码管具体管脚请看文件夹中数码管datasheet
---------------------------------------
  5 - D2
  4 - D3
  6 - D4
  7 - D5 (数字2共阴端，需串联电阻)
  9 - D6 
  10- D7
  8 - D8(数字1共阴端，需串联电阻)
  3 - D9
  1 - D10

【摇杆】
---------------------------------------
  VRX - A0
  VRY - A1
  SW  - A2

【舵机】
---------------------------------------
  servoA - D11
  servoB - D12 

【压电片】
---------------------------------------
  S - A3

  
*/


#include<Servo.h>
#include<SevSeg.h>

#define startBtn A2
#define senser A3

SevSeg myDisplay;
Servo servoA;
Servo servoB;

int defaultAngleA=94;
int defaultAngleB=101;

int joystickA_offset = 0;
int joystickB_offset = 0;

bool isStart = 0;
unsigned long timer = millis();
unsigned long blinkTimer = millis();
int countDown = 0;

void setup()
{
  Serial.begin(9600);
  joystickA_offset = 512-analogRead(A0);//需要放在servoSetup()之前
  joystickB_offset = 512-analogRead(A1);
  sevSegSetup();
  servoSetup();
  pinMode(startBtn, INPUT_PULLUP);
  
}

void loop()
{
  
//  Serial.print("A0: ");
//  Serial.print(analogRead(A0));
//  Serial.print("  A1: ");
//  Serial.print(analogRead(A1));
//  Serial.print("  offset A: ");
//  Serial.print(joystickA_offset);
//  Serial.print("  offset B:");
//  Serial.print(joystickB_offset);
//  Serial.print("  mapA ");
//  Serial.print(analogRead(A0)+joystickA_offset);
//  Serial.print("  mapB:");
//  Serial.print(analogRead(A1)+joystickB_offset);
//  Serial.print("  servoA: ");
//  Serial.print(servoA.read());
//  Serial.print("  servoB: ");
//  Serial.println(servoB.read());
  
  if(isWin() && isStart){
    wining();
  }
  if(isStart){
    servoA.write(map(analogRead(A0)+joystickA_offset,0,1023,defaultAngleA-10,defaultAngleA+10));
    servoB.write(map(analogRead(A1)+joystickB_offset,0,1023,defaultAngleB-10,defaultAngleA+10));

    //更新计时时间
    if(millis()-timer>=1000){
      timer = millis();
      if(countDown<99){//如果超过99秒依旧没通关，则为失败
        countDown++;
      }else{
        countDown=0;
        gameOver();
      }
    }
    Display(countDown);//刷新屏幕内容

    if(!digitalRead(startBtn)){//游戏中途按下按键，视为推出
      isStart=0;
      delay(300);//通过延时消抖，否则会自动开始下一局
    }
  }else{
    //如果游戏没开始，就一直在while循环中刷新屏幕内容，显示“--”，直到用户按下按钮为止
    while(digitalRead(startBtn)){
      myDisplay.DisplayString("--", 0);//display dash
    }
    
    //按下按钮后执行的游戏启动初始化
    isStart = 1;
    countDown = 0;
    //gameBegin();
    delay(500);
    timer = millis(); 
  }
}

//游戏开始动画（倒计时3下）
void gameBegin(){
  servoA.write(defaultAngleA);
  servoB.write(defaultAngleB);
  for(int i=3;i>0;i--){
    for(int k=0;k<100;k++){
      Display(i);
    }
    delay(500);
  }
  delay(300);
  
}

//游戏获胜动画（闪烁3下并一直显示时间，直到用户按下按键）
void wining(){
  blink3(countDown);
  servoA.write(defaultAngleA);
  servoB.write(defaultAngleB); 
  while(digitalRead(startBtn)){
    Display(countDown);
  }
  isStart = 0;
  delay(300);
}

//游戏失败动画（当时间超过99秒后，游戏失败）
void gameOver(){
  blink3(0);
  isStart = 0;
  servoA.write(defaultAngleA);
  servoB.write(defaultAngleB);
}

//检查弹珠是否掉落，成功通关
bool isWin(){
  if(analogRead(senser)>200){
    return true;
  }else{
    return false;  
  }
}

//屏幕闪烁3下
int blink3(int num){
  for(int i=0;i<3;i++){
    blinkTimer = millis();
    while(millis()-blinkTimer<100){
      Display(num);
    }
    delay(100);
  }
}

//数码管显示内容
void Display(int num)
{
  char tempString[10]; //Used for sprintf
  sprintf(tempString, "%2d", num); //Convert deciSecond into a string that is right adjusted
  myDisplay.DisplayString(tempString, 0); //(numberToDisplay, decimal point location in binary number [4 means the third digit])
}

//数码管初始化
void sevSegSetup(){
  int displayType = COMMON_CATHODE;
  int digit1 = 8; //Pin 1
  int digit2 = 5; //Pin 10
  int digit3 = 11; //Pin 4
  int digit4 = 13; //Pin 6
  
  int segA = 7; //Pin 12
  int segB = 6; //Pin 11
  int segC = 10; //Pin 3
  int segD = 3; //Pin 8
  int segE = 9; //Pin 2
  int segF = 4; //Pin 9
  int segG = 2; //Pin 7
  int segDP= A4; //Pin 5
  int numberOfDigits = 2; 
  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level
}

//舵机初始化
void servoSetup(){
  servoA.attach(11); 
  servoB.attach(12); 
//  defaultAngleA=servoA.read();
//  defaultAngleA=servoB.read();
//  servoA.write(defaultAngleA);
//  servoB.write(defaultAngleB); 
  servoA.write(map(analogRead(A0)+joystickA_offset,0,1023,defaultAngleA-10,defaultAngleA+10));
  servoB.write(map(analogRead(A1)+joystickB_offset,0,1023,defaultAngleB-10,defaultAngleA+10)); 
}
