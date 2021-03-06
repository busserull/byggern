#include "display.h"
#include <stdint.h>
#include <avr/pgmspace.h>

#define DISPLAY_C_MEM ((volatile uint8_t *)0x1000)
#define DISPLAY_D_MEM ((volatile uint8_t *)0x1200)

#define DISPLAY_NUMBER_OF_PAGES 8
#define DISPLAY_NUMBER_OF_SEGMENTS 128
#define DISPLAY_FONT_WIDTH 5

static const uint8_t PROGMEM m_font[95][5] = {
	{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}, //
	{0b00000000,0b00000000,0b01011111,0b00000000,0b00000000}, // !
	{0b00000000,0b00000111,0b00000000,0b00000111,0b00000000}, // "
	{0b00010100,0b01111111,0b00010100,0b01111111,0b00010100}, // #
	{0b00100100,0b00101010,0b01111111,0b00101010,0b00010010}, // $
	{0b00100011,0b00010011,0b00001000,0b01100100,0b01100010}, // %
	{0b00110110,0b01001001,0b01010101,0b00100010,0b01010000}, // &
	{0b00000000,0b00000101,0b00000011,0b00000000,0b00000000}, // '
	{0b00000000,0b00011100,0b00100010,0b01000001,0b00000000}, // (
	{0b00000000,0b01000001,0b00100010,0b00011100,0b00000000}, // )
	{0b00001000,0b00101010,0b00011100,0b00101010,0b00001000}, // *
	{0b00001000,0b00001000,0b00111110,0b00001000,0b00001000}, // +
	{0b00000000,0b01010000,0b00110000,0b00000000,0b00000000}, // ,
	{0b00001000,0b00001000,0b00001000,0b00001000,0b00001000}, // -
	{0b00000000,0b01100000,0b01100000,0b00000000,0b00000000}, // .
	{0b00100000,0b00010000,0b00001000,0b00000100,0b00000010}, // /
	{0b00111110,0b01010001,0b01001001,0b01000101,0b00111110}, // 0
	{0b00000000,0b01000010,0b01111111,0b01000000,0b00000000}, // 1
	{0b01000010,0b01100001,0b01010001,0b01001001,0b01000110}, // 2
	{0b00100001,0b01000001,0b01000101,0b01001011,0b00110001}, // 3
	{0b00011000,0b00010100,0b00010010,0b01111111,0b00010000}, // 4
	{0b00100111,0b01000101,0b01000101,0b01000101,0b00111001}, // 5
	{0b00111100,0b01001010,0b01001001,0b01001001,0b00110000}, // 6
	{0b00000001,0b01110001,0b00001001,0b00000101,0b00000011}, // 7
	{0b00110110,0b01001001,0b01001001,0b01001001,0b00110110}, // 8
	{0b00000110,0b01001001,0b01001001,0b00101001,0b00011110}, // 9
	{0b00000000,0b00110110,0b00110110,0b00000000,0b00000000}, // :
	{0b00000000,0b01010110,0b00110110,0b00000000,0b00000000}, // ;
	{0b00000000,0b00001000,0b00010100,0b00100010,0b01000001}, // <
	{0b00010100,0b00010100,0b00010100,0b00010100,0b00010100}, // =
	{0b01000001,0b00100010,0b00010100,0b00001000,0b00000000}, // >
	{0b00000010,0b00000001,0b01010001,0b00001001,0b00000110}, // ?
	{0b00110010,0b01001001,0b01111001,0b01000001,0b00111110}, // @
	{0b01111110,0b00010001,0b00010001,0b00010001,0b01111110}, // A
	{0b01111111,0b01001001,0b01001001,0b01001001,0b00110110}, // B
	{0b00111110,0b01000001,0b01000001,0b01000001,0b00100010}, // C
	{0b01111111,0b01000001,0b01000001,0b00100010,0b00011100}, // D
	{0b01111111,0b01001001,0b01001001,0b01001001,0b01000001}, // E
	{0b01111111,0b00001001,0b00001001,0b00000001,0b00000001}, // F
	{0b00111110,0b01000001,0b01000001,0b01010001,0b00110010}, // G
	{0b01111111,0b00001000,0b00001000,0b00001000,0b01111111}, // H
	{0b00000000,0b01000001,0b01111111,0b01000001,0b00000000}, // I
	{0b00100000,0b01000000,0b01000001,0b00111111,0b00000001}, // J
	{0b01111111,0b00001000,0b00010100,0b00100010,0b01000001}, // K
	{0b01111111,0b01000000,0b01000000,0b01000000,0b01000000}, // L
	{0b01111111,0b00000010,0b00000100,0b00000010,0b01111111}, // M
	{0b01111111,0b00000100,0b00001000,0b00010000,0b01111111}, // N
	{0b00111110,0b01000001,0b01000001,0b01000001,0b00111110}, // O
	{0b01111111,0b00001001,0b00001001,0b00001001,0b00000110}, // P
	{0b00111110,0b01000001,0b01010001,0b00100001,0b01011110}, // Q
	{0b01111111,0b00001001,0b00011001,0b00101001,0b01000110}, // R
	{0b01000110,0b01001001,0b01001001,0b01001001,0b00110001}, // S
	{0b00000001,0b00000001,0b01111111,0b00000001,0b00000001}, // T
	{0b00111111,0b01000000,0b01000000,0b01000000,0b00111111}, // U
	{0b00011111,0b00100000,0b01000000,0b00100000,0b00011111}, // V
	{0b01111111,0b00100000,0b00011000,0b00100000,0b01111111}, // W
	{0b01100011,0b00010100,0b00001000,0b00010100,0b01100011}, // X
	{0b00000011,0b00000100,0b01111000,0b00000100,0b00000011}, // Y
	{0b01100001,0b01010001,0b01001001,0b01000101,0b01000011}, // Z
	{0b00000000,0b00000000,0b01111111,0b01000001,0b01000001}, // [
	{0b00000010,0b00000100,0b00001000,0b00010000,0b00100000}, // "\"
	{0b01000001,0b01000001,0b01111111,0b00000000,0b00000000}, // ]
	{0b00000100,0b00000010,0b00000001,0b00000010,0b00000100}, // ^
	{0b01000000,0b01000000,0b01000000,0b01000000,0b01000000}, // _
	{0b00000000,0b00000001,0b00000010,0b00000100,0b00000000}, // `
	{0b00100000,0b01010100,0b01010100,0b01010100,0b01111000}, // a
	{0b01111111,0b01001000,0b01000100,0b01000100,0b00111000}, // b
	{0b00111000,0b01000100,0b01000100,0b01000100,0b00100000}, // c
	{0b00111000,0b01000100,0b01000100,0b01001000,0b01111111}, // d
	{0b00111000,0b01010100,0b01010100,0b01010100,0b00011000}, // e
	{0b00001000,0b01111110,0b00001001,0b00000001,0b00000010}, // f
	{0b00001000,0b00010100,0b01010100,0b01010100,0b00111100}, // g
	{0b01111111,0b00001000,0b00000100,0b00000100,0b01111000}, // h
	{0b00000000,0b01000100,0b01111101,0b01000000,0b00000000}, // i
	{0b00100000,0b01000000,0b01000100,0b00111101,0b00000000}, // j
	{0b00000000,0b01111111,0b00010000,0b00101000,0b01000100}, // k
	{0b00000000,0b01000001,0b01111111,0b01000000,0b00000000}, // l
	{0b01111100,0b00000100,0b00011000,0b00000100,0b01111000}, // m
	{0b01111100,0b00001000,0b00000100,0b00000100,0b01111000}, // n
	{0b00111000,0b01000100,0b01000100,0b01000100,0b00111000}, // o
	{0b01111100,0b00010100,0b00010100,0b00010100,0b00001000}, // p
	{0b00001000,0b00010100,0b00010100,0b00011000,0b01111100}, // q
	{0b01111100,0b00001000,0b00000100,0b00000100,0b00001000}, // r
	{0b01001000,0b01010100,0b01010100,0b01010100,0b00100000}, // s
	{0b00000100,0b00111111,0b01000100,0b01000000,0b00100000}, // t
	{0b00111100,0b01000000,0b01000000,0b00100000,0b01111100}, // u
	{0b00011100,0b00100000,0b01000000,0b00100000,0b00011100}, // v
	{0b00111100,0b01000000,0b00110000,0b01000000,0b00111100}, // w
	{0b01000100,0b00101000,0b00010000,0b00101000,0b01000100}, // x
	{0b00001100,0b01010000,0b01010000,0b01010000,0b00111100}, // y
	{0b01000100,0b01100100,0b01010100,0b01001100,0b01000100}, // z
	{0b00000000,0b00001000,0b00110110,0b01000001,0b00000000}, // {
	{0b00000000,0b00000000,0b01111111,0b00000000,0b00000000}, // |
	{0b00000000,0b01000001,0b00110110,0b00001000,0b00000000}, // }
	{0b00000010,0b00000001,0b00000011,0b00000010,0b00000001}, // ~
};

