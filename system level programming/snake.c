#include <stdint.h>
#include <stdio.h>
#include <led.h>
#include <adc.h>
#include <button.h>



void main(void){

    int8_t snake;
    int8_t length;
    int8_t head = 2;
    int8_t mode = 0;
    //long variable to increase the processing time for the active waiting loop
    long photo;

    while(1){

        //set variables
        snake = 0;
        length = 4 - (sb_adc_read(POTI) / (205));


        //draw snake
        for (int i = head; i >= (head-length); i--) {

            if(i < 0){
                snake = (1<<(8 + i)) | snake;
            }else{
                snake = (1<<i) | snake;
            }
        }
        //if mode is changed
        if(mode == 1){
            snake = ~snake;
        }

        //print the snake with leds
        sb_led_setMask(snake);


        //update head position
        head++;
        head = head%8;

        //determine speed
        photo = sb_adc_read(PHOTO);
        photo = 70000 - photo*50;


        //active waiting loop
        // wait get changed according to photosensor
        volatile long long wait = 0;
        while(wait< photo){
            //if the button is pressed during waiting
            if(sb_button_getState(BUTTON0) == PRESSED){
                if(mode == 1){
                    mode = 0;
                }else{
                    mode = 1;
                }
            }
            wait++;
        }




    }
}