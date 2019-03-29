const int LATCH = 10;
const int SHIFT = 11;
const int RESET = 12;

const int ROW = 7;
const int RED = 8;
const int GREEN = 9;

const int MATRIX_SIZE = 8;

const char COLORS_NAMES[] = {'R', 'G', 'O', ' '};
const bool COLORS_BOOLS[][2] = {{true,  false},
                                {false, true},
                                {true,  true},
                                {false, false}};

const char grid[8][8] =
        {
                {' ', 'R', 'R', ' ', 'R', 'R', ' ', ' '},
                {'R', 'R', 'R', 'R', 'R', 'R', 'R', ' '},
                {'R', 'R', 'R', 'R', 'R', 'R', 'R', ' '},
                {' ', 'R', 'R', 'R', 'R', 'R', ' ', ' '},
                {' ', ' ', 'R', 'R', 'R', ' ', ' ', ' '},
                {' ', ' ', ' ', 'R', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        };

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
            draw(x, y, charToArray(grid[x][y]));
            delay(10);
        }
}

void draw(int x, int y, bool color[]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (i == x) digitalWrite(ROW, LOW);
        if (i == y) digitalWrite(RED, HIGH);

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

bool *charToArray(char c) {
    bool result[2] = {false, false};
    for (int i = 0; i < sizeof(COLORS_NAMES); i++) {
        if (COLORS_NAMES[i] == c) {
            result = COLORS_BOOLS[i];
            break;
        }
    }
    return result;
}