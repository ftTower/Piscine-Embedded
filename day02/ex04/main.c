#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"

#define true 1
#define false 0

#define UART_BAUDRATE 115200UL
#define UBRR_HIGH_PRECISION ((F_CPU / (UART_BAUDRATE * 8UL)) - 1)

// ! ======================= BIT UTILS ============================================//

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
        if (*str == '\n') {
            uart_tx('\n');
            uart_tx('\r');
        }    
        else
            uart_tx(*str++);
    if (lignfeed_bool) {
        uart_tx('\n');
        uart_tx('\r');
    }
}

void uart_delete_last_line(void) {
    uart_printstr("\033[A\033[2K\r", false);
}

char uart_rx(void) {
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

// ! ======================= INTERRUPTS ============================================//

char buffer[25];
volatile uint8_t index;

void USART_RX_vect(void) __attribute__ ((signal, used));

void USART_RX_vect(void) { //! INTERRUPTION
    char received = UDR0;
    
    if (received == 10) {
        if (index < 24)
            buffer[index] = received;
    }
    if (received == 13){
    }
    if ((received == 8 || received == 127) && index > 0){
        index--;
        buffer[index] = '.';
    }
    else if (index < 24) {
        buffer[index] = received;
        index++;
    }
        
    // uart_printstr(buffer_username, true);
    // uart_printstr(buffer_password, true);
    
    
    // uart_tx(received);
}

// ! ======================= UTILS ============================================//


unsigned int ft_strlen(char *str){
    unsigned int len = 0;
    while(*str) {
        len++;
        str++;
    }
    return (len);
}

void mem_zero(char *buffer, char c, int size) {
    if (!buffer || size <= 0)
        return ;
    for (int i = 0; i < size; i++)
        buffer[i] = c;
}

int is_char(char *buffer, char c) {
    unsigned int len = ft_strlen(buffer);
    for (unsigned int i = 0;  i < len; i++) 
        if (buffer[i] == c)
            return true;
    return false;
}

int key_cmp(char *buffer, char *key) {
    for (int i = 0; buffer[i] && key[i]; i++) {
        if (buffer[i] != key[i])
            return false;
    }

    return true;
}

int buffer_init(char *buffer, int buffer_size) {
    if (!buffer || buffer_size <= 0)
        return true;
    buffer[buffer_size - 1] = '\0';
    mem_zero(buffer, '.', buffer_size - 1);
    return false;
}

// ! ======================= WOPR ============================================//


int prompt(char *prompt, char *buffer, char *key) {
    set_bit(&SREG, 7, true); //! ACTIVATE ALL INTERRUPTIONS ON MICROCONTROLER
    while (1) {
        uart_printstr(prompt, false);
        uart_printstr(buffer, true);
        
        if (is_char(buffer, '\n'))
            break;

        _delay_ms(10);
        uart_delete_last_line();
    }
    set_bit(&SREG, 7, false); //! DEACTIVATE ALL INTERRUPTIONS ON MICROCONTROLER


    return false;
}

int login(char *username, char *password) {
    uart_printstr("Enter your login:", true);
    
    int ret = prompt("\tusername: ", buffer, username);
    
    mem_zero(buffer, '.', 25);
    index = 0;

    uart_printstr("\n\r", false);
    uart_printstr("\n\r", false);

    if (prompt("\tpassword: ", buffer, password) || ret)
        return true;

    return false;
}


// ! ======================= MAIN ============================================//

int main()
{
    uart_init();
    
    index = 0;
    buffer_init(buffer, 25);




    login("tauer", "pass123");
    
    while (1)
    {
        

    }
    return 0;
}
