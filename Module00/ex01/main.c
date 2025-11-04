#include <avr/io.h>


int main() {

    DDRB |= (1 << PB0); //* 1.1 Pin Descriptions
    PORTB |= (1 << PB0);

}
