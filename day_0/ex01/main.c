#include <avr/io.h>
#include <util/delay.h>


int main() {

    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);

}