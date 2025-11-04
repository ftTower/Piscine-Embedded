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
    
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c + 0;
}

// void uart_printstr(char *str) {
//     if (!str)
//     return ;
//     while (*str)
//     uart_tx(*str++);
//     uart_tx('\n');
//     uart_tx('\r');
// }

char uart_rx(void) {
    while(!(UCSR0A & (1<<RXC0)));

    toggle_bit(&PORTB, PB0);

    return UDR0;
}


// ! ======================= MAIN ============================================//

int main()
{
    char received; 

    uart_init();
    while (1)
    {
        received = uart_rx();
        uart_tx(received);
    }
    return 0;
}
