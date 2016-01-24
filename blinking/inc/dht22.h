#ifndef DHT22_H_
#define DHT22_H_

#define dht22_pin	12
#define dht22_port	6
#define dht22_gpioPin	8
#define dht22_gpioPort	2
typedef int8_t byte;

//**************************************************************************
//* Prototypes
//**************************************************************************

void dht22_delay_ms (unsigned char time);
void dht22_delay_us (unsigned char time);
void dht22_pin_init (void);
byte leer_datos_dht(void);
void leer_dht22_pin (float dhthum, float dhttemp);
float leerTemperatura(void);
float leerHumedad(void);

#endif /* DHT22_H_ */