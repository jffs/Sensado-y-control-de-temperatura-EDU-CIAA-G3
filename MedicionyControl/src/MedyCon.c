/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Blinking_echo example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Blinking Blinking_echo example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * PR           Pablo Ridolfi
 * JuCe         Juan Cecconi
 * GMuro        Gustavo Muro
 * ErPe         Eric Pernia
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150603 v0.0.3   ErPe change uint8 type by uint8_t
 *                        in line 172
 * 20141019 v0.0.2   JuCe add printf in each task,
 *                        remove trailing spaces
 * 20140731 v0.0.1   PR   first functional version
 */

/*==================[inclusions]=============================================*/
#include "chip.h"
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "blinking.h"         /* <= own header */
#include "lcd.h"
#include "dht.h"
#include <stdio.h>
#include <stdlib.h>


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
float dhthum=0, dhttemp=0,temp_min=0.0,temp_max=40.0;
char menu_linea1[16],menu_linea2[16];
char numeros[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
//char estados[4] = {'0', '1', '2', '3', '4'};
enum estados { principal, menu1, menu2,menu3,menu4, set_temp_max, set_temp_min, set_temp_max_sum,set_temp_max_res, set_temp_min_sum, set_temp_min_res,mute_on,mute_off,cooler_on,cooler_off} estado;
char error[5]= {'e','r','r','o','r'};
static int32_t fd_in;
boolean mute=0,coolerON=0;

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
	menu_linea1[6] = numeros[(int)(dhttemp / 10)];
	menu_linea1[7] = numeros[((int)dhttemp % 10)];
	menu_linea1[8] = '.';
	menu_linea1[9] = numeros[(int)(dhttemp * 10) % 10];
	menu_linea1[10] = ' ';
	menu_linea1[11] = '*';
	menu_linea1[12] = 'C';
	menu_linea1[13] = ' ';
	menu_linea1[14] = ' ';
	if(mute)
	  menu_linea1[15] = 'S';
	else
	  menu_linea1[15] = ' ';
	
	menu_linea2[0] = 'H';
	menu_linea2[1] = 'u';
	menu_linea2[2] = 'm';
	menu_linea2[3] = ':';
	menu_linea2[4] = ' ';
	menu_linea2[5] = numeros[(int)(dhthum / 10)];
	menu_linea2[6] = numeros[((int)dhthum % 10)];
	menu_linea2[7] = '.';
	menu_linea2[8] = numeros[(int)(dhthum * 10) % 10];
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

	//menu_linea1="      Menu      ";
    //menu_linea2="1  Set  Temp  Max";
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
void menu_3_a_string(void) {
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

	menu_linea2[0] = '3';
	menu_linea2[1] = ' ';
	menu_linea2[2] = 'M';
	menu_linea2[3] = 'o';
	menu_linea2[4] = 'd';
	menu_linea2[5] = 'o';
	menu_linea2[6] = ' ';
	menu_linea2[7] = 'S';
	menu_linea2[8] = 'i';
	menu_linea2[9] = 'l';
	menu_linea2[10] = 'e';
	menu_linea2[11] = 'n';
	menu_linea2[12] = 'c';
	menu_linea2[13] = 'i';
	menu_linea2[14] = 'o';
	menu_linea2[15] = ' ';

}
void menu_4_a_string(void) {
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
	if(!coolerON){
		menu_linea2[0] = '4';
		menu_linea2[1] = ' ';
		menu_linea2[2] = 'A';
		menu_linea2[3] = 'c';
		menu_linea2[4] = 't';
		menu_linea2[5] = 'i';
		menu_linea2[6] = 'v';
		menu_linea2[7] = 'a';
		menu_linea2[8] = 'r';
		menu_linea2[9] = ' ';
		menu_linea2[10] = 'C';
		menu_linea2[11] = 'o';
		menu_linea2[12] = 'o';
		menu_linea2[13] = 'l';
		menu_linea2[14] = 'e';
		menu_linea2[15] = 'r';
		}
	else {
		menu_linea2[0] = '4';
		menu_linea2[1] = ' ';
		menu_linea2[2] = 'D';
		menu_linea2[3] = 'e';
		menu_linea2[4] = 's';
		menu_linea2[5] = 'a';
		menu_linea2[6] = 'c';
		menu_linea2[7] = 't';
		menu_linea2[8] = ' ';
		menu_linea2[9] = ' ';
		menu_linea2[10] = 'C';
		menu_linea2[11] = 'o';
		menu_linea2[12] = 'o';
		menu_linea2[13] = 'l';
		menu_linea2[14] = 'e';
		menu_linea2[15] = 'r';
	}
}

void set_temp_a_string(float temp) {
	menu_linea2[0] = ' ';
	menu_linea2[1] = ' ';
	menu_linea2[2] = ' ';
	menu_linea2[3] = ' ';
	menu_linea2[4] = numeros[(int)(temp / 10)];
	menu_linea2[5] = numeros[((int)temp % 10)];
	menu_linea2[6] = '.';
	menu_linea2[7] = numeros[(int)(temp * 10) % 10];;
	menu_linea2[8] = ' ';
	menu_linea2[9] = ' ';
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
	//menu_linea1=" Set  Temp  Max ";
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
	//menu_linea1=" Set  Temp  Min ";
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

	ciaaPOSIX_printf("Init Task...\n");

	fd_in = ciaaPOSIX_open("/dev/dio/in/0", ciaaPOSIX_O_RDONLY);

	/** @fn LCD_init initializes LCD*/
	LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
	
	/** Set GPIO1(cooler) and GPIO6(buzzer) OUTPUT*/
	Chip_GPIO_SetDir(LPC_GPIO_PORT,3,(1<<3),!0);
	Chip_GPIO_SetDir(LPC_GPIO_PORT,3,(1<<6),!0);


	/** activate periodic task:
	    *  - for the first time after 350 ticks (350 ms)
	    *  - and then every 150 ticks (150 ms)
	*/
    SetRelAlarm(ActivatePeriodicTask, 200, 200);

    /** activate periodic switches task:
    	* - for the first time after 350 ticks (350 ms)
    	* - and then every 100 ticks (70 ms)
    */
	SetRelAlarm(ActivateSwitchesTask, 303, 120);

	/** activate periodic Read task:
	     * - for the first time after 350 ticks (350 ms)
	     * - and then every 700 ticks (700 ms)
	*/
	SetRelAlarm(ActivateReadTask, 101, 1000);

    TerminateTask();
}

TASK(SwitchesTask)
{
	uint8 inputs;
	/** Read TEC_1,TEC_2,TEC_3, and TEC_4*/
	ciaaPOSIX_read(fd_in, &inputs, 1);


	/** The TEC_1 was pressed*/
	if((inputs&0x01) == 0){
		switch(estado) {
			case principal :
				estado = menu1;
				break;
			case menu1 :
				estado = set_temp_max;
				break;
			case menu2 :
				estado = set_temp_min;
				break;
			case menu3 :
				estado = mute_on;
				break;
			case menu4 :
				estado = cooler_on;
				break;
			default :
				estado = principal;
		}
	}
	/** The TEC_2 was pressed*/
	if((inputs&0x02) == 0){
			switch(estado) {
			case menu1 :
				estado = menu2;
				break;
			case menu2 :
				estado = menu3;
				break;
			case menu3 :
				estado = menu4;
				break;
			case menu4 :
				estado = menu1;
				break;
			case set_temp_max :
				estado = set_temp_max_res;
				break;
			case set_temp_min :
				estado = set_temp_min_res;
				break;
			default :
				ciaaPOSIX_printf("No hace nada");
		}
	}
	/** The TEC_3 was pressed*/
	if((inputs&0x04) == 0){
			switch(estado) {
			case menu2 :
				estado = menu1;
				break;
			case menu3:
				estado = menu2;
				break;
			case menu4:
				estado = menu3;
				break;
			case menu1:
				estado = menu4;
				break;
			case set_temp_max :
				estado = set_temp_max_sum;
				break;
			case set_temp_min :
				estado = set_temp_min_sum;
				break;
			default :
				ciaaPOSIX_printf("No hace nada");
		}
	}
	/** The TEC_4 was pressed*/
	if((inputs&0x08) == 0){
			switch(estado) {
			case menu1:
			case menu2:
			case menu3:
			case menu4:
				estado = principal;
				break;
			case set_temp_max :
				estado = menu1;
				break;
			case set_temp_min :
				estado = menu2;
				break;
			default :
				ciaaPOSIX_printf("No hace nada");
		}
	}


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

//	pantallaPpal_a_string();
	switch(estado) {
			case principal :
				pantallaPpal_a_string();
				break;
			case menu1 :
				menu_1_a_string();
				break;
			case menu2 :
				menu_2_a_string();
				break;
			case menu3 :
				menu_3_a_string();
				break;
			case menu4 :
				menu_4_a_string();
				break;
			case set_temp_max :
				set_temp_max_a_string(temp_max);
				break;
			case set_temp_max_sum :
				temp_max += 0.1;
				set_temp_max_a_string(temp_max);
				estado = set_temp_max;
				break;
			case set_temp_max_res :
				if(temp_max>1)temp_max -= 0.1;
				set_temp_max_a_string(temp_max);
				estado = set_temp_max;
				break;
			case set_temp_min :
				set_temp_min_a_string(temp_min);
				break;
			case set_temp_min_sum :
				temp_min += 0.1;
				set_temp_min_a_string(temp_min);
				estado = set_temp_min;
				break;
			case set_temp_min_res :
				if(temp_min>1)temp_min -=0.1;
				set_temp_min_a_string(temp_min);
				estado = set_temp_min;
				break;
			case mute_on :
				mute= !mute;
				Chip_GPIO_SetPinState(LPC_GPIO_PORT,3,6,0);
				estado = principal;
				break;
			case cooler_on :
				coolerON = !coolerON;
				Chip_GPIO_SetPinState(LPC_GPIO_PORT,3,3,coolerON);
				estado = menu4;
				break;
			default:
				ciaaPOSIX_printf("No hace nada");
		}
	
	LCD_write_char('\f');
	LCD_write_string(&menu_linea1);
	LCD_write_char('\r');
	LCD_write_string(&menu_linea2);

	TerminateTask();
}

TASK(ReadTask){

	leer_dht22(&dhthum,&dhttemp);

	if(dhttemp>temp_max){
		/** Cooler and buzzer are turned on */
		if(!mute)
			Chip_GPIO_SetPinState(LPC_GPIO_PORT,3,6,1);
	    if(!coolerON)
	    	Chip_GPIO_SetPinState(LPC_GPIO_PORT,3,3,1);}
	else if(dhttemp<temp_min){
		/** Buzzer is turned on */
		if(!mute)
			Chip_GPIO_SetPinState(LPC_GPIO_PORT,3,6,1);}
	else{
		/** Cooler and buzzer are turned off */
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,3,6,0);
		if((!coolerON) && (dhttemp<(temp_max-0.5)))
			Chip_GPIO_SetPinState(LPC_GPIO_PORT,3,3,0);}

	TerminateTask();

}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

