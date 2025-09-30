#include <LiquidCrystal.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          
#define SS_PIN          10  //就是模組上的SDA接腳

MFRC522 mfrc522;  

const int sensorPin = 6;
long laag = 0;
long hoog = 0;
long laagTijd = 0;
long hoogTijd = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

float value_1 = 0,value_2 = 0,value_5 = 0;


void setup() {
    Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  while(digitalRead(sensorPin) == LOW) {;}
  hoog = millis();
  lcd.init();
  lcd.backlight();
  pinMode(5,OUTPUT);
  SPI.begin();        

  mfrc522.PCD_Init(SS_PIN, RST_PIN); 
  Serial.print(F("Reader "));
  Serial.print(F(": "));
  mfrc522.PCD_DumpVersionToSerial(); 
}

void loop()
{
 v1_();
 v2_();
 v3_();
 v4_();
 v5_();
 
}
//void Refresh()SSs
//{
//  Serial.print("R1:");Serial.print(v1_());Serial.print("Q:");
//  Serial.print("R2:");Serial.print(v2_());Serial.print("Q:");S
//  Serial.print("R3:");Serial.print(v3_());Serial.print("Q:");
//  Serial.print("R4:");Serial.print(value_);Serial.print("Q:");
//  Serial.print("R5:");Serial.print(value_5);Serial.print("Q:");
//}

void v1_()
{
 value_1 = analogRead(A0);
 value_1 =  value_1/1024*5;
 Serial.print("R1:");Serial.print(value_1);Serial.print("Q:");
}
void v2_()
{
 value_2 = analogRead(A2);
 value_2 =  value_2/1024*5;
 Serial.print("R2:");Serial.print(value_2);Serial.print("Q:");
}
void v3_()
{
  // Check for high and low timing - low time * 2 = PPM
  while(digitalRead(sensorPin) == HIGH) {;}
  laag = millis();
  hoogTijd = laag - hoog;
  
  while(digitalRead(sensorPin) == LOW) {;}
  hoog = millis();
  laagTijd = hoog - laag;
 
  // Validity check high+low = 1004 (small margin or error allowed -> 990)
  if(laagTijd + hoogTijd > 990)
  {
//    Serial.print("H: ");
//    Serial.println(hoogTijd,DEC);
//    Serial.print("L: ");
//    Serial.println(laagTijd,DEC);
    Serial.print("R3:");
    Serial.print(2000-laagTijd * 2,DEC);
    Serial.print("Q:");
//    Serial.println(" ppm");    
  } 
  else 
  {
    // Output in case of invalid check: value instead of 1004 ms
    Serial.print("R3:");
    Serial.print(laagTijd+hoogTijd, DEC);
    Serial.println("Q:");
  }
  lcd.setCursor(6, 0);
  lcd.print("CO2");
  lcd.setCursor(5, 1);
  lcd.print(2000-laagTijd * 2,DEC);lcd.print(" ppm");
}
void v4_()
{

  // 檢查是不是一張新的卡
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      // 顯示卡片內容
      Serial.print(F("R4:"));Serial.print("1");Serial.print("Q:");

      mfrc522.PICC_HaltA();  // 卡片進入停止模式
    }
   else{
    Serial.print(F("R4:"));Serial.print("0");Serial.print("Q:"); 
}
}
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
void v5_()
{
 value_5 = analogRead(A1);
 value_5 =  value_5/1024*5;
 Serial.print("R5:");Serial.print(value_5);Serial.println("Q:");
}

//      dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size); // 顯示卡片的UID
      
//      Serial.println();
//      Serial.print(F("PICC type: "));
//      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
//      Serial.println(mfrc522.PICC_GetTypeName(piccType));  //顯示卡片的類型
      


