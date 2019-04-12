#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LATCH = 10;
const int SHIFT = 11;
const int RESET = 12;
const int ROW = 7;
const int RED = 8;
const int GREEN = 9;

const int R_LED = 2;
const int G_LED = 3;
const int B_LED = 4;

const int MATRIX_SIZE = 8;

int turn = 0;

String who_turn[2] ={"Green plays", "Red plays"};
char players[2] = {'G', 'R'};

char COLORS_NAMES[4] = {'R', 'G', 'O', ' '};
bool COLORS_BOOLS[4][2] = {{true,  false},
                          {false, true},
                          {true,  true},
                          {false, false}};

char grid[8][8];

void setup() {
    lcd.init();
    lcd.backlight();
    pinMode(LATCH, OUTPUT);
    pinMode(SHIFT, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(ROW, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(R_LED, OUTPUT);
    pinMode(G_LED, OUTPUT);
    pinMode(B_LED, OUTPUT);

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            grid[i][j] = ' ';
        }
    }

    digitalWrite(ROW, LOW);
    digitalWrite(LATCH, LOW);
    digitalWrite(RESET, HIGH);
    Serial.begin(9600);
    draw_grid();
    credits();      
    lcd.clear();
    set_status(1,1,1);
    lcd.setCursor(3,0);
    lcd.print("Game start");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Green player starts");
    set_status(0, 1, 0);
}

void credits() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Connect4");
  lcd.setCursor(0,1);
  lcd.print("On Arduino Nano");
  set_status(1,0,0);
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HardwareSoftware");
  lcd.setCursor(1,1);
  lcd.print("By Alex ROBIC");
  set_status(0,1,0);
  delay(1500);
  lcd.clear();
  set_status(0,0,1);
  lcd.setCursor(4,0);
  lcd.print("Software");
  lcd.setCursor(1,1);
  lcd.print("By Irwin MADET");
  delay(1500);
  
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
  digitalWrite(R_LED, r);
  digitalWrite(G_LED, g);
  digitalWrite(B_LED, b);
}

int button_to_row(int av) {
  if(av <= 10 ) {
    return(1);
  } else if(678 <= av and av <= 682){
    return(2);
  }else if(851 <= av and av <= 853){
    return(3);
  }else if(919 <= av and av <= 921){
    return(4);
  }else if(955 <= av  and av<= 957){
    return(5);
  }else if(974 <= av  and av <= 976){
    return(6);
  }else if(986 <= av and av <= 988){
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
  if(grid[i][row] == ' ') {
  if(color == 'G') {set_status(1,0, 0);} else {set_status(0,1, 0);}
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(who_turn[turn%2]);
  lcd.setCursor(0, 1);
  lcd.print(color);
  lcd.print(" played at ");
  lcd.print(row+1);
  grid[i][row] = color;
  while((grid[i+1][row] == ' ') and (i <= 7)) {
    draw_frames(5);
    i += 1;
    grid[i-1][row] = ' ';
    grid[i][row] = color;
  }
  turn += 1;
  }
}

void draw_frames(int number) {
  for(int a = 0; a < number; a++) {
    draw_grid();
    delay(1);
  }
}

bool check_diagonals_sw_ne(char player) {
    for (int x = 0; x <= 3; x++) {
        for (int y = 0; y <= 2; y++) {
            for (int i = 0; i < 3; i++) {
                if (grid[y + i][x + i] != player)
                    break;
                if (i == 3) return true;
            }
        }
    }
    return false;
}

bool check_diagonals_se_nw(char player) {
    for (int x = 3; x <= 7; x++) {
        for (int y = 0; y <= 2; y++) {
            for (int i = 0; i < 3; i++) {
                if (grid[y + i][x - i] != player)
                    break;
                if (i == 3) return true;
            }
        }
    }
    return false;
}

bool check_lines(char player) {
    for (int x = 0; x <= 3; x++) {
        for (int y = 0; y <= 7; y++) {
            for (int i = 0; i < 3; i++) {
                if (grid[y][x + i] != player)
                    break;
                if (i == 3) return true;
            }
        }
    }
    return false;
}

bool check_columns(char player) {
    for (int x = 0; x <= 7; x++) {
        for (int y = 0; y <= 2; y++) {
            for (int i = 0; i < 3; i++) {
                if (grid[y + i][x] != player)
                    break;
                if (i == 3) return true;
            }
        }
    }
    return false;
}

bool check_winner(char player) {
    return check_diagonals_se_nw(player) ||
           check_diagonals_sw_ne(player) ||
           check_lines(player) ||
           check_columns(player);
}

void loop() {
    draw_grid();
    int x = analogRead(0);
    int r = button_to_row(x);
    if(r != 0) {
      char current_player = players[(turn+1)%2];
      animate(current_player, r-1);
    } else {
      draw_frames(10);
    }
    for(int i = 0; i <= 1; i++) {
      char winner = players[i];
      if(check_winner(winner)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(winner);
        lcd.print(" won the game");
        lcd.setCursor(0,1);
        lcd.print("Press RESET");
        return(NULL);
      }
    }
}
