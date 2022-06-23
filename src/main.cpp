#include <Arduino.h>
#include "../include/Manager/Menu_Manager.h"
#include <Wire.h> //0x27
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <ESP32Time.h>
#include "RTClib.h"

#define EEPROM_SIZE 1024
#define ST_CP 26
#define SH_CP 27
#define DS 25
#define OUTPUT_ENABLE 14

void print(String, uint8_t, uint8_t);
void setAmountValves();
void askBounds();
bool askSaved();
void loadPreviousConfig();
void choiceMenu();
void setupShift();

LiquidCrystal_I2C lcd(0x3F,20,4);//0x3F
ESP32Time rtc;
RTC_DS1307 RTC;
Menu_Manager* mm;
uint8_t EEPROM_ADDRESS = 1, currentValve = 0, currentBound = 0;
int8_t offSet = 0;
uint16_t choice = 0, sub_menu;
bool Ok;

void setup() {
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  Wire.begin();
  mm = new Menu_Manager();


  RTC.begin();
  
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  DateTime mao {__DATE__, __TIME__};
  Serial.println("RTC: " + mao.hour());
  Serial.println(String(mao.hour()));
  RTC.adjust(DateTime(mao.year(), mao.month(), mao.day(), mao.hour(), mao.minute(), mao.second()));
  Serial.println("RTC1: " + String(RTC.now().hour()));
  rtc.setTime(mao.second(), mao.minute(), mao.hour(), mao.day(), mao.month(), mao.year());
  



  setupShift();
  
  lcd.init();
  lcd.backlight();
  lcd.clear();

  if (askSaved()){
    setAmountValves();          //asks and instantiates valve's array.
    
    lcd.clear();
    delay(1000);
    askBounds();                //asks and instantiates bounds' array.
    
    lcd.clear();
    delay(1000);
    EEPROM.commit();
    
  }
  else loadPreviousConfig();  

}
void loop() {
  Serial.println("loop");
  choiceMenu();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//##############################################################################
void setupShift(){
  pinMode(OUTPUT_ENABLE, OUTPUT);
  digitalWrite(OUTPUT_ENABLE, HIGH);
  pinMode(ST_CP, OUTPUT);
  pinMode(SH_CP, OUTPUT);
  pinMode(DS, OUTPUT);
  
  
  digitalWrite(SH_CP, LOW);
  digitalWrite(ST_CP, LOW);
  digitalWrite(DS, LOW);
  
}
//##############################################################################
void print(String text, int column, int line){
    
    lcd.setCursor(column, line);
    lcd.print(text);
    
}
//##############################################################################
void setAmountValves() {    //FUNZIONA EEPROM
uint8_t nValves = 1;
print("N. Valvole [1-8]", 0, 0);
print("Value-> ", 0, 1);
print(String(nValves), 8, 1);

while (!mm->getButtonManager()->getMenu()->isPressed() || (nValves == 0)) {
  lcd.setCursor(8, 1);
  if (mm->getButtonManager()->getUp()->isPressed()) {
    if ((nValves + 1) > Valve::MAX_VALVE)
      lcd.print(Valve::MAX_VALVE);
    else
      lcd.print(++nValves);
  }
  else if (mm->getButtonManager()->getDown()->isPressed()) {
    if ((nValves - 1) < 1)
      lcd.print(1);
    else
      lcd.print(--nValves);
  }
  delay(200);
}
mm->setStringManager(new String_Manager(nValves));
EEPROM.write(0, nValves);         //save nValves
  
}
//##############################################################################

void defineBounds(Valve* valve);

void askBounds(){
  
  Valve* valve = mm->getStringManager()->getValves();

  

  for ( uint8_t i = 0; i < Valve::VALVES; i++ ){
    lcd.clear();
    String frase = "Orari V" + (String)((valve + i)->getNumber()) + " [1-9]";
    const uint8_t MAX_BOUNDS = 9;
    uint8_t bounds = 1;   //current bound amount
    print(frase, 0, 0);
    print("Value-> ", 0, 1);
    print(String(bounds), 8, 1 );

    while (!mm->getButtonManager()->getMenu()->isPressed()) {

    lcd.setCursor(8, 1);
    if (mm->getButtonManager()->getUp()->isPressed()) {
      if ((bounds + 1) > MAX_BOUNDS)
        lcd.print(MAX_BOUNDS);
      else
        lcd.print(++bounds);
    }
    else if (mm->getButtonManager()->getDown()->isPressed()) {
      if ((bounds - 1) < 1)
        lcd.print(1);
      else
        lcd.print(--bounds);
    }

    delay(200);
  }
  
  (valve+i)->setBounds(bounds);

  lcd.clear();
  delay(1000);

  defineBounds(valve + i);
  }
  
}

//##############################################################################

uint8_t setHour(); uint8_t setMinute();

void defineBounds(Valve* valve){

  EEPROM.write(EEPROM_ADDRESS++, valve->getNoBounds());

  for( uint8_t i = 0; i < valve->getNoBounds(); i++ ){
    
    delay(300);
    lcd.clear();
    String frase = "H iniz Fascia " + (String)i;
    print(frase, 0, 0);
    valve->getBounds()[i].setFromH(setHour());    //ora iniziale
    EEPROM.write(EEPROM_ADDRESS++, valve->getBounds()[i].getFromH());
    
    delay(300);
    lcd.clear();
    frase = "M iniz Fascia " + (String)i;
    print(frase, 0, 0);
    valve->getBounds()[i].setFromM(setMinute());    //minuti iniziali
    EEPROM.write(EEPROM_ADDRESS++, valve->getBounds()[i].getFromM());

    delay(300);
    lcd.clear();
    frase = "H fine Fascia " + (String)i;
    print(frase, 0, 0);
    valve->getBounds()[i].setToH(setHour());     //ora finale
    EEPROM.write(EEPROM_ADDRESS++, valve->getBounds()[i].getToH());

    delay(300);
    lcd.clear();
    frase = "M fine Fascia " + (String)i;
    print(frase, 0, 0);
    valve->getBounds()[i].setToM(setMinute());    //minuti finale
    EEPROM.write(EEPROM_ADDRESS++, valve->getBounds()[i].getToM());

    
  }
  lcd.clear();
  delay(1000);
}

//##############################################################################

uint8_t setHour(){

  print("Value-> ", 0, 1);
  uint8_t hour = 0;
  print(String(hour), 8, 1 );
  
  while (!mm->getButtonManager()->getMenu()->isPressed()) {
    lcd.setCursor(8, 1);
    if (mm->getButtonManager()->getUp()->isPressed()) {
      if ((hour + 1) > 23) {
        lcd.print(23);
        hour = 23;
      }
      else {
        ++(hour);
        if (hour < 10) {
          lcd.print("0" + (String)(hour));
        }
        else {
          lcd.print((String)(hour));
        }
      }
    }
    else if (mm->getButtonManager()->getDown()->isPressed()) {
      if ((hour - 1) < 0) {
        lcd.print(0);
        hour = 0;
      }
      else {
        --(hour);
        if (hour < 10) {
          lcd.print("0" + (String)(hour));
        }
        else {
          lcd.print((String)(hour));
        }
      }
    }
    else if (mm->getButtonManager()->getRight()->isPressed()) {
      if ((hour - 1) > 13) {
        lcd.print("00");
        hour = 0;
      }
      else {
        hour += 10;
        if (hour < 10) {
          lcd.print("0" + (String)(hour));
        }
        else {
          lcd.print((String)(hour));
        }
      }
    }
    else if (mm->getButtonManager()->getLeft()->isPressed()) {
      if ((hour - 1) < 10) {
        lcd.print("00");
        hour = 0;
      }
      else {
        hour -= 10;
        if (hour < 10) {
          lcd.print("0" + (String)(hour));
        }
        else {
          lcd.print((String)(hour));
        }
      }
    }
    delay(200);
  }
  return hour;
}

//##############################################################################

uint8_t setMinute(){
  print("Value-> ", 0, 1);
  uint8_t minute = 0;
  print(String(minute), 8, 1 );

  while (!mm->getButtonManager()->getMenu()->isPressed()) {
    lcd.setCursor(8, 1);
    if (mm->getButtonManager()->getUp()->isPressed()) {
      if ((minute + 1) > 59) {
        lcd.print(59);
        minute = 59;
      }
      else {
        ++(minute);
        if (minute < 10) {
          lcd.print("0" + (String)(minute));
        }
        else {
          lcd.print((String)(minute));
        }
      }
    }
    else if (mm->getButtonManager()->getDown()->isPressed()) {
      if ((minute - 1) < 0) {
        lcd.print(0);
        minute = 0;
      }
      else {
        --(minute);
        if (minute < 10) {
          lcd.print("0" + (String)(minute));
        }
        else {
          lcd.print((String)(minute));
        }
      }
    }
    else if (mm->getButtonManager()->getRight()->isPressed()) {
      if ((minute - 1) > 48) {
        lcd.print("00");
        minute = 0;
      }
      else {
        minute += 10;
        if (minute < 10) {
          lcd.print("0" + (String)(minute));
        }
        else {
          lcd.print((String)(minute));
        }
      }
    }
    else if (mm->getButtonManager()->getLeft()->isPressed()) {
      if ((minute - 1) < 10) {
        lcd.print("00");
        minute = 0;
      }
      else {
        minute -= 10;
        if (minute < 10) {
          lcd.print("0" + (String)(minute));
        }
        else {
          lcd.print((String)(minute));
        }
      }
    }
    delay(200);
  }
  return minute;
}

//##############################################################################

bool askSaved(){
  lcd.clear();
  print("Caricare imp. ?", 0, 0);
  print("<--No      Si-->", 0, 1);
  uint64_t currentMillis = millis();
  bool Pressed = false, Timeout = false;
  bool Answer = false;
  while ((!mm->getButtonManager()->getMenu()->isPressed() || !Pressed) && !Timeout){

    if(((millis() - currentMillis) > 15000) && !Pressed){
      Answer = false;
      Timeout = true;
    }    
    if(mm->getButtonManager()->getLeft()->isPressed()){//sx no
      Pressed = true;
      Answer = true;
      print("<-", 7, 1);
    }
    else if(mm->getButtonManager()->getRight()->isPressed()){//dx si
      Pressed = true;
      Answer = false;
      print("->", 7, 1);
    }
    delay(100);
  }
  lcd.clear();
  delay(1000);
  return Answer;
}

//##############################################################################

void loadPreviousConfig(){

  uint8_t address_count = 0;

  uint8_t nValves = EEPROM.read(address_count++);
  
  mm->setStringManager(new String_Manager(nValves));    //valve array
  
  for (uint8_t i = 0; i < nValves; i++){               //per tutte le valvole

    mm->getStringManager()->getValves()[i].setBounds(EEPROM.read(address_count++));
    
    for ( uint8_t j = 0; j < mm->getStringManager()->getValves()[i].getNoBounds(); j++ ){

      mm->getStringManager()->getValves()[i].getBounds()[j].setFromH(EEPROM.read(address_count++));
      
      mm->getStringManager()->getValves()[i].getBounds()[j].setFromM(EEPROM.read(address_count++));
      
      mm->getStringManager()->getValves()[i].getBounds()[j].setToH(EEPROM.read(address_count++));
      
      mm->getStringManager()->getValves()[i].getBounds()[j].setToM(EEPROM.read(address_count++));
      
    }
  }
}

//##############################################################################
boolean difference(Valve*, uint8_t);

void choiceMenu(){
  Ok = true;
  
  switch (choice)
  {
  case 0:
    Serial.println(String(rtc.getTime()));
    print(rtc.getTime(), 0, 0);

    //stampare la temparatura
    for (uint8_t i = 0; i < Valve::VALVES; i++){
      mm->getStringManager()->getValves()[i].setGate(false);
    }

    for ( uint8_t i = 0; i < Valve::VALVES; i++ ){
      for ( uint8_t j = 0; j <  mm->getStringManager()->getValves()[i].getNoBounds(); j++ ){
        if (difference(&mm->getStringManager()->getValves()[i], j)){
          mm->getStringManager()->getValves()[i].setGate(true);
          print("RUNNING", 0, 1);
          if ( Valve::output[mm->getStringManager()->getValves()[i].getNumber()] )
            print(String(mm->getStringManager()->getValves()[i].getNumber()), 8 + mm->getStringManager()->getValves()[i].getNumber() , 1);
          else
            print(" ", 8 + mm->getStringManager()->getValves()[i].getNumber() , 1);

        }
      }

      if( mm->getStringManager()->getValves()[i].getGate() ){
        if(!mm->getStringManager()->getValves()[i].isOpen()){
          mm->getStringManager()->getValves()[i].openValve();
          //print(String(mm->getStringManager()->getValves()[i].getNumber()), 8 + mm->getStringManager()->getValves()[i].getNumber() , 1);
        }
      }
      else{
        if(mm->getStringManager()->getValves()[i].isOpen()){
          mm->getStringManager()->getValves()[i].closeValve();
          lcd.clear();
        }
      }

    }

    break;
  
  case 1:
    lcd.clear();
    print("MENU", 0, 0);
    print("<--   EXIT   -->", 0, 1);
    while (Ok) {
        delay(300);
        if (mm->getButtonManager()->getMenu()->isPressed()) {
          lcd.clear();
          choice = 0;
          Ok = false;
        }
        else if (mm->getButtonManager()->getRight()->isPressed()) {
          choice = 10;
          Ok = false;
        }
        else if (mm->getButtonManager()->getLeft()->isPressed()) {
          choice = 40;
          Ok = false;
        }
      }
    break;

    case 10:
      print("MENU", 0, 0);
      //SET TIME
      print("          +- ora", 0, 1);
      
      while (Ok) {
        if (mm->getButtonManager()->getMenu()->isPressed()) {
          choice = 300;
          Ok = false;
        }
        else if (mm->getButtonManager()->getLeft()->isPressed()) {
          choice = 1;
          Ok = false;
        }
        else if (mm->getButtonManager()->getRight()->isPressed()) {
          choice = 20;
          Ok = false;
        }
        delay(300);
      }
      break;

      case 20:
      print("MENU", 0, 0);
      //Mod. manuale?
      
      print("   Mod. manuale?", 0, 1);
      
      while (Ok) {
        if (mm->getButtonManager()->getMenu()->isPressed()) {
          choice = 200;
          Ok = false;
        }
        else if (mm->getButtonManager()->getLeft()->isPressed()) {
          choice = 10;
          Ok = false;
        }
        else if (mm->getButtonManager()->getRight()->isPressed()) {
          choice = 30;
          Ok = false;
        }

        delay(300);

      }
      break;

      case 30:
      print("MENU", 0, 0);
      //Mod. valv./fasc.
     
      print("Mod. valv./fasc. ", 0, 1);
      
      while (Ok) {
        if (mm->getButtonManager()->getMenu()->isPressed()) {                                       
          Ok = false;
          choice = 400;
        }
        else if (mm->getButtonManager()->getRight()->isPressed()) {
          choice = 40;
          Ok = false;
        }
        else if (mm->getButtonManager()->getLeft()->isPressed()) {
          choice = 20;
          Ok = false;
        }
        delay(300);
      }
      break;

      case 40:
      print("MENU", 0, 0);
      //"EXIT"
      
      print("Set time            ", 0, 1);
      
      while (Ok) {
        if (mm->getButtonManager()->getMenu()->isPressed()) {
          choice = 0;
          Ok = false;
          lcd.clear();
        }
        else if (mm->getButtonManager()->getRight()->isPressed()) {
          choice = 1;
          Ok = false;
        }
        else if (mm->getButtonManager()->getLeft()->isPressed()) {
          choice = 30;
          Ok = false;
        }
        delay(250);
      }
      
      break;

      case 200:         //manual ON/OFF

      currentValve = 0;

      print("                ", 0, 1);
      print("Valv. " + (String)currentValve, 0, 1);
      print("    ", 8, 1);

      if (mm->getStringManager()->getValves()[currentValve].isOpen()) print(F("ON"), 8, 1); else print(F("OFF"), 8, 1);      
      while (Ok) {

        if (mm->getButtonManager()->getRight()->isPressed() && (currentValve < (Valve::VALVES - 1))) {

          print((String)(++currentValve), 6, 1);
          print("    ", 8, 1);
          if (mm->getStringManager()->getValves()[currentValve].isOpen()) print(F("ON"), 8, 1); else print(F("OFF"), 8, 1);
        }
        else if (mm->getButtonManager()->getLeft()->isPressed() && (currentValve > 0)) {

          print((String)(--currentValve), 6, 1);
          print("    ", 8, 1);
          if (mm->getStringManager()->getValves()[currentValve].isOpen()) print(F("ON"), 8, 1); else print(F("OFF"), 8, 1);
        }

        else if (mm->getButtonManager()->getUp()->isPressed()) {
          mm->getStringManager()->getValves()[currentValve].openValve();
          
          print("    ", 8, 1);
          if (mm->getStringManager()->getValves()[currentValve].isOpen()) print(F("ON"), 8, 1); else print(F("OFF"), 8, 1);
        }
        else if (mm->getButtonManager()->getDown()->isPressed()) {
          mm->getStringManager()->getValves()[currentValve].closeValve();
          print("    ", 8, 1);
          
          if (mm->getStringManager()->getValves()[currentValve].isOpen()) print(F("ON"), 8, 1); else print(F("OFF"), 8, 1);
        }
        else if (mm->getButtonManager()->getMenu()->isPressed()) {
          Ok = false;
          choice = 1;
        }
        delay(300);
      }
      break;

      case 300:

      print("Offset:             ", 0, 1 );
      print(String(offSet), 8, 1);
      while (Ok) {

        if (mm->getButtonManager()->getUp()->isPressed() && (offSet < 3)) {
          print("    ", 8, 1);
          print((String)(++offSet), 8, 1);
        }
        else if (mm->getButtonManager()->getDown()->isPressed() && (offSet > -3)) {
          print("    ", 8, 1);
          print((String)(--offSet), 8, 1);
        }
        else if (mm->getButtonManager()->getMenu()->isPressed()) {
          Ok = false;
          rtc.setTime(rtc.getSecond(), rtc.getMinute(), rtc.getHour(true) + offSet, rtc.getDay(), rtc.getMonth(), rtc.getYear());
          choice = 1;
        }
        delay(300);
        
      }
      break;

      case 400:

      print("Reset               ", 0, 1 );
      print("<-+          -->", 0, 1);

      while (Ok) {
        if (mm->getButtonManager()->getMenu()->isPressed()) {
          delete mm;
          mm = nullptr;
          setup();
        }
        else if (mm->getButtonManager()->getRight()->isPressed()) {
          choice = 500;
          Ok = false;
        }
        else if (mm->getButtonManager()->getLeft()->isPressed()) {
          choice = 30;
          Ok = false;
        }
        delay(300);
      }

      break;

      case 500:

      print("Mod. Valv.          ", 0, 1 );
      print("<--          -->", 0, 1);

      while (Ok) {
        if (mm->getButtonManager()->getMenu()->isPressed()) {
          choice = 1000;
          Ok = false;
        }
        else if (mm->getButtonManager()->getRight()->isPressed()) {
          choice = 400;
          Ok = false;
        }
        else if (mm->getButtonManager()->getLeft()->isPressed()) {
          choice = 400;
          Ok = false;
        }
        delay(300);
      }

      break;

      case 1000:
      
      break;

  }

  delay(300);

  if (mm->getButtonManager()->getMenu()->isPressed())
    choice = 1;

}

//##############################################################################

int64_t toSec(uint8_t FROM_H, uint8_t FROM_M) {

  uint64_t a = (uint64_t)(FROM_M * 60L + FROM_H * 3600L + 0L);
  return (int64_t)a;
}

//##############################################################################

boolean difference(Valve* valve, uint8_t j) {

  uint64_t from = toSec(valve->getBounds()[j].getFromH(), valve->getBounds()[j].getFromM());
  uint64_t now = toSec(rtc.getHour(true), rtc.getMinute());
  uint64_t to = toSec(valve->getBounds()[j].getToH(), valve->getBounds()[j].getToM());
  
  int64_t difference1 = (from - now);
  int64_t difference2 = (now - to);
  
  return (difference1 <= 0) && (difference2 < 0);

}

//##############################################################################

