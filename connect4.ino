#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LATCH = 10;
const int SHIFT = 11;
const int RESET = 12;
const int ROW = 7;
const int RED = 8;
const int GREEN = 9;

const int redled = 2;
const int greled = 3;
const int bluled = 4;

const int MATRIX_SIZE = 8;

char COLORS_NAMES[] = {'R', 'G', 'O', ' '};
bool COLORS_BOOLS[][2] = {{true,  false},
                          {false, true},
                          {true,  true},
                          {false, false}};

char grid[8][8] = {
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
};

void setup() {
    lcd.init();
    lcd.backlight();
    pinMode(LATCH, OUTPUT);
    pinMode(SHIFT, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(ROW, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(5, INPUT);
    pinMode(redled, OUTPUT);
    pinMode(greled, OUTPUT);
    pinMode(bluled, OUTPUT);
    digitalWrite(ROW, LOW);
    digitalWrite(LATCH, LOW);
    digitalWrite(RESET, HIGH);
    Serial.begin(9600);
    draw_grid();
    while(digitalRead(5) == LOW) {
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Connect4");
      lcd.setCursor(0,1);
      lcd.print("On Arduino Nano");
      set_status(1,0,0);
      delay(3000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("HardwareSoftware");
      lcd.setCursor(1,1);
      lcd.print("By Alex ROBIC");
      set_status(0,1,0);
      delay(3000);
      lcd.clear();
      set_status(0,0,1);
      lcd.setCursor(4,0);
      lcd.print("Software   *");
      lcd.setCursor(1,1);
      lcd.print("By Irwin Madet");
      delay(3000);
    }
    lcd.clear();
    set_status(1,1,1);
    lcd.setCursor(3,0);
    lcd.print("Game start");
    grid[4][2] = 'G';
    animate('R', 5);
}

void pulse(int pin, bool reversed) {
    digitalWrite(pin, !reversed);
    digitalWrite(pin, reversed);
}

void draw(int x, int y, bool color[]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (i == x) digitalWrite(ROW, HIGH);
        if (i == y) {
            digitalWrite(RED, !color[0]);
            digitalWrite(GREEN, !color[1]);
        }

        pulse(SHIFT, true);

        digitalWrite(ROW, LOW);
        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, HIGH);
    }
    pulse(LATCH, true);
}

void draw_grid() {
  for (int x = 0; x < MATRIX_SIZE; x++)
        for (int y = 0; y < MATRIX_SIZE; y++) {
            draw(x, y, charToArray(grid[x][y]));
        }
}

void set_status(bool r, bool g, bool b) {
  digitalWrite(redled, r);
  digitalWrite(greled, g);
  digitalWrite(bluled, b);
}

int button_to_row(int av) {
  if(av == 0 ) {
    return(1);
  } else if(av == 680){
    return(2);
  }else if(av == 852 or av == 851){
    return(3);
  }else if(av == 920){
    return(4);
  }else if(av == 955){
    return(5);
  }else if(av == 974 or av == 975){
    return(6);
  }else if(av == 987){
    return(7);
  }else{
    return(NULL);
  }
}

bool *charToArray(char c) {
    for (int i = 0; i < sizeof(COLORS_NAMES); i++) {
        if (COLORS_NAMES[i] == c) return COLORS_BOOLS[i];
    }
    return NULL;
}

void animate(char color, int row) {
  int i = 2;
  grid[i][row] = color;
  //for(int j = 0; j < 6; j++
  while((grid[i+1][row] == ' ') and (i <= 7)) {
    draw_frames(50);
    i += 1;
    grid[i-1][row] = ' ';
    grid[i][row] = color;
  }
}

void draw_frames(int number) {
  for(int a = 0; a < number; a++) {
    draw_grid();
    delay(1);
  }
}

void loop() {
    draw_grid();       
}
