#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <7seg.h>
#include <led.h>

//def types
typedef enum { PD_RED,PD_WAITING,ERROR } state;

//events variables
//volatile to make sure the variables are not optimized
//and detect the change of variables everytime it is changed and read
static volatile uint8_t button_pressed=0;
static volatile uint8_t timer_counter=0;
static volatile uint8_t error=0;
static volatile state current_state;

//initialize
static void init(void){
    //button0/1 input
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    DDRD &= ~(1 << PD3);
    PORTD |= (1 << PD3);
    //falling edge for button0
    EICRA &= ~(1 << ISC00);
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
    //low level for button1
    EICRA &= ~(1 << ISC10);
    EICRA &= ~(1 << ISC11);
    EIMSK |= (1<<INT1);
    //timer prescaler 1024 with overflow interrupt
    TCCR0B &= ~(1 << CS01);
    TCCR0B |= (1 << CS02)|(1 << CS00);
    TIMSK0 |=(1 << TOIE0);
    current_state = PD_RED;
}

//button0 interrupt
ISR(INT0_vect){
    button_pressed = 1;
    sb_led_on(BLUE1);
}

//timer interrupt
ISR(TIMER0_OVF_vect){
    timer_counter++;
}

//button1 interrupt
ISR(INT1_vect){
    error = 1;
    button_pressed = 0;
    current_state = ERROR;

}

//wait function with timer overflow interrupt
static void wait(void){
    //if press error then skip waiting and go to error state
    if(error == 0){
        sleep_enable();
        timer_counter = 0;
        cli();
        //if there is an interrupt after if statement
        while(timer_counter <= 61 && error == 0){
            sei();
            sleep_cpu();
            cli();
        }
        sei();
        sleep_disable();
    }
    //go to error state
    else{
        current_state = ERROR;
    }
}

//run lights depends on current state
static void setState(state current_state){
    sb_led_setMask(0);
    switch(current_state){
        case PD_RED:
            sb_led_on(RED0);
            sb_led_on(RED1);
            wait();
            sb_led_on(YELLOW0);
            wait();
            sb_led_off(RED0);
            sb_led_off(YELLOW0);
            sb_led_on(GREEN0);
            sleep_enable();
            cli();
            while(button_pressed==0&&error==0){
                sei();
                sleep_cpu();
                cli();
            }
            sei();
            sleep_disable();
            break;
        case PD_WAITING:
            EIMSK &= ~(1<<INT0);
            sb_led_on(BLUE1);
            sb_led_on(GREEN0);
            sb_led_on(RED1);

            for(uint8_t i=0;i<5;i++){
                sb_7seg_showNumber(8-i);
                wait();
            }
            sb_led_off(GREEN0);
            sb_led_on(YELLOW0);
            sb_7seg_showNumber(3);
            wait();
            sb_led_off(YELLOW0);
            sb_led_on(RED0);
            for(uint8_t i=0;i<2;i++){
                sb_7seg_showNumber(2-i);
                wait();
            }
            sb_7seg_disable();
            sb_led_off(RED1);
            sb_led_on(GREEN1);
            sb_led_off(BLUE1);
            for(uint8_t i=0;i<5;i++){
                wait();
            }
            EIMSK |= (1<<INT0);
            break;
        case ERROR:
            sleep_enable();
            cli();
            while(error == 1){
                error = 0;
                sb_led_toggle(YELLOW0);
                sei();
                sleep_cpu();
                cli();
            }
            sei();
            sleep_disable();
            break;
    }
}

void main(void){

    //run once
    init();
    sei();

    while(1){

        setState(current_state);

        //if button0 is pressed, with different current state
        //if statement under each case to make enter error state at anytime
        switch (current_state) {
            case PD_RED:
                if(button_pressed == 1 && error == 0){
                    current_state = PD_WAITING;
                    break;
                }
                if(error == 1){
                    current_state = ERROR;
                }
                break;
            case PD_WAITING:
                if(error == 0){
                    button_pressed = 0;
                    current_state = PD_RED;
                }
                if(error == 1){
                    current_state = ERROR;
                }
                break;
            case ERROR:
                error = 0;
                current_state = PD_RED;
                break;
        }

    }
}