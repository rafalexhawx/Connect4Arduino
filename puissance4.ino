#define LATCH 10
#define SHIFT 11
#define RESET 12

#define ROW 7
#define RED 8
#define GREEN 9

const int MATRIX_SIZE = 8;

char COLORS_NAMES[] = {'R', 'G', 'O', ' '};
bool COLORS_BOOLS[][2] = {{true,  false},
                          {false, true},
                          {true,  true},
                          {false, false}};

char grid[8][8];

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

    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; i < MATRIX_SIZE; i++) {
            grid[i][j] = ' ';
        }
}

void pulse(int pin, bool reversed) {
    digitalWrite(pin, !reversed);
    digitalWrite(pin, reversed);
}

void draw(int x, int y, bool color[]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (i == x) digitalWrite(ROW, LOW);
        if (i == y) {
            digitalWrite(RED, color[0]);
            digitalWrite(GREEN, color[1]);
        }

        pulse(SHIFT, true);

        digitalWrite(ROW, HIGH);
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);
    }
    pulse(LATCH, true);
}

bool *charToArray(char c) {
    for (int i = 0; i < sizeof(COLORS_NAMES); i++) {
        if (COLORS_NAMES[i] == c) return COLORS_BOOLS[i];
    }
    return NULL;
}

void loop() {
    for (int x = 0; x < MATRIX_SIZE; x++)
        for (int y = 0; y < MATRIX_SIZE; y++) {
            draw(x, y, charToArray(grid[x][y]));
        }
}