static void display_seek_page(uint8_t page){
    *DISPLAY_C_MEM = (0xb0 | page);

    /* Start at column 0 */
    *DISPLAY_C_MEM = 0x00;
    *DISPLAY_C_MEM = 0x10;
}

void display_init(){
    /* Display off */
    *DISPLAY_C_MEM = 0xae;

    /* Segments remap */
    *DISPLAY_C_MEM = 0xa1;

    /* COM lines according to display wire up */
    *DISPLAY_C_MEM = 0xda;
    *DISPLAY_C_MEM = 0x12;

    /* COM lines scan direction 63..0 */
    *DISPLAY_C_MEM = 0xc8;

    /* Multiplex ratio 63 */
    *DISPLAY_C_MEM = 0xa8;
    *DISPLAY_C_MEM = 0x3f;

    /* Display clock divider */
    *DISPLAY_C_MEM = 0xd5;
    *DISPLAY_C_MEM = 0x80;

    /* Contrast */
    *DISPLAY_C_MEM = 0x81;
    *DISPLAY_C_MEM = 0xaa;

    /* Pre-charge period */
    *DISPLAY_C_MEM = 0xd9;
    *DISPLAY_C_MEM = 0x21;


    /* Page addressing mode */
    *DISPLAY_C_MEM = 0x20;
    *DISPLAY_C_MEM = 0x02;


    /* Voltage output */
    *DISPLAY_C_MEM = 0xdb;
    *DISPLAY_C_MEM = 0x30;

    /* External 510k current sense */
    *DISPLAY_C_MEM = 0xad;
    *DISPLAY_C_MEM = 0x00;

    /* OLED follows GDDSRAM */
    *DISPLAY_C_MEM = 0xa4;

    /* Display start line 0 */
    //*DISPLAY_C_MEM = 0x40;

    /* Normal (non-inverted) display */
    *DISPLAY_C_MEM = 0xa6;

    display_clear();

    /* Turn display on */
    *DISPLAY_C_MEM = 0xaf;
}

