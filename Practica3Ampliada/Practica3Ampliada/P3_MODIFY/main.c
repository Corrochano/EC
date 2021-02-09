#include <stdio.h>
#include "44b.h"
#include "button.h"
#include "leds.h"
#include "utils.h"
#include "D8Led.h"
#include "intcontroller.h"
#include "timer.h"
#include "gpio.h"
#include "keyboard.h"

struct RLstat {
	int moving;
	int speed;
	int direction;
	int position;
};

static struct RLstat RL = {
	.moving = 0,
	.speed = 5,
	.direction = 0,
	.position = 0,
};

void timer_ISR(void) __attribute__ ((interrupt ("IRQ")));
void button_ISR(void) __attribute__ ((interrupt ("IRQ")));
void keyboard_ISR(void) __attribute__ ((interrupt ("IRQ")));

void timer_ISR(void)
{
	//COMPLETAR: tomar el código de avance de posición del led rotante de la práctica anterior
	if(RL.direction == 0){
		RL.position += 1;
	if(RL.position == 6)
		RL.position = 0;
		}
	/*else {
		RL.position -= 1;
	if(RL.position == -1)
		RL.position = 5;
				}*///solo van en un sentido
	D8Led_segment(RL.position);
	ic_cleanflag(INT_TIMER0);
	ic_cleanflag(INT_TIMER1);
}


void button_ISR(void)

{
	unsigned int whicheint = rEXTINTPND;
	unsigned int buttons = (whicheint >> 2) & 0x3;

	//COMPLETAR: usar el código de la primera parte parte de atención a los
	//pulsadores
	/*if (buttons & BUT1) {
			led1_switch();
			if(RL.direction == 0)
				RL.direction = 1;
			else if(RL.direction == 1)
				RL.direction = 0;
		}*///solo va hacia un sentido

	if (buttons & BUT2) {
			led2_switch();
			if(RL.moving == 0){
				RL.moving = 1;
				tmr_update(TIMER0);
				tmr_start(TIMER0);
			}
			else if(RL.moving == 1){
				RL.moving = 0;
				tmr_stop(TIMER0);
			}
		}

	if (buttons & BUT1) {
				led1_switch();
				if(RL.moving == 0){
					RL.moving = 1;
					tmr_update(TIMER1);
					tmr_start(TIMER1);
				}
				else if(RL.moving == 1){
					RL.moving = 0;
					tmr_stop(TIMER1);
				}
			}

	// eliminamos rebotes
	Delay(2000);

	if (rEXTINTPND & 0x4){
			rEXTINTPND = rEXTINTPND | (0x1 << 2);
		}
		else if(rEXTINTPND & 0x8){
			rEXTINTPND = rEXTINTPND | (0x1 << 3);
		}
		ic_cleanflag(INT_EINT4567);
}

void keyboard_ISR(void)
{
	int key;

	/* Eliminar rebotes de presión */
	Delay(200);

	/* Escaneo de tecla */
	key = kb_scan();

	if ((key != -1) && ((key == 4) || (key == 5) || (key == 6) || (key == 7))) {
		/* Visualizacion en el display */
		//COMPLETAR: mostrar la tecla en el display utilizando el interfaz
		//definido en D8Led.h
		D8Led_digit(key);
		switch (key) {
			case 4:
				tmr_set_divider(TIMER1, D1_8);
				tmr_set_count(TIMER1, 62500, 62495);
				tmr_update(TIMER1);
				break;
			case 5:
				tmr_set_divider(TIMER1, D1_8);
				tmr_set_count(TIMER1, 31250, 31195);
				tmr_update(TIMER1);
				break;
			case 6:
				tmr_set_divider(TIMER1, D1_8);
				tmr_set_count(TIMER1, 15625, 15600);
				tmr_update(TIMER1);
				break;
			case 7:
				tmr_set_divider(TIMER1, D1_4);
				tmr_set_count(TIMER1, 15625, 15600);
				tmr_update(TIMER1);
				break;
			default:
				break;
		}
	}

	else if ((key != -1) && ((key == 0) || (key == 1) || (key == 2) || (key == 3))) {
				/* Visualizacion en el display */
				//COMPLETAR: mostrar la tecla en el display utilizando el interfaz
				//definido en D8Led.h
				D8Led_digit(key);
				switch (key) {
					case 0:
						tmr_set_divider(TIMER0, D1_8);
						tmr_set_count(TIMER0, 62500, 62495);
						tmr_update(TIMER0);
						break;
					case 1:
						tmr_set_divider(TIMER0, D1_8);
						tmr_set_count(TIMER0, 31250, 31195);
						tmr_update(TIMER0);
						break;
					case 2:
						tmr_set_divider(TIMER0, D1_8);
						tmr_set_count(TIMER0, 15625, 15600);
						tmr_update(TIMER0);
						break;
					case 3:
						tmr_set_divider(TIMER0, D1_4);
						tmr_set_count(TIMER0, 15625, 15600);
						tmr_update(TIMER0);
						break;
					default:
						break;
				}

		/* Esperar a que la tecla se suelte, consultando el registro de datos */
		while((rPDATG & 0x2) == 0){}

	}

    /* Eliminar rebotes de depresión */
    Delay(200);

    /* Borrar interrupciones pendientes */
	//COMPLETAR
	//borrar la interrupción por la línea EINT1 en el registro rI_ISPC
    ic_cleanflag(INT_EINT1);
}


