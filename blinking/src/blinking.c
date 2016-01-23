
/*==================[inclusions]=============================================*/
#include "chip.h"
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "blinking.h"         /* <= own header */
#include "lcd.h"  
#include "dht22.h"



/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
float dhthum, dhttemp;
char menu_linea1[16],menu_linea2[16];
char numeros[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(AppMode1);

	
   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

void pantallaPpal_a_string(void) {
	menu_linea1[0] = 'T';
	menu_linea1[1] = 'e';
	menu_linea1[2] = 'm';
	menu_linea1[3] = 'p';
	menu_linea1[4] = ':';
	menu_linea1[5] = ' ';
	menu_linea1[6] = numeros[((int)(dhttemp / 10))];
	menu_linea1[7] = numeros[((int)dhttemp % 10)];
	menu_linea1[8] = '.';
	menu_linea1[9] = numeros[((int)(dhttemp * 10) % 10)];
	menu_linea1[10] = ' ';
	menu_linea1[11] = '°';
	menu_linea1[12] = 'C';
	menu_linea1[13] = ' ';
	menu_linea1[14] = ' ';
	menu_linea1[15] = ' ';
	
	menu_linea2[0] = 'H';
	menu_linea2[1] = 'u';
	menu_linea2[2] = 'm';
	menu_linea2[3] = ':';
	menu_linea2[4] = ' ';
	menu_linea2[5] = numeros[((int)(dhthum / 10))];
	menu_linea2[6] = numeros[((int)dhthum % 10)];
	menu_linea2[7] = '.';
	menu_linea2[8] = numeros[((int)(dhthum * 10) % 10)];
	menu_linea2[9] = ' ';
	menu_linea2[10] = '%';
	menu_linea2[11] = ' ';
	menu_linea2[12] = ' ';
	menu_linea2[13] = ' ';
	menu_linea2[14] = ' ';
	menu_linea2[15] = ' ';
}	

void menu_1_a_string(void) {
	menu_linea1[0] = ' ';
	menu_linea1[1] = ' ';
	menu_linea1[2] = ' ';
	menu_linea1[3] = ' ';
	menu_linea1[4] = ' ';
	menu_linea1[5] = ' ';
	menu_linea1[6] = 'M';
	menu_linea1[7] = 'e';
	menu_linea1[8] = 'n';
	menu_linea1[9] = 'u';
	menu_linea1[10] = ' ';
	menu_linea1[11] = ' ';
	menu_linea1[12] = ' ';
	menu_linea1[13] = ' ';
	menu_linea1[14] = ' ';
	menu_linea1[15] = ' ';
	
	menu_linea2[0] = '1';
	menu_linea2[1] = ' ';
	menu_linea2[2] = 'S';
	menu_linea2[3] = 'e';
	menu_linea2[4] = 't';
	menu_linea2[5] = ' ';
	menu_linea2[6] = ' ';
	menu_linea2[7] = 'T';
	menu_linea2[8] = 'e';
	menu_linea2[9] = 'm';
	menu_linea2[10] = 'p';
	menu_linea2[11] = ' ';
	menu_linea2[12] = ' ';
	menu_linea2[13] = 'M';
	menu_linea2[14] = 'a';
	menu_linea2[15] = 'x';
}

void menu_2_a_string(void) {
	menu_linea1[0] = ' ';
	menu_linea1[1] = ' ';
	menu_linea1[2] = ' ';
	menu_linea1[3] = ' ';
	menu_linea1[4] = ' ';
	menu_linea1[5] = ' ';
	menu_linea1[6] = 'M';
	menu_linea1[7] = 'e';
	menu_linea1[8] = 'n';
	menu_linea1[9] = 'u';
	menu_linea1[10] = ' ';
	menu_linea1[11] = ' ';
	menu_linea1[12] = ' ';
	menu_linea1[13] = ' ';
	menu_linea1[14] = ' ';
	menu_linea1[15] = ' ';
	
	menu_linea2[0] = '2';
	menu_linea2[1] = ' ';
	menu_linea2[2] = 'S';
	menu_linea2[3] = 'e';
	menu_linea2[4] = 't';
	menu_linea2[5] = ' ';
	menu_linea2[6] = ' ';
	menu_linea2[7] = 'T';
	menu_linea2[8] = 'e';
	menu_linea2[9] = 'm';
	menu_linea2[10] = 'p';
	menu_linea2[11] = ' ';
	menu_linea2[12] = ' ';
	menu_linea2[13] = 'M';
	menu_linea2[14] = 'i';
	menu_linea2[15] = 'n';
}


void set_temp_a_string(float temp) {
	menu_linea2[0] = ' ';
	menu_linea2[1] = ' ';
	menu_linea2[2] = ' ';
	menu_linea2[3] = ' ';
	menu_linea2[4] = numeros[((int)(temp / 10))];
	menu_linea2[5] = numeros[((int)temp % 10)];
	menu_linea2[6] = '.';
	menu_linea2[7] = numeros[((int)(temp * 10) % 10)];
	menu_linea2[8] = ' ';
	menu_linea2[9] = '°';
	menu_linea2[10] = 'C';
	menu_linea2[11] = ' ';
	menu_linea2[12] = ' ';
	menu_linea2[13] = ' ';
	menu_linea2[14] = ' ';
	menu_linea2[15] = ' ';
}

void set_temp_max_a_string(float temp) {
	menu_linea1[0] = ' ';
	menu_linea1[1] = 'S';
	menu_linea1[2] = 'e';
	menu_linea1[3] = 't';
	menu_linea1[4] = ' ';
	menu_linea1[5] = ' ';
	menu_linea1[6] = 'T';
	menu_linea1[7] = 'e';
	menu_linea1[8] = 'm';
	menu_linea1[9] = 'p';
	menu_linea1[10] = ' ';
	menu_linea1[11] = ' ';
	menu_linea1[12] = 'M';
	menu_linea1[13] = 'a';
	menu_linea1[14] = 'x';
	menu_linea1[15] = ' ';
	
	set_temp_a_string(temp);
}

void set_temp_min_a_string(float temp) {
	menu_linea1[0] = ' ';
	menu_linea1[1] = 'S';
	menu_linea1[2] = 'e';
	menu_linea1[3] = 't';
	menu_linea1[4] = ' ';
	menu_linea1[5] = ' ';
	menu_linea1[6] = 'T';
	menu_linea1[7] = 'e';
	menu_linea1[8] = 'm';
	menu_linea1[9] = 'p';
	menu_linea1[10] = ' ';
	menu_linea1[11] = ' ';
	menu_linea1[12] = 'M';
	menu_linea1[13] = 'i';
	menu_linea1[14] = 'n';
	menu_linea1[15] = ' ';

	set_temp_a_string(temp);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */ 
	ciaak_start();
	 /* Initializes GPIO */
   Chip_GPIO_Init(LPC_GPIO_PORT);
	//SystemCoreClockUpdate();
   /* print message (only on x86) */
	ciaaPOSIX_printf("Init Task...\n");

	LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
	dht22_pin_init();
   /* activate periodic task:
    *  - for the first time after 350 ticks (350 ms)
    *  - and then every 250 ticks (250 ms)
    */
   SetRelAlarm(ActivatePeriodicTask, 350, 250);

   /* terminate task */
   TerminateTask();
}

/** \brief Periodic Task
 *
 * This task is started automatically every time that the alarm
 * ActivatePeriodicTask expires.
 *
 */
TASK(PeriodicTask)
{
  // uint8_t outputs;

 
	ciaaPOSIX_printf("--Prueba dht22--");
	
	dhthum = leerTemperatura();
	dhttemp = leerHumedad(); 
	pantallaPpal_a_string();
	
	LCD_write_char('\f');
	LCD_write_string(&menu_linea1);
	LCD_write_char('\r');
	LCD_write_string(&menu_linea2);
	
	ciaaPOSIX_printf(menu_linea1);
	ciaaPOSIX_printf(menu_linea2);
	

   /* terminate task */
   TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