void display_clear(){
    for(uint8_t p = 0; p < DISPLAY_NUMBER_OF_PAGES; p++){
        display_seek_page(p);
        for(uint8_t i = 0; i < DISPLAY_NUMBER_OF_SEGMENTS; i++){
            *DISPLAY_D_MEM = 0x00;
        }
    }
}

void display_print(uint8_t line, const char * string, uint8_t arrow){
    display_seek_page(line);
    uint8_t segment_cursor = 0;

    if(arrow){
        for(uint8_t i = 0; i < DISPLAY_FONT_WIDTH; i++){
            *DISPLAY_D_MEM = pgm_read_byte(m_font['-' - 0x20] + i);
            segment_cursor++;
        }
        for(uint8_t i = 0; i < DISPLAY_FONT_WIDTH; i++){
            *DISPLAY_D_MEM = pgm_read_byte(m_font['>' - 0x20] + i);
            segment_cursor++;
        }
        for(uint8_t i = 0; i < 4; i++){
            *DISPLAY_D_MEM = 0x00;
            segment_cursor++;
        }
    }
        
    while(*string != '\0'){
        for(uint8_t i = 0; i < DISPLAY_FONT_WIDTH; i++){
            *DISPLAY_D_MEM = pgm_read_byte(m_font[*string - 0x20] + i);
            segment_cursor++;
        }

        /* Font kerning 2 points */
        *DISPLAY_D_MEM = 0x00;
        *DISPLAY_D_MEM = 0x00;
        segment_cursor += 2;

        string++;
    }

    while(segment_cursor < DISPLAY_NUMBER_OF_SEGMENTS){
        *DISPLAY_D_MEM = 0x00;
        segment_cursor++;
    }
}

