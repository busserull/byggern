#include <avr/io.h>
#include "uart.h"
#include "hid.h"
#include "display.h"

#define F_CPU 4915200UL
#include <util/delay.h>

#include <stdio.h>
#include <avr/interrupt.h>

static void system_init(){
    /* System clock prescaler of 1 */
    CLKPR = (1 << CLKPCE);

    /* Enable external memory interface */
    MCUCR |= (1 << SRE);

    /* Mask out JTAG pins from external address lines */
    SFIOR |= (1 << XMM2);
}

int main(){
    system_init();

    hid_init();
    sei();

    display_init();

    while(1){
        /* Nop */
    }

    uart_init();
    fdevopen((int (*)(char, FILE*)) uart_send,(int (*)(FILE*)) uart_recv);


    volatile uint8_t * disp_c = (volatile uint8_t *)0x1000;
    volatile uint8_t * disp_d = (volatile uint8_t *)0x1200;

    uint16_t loop = 0;

    HidJoystick joystick;
    HidSlider slider;
    HidButton button;

    
    //display_push_ram_to_oled();
    
    while(1){
        
        //loop++;
        if(loop > 9){
            loop = 0;

            *disp_d = 0xa6;
            _delay_ms(1000);
            *disp_d = 0xa7;
        }
        
        
        _delay_ms(5);

        joystick = hid_joystick_read();
        slider = hid_slider_read();
        button = hid_button_read();

        printf(
            "BL: %1d BR: %1d JX: %3d JY: %3d SL: %3d SR: %3d\n\r",
            button.left,
            button.right,
            joystick.x,
            joystick.y,
            slider.left,
            slider.right
        );
    }

    return 0;
}
