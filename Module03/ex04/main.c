#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define true 1
#define false 0

#define LED_RED PD5
#define LED_GREEN PD6
#define LED_BLUE PD3

//! ============================= UART ===============================

#define UART_BAUDRATE 115200UL
#define UBRR_HIGH_PRECISION ((F_CPU / (UART_BAUDRATE * 8UL)) - 1)

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

//! ============================= SETTERS ===============================

void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    OCR0B = r;
    OCR0A = g;
    OCR2B = b;
}

//! ============================= UTILS ===============================

unsigned int ft_strlen(const char *str) {
    unsigned int len = 0;
    if (!str)
        return len;
    while(*str++)
        len++;
    return len;
}

int format_checker(char *str) {
    if (!str || str[0] != '#')
        return false;
    str++;
    while (*str++)
        if (*str < '0' || *str > '9')
            return false;
    return true;
}

//! ============================= RGB ===============================

void init_rgb() {

    DDRD |= ((1 << LED_BLUE) | (1 << LED_RED) | (1 << LED_GREEN)); //! LED PINS OUTPUT 

    //? TIMER 0
    TCCR0A = ((1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01)); //* FAST PWM
    TCCR0B = (1 << CS01) | (1 << CS00); //! PRESCALER /64

    //? TIMER 2
    TCCR2A = ((1 << COM2B1) | (1 << WGM20) | (1 << WGM21)); //* FAST PWM
    TCCR2B = (1 << CS22); //! PRESCALER /64

}

int hex_to_decimal(const char *hex)
{
    int decimal = 0;
    int base = 1;

    int len = ft_strlen(hex);
    for (int i = len - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            decimal += (hex[i] - '0') * base;
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            decimal += (hex[i] - 'A' + 10) * base;
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            decimal += (hex[i] - 'a' + 10) * base;
        }
        base *= 16;
    }
    return decimal;
}

volatile uint8_t r = 0;
volatile uint8_t g = 0;
volatile uint8_t b = 0;

void hex_rgb_splitter(char *hex_string) {
    char buffer[3];
    buffer[2] = '\0';
    
    buffer[0] = hex_string[1];
    buffer[1] = hex_string[2];
    r = hex_to_decimal(buffer); 

    buffer[0] = hex_string[3];
    buffer[1] = hex_string[4];
    g = hex_to_decimal(buffer); 

    buffer[0] = hex_string[5];
    buffer[1] = hex_string[6];
    b = hex_to_decimal(buffer); 
}

//! ============================= MAIN ===============================

int main() {
    
    char buffer[] = "#000000";

    init_rgb();
    
    // set_rgb(255, 255, 255);

    // // while (1) {
        
    // // }

    // if (format_checker(buffer))
    hex_rgb_splitter(buffer);
    set_rgb(r, g, b);

    

    return 0;
}