#include <avr/io.h>
#include <util/delay.h>


int main() {
    //* 14.2.1 Configuring the Pin (Explanation about register)
    DDRB = 0xFF; //! SET LED DIRECTION IN OUTPUT 
    DDRD &= ~(1 << PD2); //! SET BUTON DIRECTION IN INPUT
    PORTD |= (1 << PD2); //! FORCE BUTTON TO CHOOSE HIGH STATE (pull up)

    while(1) {

        //* 2.1 Block Diagram (Explanation about Arithmetic Logic Unit)
        if (!(PIND & (1 << PD2))) //! LOOK IF PD2 BIT IS HIGH 
            PORTB |= (1 << PB0); //? PUT PB0 TO HIGH
        else
            PORTB &= ~(1 << PB0); //? PUT PB0 TO LOW

    }
   
    return 0;
}

