#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TASTER 8
#define DISPLAY_LENGTH 16
#define DISPLAY_WIDTH 2

LiquidCrystal_I2C lcd(0x27,DISPLAY_LENGTH,2); //16 Spalten und 2 Zeilen

char terrain[DISPLAY_LENGTH];

int dinopos = 4; 

unsigned long jumptime = 0;
const unsigned long jumpdauer = 500;

void init_terrain()
{
  
  for(int i = 0; i < DISPLAY_LENGTH; ++i)
  {
    int randomValue = random(2); // 0 oder 1

        if (randomValue == 0)
            terrain[i] = ' '; // Leeres Terrain

        else
            terrain[i] = 'X'; // Volles Terrain

    lcd.setCursor(i,1);
    lcd.print(terrain[i]);
  } 
}


void displayDino()
{
 lcd.setCursor(4, 1); // Setze den Cursor auf die zweite Zeile
 lcd.print("D"); // Zeige den Dino an
}

void jump()
{
  if(dinopos == 0)
  {
    dinopos = 1;  
    jumptime = millis(); 
  }
  if(dinopos == 1 && millis()-jumptime > jumpdauer)
  {
    dinopos = 0;  
  } 
}


void setup() {
 pinMode(TASTER, INPUT_PULLUP);
 lcd.init();
 lcd.backlight();
 init_terrain(); 
 displayDino();
}

void loop() {
 
 int buttonState = digitalRead(TASTER);
  if (buttonState == LOW) {
    jump();
  }
  
  delay(1000);
  lcd.scrollDisplayLeft();
  
}