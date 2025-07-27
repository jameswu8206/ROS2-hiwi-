
#include <avr/io.h>
// #include <led.h>

static volatile uint8_t * const ports[8] = {&PORTD, &PORTD, &PORTD, &PORTD, &PORTB, &PORTB, &PORTC, &PORTC};
static volatile uint8_t * const ddr[8] = {&DDRD, &DDRD, &DDRD, &DDRD, &DDRB, &DDRB, &DDRC, &DDRC};
static uint8_t const pins[8] = {PD6, PD5, PD4, PD7, PB0, PB1, PC3, PC2};

typedef enum{
    RED0 = 0,
    YELLOW0 = 1,
    GREEN0 = 2,
    BLUE0 = 3,
    RED1 = 4,
    YELLOW1 = 5,
    GREEN1 = 6,
    BLUE1 = 7
}  __attribute__ ((__packed__)) LED;

int8_t sb_led_on(LED led){
    if (led >= 0 && led <= 7) {
        *(ddr[led]) |= (1<<pins[led]);
        *(ports[led]) &= ~(1<<pins[led]);
        return 0;
    }
    else{
        return -1;
    }

}

int8_t sb_led_off(LED led){
    if (led >= 0 && led <= 7) {
        *(ddr[led]) |= (1<<pins[led]);
        *(ports[led]) |= (1<<pins[led]);
        return 0;
    }
        return -1;
}

int8_t sb_led_toggle(LED led){
    if (led >= 0 && led <= 7) {
        *(ddr[led]) |= (1<<pins[led]);
        *(ports[led]) ^= (1<<pins[led]);
        return 0;
    }
    return -1;
}

void sb_led_setMask(uint8_t mask){
    for(uint8_t i = 0; i<8; i++){
        if((mask >> i) & 1){
            sb_led_on(i);
        }
        sb_led_off(i);
    }
}

int8_t sb_led_showLevel(uint8_t level, uint8_t max){
    if(max == 0){
        return -2;
    }
    if(level > max){
        return -1;
    }
    for(uint8_t i = 0; i < (9*level/max); i++){
        sb_led_on(i);
    }
    return 0;
}