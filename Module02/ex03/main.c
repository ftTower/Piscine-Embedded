#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"

#define true 1
#define false 0

#define UART_BAUDRATE 115200UL
#define UBRR_HIGH_PRECISION ((F_CPU / (UART_BAUDRATE * 8UL)) - 1)

// ! ======================= UTILS ============================================//

char    toggle_case(char c) {
    if (c >= 'a' && c <= 'z')
        return(c - 32);
    else if (c >= 'A' && c <= 'Z')
        return(c + 32);
    return (c);
}

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

// ! ======================= UART ============================================//

void    uart_init() {

    UBRR0H = (uint8_t)(UBRR_HIGH_PRECISION >> 8);
    UBRR0L = (uint8_t)UBRR_HIGH_PRECISION;        
  
    UCSR0A = (1 << U2X0); 
    
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

char uart_rx(void) {
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

// ! ======================= INTERRUPTS ============================================//

void USART_RX_vect(void) __attribute__ ((signal, used));

void USART_RX_vect(void) { //! INTERRUPTION
    char received = UDR0;
    
    uart_tx(toggle_case(received));
}

// ! ======================= MAIN ============================================//

int main()
{
    uart_init();
    set_bit(&SREG, 7, true); //! ACTIVATE ALL INTERRUPTIONS ON MICROCONTROLER
    
    while (1)
    {

    }
    return 0;
}
