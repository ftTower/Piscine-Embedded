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

void uart_printstr(char *str, int lignfeed_bool) {
    if (!str)
        return ;
    while (*str)
        uart_tx(*str++);
    if (lignfeed_bool) {
        uart_tx('\n');
        uart_tx('\r');
    }
}

void uart_delete_last_char() {
    uart_tx('\b');      // Move cursor back
    uart_tx(' ');       // Overwrite character with space
    uart_tx('\b');      // Move cursor back again
}

char uart_rx(void) {
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

// ! ======================= INTERRUPTS ============================================//

char *buffer_username = NULL;
char *buffer_password = NULL;


void USART_RX_vect(void) __attribute__ ((signal, used));

void USART_RX_vect(void) { //! INTERRUPTION
    char received = UDR0;
    
    
    
    
    
    
    uart_tx(received);
}

// ! ======================= WOPR ============================================//



int login(char *username, char *password) {
    uart_printstr("Enter your login:", true);
    uart_printstr("\tusername: ", false);

    return false;
}


// ! ======================= MAIN ============================================//

int main()
{
    uart_init();
    
    set_bit(&SREG, 7, true); //! ACTIVATE ALL INTERRUPTIONS ON MICROCONTROLER

    // login("tauer", "pass123");
    while (1)
    {

    }
    return 0;
}
