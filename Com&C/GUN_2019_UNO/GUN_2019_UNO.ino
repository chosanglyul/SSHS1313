#include<IRremote.h>
#include <MFRC522.h> 
#include<DFPlayer_Mini_Mp3.h>
#include<SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#define SS_PIN 10
#define RST_PIN 9
#define busy A0
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
#define fset 1 //연사
#define del 100 // delay time(ms)
#define re 2000 //reload time(ms)
#define ircode 5 //ir code (blit)
#define SS 3 //소음기 총소리 파일번호
#define SV 30 //소음기 소리 크기
#define RV 15 // 재장전 소리 크기
IRsend irsend;
SoftwareSerial mp3Serial(4, 5); // RX, TX
SoftwareSerial data(A2, A3);
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C LCD 객체 선언
int maxblit=15; //10발
long long int gt = 0;
int RECV_PIN1 = A1;
int Hp = 200;
int gauge = 0;
int killcode = 0;
int set=1; // 0: 단발 1: 연사 2: 장전중
int reloadPin=8;
int firePin=7;
int fire=1; //발사버튼누름
int prevfire=1;
long long int st=0; // 최근총발사시간
int blit=0; //총알수
int totblit=0;
//int totblit=120; //실험용
//String remnant[30];
int i;
long long int st2=0; // 재장전 시작시간
long long int T;  // 현재 시간
int firesound=1;
int firevolume=30;
//13-9 8 7 6 3 핀 사용
int helmet_level = 0;  //뚝배기 레벨
int armor_level = 0;  // 갑옷 레벨
long long int itemtime=0;
long long int Temtime[4]={10000,6000,6000,4000};//기다리는시간   //
int tem=-1; //템종류 1 의키 2 구상 3 진통 4 드링크
int (*restem)[5];
int card;
IRrecv irrecv1(RECV_PIN1);
decode_results results;
int medickitList [4][5]={
  {114,241,236,28,0},
  {114,186,100,28,0},
  {114,170,146,28,0},
  {114,207,27,28,0}
};
  int gusangList [7][5]={
  {114,134,87,28,0},
  {114,120,1,28,0},
  {130,18,157,28,0},
  {130,48,94,28,0},
  {114,156,13,28,0},
  {114,97,139,28,0},
  {114,151,187,28,0}
};
int drinkList [8][5]={
  {114,211,121,28,0},
  {114,190,62,28,0},
  {114,73,60,14,0},
  {114,119,247,14,0},
  {114,104,153,28,0},
  {114,192,8,28,0},
  {114,77,39,28,0},
  {130,3,31,28,0}
  };
int jintongList [5][5]={
  {114,123,48,14,0},
  {130,40,25,28,0},
  {130,34,221,28,0},
  {114,76,220,28,0},
  {114,67,220,14,0}
  };
int gapba1List[5][5]={
  {82,48,45,14,0},
  {2,41,216,14,0},
  {2,71,6,14,0},
  {130,31,74,28,0},
  {23,186,24,195,0}
};
int gapba2List[4][5]={
  {2,228,254,14,0},
  {2,48,71,14,0},
  {2,114,31,14,0},
  {82,43,243,14,0}
};
int gapba3List[3][5]={
  {82,22,182,14,0},
  {2,101,131,28,0},
  {34,59,194,28,0}
};
int helmet1List[5][5]={
  {66,226,131,14,0},
  {66,151,56,14,0},
  {226,183,7,13,0},
  {66,119,238,14,0},
  {66,135,137,14,0}
};
int helmet2List[4][5]={
  {66,173,74,14,0},
  {66,192,159,14,0},
  {226,70,153,13,0},
  {66,196,34,14,0}
};
int helmet3List[3][5]={
  {82,52,94,14,0},
  {98,221,193,14,0},
  {66,231,35,14,0}
};
int blitList[15][5]={ //총알
  {226,191,150,13,0},
  {226,132,164,13,0},
  {226,95,203,13,0},
  {210,219,130,13,0},
  {226,71,167,13,0},
  {114,55,78,28,0},
  {226,94,140,13,0},
  {226,153,149,13,0},
  {226,20,231,13,0},
  {114,61,57,28,0},
  {226,71,242,13,0},
  {226,45,4,13,0},
  {210,226,189,13,0},
  {226,5,79,13,0},
  {114,53,136,28,0}
};
int suppressorList[3][5]={ //소음기
  {2,89,144,28,0},
  {2,92,245,14,0},
  {2,11,125,28,0}
};
int exmagList[5][5]={ //대용량탄창
  {98,200,185,14,0},
  {66,245,126,14,0},
  {82,3,11,14,0},
  {82,49,30,14,0},
  {82,66,245,14,0}
};
int arrrange[]={4,7,8,5,5,4,3,5,4,3,15,3,5};

