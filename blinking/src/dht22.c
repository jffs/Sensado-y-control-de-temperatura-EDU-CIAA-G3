#include "chip.h"
#include "dht22.h"


byte dht22_pin_dat[5];

byte leer_datos_dht();
float dhthum, dhttemp;

void dht22_delay_ms (unsigned char time)
{
	volatile unsigned long i;
	volatile unsigned long delay;

	delay = 20400 * time;

	for( i=delay; i>0; i-- );
}

void dht22_delay_us (unsigned char time)
{
	volatile unsigned long i;
	volatile unsigned long delay;

	delay = 204 * time;

	for( i=delay; i>0; i-- );
}

void dht22_pin_init (void){
   Chip_SCU_PinMux(dht22_port,dht22_pin,SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,FUNC0);
   Chip_GPIO_SetDir(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin),!1);//dth22 como input
   //dht_io=0;
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,0);//dth22 -> 0
   dht22_delay_ms(1);
   //output_high(dht22_pin);
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,1);//dth22 -> 1
}

byte leer_datos_dht(){
   byte i = 0;
   byte result=0;
   for (i=0; i< 8; i++) {
      //We enter this during the first start bit (low for 50uS) of the byte
      //Next: wait until pin goes high
      while(Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin)==0);
      dht22_delay_us(30);
      if (Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin)==1){//Was: if(PINC * _BV(dht_PIN))
         result |=(1<<(7-i));
      }
      while (Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin)==1);
      //Was: while((PINC * _BV(dht_PIN)));
   }//end of "for.."
   
   return result;
}

void leer_dht22_pin (float dhthum, float dhttemp){
   //byte GlobalErr=0;
   byte dht22_pin_in, i, dht22_pin_checksum;
  
   
   //dht_io=0; // configurar el pin como salida
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,0);//dth22 -> 0
   //output_high(dht22_pin);
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,1);//dth22 -> 1
   dht22_delay_us(20);
   //output_low(dht22_pin);
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,0);//dth22 -> 0
   dht22_delay_ms(18);
   
   //output_high(dht22_pin);
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,1);//dth22 -> 1
   dht22_delay_us(22);
   //dht_io=1;// configurar el pin como entrada
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,1);//dth22 -> 0
   dht22_delay_us(5);
   dht22_pin_in=Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin);
   
   if(dht22_pin_in) {
      //GlobalErr=1;
      printf("\r\ndht condicion 1 de inicio no encontrada");
      return;
   }
   
   dht22_delay_us(80);
   dht22_pin_in=Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin);
   
   if(!dht22_pin_in){
      //GlobalErr=2;
      printf("\r\ndht condicion 2 de inicio no encontrada");
      return;
   }
   
   dht22_delay_us(80);
   
   for (i=0; i<5; i++){
      dht22_pin_dat[ i ] = leer_datos_dht(); // capturando datos
   }
   
   //dht_io=0;
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,0);//dth22 -> 0
   dht22_delay_us(10);
   //output_high(dht22_pin);
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin,1);//dth22 -> 1
   
   dht22_pin_checksum= dht22_pin_dat[0]+dht22_pin_dat[1]+dht22_pin_dat[2]+dht22_pin_dat[3];
   
   if(dht22_pin_dat[4]!=dht22_pin_checksum){
      //GlobalErr=3;
      printf("\r\nDHT checksum error");
   }
   
   dhthum  = (float)dht22_pin_dat[0] * 256 + (float)dht22_pin_dat[1];
   dhthum  /= 10;
   dhttemp = (float)(dht22_pin_dat[2] & 0x7F)* 256 + (float)dht22_pin_dat[3];
   dhttemp /= 10.0;
   if ((dht22_pin_dat[2] & 0x80) != 0)  dhttemp *= -1;
}

float leerTemperatura() {
	
	leer_dht22_pin(dhthum,dhttemp);
	return dhttemp;
	
}

float leerHumedad() {
	
	leer_dht22_pin(dhthum,dhttemp);
	return dhthum;
	
}