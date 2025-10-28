#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void set_led_state(int mask, bool state) {
    if (state)
        PORTB |= (1 << mask);
    else
        PORTB &= ~(1 << mask);
}

void decimal_to_binary(int n, char *binary, int size) {
    for (int i = 0; i < size; i++) {
        binary[i] = (n >> i) & 1;
    }
}

void binary_display(char *binary) {
    PORTB = 0x00;
    if (binary[0])
        set_led_state(PB0, true);
    if (binary[1])
        set_led_state(PB1, true);
    if (binary[2])
        set_led_state(PB2, true);
    if (binary[3])
        set_led_state(PB4, true);
}

void wait_button(int mask) {
    while (!(PIND & (1 << mask))) {  //! WAIT FOR BUTTON TO BE RELEASED
        _delay_ms(10);
    }
}

void animation(char *binary) {
    for (int i = 0; i < 5; i++) {
        for (int y = 0; y < 16; y++)  {          
            decimal_to_binary(y, binary, 4);
            binary_display(binary);
            _delay_ms(100);
        }
        _delay_ms(1000);
    }
}

int main() {
    
    
    int value = 0;
    char binary[4] = {0};
    
    animation(binary);
    
    DDRB = 0xFF; //! SET LED DIRECTION IN OUTPUT
    DDRD &= ~(1 << PD2); //! SET SW1 BUTON DIRECTION IN INPUT
    DDRD &= ~(1 << PD4); //! SET SW2 BUTON DIRECTION IN INPUT

    PORTD |= (1 << PD2); //! FORCE BUTTON TO CHOOSE HIGH STATE (pull up)
    PORTD |= (1 << PD4); //! 
    
    while (1) {
        if (!(PIND & (1 << PD2)) && value < 15) //! LOOK IF PD2 BIT IS LOW (button pressed)
        {
            value++;
            wait_button(PD2);
        }
        if (!(PIND & (1 << PD4)) && value > 0) //! LOOK IF PD4 BIT IS LOW (button pressed)
        {
            value--;
            wait_button(PD4);   
        }
    
        decimal_to_binary(value, binary, 4); //! CONVERT DEC TO BIN
        binary_display(binary); //! DISPLAY BIN ON LEDS
    }

    return 0;
}