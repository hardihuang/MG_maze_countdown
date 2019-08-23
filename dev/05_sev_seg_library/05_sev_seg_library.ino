/*
========================================
  摇杆弹珠迷宫游戏 v0.5
  by Hardi Huang  (2019.07.20)
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
  9 - D6 (数字1共阴端，需串联电阻)
  10- D7
  8 - D8
  3 - D9
  1 - D10

【摇杆】
---------------------------------------
  VRX - A0
  VRY - A1
  SW  - A3

【舵机】
---------------------------------------
  servo2 - D11
  servo3 - A5 

【压电片】
---------------------------------------
  S - A2

  
*/


#include<Servo.h>
#include<SevSeg.h>

#define startBtn A3
#define senser A2

SevSeg myDisplay;
Servo servo_2;
Servo servo_3;

bool isStart = 0;
unsigned long timer = millis();
unsigned long blinkTimer = millis();
int countDown = 0;

void setup()
{
  sevSegSetup();
  servoSetup();
  pinMode(startBtn, INPUT_PULLUP);
}

void loop()
{
  if(isWin() && isStart){
    wining();
  }
  if(isStart){
    servo_2.write(map(analogRead(A1),0,1023,74,94));
    servo_3.write(map(analogRead(A0),0,1023,71,91));

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
    gameBegin();
    timer = millis(); 
  }
}

//游戏开始动画（倒计时3下）
void gameBegin(){
  servo_2.write(84);
  servo_3.write(81);
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
  servo_2.write(84);
  servo_3.write(81); 
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
  servo_2.write(84);
  servo_3.write(81);
}

//检查弹珠是否掉落，成功通关
bool isWin(){
  if(analogRead(senser)>50){
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
  int segDP= 12; //Pin 5
  int numberOfDigits = 2; 
  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level
}

//舵机初始化
void servoSetup(){
  servo_2.attach(11); 
  servo_3.attach(A5); 
  servo_2.write(84);
  servo_3.write(81);  
}
