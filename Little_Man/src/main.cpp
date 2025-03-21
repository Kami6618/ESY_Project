#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TASTER 8
#define DISPLAY_LENGTH 16
#define DISPLAY_WIDTH 2

LiquidCrystal_I2C lcd(0x27, DISPLAY_LENGTH, DISPLAY_WIDTH); // 16 Spalten und 2 Zeilen

char terrain[DISPLAY_LENGTH];
int dinopos = 4; 
bool gameOver = false;
unsigned long jumptime = 0;
const unsigned long jumpdauer = 500;
bool buttonPressed = false;
int gapCounter = random(3, 7); // Zufälliger Abstand zwischen Hindernissen

void checkCollision() {
  if (terrain[4] == 'X' && dinopos == 4) {
    gameOver = true;
  }
}

void init_terrain() {
  for (int i = 0; i < DISPLAY_LENGTH; ++i) {
    terrain[i] = ' '; // Start mit leerem Terrain
  }
}

void displayTerrain() {
  lcd.setCursor(0, 1);
  for (int i = 0; i < DISPLAY_LENGTH; ++i) {
    lcd.print(terrain[i]);
  }
}

void displayDino() {
  lcd.setCursor(4, dinopos == 3 ? 0 : 1);
  lcd.print("D");
}

void jump() {
  if (dinopos == 4) {  
    dinopos = 3;
    jumptime = millis();
  }
}

void updateJump() {
  if (dinopos == 3 && millis() - jumptime > jumpdauer) {
    if (terrain[4] == ' ') {  
      dinopos = 4;  
    }
  }
}

void updateTerrain() {
  for (int i = 0; i < DISPLAY_LENGTH - 1; ++i) {
    terrain[i] = terrain[i + 1];
  }

  if (gapCounter > 0) {  
    terrain[DISPLAY_LENGTH - 1] = ' ';  
    gapCounter--;
  } else {
    terrain[DISPLAY_LENGTH - 1] = 'X';  
    gapCounter = random(3, 7);  
  }
}

void resetGame() {
  gameOver = false;
  dinopos = 4;
  gapCounter = random(3, 7);
  init_terrain();
  lcd.clear();
}

void setup() {
  pinMode(TASTER, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  init_terrain();
  displayTerrain();
  displayDino();
}

void loop() {
  if (!gameOver) {
    int buttonState = digitalRead(TASTER);

    if (buttonState == LOW && !buttonPressed) {  
      jump();  
      buttonPressed = true;
    }

    if (buttonState == HIGH) {  
      buttonPressed = false;
    }

    updateJump();  
    updateTerrain();
    lcd.clear();
    displayTerrain();
    displayDino();
    checkCollision();
    delay(500);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Press Button");

    while (digitalRead(TASTER) == HIGH); // Warten auf Tastendruck
    delay(300); // Entprellen
    resetGame();
  }
}