void sendInfo() {
  static long long last = 0;
  if(millis() - last < 1000) return;
  last = millis();
  data.write((byte)0);
  data.write((byte)ircode);
  data.write((byte)fset);
  data.write((byte)Hp);
  data.write((byte)gauge);
  data.write((byte)maxblit);
  data.write((byte)blit);
  data.write((byte)totblit);
  data.write((byte)armor_level);
  data.write((byte)firesound);
}

int TemCheck (int itemnum,int *code, int list[][5])
{
  //int res=1;
  for(i=0;i<arrrange[itemnum];i++)
  {
    if(code[0] && list[i][0] && code[1] == list[i][1] && code[2] == list[i][2] && code[3] == list[i][3] && code[4] ==list[i][4])
    {
      list[i][4]+=1;
      return 1;
    }
  }
  return 0;
}


void Fire()
{
  if(blit>0)
  {
      irsend.sendSony(ircode, 20);
      blit--;
      mp3_play(firesound);//SD카드에 저장 ex)0001총소리.MP3------------------------------------------------------------
      //analogWrite(shake,motorpower);//진동모터를 200의 힘으로 진동
      st=millis();
//      Serial.print(1);
  }
}

 byte SpecialChar1[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte SpecialChar2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
int damage(int n)//적외선 코드받아 데미지 리턴
{
  double damageconstant = 1.000000;
  switch(armor_level){
    case 1:
    damageconstant = 0.700000;
    break;
    case 2:
    damageconstant = 0.600000;
    break;
    case 3:
    damageconstant = 0.450000;
    break;
    default:
    damageconstant = 1.000000;
    break;
  }
  data.write((byte)1);
  data.write((byte)ircode);
  data.write((byte)n);
  data.write((byte)(damageconstant*40));
  return (int)(damageconstant*40);
   
  //맞은 소리 내기------------------------------------------------------------
  //mp3_play(2);//0002맞는소리.MP3
}
void setup() {
  Serial.begin(9600);
  mp3Serial.begin(9600);
  SPI.begin();
  data.begin(9600);
  pinMode(firePin,INPUT_PULLUP);
  pinMode(reloadPin,INPUT_PULLUP);
  //pinMode(shake,OUTPUT);
  pinMode(busy,INPUT);
  mfrc522.PCD_Init();
  mp3_set_serial(mp3Serial); 
  lcd.begin(); // lcd를 사용을 시작합니다.
  lcd.clear();
  lcd.backlight();
  lcd.createChar(0, SpecialChar1);
  lcd.createChar(1, SpecialChar2);
  lcd.setCursor(12,0);
  lcd.print("[ / ]");
  lcd.setCursor(13,0);
  lcd.print(helmet_level);
  lcd.setCursor(15,0);
  lcd.print(armor_level);
  lcd.setCursor(3, 2);
  lcd.print("max15");
  irrecv1.enableIRIn(); // Start the receiver
}

void loop() {
if(Hp>0){
  T=millis();
  prevfire=fire;
  fire=digitalRead(firePin);
  switch(set) {
    case 0://단발
    if(fire==0&&prevfire==1)
      Fire();
    break;
    case 1://연사
    if(fire==0 && (prevfire==1 || T-st>del))
      Fire();
    break;
    case 2://장전 대기중
    if(digitalRead(busy)) {
      st2=T;
      set=fset;
      if(totblit<maxblit-blit) { 
        blit+=totblit;
        totblit=0; 
      } else {
        totblit-=maxblit-blit;
        blit=maxblit;
      }
      mp3_set_volume(firevolume);
    }
    break;
  }

  byte irrecieving = irrecv1.decode(&results);
  irrecv1.enableIRIn(); // Start the receive
  if (irrecieving) {
      int n = results.value;
      Serial.println(n);
      if(n>=1 && n<=5 ){
        irrecv1.resume(); // Receive the next value
        Hp -= damage(n);
        killcode = n;
     }
    }
    //GAUGE->Hp
    if (gauge <= 0)
      gt = millis();
    if (millis() - gt > 2000)
    {
      if(Hp<200) 
      Hp += 1;
      gauge -= 2;
      gt = millis();
    }
    //LCD 에 정보 띄우기----------------------------------------------------
    //HP XXX/200  XX KILL
    //M416 30/270
    //GAUGE
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HP ");
    lcd.setCursor(4,0);
    if(Hp<10)
      lcd.write(byte(1));
    lcd.setCursor(5,0);
    if(Hp<100)
      lcd.write(byte(1));
    lcd.setCursor(3,1);
    lcd.print(blit);
    if(blit<10)
      lcd.write(byte(1));
    lcd.print("/");
    lcd.print(totblit);
    if(totblit<100)
    lcd.write(byte(1));
    if(totblit<10)
    lcd.write(byte(1));
    lcd.setCursor(3, 0);
    lcd.print(Hp);
    lcd.setCursor(6, 0);
    lcd.print("/200");
    lcd.setCursor(0, 3);
    for (int i = 0; i < 20; i++) {
      if (gauge <= i * 5)
        lcd.write(byte(1));
      else
        lcd.write(byte(0));
    }

    if(T-st>100)

    //analogWrite(shake,0);//진동모터 끄기



  //재장전

  if(digitalRead(reloadPin)==0 && set!=2 && set != -1 && totblit>0)
    {
      set=2;
      st2=millis();
      mp3_set_volume(RV);
      mp3_play(2);
    }   



  //RFID 인식---------------------------------------------------------------------------------------------------------

// Serial.println((long)T-millis());

  if( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() )

  {
//    Serial.print(1);
    int code[5];

    for (byte i = 0; i < 4; i++) {               // 태그의 ID출력하는 반복문.태그의 ID사이즈(4)까지

    code[i]=mfrc522.uid.uidByte[i];        // mfrc.uid.uidByte[0] ~ mfrc.uid.uidByte[3]까지 출력

    }

    code[4]=0;

  //총알
   if (Hp<200)
  {
    if(TemCheck(0,code,medickitList)) 
    {
//      Serial.print(1);
      itemtime=millis();
      tem=0;
    }
  }
     if (Hp<140)
  {
    if (TemCheck(1,code,gusangList)) 
    {
//      Serial.print(1);
      itemtime=millis();
      tem=1;
    }
  }
  //에너지드링크
  if (TemCheck(3,code,drinkList)) 
  {
//    Serial.print(1);
    itemtime=millis();
    tem=3;
  }
 if (TemCheck(2,code,jintongList)) 
  {
//    Serial.print(1);
    itemtime=millis();
    tem=2;
  }
 if (TemCheck(4,code,gapba1List)){
  armor_level=1;
    lcd.setCursor(15,0);
    lcd.print(armor_level);
 }
  if (TemCheck(5,code,gapba2List)){
  armor_level=2;
      lcd.setCursor(15,0);
    lcd.print(armor_level);
 }
  if (TemCheck(6,code,gapba3List)){
  armor_level=3;
      lcd.setCursor(15,0);
    lcd.print(armor_level);
 }
  if (TemCheck(7,code,helmet1List)){
  helmet_level=1;
      lcd.setCursor(13,0);
    lcd.print(helmet_level);
 }
  if (TemCheck(8,code,helmet2List)){
  helmet_level=2;
        lcd.setCursor(13,0);
    lcd.print(helmet_level);
 }
  if (TemCheck(9,code,helmet3List)){
  helmet_level=3;
        lcd.setCursor(13,0);
    lcd.print(helmet_level);
 }
  if (TemCheck(10,code,blitList)) 
  {
//    Serial.print("blit + 30");
    totblit+=30;
  }
  if (TemCheck(11,code,suppressorList)) 
  { 
//    Serial.println("suppressor");
    firesound=SS;
    firevolume=SV;
    mp3_play(2);
    mp3_set_volume(SV);
    lcd.setCursor(10, 2);
    lcd.print("suppressed");
    //remnant[cnt]=content.substring(1);
    //cnt++;
  }
  if(TemCheck(12,code,exmagList)){
    if(maxblit == 15){
      maxblit = 25;
      lcd.setCursor(3, 2);
      lcd.print("max25");
    }
    //대용량탄창
  }
//   {
//    Serial.println(code[0]);
//    Serial.println(code[1]);
//        Serial.println(code[2]);
//        Serial.println(code[3]);
//    Serial.println("not working");
//  }

  }//RFID끝--------------------------------------------------------------------------------------------



  //한번쓴아이템 재사용 방지

 
if(tem<4 &&tem>=0 )
    {
      lcd.setCursor(0, 2);
      lcd.print(((long)Temtime[tem]-millis()+(long)itemtime)/1000);
      lcd.print('.');
      lcd.print((((long)Temtime[tem]-millis()+(long)itemtime)%1000)/100);
      if( millis()-itemtime>Temtime[tem])
      {
      if(tem==0)
      {Hp=200;}
      if(tem==1)
      {Hp=140;}
      if(tem==2)
      {gauge= gauge<40? gauge+60 : 100;}
      if(tem==3)
      {gauge= gauge<60? gauge+40 : 100;}
      tem=-1;
      lcd.setCursor(0, 2);
      lcd.print("          ");
      }
    }
    if (Hp <= 0)
  {
//    Serial.print('1');
    //죽은 소리내기-------------------------------------------------------
    //mp3_play(3);//0003.mp3
    //LCD로 죽음 표시, 누구에게 죽었는지(killcode) 표시-------------------
    data.write((byte)2);
    data.write((byte)ircode);
    data.write((byte)killcode);
    lcd.clear();
    lcd.setCursor(6, 1);
    lcd.print("YOU DIED!!!!");
    lcd.setCursor(3, 2);
    lcd.print("Killed by : ");
    lcd.setCursor(15, 2);
    lcd.print(killcode);
    Serial.println(killcode);
    delay(50000000);
  }
 sendInfo();
 //Serial.println((long)(T-millis()));



}}
