#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"

#define true 1
#define false 0

#define UART_BAUDRATE 115200UL
#define UBRR_HIGH_PRECISION ((F_CPU / (UART_BAUDRATE * 8UL)) - 1)

// ! ======================= UTILS ============================================//


void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}

// ! ======================= UART ============================================//
void    uart_init() {

    UBRR0H = (uint8_t)(UBRR_HIGH_PRECISION >> 8);
    UBRR0L = (uint8_t)UBRR_HIGH_PRECISION;        
  
    UCSR0A = (1 << U2X0); 
    
    UCSR0B = (1 << TXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c) {
    while (!(UCSR0A & (1 << UDRE0)));

    UDR0 = c + 0;
}

void uart_printstr(char *str) {
    if (!str)
        return ;
    while (*str)
        uart_tx(*str++);
    uart_tx('\n');
    uart_tx('\r');
}

// ! ======================= INTERRUPTS ============================================//


void TIMER1_OVF_vect(void) __attribute__ ((signal, used)); //! HEADER OF INTURRUPTION SERVICE

void TIMER1_OVF_vect(void) { //! INTERRUPTION (execute anytime timer 1 overflow)
    uart_printstr("Hello World!");    
}

void    configure_timer1() {
    TCCR1A = 0x00; //! FULLFILL TIMER REGISTER OF 0
    TCCR1B = 0x00; //! 

    set_bit(&TCCR1B, CS12, true); //!
    set_bit(&TCCR1B, CS11, false);  //! SET TIMER 1 PRESCALER ON 256 (microcontroller clock divide by 256)
    set_bit(&TCCR1B, CS10, false); //!

    TCNT1 = 0; //! RESET TIMER ON 0

    set_bit(&TIMSK1, TOIE1, true); //! SET 1 ON TIMSKI TOIE1 BIT (authorize timer to generate an interruption)
}

// ! ======================= MAIN ============================================//


int main()
{
    uart_init();
    configure_timer1();
    
    set_bit(&SREG, 7, true); //! ACTIVATE ALL INTERRUPTIONS ON MICROCONTROLER

    while (1)
    {
        
    }

    return 0;
}
