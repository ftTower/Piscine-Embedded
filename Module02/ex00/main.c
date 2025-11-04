#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"

#define UART_BAUDRATE 115200UL
#define UBRR_HIGH_PRECISION ((F_CPU / (UART_BAUDRATE * 8UL)) - 1)

void    uart_init() {

    UBRR0H = (uint8_t)(UBRR_HIGH_PRECISION >> 8);
    UBRR0L = (uint8_t)UBRR_HIGH_PRECISION;        
  
    UCSR0A = (1 << U2X0); 
    
    UCSR0B = (1 << TXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0)));

    UDR0 = c;
}

int main()
{
    uart_init();

    while (1)
    {
        uart_tx('z' + 0);
        
        _delay_ms(1000);
    }

    return 0;
}