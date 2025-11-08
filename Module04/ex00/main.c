#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define true 1
#define false 0

#define BUTTON INT0
#define LED PB0 

void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}

void    toggle_bit(volatile uint8_t *regis, uint8_t bit) {
    *regis ^= bit; //! TOGGLE THE VALUE IN THE REGISTER BIT SPECIFIED
}

ISR(INT0_vect) {

    toggle_bit(&PORTB, (1 << PB0));   
    _delay_ms(200);

}

void init_interrupts() {
    DDRD = 0b00000001;
    PORTD = 0b00000001;
    DDRB = (1 << PB0);  // Configure PB0 as output

    set_bit(&EIMSK, INT0, true);
    set_bit(&EICRA, ISC00, true);
    set_bit(&EICRA, ISC01, true);
    
    set_bit(&SREG, 7, true);
}

int main() {


    init_interrupts();

    while(1) {}

    return 0;
}