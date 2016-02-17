
/** @file lcd.c
 *  @brief lcd library
 *
 */
/*==================[inclusions]=============================================*/
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

/** @brief Función que configura e inicializa los puertos de la EDU-CIAA como salida
 */
void lcd_InitIO(void){
	     /** Configure the pins function*/
		 Chip_SCU_PinMux(LCD_PORT,LCD_D4,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC0);
		 Chip_SCU_PinMux(LCD_PORT,LCD_D5,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC0);
		 Chip_SCU_PinMux(LCD_PORT,LCD_D6,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC0);
		 Chip_SCU_PinMux(LCD_PORT,LCD_D7,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC4);
		 Chip_SCU_PinMux(LCD_PORT,LCD_RS,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC4);
		 Chip_SCU_PinMux(LCD_PORT,LCD_ENABLE,SCU_MODE_INACT|SCU_MODE_ZIF_DIS,FUNC4);
		 /** Configure the pins(LCD4,LCD5,LCD6,LCD7,LCD_RS,LCD_EN) as outputs*/
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,2,(1<<4),!0);
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,2,(1<<5),!0);
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,2,(1<<6),!0);
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,5,(1<<14),!0);
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,5,(1<<12),!0);
		 Chip_GPIO_SetDir(LPC_GPIO_PORT,5,(1<<13),!0);
}

/** @brief Función que realiza un retardo en ms
 * @param time Cantidad de tiempo a retardar
 */
void LCD_delay_ms (unsigned char time)
{
	volatile unsigned long i;
	volatile unsigned long delay;

	delay = 20400 * time;

	for( i=delay; i>0; i-- );
}

/** @brief Función que envía datos al display en grupos de a 4-bits
 * @param data Cantidad de tiempo a retardar
 */
void LCD_send_nibble(char data)
{
	union ubyte my_union;
	my_union._byte = data;
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D4_gpioPin,my_union.bit.b0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D5_gpioPin,my_union.bit.b1);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D6_gpioPin,my_union.bit.b2);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_D7_gpioPin,my_union.bit.b3);

	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_EN_gpioPin,1);
	for (data=20; data; data--);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_EN_gpioPin,0);
}

/** @brief Función que envía datos al display en grupos de a 8-bits
 * @param data Cantidad de tiempo a retardar
 */
void LCD_send_byte(char address, char data)
{
  unsigned int temp;
	if(address==0)
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_RS_gpioPin,0);
	else
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_RS_gpioPin,1);
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_EN_gpioPin,0);
		/** send the higher nibble*/
		LCD_send_nibble(data >> 4);
		/** send the lower nibble*/
		LCD_send_nibble(data & 0x0f);
		for (temp=1000; temp; temp--);
}

/** @brief Función que incializa la comunicación entre el LCD y el MCU
 * En primer lugar se envía 0 a los puertos LCD4, LCD5, LCD6, LCD7, LCD_RS, LCD_ENABLE
 * Se realiza un retardo de 15ms.
 * Luego se envían tres 0x03 conecutivos y un 0x02 para finalizar el establecimiento de la comunicación.
 */
void LCD_init(char mode1, char mode2)
{
	lcd_InitIO();
	char aux;
	/** Se envia un cero a los siguietes pines del lcd: LCD4,LCD5,LCD6,LCD7,LCD_RS,LCD_EN*/
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D4_gpioPin,0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D5_gpioPin,0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,2,LCD_D6_gpioPin,0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_D7_gpioPin,0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_RS_gpioPin,0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,5,LCD_EN_gpioPin,0);

	LCD_delay_ms(15);

	for(aux=0;aux<3;++aux)
	{
	  LCD_send_nibble(3);
	  LCD_delay_ms(5);
	}
	LCD_send_nibble(2);
	/** Se configura el modo en el que se enviaran los datos al LCD en nuestro caso de a 4-bits*/
	LCD_send_byte(0,0x20 | mode1);
	LCD_send_byte(0,0x08 | mode2);
	lcd_mode = 0x08 | mode2;

	/** Se configura el mode de entrada I/D=1 S=0*/
	LCD_send_byte(0,1);
	LCD_delay_ms(5);
	LCD_send_byte(0,6);
}

/** @brief Función que posiciona el cursor en el display a través de las coordenadas x e y
 	* @param x Coordenada en x
 	* @param y Coordenada en y
 */
void LCD_pos_xy(char x, char y)
{
  char address;
  if (y) address = LCD_SEC_LINE; else address = 0;
  address += x;
  LCD_send_byte(0,0x80|address);
}

/** @brief Función que envia un caracter para ser mostrado en el display
 	* @param c Caracter que se envía al display
 */
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

/** @brief Función que envia un cadena de caracteres para ser mostrado en el display
 	* @param c Cadena de caracteres que se envía al display
 */
void LCD_write_string (char *c)
{
	while (*c)
	{
	  LCD_write_char(*c);
	  c++;
	}
}

void LCD_display_on(void)
{
	lcd_mode |= 4;
	LCD_send_byte (0,lcd_mode);
}

void LCD_display_off(void)
{
	lcd_mode &= 0b11111011;
	LCD_send_byte (0,lcd_mode);
}

void LCD_cursor_on(void)
{
  lcd_mode |= 2;
	LCD_send_byte (0,lcd_mode);
}

void LCD_cursor_off(void)
{
	lcd_mode &= 0b11111101;
	LCD_send_byte (0,lcd_mode);
}

void LCD_cursor_blink_on(void)
{
	lcd_mode |= 1;
	LCD_send_byte (0,lcd_mode);
}


void LCD_cursor_blink_off(void)
{
	lcd_mode &= 0b11111110;
	LCD_send_byte (0,lcd_mode);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