void display_print_heading(const char * string){
    display_seek_page(0);
    uint8_t segment_cursor = 0;

    for(uint8_t i = 0; i < 4; i++){
        *DISPLAY_D_MEM = 0x00;
    }
    *DISPLAY_D_MEM = 0x1c;
    *DISPLAY_D_MEM = 0x3e;
    *DISPLAY_D_MEM = 0x3e;
    *DISPLAY_D_MEM = 0x1c;
    *DISPLAY_D_MEM = 0x00;
    for(uint8_t i = 0; i < 6; i++){
        *DISPLAY_D_MEM = 0x08;
    }
    *DISPLAY_D_MEM = 0x00;
    *DISPLAY_D_MEM = 0x00;

    segment_cursor = 17;

    while(*string != '\0'){
        for(uint8_t i = 0; i < DISPLAY_FONT_WIDTH; i++){
            *DISPLAY_D_MEM = pgm_read_byte(m_font[*string - 0x20] + i);
            segment_cursor++;
        }

        /* Font kerning 2 points */
        *DISPLAY_D_MEM = 0x00;
        *DISPLAY_D_MEM = 0x00;
        segment_cursor += 2;

        string++;
    }

    while(segment_cursor < DISPLAY_NUMBER_OF_SEGMENTS - 19){
        *DISPLAY_D_MEM = 0x00;
    }

    for(uint8_t i = 0; i < 6; i++){
        *DISPLAY_D_MEM = 0x08;
    }
    *DISPLAY_D_MEM = 0x1c;
    *DISPLAY_D_MEM = 0x3e;
    *DISPLAY_D_MEM = 0x3e;
    *DISPLAY_D_MEM = 0x1c;
    *DISPLAY_D_MEM = 0x00;
    for(uint8_t i = 0; i < 4; i++){
        *DISPLAY_D_MEM = 0x00;
    }
}

void display_print_number(uint8_t line, uint16_t number){
    display_seek_page(line);

    for(uint8_t i = 0; i < 50; i++){
        *DISPLAY_D_MEM = 0x00;
    }

    uint8_t str_buffer[8] = {0};

    uint16_t burn_down = number;
    uint8_t digits = 0;

    while(burn_down){
        digits++;
        str_buffer[8 - digits] = (burn_down % 10) + '0';
        burn_down /= 10;
    }

    for(uint8_t d = 0; d < digits - 1; d++){
        *DISPLAY_D_MEM = 0x00;
        *DISPLAY_D_MEM = 0x00;
        *DISPLAY_D_MEM = 0x00;
        *DISPLAY_D_MEM = 0x00;
        *DISPLAY_D_MEM = 0x00;
    }

    while(digits){
        uint8_t letter = str_buffer[8 - digits];

        for(uint8_t i = 0; i < DISPLAY_FONT_WIDTH; i++){
            *DISPLAY_D_MEM = pgm_read_byte(m_font[letter - 0x20] + i);
        }

        digits--;
    }
}