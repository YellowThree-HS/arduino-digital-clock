
#include <U8glib.h>
#include<Keypad.h>
#include <EEPROM.h>
int addr[8] = {0,1,2,3,4,5,6,7};
U8GLIB_SSD1306_128X64 u8g(13, 11/*d1*/, 10/*cs*/, 12); 

const byte ROWS=4;
const byte COLS=4;
char keys[ROWS][COLS]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
byte rowPins[ROWS]={2,3,4,5};
byte colPins[COLS]={6,7,8,9};
Keypad keypad=Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);
char getkey();  //获取一个字符并显示在屏幕上
int transform(char c);  //将获取的数字类字符变成整型变量


static int h=15;static int mi=35;static int s=20;static int y=2022;static int mo=1;static int d=11; //定义年月日时分秒为静态变量
static int x,a,b,c;//a是世纪，b是年，c是月


void setup()
{
  u8g.setColorIndex(255);  //字体颜色，貌似不会改变  
  Serial.begin(9600); 
  u8g.setFont(u8g_font_8x13);   //字体大小
  u8g.setFontRefHeightExtendedText();   
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();   //颠倒过来
  pinMode(A1, OUTPUT); //设置pinBuzzer脚为输出状态

  static int value=0;
  value=EEPROM.read(addr[7]);
  if(value>=1&&value<255)
  {y=EEPROM.read(addr[0])*100+EEPROM.read(addr[1]);
  mo=EEPROM.read(addr[2]);
  d=EEPROM.read(addr[3]);
  h=EEPROM.read(addr[4]);
  mi=EEPROM.read(addr[5]);
  s=EEPROM.read(addr[6]);}
  value+=1;if(value>=255) value==1;
  EEPROM.write(addr[7],value);
}

void menu();  //显示菜单
char key1;   //避免修改时间时时间不对
void SwitchTimeSystem(); //修改时制A
int flag=24;  //表示24时制
void ChangeTheTime();  //修改时间B
void SetAlarms();  //设置闹钟C
int ah[3]={24};int ami[3]={60};char openornot[3] ={'#'}; //使用整型数组来存放闹钟，字符来判断闹钟开关
void buzzer();   //蜂鸣器响
void TimeKeeping();   //计时D

void loop()
{
  s++;
  if(s>=60)
  {mi++;s=s-60;
  if(mi>=60)
  {h++;mi=mi-60;
  if(h>=24)
  {h=0;d+=1;}
  switch (mo)  //判断是否月末
  {
    case 1:if(d==32) {mo+=1;d=1;}break;
    case 2:if(y%100==0)
    if(y%400==0) {if(d==30) {mo+=1;d=1;}
    else if(d==29) {mo+=1;d=1;}}
    else if(y%4==0)  {if(d==30) {mo+=1;d=1;}
    else if(d==29) {mo+=1;d=1;}}break;
    case 3:if(d==32) {mo+=1;d=1;}break;
    case 4:if(d==31) {mo+=1;d=1;}break;
    case 5:if(d==32) {mo+=1;d=1;}break;
    case 6:if(d==31) {mo+=1;d=1;}break;
    case 7:if(d==32) {mo+=1;d=1;}break;
    case 8:if(d==32) {mo+=1;d=1;}break;
    case 9:if(d==31) {mo+=1;d=1;}break;
    case 10:if(d==32) {mo+=1;d=1;}break;
    case 11:if(d==31) {mo+=1;d=1;}break;
    case 12:if(d==32) {mo+=1;d=1;y+=1; }break;
  }
  }
  }
  a=y/100;
  b=y%100;
  if(mo<3)
  {c=mo+12;
  b=b-1;}
  else
  c=mo;
  x=(b+(int)(b/4.0)+(int)(a/4.0)-2*a+(int)(26*(c+1)/10.0)+d-1)%7;  //计算星期几

for(int i=0;i<3;i++)//判断闹钟
{
  if((ah[i]==h)&&(ami[i]==mi))
   if(s==0) 
  {int lasttime0,duration0; 
  lasttime0=millis();
  if(ah[i]==h&&ami[i]==mi)
  if(s==0&&openornot[i]=='*')
     buzzer();   //蜂鸣器响
 duration0 = (millis()-lasttime0)/5000;
 s=duration0+s;mi=s/60+mi;s=s%60;} //进入闹钟的时间差   
 }                                        


  
  
    u8g.firstPage();  //屏幕显示
   do{
    u8g.setPrintPos(5,0);
    u8g.print(y);
    u8g.print("-");
    u8g.print(mo);
    u8g.print("-");
    u8g.print(d);
     u8g.setPrintPos(90,0);
      if(x==0)
      u8g.print("SUN");
      else if(x==1)
      u8g.print("MON");
      else if(x==2)
      u8g.print("TUE");
       else if(x==3)
      u8g.print("WEN");
       else if(x==4)
      u8g.print("THR");
       else if(x==5)
      u8g.print("FRI");
       else if(x==6)
      u8g.print("SAT");
    
      u8g.setPrintPos(30,25);
        if(flag==24)
        u8g.print(h);
        else if(flag==12)  //判断时制
        u8g.print(h-12);
      u8g.print(":");
      u8g.print(mi);
      u8g.print(":");
      u8g.print(s);  
        if(flag==12)
              if(h>=12)
                u8g.print(" p.m.");
              else
                u8g.print(" a.m.");
      
      u8g.setPrintPos(0,50);
      u8g.print("menu(*)");
      //右下角是最后温度传感器的温度显示
      }
      while(u8g.nextPage());
  
  char key=keypad.getKey(); //判断是否进入菜单
  if(key=='*')
 {int lasttime,duration; 
  lasttime=millis();
 menu();
 if(key1!='B')
 {duration = (millis()-lasttime)/5000;
 s=duration+s;mi=s/60+mi;s=s%60;}}//进入菜单的时间不能超过1小时 
 
  EEPROM.write(addr[0], y/100);
  EEPROM.write(addr[1],y%100);
  EEPROM.write(addr[2],mo);
  EEPROM.write(addr[3],d);
  EEPROM.write(addr[4],h);
  EEPROM.write(addr[5],mi);
  EEPROM.write(addr[6],s);
  delay(996);
}



