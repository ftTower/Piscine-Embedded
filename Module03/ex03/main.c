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

    DDRD |= ((1 << PD3) | (1 << PD5) | (1 << PD6));

    TCCR0A |= ((1 << COM0A1) | (1 << WGM00) | (1 << WGM01));
    TCCR0B |= (1 << CS00);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {

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

    uint8_t pwm = 0x00;
	int up = true;
    
	for(;;) {

		OCR0A = pwm;

		pwm += up ? 1 : -1;
		if (pwm == 0xff)
			up = false;
		else if (pwm == 0x00)
			up = true;

		_delay_ms(10);
	}

    while (1) {

    }


    return 0;
}