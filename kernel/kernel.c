#include "../include/io.h"
#include "screen.c"
#include "input.c"

#define STATE_LOCKED   0
#define STATE_HOME     1

int current_state = STATE_LOCKED;
char pin_input[5] = {0};
int pin_len = 0;
const char *correct_pin = "1234";

void draw_lock_screen() {
    fill_rect(0, 0, WIDTH, HEIGHT, 2); // тёмно-синий фон
    draw_text(120, 60, "CzoS", 1);
    draw_text(100, 100, "Enter PIN:", 1);

    for (int i = 0; i < 4; i++) {
        int cx = 140 + i * 20;
        if (i < pin_len)
            fill_rect(cx, 130, 12, 12, 1); // заполненная точка
        else
            fill_rect(cx, 130, 12, 12, 4); // пустая точка
    }
}

void draw_home_screen() {
    // Статус-бар
    fill_rect(0, 0, WIDTH, 16, 4);
    draw_text(4, 2, "CzoS Home", 1);
    draw_text(260, 2, "CPU 100%", 1);

    // Фон
    fill_rect(0, 16, WIDTH, HEIGHT - 16, 0);

    const char *apps[] = {"Calc", "Notes", "Time",
                          "File", "Set",  "Game",
                          "Info", "Mus",  "Sys"};
    unsigned char colors[] = {3, 5, 6, 7, 3, 5, 6, 7, 3};

    for (int i = 0; i < 9; i++) {
        int col = i % 3;
        int row = i / 3;
        int ix = 40 + col * 90;
        int iy = 48 + row * 50;

        fill_rect(ix, iy, 24, 24, colors[i]); // иконка
        draw_text(ix, iy + 28, apps[i], 1);  // подпись
    }
}

void handle_key(unsigned char scancode) {
    if (current_state == STATE_LOCKED) {
        // Цифры: сканкоды 2–10 (1–9), 11 (0)
        if (scancode >= 2 && scancode <= 11) {
            char digit = (scancode == 11) ? '0' : ('0' + (scancode - 2));
            if (pin_len < 4) {
                pin_input[pin_len++] = digit;
            }
        } else if (scancode == 28) { // Enter
            if (pin_len == 4) {
                int ok = 1;
                for (int i = 0; i < 4; i++)
                    if (pin_input[i] != correct_pin[i]) ok = 0;
                if (ok) current_state = STATE_HOME;
                else pin_len = 0; // ошибка
            }
        } else if (scancode == 14) { // Backspace
            if (pin_len > 0) pin_len--;
        }
    } else if (current_state == STATE_HOME) {
        if (scancode == 1) { // ESC
            current_state = STATE_LOCKED;
            pin_len = 0;
        }
        // Здесь потом можно добавить обработку кликов по иконкам
    }
}

void kernel_main() {
    set_palette();

    while (1) {
        if (current_state == STATE_LOCKED)
            draw_lock_screen();
        else if (current_state == STATE_HOME)
            draw_home_screen();

        unsigned char sc = get_scancode();
        if (sc != 0xFF)
            handle_key(sc);
    }
}
