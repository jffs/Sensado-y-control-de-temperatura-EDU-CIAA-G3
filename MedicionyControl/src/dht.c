#include "chip.h"
#include "dht.h"
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */


byte dht22_pin_dat[5];

float dhthum, dhttemp;

/** @brief Funci�n que realiza un retardo en ms
 * @param time Cantidad de tiempo a retardar
 */
void dht22_delay_ms (unsigned char time)
{
	volatile unsigned long i;
	volatile unsigned long delay;

	delay = 20400 * time;

	for( i=delay; i>0; i-- );
}
/** @brief Funci�n que realiza un retardo en microsegundos
 * @param time CAntidad de tiempo para retardar
 */
void dht22_delay_us (unsigned char time)
{
	volatile unsigned long i;
	volatile unsigned long delay;

	delay = 22 * time;

	for( i=delay; i>0; i-- );
}
/** @brief Funci�n que lee 1 byte de datos del dht
 *El sensor devuelve 1s y 0s con el siguiente formato:
 *1 -> 50 uS en bajo y 70 uS en alto
 *0 -> 50 uS en bajo y 30 uS en alto
 @return un byte de datos de los 5 que devuelve el sensor
 */
byte leer_datos_dht(){
   int i = 0;
   byte result=0;
   int res[8]={0,0,0,0,0,0,0,0,0};
   for (i=0; i< 8; i++) {
      //We enter this during the first start bit (low for 50uS) of the byte

      while(!Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin));
      dht22_delay_us(30);
      if (Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin)){//Was: if(PINC * _BV(dht_PIN))
         result |=(1<<(7-i));
      }
      while (Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin));

   }

   return result;
}
/** @brief funcion que lee los datos del sensor DHT22
 * primero realiza la inicializaci�n con los tiempos especificados
 *
 *luego lee 40 bits de datos que envia el sensor
 *
 *@param dhthum parametro pasado como referencia donde se modificara el valor de la humedad
 *@param dhttemp parametro pasado como referencia donde se modificara el valor de la temperatura
 */
void leer_dht22(float *dhthum,float *dhttemp){
	    int dht_pin_in;
	    float dhth,dhtt;
	    int i;
	    byte dht22_dat[5],dht22_pin_checksum;
	    for(i=0;i<5;i++){
	    dht22_dat[i]=0;
	    }
	    //START SIGNAL
	    Chip_SCU_PinMux(dht22_port,dht22_pin,SCU_MODE_PULLUP | SCU_MODE_ZIF_DIS,FUNC0); //dth22 como output
		Chip_GPIO_SetDir(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin),!0);
		dht22_delay_us(20);

	    Chip_GPIO_ClearValue(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin));

		dht22_delay_ms(5);

	    Chip_GPIO_SetValue(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin));

	    dht22_delay_us(30);
		//WAIT RESPONSE
	    Chip_SCU_PinMux(dht22_port,dht22_pin,SCU_MODE_PULLUP  | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,FUNC0);
	    Chip_GPIO_SetDir(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin),!1);//dth22 como input
	    dht_pin_in=Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin);
	    if(dht_pin_in){
	    	ciaaPOSIX_printf("Error de inicializacion nro 1");
	    }
	    dht22_delay_us(80);
	    dht_pin_in=Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin);
	    if(!dht_pin_in){
	    	 ciaaPOSIX_printf("Error de inicializacion nro 1");
	    }
	    dht22_delay_us(70);
	    for(i=0;i<5;i++){
	    	dht22_dat[i]=leer_datos_dht();

	    }
	    dht22_pin_checksum= dht22_pin_dat[0]+dht22_pin_dat[1]+dht22_pin_dat[2]+dht22_pin_dat[3];

	    if(dht22_pin_dat[4]!=dht22_pin_checksum){
	          //GlobalErr=3;
	         ciaaPOSIX_printf("\r\nDHT checksum error");
	    }

	    dhth  = (dht22_dat[0]<<8) | dht22_dat[1];
	    dhth  /= 10;
	    dhtt  = (dht22_dat[2]<<8) | dht22_dat[3];
	    dhtt /= 10;
	    *dhttemp = dhtt;
	    *dhthum = dhth;
	    //if ((dht22_pin_dat[2] & 0x80) != 0)  *dhttemp *= -1;
}
