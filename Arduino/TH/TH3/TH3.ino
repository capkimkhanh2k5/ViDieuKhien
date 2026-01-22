#include <LiquidCrystal.h>

// --- Định nghĩa tần số nốt nhạc ---
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_E3  165
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_F5  698

// --- Cấu hình phần cứng ---
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int leds[] = {9, 8, A4, A5}; 
const int pinCoi = 13;
const int buttons[] = {A0, A1, A2, A3};

// --- Dữ liệu nhạc ---
const int melody1[] = {NOTE_E4, NOTE_E4, 0, NOTE_E4, 0, NOTE_C4, NOTE_E4, 0, NOTE_G4, 0, NOTE_G3, 0, NOTE_C4, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_AS3, NOTE_A3};
const int dur1[] = {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 200, 200, 200, 200, 200, 200, 200};

const int melody2[] = {NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5, NOTE_GS4, NOTE_F4, NOTE_C5, NOTE_A4};
const int dur2[] = {500, 500, 500, 350, 150, 500, 350, 150, 1000, 500, 500, 500, 350, 150, 500, 350, 150, 1000};

const int melody3[] = {NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, 0, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, 0, NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4};
const int dur3[] = {200, 200, 400, 400, 400, 800, 200, 200, 200, 400, 400, 400, 800, 200, 200, 200, 400, 400, 400, 400, 400};

const int melody4[] = {NOTE_E4, NOTE_E4, NOTE_E4, 0, NOTE_E4, NOTE_E4, NOTE_E4, 0, NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4, 0, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4};
const int dur4[] = {200, 200, 400, 100, 200, 200, 400, 100, 200, 200, 300, 100, 800, 100, 200, 200, 200, 200, 200, 200, 200, 100, 200, 200, 200, 200, 800};

// --- Biến điều khiển ---
int currentMode = 0;
int noteIndex = 0;
unsigned long prevNoteMillis = 0;
unsigned long noteDuration = 0;
bool isPlaying = false;

char txt[] = "HELLO WORLD";
int currentIndex = -11; // Vị trí bắt đầu của chữ chạy

unsigned long prevScrollMillis = 0;
const int scrollInterval = 250;

void setup() {
    lcd.begin(16, 2);
    for(int i=0; i<4; i++) {
        pinMode(buttons[i], INPUT_PULLUP);
        pinMode(leds[i], OUTPUT);
    }
    pinMode(pinCoi, OUTPUT);
    lcd.setCursor(0, 1);
    lcd.print(">I Love Arduino");
}

void loop() {
    handleLCDScroll();
    handleButtons();
    playMusicAsync();
}

// 1. Xử lý chữ chạy (Không dùng delay)
void handleLCDScroll() {
    if (millis() - prevScrollMillis >= scrollInterval) {
        prevScrollMillis = millis();

        lcd.setCursor(0, 0);
        for(int i = 0; i < 16; i++) lcd.print(" ");

        for (int i = 0; i < 11; i++) {
            int pos = currentIndex + i;
            if (pos >= 0 && pos < 16) {
                lcd.setCursor(pos, 0);
                lcd.print(txt[i]);
            }
        }

        currentIndex++;
        if (currentIndex >= 16) currentIndex = -11;
    }
}

// 2. Xử lý nút bấm
void handleButtons() {
    for (int i = 0; i < 4; i++) {
        if (digitalRead(buttons[i]) == LOW) {
            if (currentMode != i + 1) {
                currentMode = i + 1;
                noteIndex = 0;
                isPlaying = true;
                prevNoteMillis = 0;
                
                // Cập nhật giao diện
                for(int j=0; j<4; j++) digitalWrite(leds[j], (j == i));
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                switch(currentMode) {
                    case 1: lcd.print(">M1: MarioTheme"); break;
                    case 2: lcd.print(">M2: StarWars"); break;
                    case 3: lcd.print(">M3: HappyB-Day"); break;
                    case 4: lcd.print(">M4: JingleBells"); break;
                }
            }
        }
    }
}

// 3. Xử lý nhạc đa nhiệm
void playMusicAsync() {
    if (!isPlaying) return;

    if (millis() - prevNoteMillis >= noteDuration) {
        noTone(pinCoi);
        
        const int* melody;
        const int* durations;
        int totalNotes;

        // Chọn bài hát
        if (currentMode == 1) { melody = melody1; durations = dur1; totalNotes = 19; }
        else if (currentMode == 2) { melody = melody2; durations = dur2; totalNotes = 18; }
        else if (currentMode == 3) { melody = melody3; durations = dur3; totalNotes = 21; }
        else if (currentMode == 4) { melody = melody4; durations = dur4; totalNotes = 27; }

        if (noteIndex < totalNotes) {
            int currentNoteFreq = melody[noteIndex];
            int currentNoteDur = durations[noteIndex];
            
            if (currentNoteFreq != 0) {
                tone(pinCoi, currentNoteFreq, currentNoteDur);
            }
            
            noteDuration = currentNoteDur * 1.3; // Thời gian nghỉ giữa các nốt
            prevNoteMillis = millis();
            noteIndex++;
        } else {
            isPlaying = false;
            noteIndex = 0;
            noTone(pinCoi);
        }
    }
}