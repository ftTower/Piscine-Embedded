#include <avr/io.h>

#define true 1
#define false 0

void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}

volatile uint8_t overflow_count = 0; //! COUNT EACH TIME TIMER1 OVERFLOW (volatile because it will change at anytime in program)

void TIMER1_OVF_vect(void) __attribute__ ((signal, used)); //! HEADER OF INTURRUPTION SERVICE

void TIMER1_OVF_vect(void) { //! INTERRUPTION (execute anytime timer 1 overflow)

    overflow_count++;

    if (overflow_count >= 2) {
        set_bit(&PINB, PB1, true); //! PORT FORBIDDEN IN SUBJECT SO IM USING PINB (set a 1 on a pin toggle the state)
    }
}

int main() {

    set_bit(&DDRB, PB1, true); //! SET PB1 DIRECTION TO OUTPUT

    TCCR1A = 0x00; //! FULLFILL TIMER REGISTER OF 0
    TCCR1B = 0x00; //! 

    set_bit(&TCCR1B, CS12, true); //!
    set_bit(&TCCR1B, CS11, false);  //! SET TIMER 1 PRESCALER ON 256 (microcontroller clock divide by 256)
    set_bit(&TCCR1B, CS10, false); //!

    TCNT1 = 0; //! RESET TIMER ON 0

    set_bit(&TIMSK1, TOIE1, true); //! SET 1 ON TIMSKI TOIE1 BIT (authorize timer to generate an interruption)
    set_bit(&SREG, 7, true); //! ACTIVATE ALL INTERRUPTIONS ON MICROCONTROLER


    while (1) { //! THE LOOP NEED TO BE EMPTY

    }

    return 0;
}