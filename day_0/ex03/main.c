#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

int main() {

    
    bool led_state = false;

    DDRB = 0xFF; //! SET LED DIRECTION IN OUTPUT
    DDRD &= ~(1 << PD2); //! SET BUTON DIRECTION IN INPUT
    PORTD |= (1 << PD2); //! FORCE BUTTON TO CHOOSE HIGH STATE (pull up)

    while(1) {

        if (!(PIND & (1 << PD2))) //! LOOK IF PD2 BIT IS LOW (button pressed)
        {
            led_state = !led_state; //! TOGGLE LED STATE
            _delay_ms(200);

            while (!(PIND & (1 << PD2))) {  //! WAIT FOR BUTTON TO BE RELEASED
                _delay_ms(10);
            }
        }
        if (led_state)
            PORTB |= (1 << PB0); //? PUT PB0 TO HIGH
        else
            PORTB &= ~(1 << PB0); //? PUT PB0 TO LOW

    }
    return 0;
}