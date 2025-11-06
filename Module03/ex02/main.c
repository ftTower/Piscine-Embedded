#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define true 1
#define false 0

#define LED_RED PD5
#define LED_GREEN PD6
#define LED_BLUE PD3


void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}



typedef enum rgb{
    BLUE,
    RED,
    GREEN,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE,
} rgb;

void set_rgb(volatile uint8_t *regis, rgb color) {
    set_bit(regis, LED_BLUE, false);
    set_bit(regis, LED_GREEN, false);
    set_bit(regis, LED_RED, false);
    
    switch (color) {
        case BLUE:
            set_bit(regis, LED_BLUE, true);
            break;
        case RED:
            set_bit(regis, LED_RED, true);
            break;
        case GREEN:
            set_bit(regis, LED_GREEN, true);
            break;
        case YELLOW:
            set_bit(regis, LED_RED, true);
            set_bit(regis, LED_GREEN, true);
            break;
        case CYAN:
            set_bit(regis, LED_BLUE, true);
            set_bit(regis, LED_GREEN, true);
            break;
        case MAGENTA:
            set_bit(regis, LED_RED, true);
            set_bit(regis, LED_BLUE, true);
            break;
        case WHITE:
            set_bit(regis, LED_RED, true);
            set_bit(regis, LED_GREEN, true);
            set_bit(regis, LED_BLUE, true);
            break;
        }
}

int main() {

    int colors[7] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};
    int index = 0;

    set_bit(&DDRD, PD3, true);
    set_bit(&DDRD, PD5, true);
    set_bit(&DDRD, PD6, true);


    set_bit(&PORTD, PD3, false);
    set_bit(&PORTD, PD5, false);
    set_bit(&PORTD, PD6, false);

    while (1) {

        if (index == 7)
            index = 0;

        set_rgb(&PORTD, colors[index++]);
        _delay_ms(1000);
    }


    return 0;
}