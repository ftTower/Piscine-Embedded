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

void init_rgb() {

    DDRD |= ((1 << LED_BLUE) | (1 << LED_RED) | (1 << LED_GREEN)); //! LED PINS OUTPUT 

    //? TIMER 0
    TCCR0A = ((1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01)); //* FAST PWM
    TCCR0B = (1 << CS01) | (1 << CS00); //! PRESCALER /64

    //? TIMER 2
    TCCR2A = ((1 << COM2B1) | (1 << WGM20) | (1 << WGM21)); //* FAST PWM
    TCCR2B = (1 << CS22); //! PRESCALER /64

}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    OCR0B = r;
    OCR0A = g;
    OCR2B = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    }
    else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } 
    else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

int main() {

    init_rgb();

    while (1) {

        for (uint8_t pos = 0; pos < 256; pos++) {
            wheel(pos);
            _delay_ms(10);
        }
    }


    return 0;
}