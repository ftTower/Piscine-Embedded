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

void uart_printstr(volatile char *str, int lignfeed_bool) {
    if (!str)
        return ;
    while (*str && *str != '\r' && *str != '\n')  
        uart_tx(*str++);
    if (lignfeed_bool) {
        uart_tx('\n');
        uart_tx('\r');
    }
}

void uart_delete_last_line(void) {
    uart_tx('\r'); 
    // Efface la ligne entière (ANSI \033[2K)
    uart_printstr("\033[2K", false); }

char uart_rx(void) {
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

// ! ======================= INTERRUPTS ============================================//

#define BUFFER_SIZE 25

volatile char buffer[BUFFER_SIZE];
volatile uint8_t i;

void USART_RX_vect(void) __attribute__ ((signal, used));

void USART_RX_vect(void) { //! INTERRUPTION
    char received = UDR0;
    
    if (received == 10) {
        if (i < BUFFER_SIZE) {
            buffer[i] = '\0';
        }
    }
    else if ((received == 8 || received == 127) && i > 1){
        i--;
        buffer[i] = '.';
    }
    else if (i < BUFFER_SIZE) {
        buffer[i] = received;
        i++;
    }
}

// ! ======================= UTILS ============================================//


unsigned int ft_strlen(volatile char *str){
    unsigned int len = 0;
    while(*str) {
        len++;
        str++;
    }
    return (len);
}

void mem_zero(volatile char *buffer, char c, int size) {
    if (!buffer || size <= 0)
        return ;
    for (int j = 0; j < size - 1; j++)
        buffer[j] = c;
}

int buffer_init(volatile char *buffer, int buffer_size) {
    if (!buffer || buffer_size <= 0)
        return true;
    mem_zero(buffer, '.', buffer_size);
    buffer[buffer_size - 1] = '\0';
    return false;
}

// ! ======================= WOPR ============================================//


int prompt(char *prompt, volatile char *buffer, char *key) {
    i = 0;
    while (1) {
        
        if (buffer[i] == '\0')
            break;
        else {
            uart_delete_last_line();
            uart_printstr(prompt, false);
            uart_printstr(buffer, false);
    
            _delay_ms(50);
        }
        
    }
    
    uart_delete_last_line();
    uart_printstr(prompt, false);
    uart_printstr(buffer, true);
    buffer_init(buffer, BUFFER_SIZE);

    //? RETURN TRUE IF KEY == BUFFER

    return false;
}

int login(char *username, char *password) {
    uart_printstr("Enter your login:", true);
    
    set_bit(&SREG, 7, true); //! ACTIVATE ALL INTERRUPTIONS ON MICROCONTROLER


    int ret_user = prompt("\tusername: ", buffer, username);
    
    // _delay_ms(500);

    int ret_pass = prompt("\tpassword: ", buffer, password);

    if (ret_user || ret_pass)
        return set_bit(&SREG, 7, false), true;

    return false;
}


// ! ======================= MAIN ============================================//
int main()
{
    uart_init();
    
    buffer_init(buffer, BUFFER_SIZE);

    login("tauer", "pass123");
    

    uart_printstr("Bye, Bye ...", true);
    // while (1)
    // {
        

    // }
    return 0;
}
