#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  //ANYTHING INSIDE HERE DO NOT FUCKING TOUCH
  lcd.init();
  lcd.clear();
  lcd.backlight();
  //Anything outside here feel freen to fuck around with
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
