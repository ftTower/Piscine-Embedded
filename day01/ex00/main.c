#include <avr/io.h>

#define true 1
#define false 0

#define ONE_MS_ITERATION_COUNT 350UL 

void    delayer(uint16_t ms) {
    volatile uint32_t total_iterations = (uint32_t)ms * ONE_MS_ITERATION_COUNT; //? PUT A VOLATILE TO LIMIT OPTIMISATION FOR UINT_32

    for (volatile uint32_t i = 0; i < total_iterations; i++) {}
}

void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}

void    toggle_bit(volatile uint8_t *regis, uint8_t bit) {
    *regis ^= bit; //! TOGGLE THE VALUE IN THE REGISTER BIT SPECIFIED
}

int main() {

    set_bit(&DDRB, PB1, true); //! SET DDRB PB0 BIT IN OUTPUT DIRECTION

    while (1) {

        delayer(500);
        toggle_bit(&PORTB , 0b00000010u); //! TOGGLE THE PORTB PB0 BIT STATE  

    }

    return 0;
}