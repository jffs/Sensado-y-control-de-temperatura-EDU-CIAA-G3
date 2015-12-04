
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
char temperatura[10],humedad[10];
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
 
 
	ciaaPOSIX_printf("--Prueba dht22--");
	
	leer_datos_dht(dhthum, dhttemp);
	sprintf(temperatura,"Temp: ",dhttemp);
	sprintf(humedad,"Hum: ",dhthum);
	
	LCD_write_string(temperatura);
	ciaaPOSIX_printf(temperatura);

	LCD_write_string(humedad);
	ciaaPOSIX_printf(humedad);
	
	/* terminate task */
   TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

