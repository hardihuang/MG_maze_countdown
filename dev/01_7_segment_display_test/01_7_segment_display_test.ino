#define COM1 4
#define COM2 5

unsigned char segment[7]={6,7,8,9,10,11,12};//pin number of segment A, segment B ...segment G
unsigned long timer = millis();
int countDown = 60;
unsigned char table[10][8] =                
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
  {0, 1,  1,  0,  1,  1,  1,  1}      //9
};

void setup()
{
  for(int i=0; i<7;i++){
    pinMode(segment[i],OUTPUT);
  }
  pinMode(COM1,OUTPUT);
  pinMode(COM2,OUTPUT);
}

void loop()
{
  if(millis()-timer>=1000){
    timer = millis();
    if(countDown>0){
      countDown--;  
    }else{
      countDown=60;
    }
  }
  Display(countDown);
}

void Display(int num)
{
  int digitA;
  int digitB;
  if(num<10 && num>=0){
    digitB = num;
    digitA = 0;
  }else if(num>=10 && num<=99){
    digitA = (num/10)%10;
    digitB = num%10;
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
    delay(10);
    digitalWrite(COM1,HIGH);
    digitalWrite(COM2,HIGH);
  }
}
