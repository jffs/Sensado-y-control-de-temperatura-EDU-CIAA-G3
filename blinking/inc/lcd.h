#ifndef LCD_H_
#define LCD_H_

// The following defines set the default pins for the LCD display
#ifndef LCD_ENABLE			 // if lcd_enable is not defined

#define LCD_ENABLE      9	 // LCD enable pin

#define LCD_RS		    8 // LCD r/s pin

#define LCD_D4          4     // LCD data D1 pin


#define LCD_D5          5   // LCD data D2 pin


#define LCD_D6          6    // LCD data D3 pin


#define LCD_D7          10    // LCD data D4 pin

#define LCD_PORT		4 

#define LCD_EN_gpioPin	13

#define LCD_RS_gpioPin	12

#define LCD_D4_gpioPin	4

#define LCD_D5_gpioPin	5

#define LCD_D6_gpioPin	6

#define LCD_D7_gpioPin	14

#endif

#define LCD_SEC_LINE    0x40    // Address of the second line of the LCD

// LCD configuration constants
#define CURSOR_ON       2
#define CURSOR_OFF      0
#define CURSOR_BLINK    1
#define CURSOR_NOBLINK  0
#define DISPLAY_ON      4
#define DISPLAY_OFF     0
#define DISPLAY_8X5     0
#define DISPLAY_10X5    4
#define _2_LINES        8
#define _1_LINE         0

//**************************************************************************
//* Prototypes
//**************************************************************************
void LCD_delay_ms (unsigned char time);
void LCD_send_nibble(char data);
void LCD_send_byte(char address, char data);
void LCD_init(char mode1, char mode2);
void LCD_pos_xy(char x, char y);
void LCD_write_char(char c);
void LCD_write_string (char *c);
void LCD_display_on(void);
void LCD_display_off(void);
void LCD_cursor_on(void);
void LCD_cursor_off(void);
void LCD_cursor_blink_on(void);
void LCD_cursor_blink_off(void);


#endif /* LCD_H_ */
