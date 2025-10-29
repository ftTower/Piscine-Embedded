#include <avr/io.h>

#define true 1
#define false 0

void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}
int main() {
    
    set_bit(&DDRB, PB1, true); //! SET PB1 DIRECTION TO OUTPUT (PB1 est la broche OC1A)

    //? CONFIGURATION FOR PWM (mode 14)
    set_bit(&TCCR1A, WGM11, true); //!
    set_bit(&TCCR1B, WGM13, true); //! 1:1:1:0 -> FAST PWM, TOP = ICR1 (mode 14)
    set_bit(&TCCR1B, WGM12, true); //!

    set_bit(&TCCR1A, COM1A1, true); //! TELL MICROCONTROLER WHAT TO DO WITH OCR1A (COM = Compare Output Mode)
    set_bit(&TCCR1A, COM1A0, false); //! TCNT1 == BOTTOM(0) - HIGH -> TCNT1 == TOP(1562) - LOW -> TCNT1 <=>
    
    ICR1 = 15624;  //! SET OVERFLOW SIZE (TOP)
    OCR1A = 1562;  //! (Output Compare Register 1A) SET THE DUTY CYCLE 

    set_bit(&TCCR1B, CS12, true);  //! 
    set_bit(&TCCR1B, CS11, false); //! 1:0:1 -> SET PREDIVISOR ON 1024 
    set_bit(&TCCR1B, CS10, true);  //! 

    while (1) {

    }

    return 0;
}