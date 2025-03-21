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

void checkCollision() {

  if (terrain[dinopos] == 'X' && dinopos == 4) {
    gameOver = true;
  }

}

void init_terrain() {
  for (int i = 0; i < DISPLAY_LENGTH; ++i) {
    int randomValue = random(2); // 0 oder 1
    if (randomValue == 0)
      terrain[i] = ' '; // Leeres Terrain
    else
      terrain[i] = 'X'; // Volles Terrain
  }
}

void displayTerrain() {
  lcd.setCursor(0, 1); // Setze den Cursor auf die zweite Zeile
  for (int i = 0; i < DISPLAY_LENGTH; ++i) {
    lcd.print(terrain[i]); // Zeige das Terrain an
  }
}

void displayDino() {
  
if (dinopos == 3) {
    lcd.setCursor(4, 0); // Setze den Cursor auf die erste Zeile, Position 4
    lcd.print("D"); // Zeige den Dino in der Luft an
  } else {
    lcd.setCursor(4, 1); // Setze den Cursor auf die zweite Zeile, Position 4
    lcd.print("D"); // Zeige den Dino am Boden an

  }
}


void jump() {
  if (dinopos == 4) { // Wenn der Dino auf der Grundposition ist
    dinopos = 3;
    jumptime = millis(); 
  }
  if (dinopos == 3 && millis() - jumptime > jumpdauer) {
    if (terrain[4] != 'X') {

      dinopos = 4; // Zurück auf den Boden

    }
  } 
}

void updateTerrain() {
  // Terrain nach links verschieben
  for (int i = 0; i < DISPLAY_LENGTH - 1; ++i) {
    terrain[i] = terrain[i + 1]; // Verschiebe jedes Element nach links
  }
  // Neues Terrain am Ende hinzufügen
  int randomValue = random(2); // 0 oder 1
  if (randomValue == 0)
    terrain[DISPLAY_LENGTH - 1] = ' '; // Leeres Terrain
  else
    terrain[DISPLAY_LENGTH - 1] = 'X'; // Volles Terrain
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
    if (buttonState == LOW) {
      jump();
    }
    updateTerrain(); // Terrain aktualisieren
    lcd.clear(); // LCD löschen
    displayTerrain(); // Terrain anzeigen
    displayDino(); // Dino anzeigen
    checkCollision(); // Überprüfen auf Kollision
    delay(500); // Wartezeit für die Animation

  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Press Reset");
  }
}