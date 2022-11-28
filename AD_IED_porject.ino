/*
 * This will be the code for the RPI IED course
 * Team project: Auto drawer
 * Subsystem: lock system
 * It will allow user to input through the keypad
 * and the system will check the pin
 * if all good then run the main system
 */

// Include
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//define
#define passwordLength 8
#define maxLength 16
#define motorOn 1
#define motorOff 0

//Global set
int count = 0;
char input[maxLength] = "";

int permit = 0;
int begin = 0;

const byte ROWS = 4;
const byte COLS = 4;
char pin[maxLength];
char defaultPin[passwordLength] = "*******";

//Global var
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connect to Arduino
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

//Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Begin");
  Serial.println(pin);
  permit = 0;
  begin = 0;
  count = 0;
  //lcd setting
  lcd.backlight();
  lcd.init();
  lcd.print("Welcome");
  Serial.write(motorOff);
}

void loop() {
  while(begin == 0){
    char customKey = customKeypad.getKey();
    if(customKey){
      if(customKey == 'A'){
        begin = 1;
        lcd.clear();
        lcd.print("Enter Password:");
      }
    }
  }
  while(permit == 0) {
    char customKey = customKeypad.getKey();
    if(customKey) {
      //Serial.println(customKey);
      lcd.clear();
      lcd.setCursor(0,0);
      if(customKey == 'C'){
        input[count-1] = NULL;
        count = count - 1;
      }
      else if(customKey == 'A' | count >= maxLength){
        lcd.clear();
        if(!strcmp(input,defaultPin) | !strcmp(input,pin)) {
          lcd.print("Correct");
          permit = 1;
          delay(2000);
          lcd.clear();
        }
        else{
          lcd.print("Incorrect");
          delay(2000);
          lcd.clear();
          lcd.print("Enter Password:");
        }
        //empty string
        int i = 0;
        for(i = 0; i < strlen(input); i++){
          input[i] = NULL;
        }
        count = 0;
      }
      else if(customKey == 'B'){
        lcd.clear();
        //empty string
        int i = 0;
        for(i = 0; i < strlen(input); i++){
          input[i] = NULL;
        }
        count = 0;
        setup();
      }
      else{
        input[count] = customKey;
        count++;
      }
      lcd.print(input);
    }  
  }
  lcd.print("System");
  while(begin == 1 & permit == 1){
    //delay(1000);
    char customKey = customKeypad.getKey();
    if(customKey){
      if(customKey == 'A'){
        //ready to use
        lcd.clear();
        Serial.write(motorOn);
        lcd.print("Motor online");
      }
      else if(customKey == 'B'){
      //reset password
      lcd.clear();
      lcd.print("reset password");
      delay(2000);
      int finish = 0;
      int i = 0;
          for(i = 0; i < strlen(pin); i++){
            pin[i] = NULL;
          }
      count = 0;
      while(finish == 0){
        //Serial.println(count);
        customKey = customKeypad.getKey();
        if(customKey){
        lcd.clear();
        lcd.setCursor(0,0);
        if(customKey == 'A'){
          //confirm
          lcd.print("New password");
          delay(2000);
          lcd.clear();
          lcd.print(pin);
          Serial.print(pin);
          delay(5000);
          finish = 1;
          lcd.clear();
          lcd.print("System");
        }
        else if(customKey == 'B' | customKey == 'C'){
          //go back
          int i = 0;
          for(i = 0; i < strlen(pin); i++){
            pin[i] = NULL;
          }
          count = 0;
          finish = 1;
          lcd.clear();
        }
        else if(customKey == 'D'){
          //delete
          pin[count] = NULL;
          count = count - 1;
          lcd.print(pin);
        }
        else if(count >= maxLength){
          //error message
          lcd.clear();
          lcd.print("Invalid");
          int i = 0;
          for(i = 0; i < strlen(pin); i++){
            pin[i] = NULL;
          }
          count = 0;
        }
        else {
          pin[count] = customKey;
          count++;
          lcd.clear();
          lcd.print(pin);
        }
        }
      }
    }
    else if(customKey == 'C'){
      setup();
    }
    else if(customKey == 'D'){
      Serial.write(motorOff);
      lcd.clear();
      lcd.print("Motor offline");
    }
    }
  }
  //lcd.clear();

}