char getkey()  //获取一个字符并显示在屏幕上
{
  char c;
  do{
  c=keypad.getKey();
  }while(c=='\0');
 u8g.firstPage();
 do{
  u8g.setPrintPos(5,5);
  u8g.print(c);
 }while(u8g.nextPage());
  return c;
}

int transform(char c)   //将获取的数字类字符变成整型变量
{
  switch (c)
  {
    case '1':return 1;break;
    case '2':return 2;break;
    case '3':return 3;break;
    case '4':return 4;break;
    case '5':return 5;break;
    case '6':return 6;break;
    case '7':return 7;break;
    case '8':return 8;break;
    case '9':return 9;break;
    case '0':return 0;break;
    default:return 0;
  }
}

void buzzer()   //蜂鸣器响
{  
   analogWrite(A1,255);
   delay(2000);
   analogWrite(A1,225);
   delay(2000);
   analogWrite(A1,195);
   delay(2000);
   analogWrite(A1,165);
   delay(2000);
   analogWrite(A1,135);
   delay(2000);
   analogWrite(A1,105);
   delay(2000);
   analogWrite(A1,75);
   delay(2000);
   analogWrite(A1,45);
   delay(2000);
   analogWrite(A1,15);
   delay(2000);
}

void menu()
{  u8g.firstPage();  //屏幕显示
   do{
  
  u8g.setPrintPos(5,20);
  u8g.println("switch time system(A)");
  u8g.setPrintPos(5,30);
  u8g.print("change the time(B)");
  u8g.setPrintPos(5,40);
  u8g.print("set alarms(C)");
  u8g.setPrintPos(5,50);
  u8g.print("timekeeping(D)");
  u8g.setPrintPos(60,0);
  u8g.print("cancel(#)");
  
  }
      while(u8g.nextPage());

  
  do{
  key1=keypad.getKey();
  }while(key1=='\0');
  if(key1=='A')
  SwitchTimeSystem();
  if(key1=='B')
  ChangeTheTime();
  if(key1=='C')
  SetAlarms();
  if(key1=='D')
  TimeKeeping();
  if(key1=='#');
}


void SwitchTimeSystem() //修改时制A
{
 if(flag==24)flag=12;
 else flag=24;
 }

