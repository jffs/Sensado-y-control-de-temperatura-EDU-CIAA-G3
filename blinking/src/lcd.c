/**************************************************************************
//* Basic Character LCD functions
//* By Fábio Pereira
//* 01/15/08
//**************************************************************************
 * 
 */
#include "chip.h"
#include "lcd.h"

union ubyte
{
	char _byte;
	struct
	{
		unsigned char b0 : 1;
		unsigned char b1 : 1;
		unsigned char b2 : 1;
		unsigned char b3 : 1;
		unsigned char b4 : 1;
		unsigned char b5 : 1;
		unsigned char b6 : 1;
		unsigned char b7 : 1;
	} bit;
};

// Display configuration global variable
static char lcd_mode;	

//**************************************************************************
//* A simple delay function (used by LCD functions)
//**************************************************************************
//* Calling arguments
//* unsigned char time: aproximate delay time in microseconds
//**************************************************************************
void lcd_InitIO(void){
	// Configure the pins function
		 Chip_SCU_PinMux(LCD_PORT,LCD_D4,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC0);//SCU_MODE_INACT=Desactivo pull-down y pull-up
		 Chip_SCU_PinMux(LCD_PORT,LCD_D5,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC0);//SCU_MODE_ZIF_DIS=Disable input glitch filter
		 Chip_SCU_PinMux(LCD_PORT,LCD_D6,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC0);
		 Chip_SCU_PinMux(LCD_PORT,LCD_D7,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC4);
		 Chip_SCU_PinMux(LCD_PORT,LCD_RS,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC4);
		 Chip_SCU_PinMux(LCD_PORT,LCD_ENABLE,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC4);
		 // Configure the pins as outputs
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,2,(1<<4),!0);//LCD4 como output
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,2,(1<<5),!0);//LCD5 como output
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,2,(1<<6),!0);//LCD6 como output
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,5,(1<<14),!0);//LCD7 como output
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,5,(1<<12),!0);//LCD_RS como output
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,5,(1<<13),!0);//LCD_ENABLE como output
}

void LCD_delay_ms (unsigned char time)
{
	volatile unsigned long i;
	volatile unsigned long delay;

	delay = 20400 * time;

	for( i=delay; i>0; i-- );
}

//**************************************************************************
//* Send a nibble to the LCD
//**************************************************************************
//* Calling arguments
//* char data : data to be sent to the display
//**************************************************************************
void LCD_send_nibble(char data)
{
	union ubyte my_union;
	my_union._byte = data;

	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D4_gpioPin,my_union.bit.b0);//LCD4 -> b0
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D5_gpioPin,my_union.bit.b1);//LCD5 -> b1
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D6_gpioPin,my_union.bit.b2);//LCD6 -> b2
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_D7_gpioPin,my_union.bit.b3);//LCD7 -> b3
	// pulse the LCD enable line
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_EN_gpioPin,1);//LCD_ENABLE -> 1
	for (data=20; data; data--);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_EN_gpioPin,0);//LCD_ENABLE -> 0
}

//**************************************************************************
//* Write a byte into the LCD
//**************************************************************************
//* Calling arguments:
//* char address : 0 for instructions, 1 for data
//* char data : command or data to be written
//**************************************************************************
void LCD_send_byte(char address, char data)
{
  unsigned int temp;
	if(address==0)
		// config the R/S line
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_RS_gpioPin,0);//LCD_RS -> 0, config the R/S line
	else
	
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_RS_gpioPin,1);//LCD_RS -> 1, config the R/S line
		// set LCD enable line to 0
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_EN_gpioPin,0);//LCD_ENABLE -> 0
		LCD_send_nibble(data >> 4);     // send the higher nibble
		LCD_send_nibble(data & 0x0f);   // send the lower nibble
		for (temp=1000; temp; temp--);
}

//**************************************************************************
//* LCD initialization
//**************************************************************************
//* Calling arguments:
//* char mode1 : display mode (number of lines and character size)
//* char mode2 : display mode (cursor and display state)
//**************************************************************************
void LCD_init(char mode1, char mode2)
{	
	lcd_InitIO();
	char aux;
	// Set the LCD data pins to zero
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D4_gpioPin,0);//LCD4 -> 0
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D5_gpioPin,0);//LCD5 -> 0
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D6_gpioPin,0);//LCD6 -> 0
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_D7_gpioPin,0);//LCD7 -> 0
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_RS_gpioPin,0);//LCD_RS -> 0, config the R/S line
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_EN_gpioPin,0);//LCD_ENABLE -> 0

	LCD_delay_ms(15);
	// LCD 4-bit mode initialization sequence
	// send three times 0x03 and then 0x02 to finish configuring the LCD
	for(aux=0;aux<3;++aux)
	{		
	  LCD_send_nibble(3);
	  LCD_delay_ms(5);
	}
	LCD_send_nibble(2);
	// Now send the LCD configuration data
	LCD_send_byte(0,0x20 | mode1);
	LCD_send_byte(0,0x08 | mode2);
	lcd_mode = 0x08 | mode2;
	
	LCD_send_byte(0,1);
	LCD_delay_ms(5);
	LCD_send_byte(0,6); //entry mode set I/D=1 S=0
}

//**************************************************************************
//* LCD cursor position set
//**************************************************************************
//* Calling arguments:
//* char x : column (starting by 0)
//* char y : line (0 or 1)
//**************************************************************************
void LCD_pos_xy(char x, char y)
{
  char address;
  if (y) address = LCD_SEC_LINE; else address = 0;
  address += x;
  LCD_send_byte(0,0x80|address);
}

//**************************************************************************
//* Write a character on the display
//**************************************************************************
//* Calling arguments:
//* char c : character to be written
//**************************************************************************
//* Notes :
//* \f clear the display
//* \n and \r return the cursor to line 1 column 0
//**************************************************************************
void LCD_write_char(char c)
{
  switch (c)
	{
	  case '\f' :	
	    LCD_send_byte(0,1);
	    LCD_delay_ms(5);
	    break;
	  case '\n' :
	  case '\r' :	
	    LCD_pos_xy(0,1);
	    break;
	  default:
	    LCD_send_byte(1,c);
   }
}

//**************************************************************************
//* Write a string on the display
//**************************************************************************
//* Calling arguments:
//* char *c : pointer to the string
//**************************************************************************
void LCD_write_string (char *c)
{
	while (*c)
	{
	  LCD_write_char(*c);
	  c++;
	}
}

//**************************************************************************
//* Turn the display on
//**************************************************************************
void LCD_display_on(void)
{
	lcd_mode |= 4;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the display off
//**************************************************************************
void LCD_display_off(void)
{
	lcd_mode &= 0b11111011;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the cursor on
//**************************************************************************
void LCD_cursor_on(void)
{
  lcd_mode |= 2;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the cursor off
//**************************************************************************
void LCD_cursor_off(void)
{
	lcd_mode &= 0b11111101;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn on the cursor blink function
//**************************************************************************
void LCD_cursor_blink_on(void)
{
	lcd_mode |= 1;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn off the cursor blink function
//**************************************************************************
void LCD_cursor_blink_off(void)
{
	lcd_mode &= 0b11111110;
	LCD_send_byte (0,lcd_mode);
}
