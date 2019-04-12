#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LATCH 10;
#define SHIFT 11;
#define RESET 12;
#define ROW 7;
#define RED 8;
#define GREEN 9;

#define R_LED 2;
#define G_LED 3;
#define B_LED 4;

const int MATRIX_SIZE = 8;

int turn = 0;

String who_turn[2] = {"Red plays", "Green plays"};
char players[2] = {'R', 'G'};

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
        for (int j = 0; i < MATRIX_SIZE; i++) {
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
    set_status(1, 1, 1);
    lcd.setCursor(3, 0);
    lcd.print("Game start");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Red player starts");
    set_status(1, 0, 0);
}

void credits() {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Connect4");
    lcd.setCursor(0, 1);
    lcd.print("On Arduino Nano");
    set_status(1, 0, 0);
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HardwareSoftware");
    lcd.setCursor(1, 1);
    lcd.print("By Alex ROBIC");
    set_status(0, 1, 0);
    delay(3000);
    lcd.clear();
    set_status(0, 0, 1);
    lcd.setCursor(4, 0);
    lcd.print("Software   *");
    lcd.setCursor(1, 1);
    lcd.print("By Irwin MADET");
    delay(3000);

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
    if (av == 0) {
        return (1);
    } else if (av == 680) {
        return (2);
    } else if (av == 852 or av == 851) {
        return (3);
    } else if (av == 920) {
        return (4);
    } else if (av == 955) {
        return (5);
    } else if (av == 974 or av == 975) {
        return (6);
    } else if (av == 987) {
        return (7);
    } else {
        return (NULL);
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
    if (grid[i][row] == ' ') {
        grid[i][row] = color;
        while ((grid[i + 1][row] == ' ') and (i <= 7)) {
            draw_frames(25);
            i += 1;
            grid[i - 1][row] = ' ';
            grid[i][row] = color;
        }
    }
}

void draw_frames(int number) {
    for (int a = 0; a < number; a++) {
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
    if (r != 0) {
        char current_player = players[turn % 2];
        if (current_player == 'R') { set_status(1, 0, 0); } else { set_status(0, 1, 0); }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(who_turn[(turn + 1) % 2]);
        lcd.setCursor(0, 1);
        lcd.print(r);
        animate(current_player, r - 1);
        turn += 1;
    } else {
        draw_frames(10);
    }
}