void ChangeTheTime()  //修改时间B
{
  u8g.firstPage();  //屏幕显示
   do{
    u8g.setPrintPos(0,20);
    u8g.println("Please enter a");
    u8g.setPrintPos(0,30);
    u8g.println("14-digit integer");
    u8g.setPrintPos(0,40);
    u8g.println("Press any key to continue");
    u8g.setPrintPos(0,50);
    u8g.println("Enter (*) to end");
   }while(u8g.nextPage());
   do{
  y=transform(getkey())*1000+transform(getkey())*100+transform(getkey())*10+transform(getkey());
            mo=transform(getkey())*10+transform(getkey());
            d=transform(getkey())*10+transform(getkey());
            h=transform(getkey())*10+transform(getkey());
            mi=transform(getkey())*10+transform(getkey());
            s=0; 
   }while(getkey()!='*');
}

void SetAlarms()  //设置闹钟C
{
   A:u8g.firstPage();//显示三个闹钟
  do{
    u8g.setPrintPos(0,0);
    u8g.print("Press ABC to choose alarms");
    u8g.setPrintPos(0,10);
    u8g.print("Press # to cancel");
    for(int i=0;i<3;i++)
    {u8g.setPrintPos(0,20+i*10);
    u8g.print(ah[i]);
    u8g.print(":");
    u8g.print(ami[i]);
    u8g.print(openornot[i]);}
  }while(u8g.nextPage());
  int i;
  switch (getkey())//选择A B C 闹钟
  {
    case 'A':i=0;break;
    case 'B':i=1;break;
    case 'C':i=2;break;
  }
  u8g.firstPage();//按A修改时间，按*打开或关闭闹钟
    do{u8g.setPrintPos(0,0);u8g.print(ah[i]);u8g.print(":");u8g.print(ami[i]);
    u8g.setPrintPos(0,25);u8g.print("Press A to change");
    u8g.setPrintPos(0,35);u8g.print("Press * to open or not");
    u8g.setPrintPos(0,45);u8g.print("Press # to cancel");
   }while (u8g.nextPage());
   char key2=getkey();if(key2=='*'){if(openornot[i]=='#')openornot[i]='*';else openornot[i]='#';} 
   else if(key2=='A')
  {
  u8g.firstPage();
  do{
    u8g.setPrintPos(0,20);
    u8g.print("Follow the 24 hour clock");
    u8g.setPrintPos(0,30);
    u8g.print("Enter a two-digit hour");
    u8g.setPrintPos(0,40);
    u8g.print("a two-digit minutes.");
  }while(u8g.nextPage());
 ah[i]=(getkey()-'0')*10+(getkey()-'0');
 ami[i]=(getkey()-'0')*10+(getkey()-'0');goto A; //重新回到闹钟的页面}
}
}

void TimeKeeping()   //计时D
{
  u8g.firstPage();
  do{
    u8g.setPrintPos(0,20);
    u8g.println("To start timing（A)");
    u8g.setPrintPos(0,30);
    u8g.println("To pause the timer（B）");
    u8g.setPrintPos(0,40);
    u8g.println("To reset the timer（C）");
    u8g.setPrintPos(0,50);
    u8g.println("To close the timer（#）");
  }while(u8g.nextPage());
  char k;  
  do{
  k=keypad.getKey();
  }while(k=='\0');
  if(k=='A')
    {retime:int kh=0;int kmi=0;int ks=0;
  do{
   cont:u8g.firstPage();  //屏幕显示
   do{
      u8g.setPrintPos(30,25);
      u8g.print(kh);
      u8g.print(":");
      u8g.print(kmi);
      u8g.print(":");
      u8g.print(ks);
     }while(u8g.nextPage());
  ks++;
  if(ks>=60)
     {kmi++;ks=ks-60;}
  if(kmi>=60)
     {kh++;kmi=kmi-60;}
     k=keypad.getKey();
  delay(996); 
}while(k!='B'&&k!='C'&&k!='#');}
  
  switch (k)
  {
    case 'B': {u8g.firstPage() ;do{u8g.setPrintPos(0,35);u8g.print("continue B");}
    while(u8g.nextPage());char key4;do{key4=keypad.getKey();
  }while(key4=='\0');if(key4=='B') goto cont;}  //cont指向继续计时
    case 'C': {u8g.firstPage() ;do{u8g.setPrintPos(0,35);u8g.print("retime A");}
    while(u8g.nextPage());char key5; do{key5=keypad.getKey();
  }while(key5=='\0');if(key5=='A') goto retime;}  //retime指向重新计时
    case '#':break;
  }
}
