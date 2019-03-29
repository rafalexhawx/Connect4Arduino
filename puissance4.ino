const int LATCH = 10;
const int SHIFT = 11;
const int RESET = 12;

const int ROW = 7;
const int RED = 8;
const int GREEN = 9;

const int MATRIX_SIZE = 8;

const char COLORS_NAMES = {'R', 'G', 'O', 'B'};
const bool COLORD_BOOLS = {{true,  false},
                           {false, true},
                           {true,  true},
                           {false, false}};
void setup() {
    pinMode(LATCH, OUTPUT);
    pinMode(SHIFT, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(ROW, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);


    digitalWrite(ROW, HIGH);
    digitalWrite(LATCH, HIGH);
    digitalWrite(RESET, HIGH);
}

void loop() {
    for (int x = 0; x < MATRIX_SIZE; x++)
        for (int y = 0; y < MATRIX_SIZE; y++) {
            if (grid[x][y] == 'R') draw(x, y);
            delay(10);
        }
}

void draw(int x, int y, bool red, bool green) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
      if(i == y) {
        digitalWrite(GREEN, green);
        digitalWrite(RED, red);
      }
        if(i == x) {
          digitalWrite(ROW, LOW);
        }
        pulse(SHIFT, true);
        digitalWrite(ROW, HIGH);
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);
        
    }
    pulse(LATCH, true);
}

void pulse(int pin, bool reversed) {
    digitalWrite(pin, !reversed);
    digitalWrite(pin, reversed);
}



void draw_board(board) {
  for(int i = 0; i < board.length();i++) {
    for(int j = 0; j < board.length(); j++) {
      bool colors{2} = chartoArray(board[i][j]);
      draw(i, j, colors[0], colors[1]);
    }
  }
}
