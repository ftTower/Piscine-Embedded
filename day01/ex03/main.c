#include <avr/io.h>
#include <util/delay.h>

#define true 1
#define false 0

#define DUTY_CYCLE_STEP 1562
#define DUTY_CYCLE_MIN 1562
#define DUTY_CYCLE_MAX 15624

void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}

int     read_bit(volatile uint8_t *regis, uint8_t bit) {
    if (*regis & (1 << bit)) //! LOOK IF REGIS BIT IS HIGH 
        return false;
    return true;
}

void    wait_button(uint32_t bit) {
    while (!(PIND & (1 << bit))) {  //! WAIT FOR BUTTON TO BE RELEASED
        _delay_ms(10);
    }
}

int main() {
    
    set_bit(&DDRB, PB1, true); //! SET PB1 DIRECTION TO OUTPUT (PB1 est la broche OC1A)

    //? CONFIGURATION FOR PWM (mode 14)
    set_bit(&TCCR1A, WGM11, true); //!
    set_bit(&TCCR1B, WGM13, true); //! 1:1:1:0 -> FAST PWM, TOP = ICR1 (mode 14)
    set_bit(&TCCR1B, WGM12, true); //!

    set_bit(&TCCR1A, COM1A1, true); //! TELL MICROCONTROLER WHAT TO DO WITH OCR1A (COM = Compare Output Mode)
    set_bit(&TCCR1A, COM1A0, false); //! TCNT1 == BOTTOM(0) - HIGH -> TCNT1 == TOP(1562) - LOW -> TCNT1 <=>
    
    ICR1 = DUTY_CYCLE_MAX;  //! SET OVERFLOW SIZE (TOP)


    //? DUTY CYCLE = TOP (15624) * WANTED DUTY CYCLE (0.10) = 1562
    //* #define DUTY_CYCLE_STEP 1562
    //* #define DUTY_CYCLE_MIN 1562
    //* #define DUTY_CYCLE_MAX 15624

    OCR1A = DUTY_CYCLE_MIN;  //! (Output Compare Register 1A) SET THE DUTY CYCLE 

    set_bit(&TCCR1B, CS12, true);  //! 
    set_bit(&TCCR1B, CS11, false); //! 1:0:1 -> SET PREDIVISOR ON 1024 
    set_bit(&TCCR1B, CS10, true);  //! 

    while (1) {
        if (read_bit(&PIND, PD4) && OCR1A > 1562) {
            wait_button(PD4);
            OCR1A -= DUTY_CYCLE_STEP;
            _delay_ms(20);
        }

        if (read_bit(&PIND, PD2) && OCR1A < ICR1) {
            wait_button(PD2);
            OCR1A += DUTY_CYCLE_STEP;
            _delay_ms(20);
        }
    }

    return 0;
}