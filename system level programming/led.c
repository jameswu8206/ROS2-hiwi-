#include <led.h>
void main(void){
    //use while loop to let it run forever
    while(1){
        //light on
        sb_led_on(BLUE0);

        //set waiting time variable
        volatile long long wait_time=1;
        //wait
        while(wait_time<90000){
            wait_time++;
        }
        //light1 off and light2 on
        sb_led_off(YELLOW0);
        sb_led_on(YELLOW1);

        //wait
        while(wait_time<180000){
            wait_time++;
        }
        //light off
        sb_led_off(YELLOW1);


    }


}