int setup(void)
{
	leds_init();
	D8Led_init();
	int value = RL.position +1;
	D8Led_2segments(RL.position, value);

	/* Port G: configuración para generación de interrupciones externas,
	 *         botones y teclado
	 **/

	//COMPLETAR: utilizando el interfaz para el puerto G definido en gpio.h
	//configurar los pines 1, 6 y 7 del puerto G para poder generar interrupciones
	//externas por flanco de bajada por ellos y activar las correspondientes
	//resistencias de pull-up.
	portG_conf(6, EINT);
	portG_conf(7, EINT);
	portG_conf(1, EINT);//Interrupción del teclado

	portG_eint_trig(6, FALLING);
	portG_eint_trig(7, FALLING);
	portG_eint_trig(1, FALLING);

	portG_conf_pup(1, ENABLE);
	portG_conf_pup(6, ENABLE);
	portG_conf_pup(7, ENABLE);

	/********************************************************************/

	/* Configuración del timer */

	//COMPLETAR: tomar el código de la segunda parte
	tmr_set_prescaler(0,255);
	tmr_set_divider(0, D1_8);
	tmr_set_count(TIMER0, 62500, 62495);
	tmr_update(TIMER0);
	tmr_set_count(TIMER1, 62500, 62495);
	tmr_update(TIMER1);
	tmr_set_mode(0, RELOAD);
	tmr_stop(TIMER0);
	tmr_stop(TIMER1);

	if (RL.moving)
		tmr_start(TIMER0);
	if (RL.moving)
		tmr_start(TIMER1);
	/***************************/

	// Registramos las ISRs
	pISR_TIMER0   = (unsigned) timer_ISR;
	pISR_TIMER1   = (unsigned) timer_ISR;
	pISR_EINT4567 = (unsigned) button_ISR;
	pISR_EINT1    = (unsigned) keyboard_ISR;

	/* Configuración del controlador de interrupciones
	 * Habilitamos la línea IRQ, en modo vectorizado
	 * Configuramos el timer 0 en modo IRQ y habilitamos esta línea
	 * Configuramos la línea EINT4567 en modo IRQ y la habilitamos
	 * Configuramos la línea EINT1 en modo IRQ y la habilitamos
	 */

	ic_init();
	//COMPLETAR: utilizando el interfaz definido en intcontroller.h
	//		habilitar la línea IRQ en modo vectorizado
	//		deshabilitar la línea FIQ
	//		configurar la línea INT_TIMER0 en modo IRQ
	//		configurar la línea INT_EINT4567 en modo IRQ
	//		configurar la línea INT_EINT1 en modo IRQ
	//		habilitar la línea INT_TIMER0
	//		habilitar la línea INT_EINT4567
	//		habilitar la línea INT_EINT1
	ic_conf_irq(ENABLE, VEC);
	ic_conf_fiq(DISABLE);
	ic_conf_line(INT_TIMER0, IRQ);
	ic_conf_line(INT_EINT4567, IRQ);
	ic_enable(INT_TIMER0);
	ic_enable(INT_EINT4567);
	ic_enable(INT_EINT1);
	/***************************************************/

	Delay(0);
	return 0;
}





int main(void)
{
	setup();

	while (1) {
	}
}
