#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define true 1
#define false 0

#define PWM_TOP_VALUE 99
#define PWM_MAX_VALUE 100

#define IT_ARR_VALUE 155



#define LED PB1 

void    set_bit(volatile uint8_t *regis, uint8_t bit, int state) {
    if (state)
        *regis |= (1 << bit); //! PUT REGISTER BIT TO HIGH
    else
        *regis &= ~(1 << bit); //! PUT REGISTER BIT TO LOW
}

void    toggle_bit(volatile uint8_t *regis, uint8_t bit) {
    *regis ^= bit; //! TOGGLE THE VALUE IN THE REGISTER BIT SPECIFIED
}



void init_timer_1() {
    set_bit(&DDRB, LED, true);

    set_bit(&TCCR1A, COM1A1, true); //! NON_INVERTING MODE (0 on top)
    set_bit(&TCCR1A, WGM10, true); //! FAST PWM 8 (Mode 5)

    set_bit(&TCCR1B, WGM12, true);
    set_bit(&TCCR1B, CS11, true); //! 64 PRESCALER
    set_bit(&TCCR1B, CS10, true);

    ICR1 = PWM_TOP_VALUE; //! 99

    OCR1A = 0; //! DUTY CYCLE (0%)
}

void init_timer_0() {
    set_bit(&TCCR0A, WGM01, true); //! CLEAR TIMER ON COMPARE MATCH
    
    OCR0A = IT_ARR_VALUE; //! TIMER RESET ON THIS VALUE

    set_bit(&TCCR0B, CS02, true);
    set_bit(&TCCR0B, CS00, true); //! 1024 PRESCALER
    
    set_bit(&TIMSK0, OCIE0A, true); //! ACTIVATE COMPARAISON INTERRUPTION
}

volatile int8_t pwm_step = 2; // Le pas de variation à chaque IT (ex: +2 ou -2)
volatile uint8_t g_duty_cycle = 0; // La valeur actuelle du Duty Cycle (0 à 100)

ISR(TIMER0_COMPA_vect) {

    g_duty_cycle += pwm_step;

    if (g_duty_cycle >= PWM_MAX_VALUE) {
        g_duty_cycle = PWM_MAX_VALUE;
        pwm_step = -2;
    }

    if (g_duty_cycle <= 0) {
        g_duty_cycle = 0;
        pwm_step = 2;
    }

    OCR1A = g_duty_cycle;
}

int main() {


    init_timer_1();
    init_timer_0();

    set_bit(&SREG, 7, true); //! PERMIT ALL INTERRUPTIONS


    while(1) {}

    return 0;
}