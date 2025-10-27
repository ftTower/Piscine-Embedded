#include <avr/io.h>
#include <util/delay.h>


int main() {

    DDRB |= (1 << PB0); // Configure PB0 as output

    PORTB |= (1 << PB0);

}