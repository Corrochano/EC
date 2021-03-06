#include "44b.h"
#include "gpio.h"

/* Port B interface implementation */

int portB_conf(int pin, enum port_mode mode)
{
	int ret = 0;
	if (pin < 0 || pin > 10)
		return -1; // indica error

	if (mode == SIGOUT)
		rPCONB |= (0x1 << pin);
	else if (mode == OUTPUT)
		rPCONB = rPCONB &~(0x1 << pin);
	else
		ret = -1; // indica error

	return ret;
}

int portB_write(int pin, enum digital val)
{
	if (pin < 0 || pin > 10)
		return -1; // indica error

	if (val < 0 || val > 1)
		return -1; // indica error

	if (val)
		rPDATB = rPDATB | (0x1 << pin);
	else
		rPDATB = rPDATB & ~(0x1 << pin);

	return 0;
}

/* Port G interface implementation */

int portG_conf(int pin, enum port_mode mode)
{
	int pos  = pin*2;

	if (pin < 0 || pin > 7)
		return -1; // indica error

	switch (mode) {
		case INPUT:
			rPCONG = rPCONG &~(0X3 << pos);
			break;
		case OUTPUT:
			rPCONG = rPCONG &~(0X3 << pos);
			rPCONG = rPCONG | (0x1<<pos);
			break;
		case SIGOUT:
			rPCONG = rPCONG &~(0X3 << pos);
			rPCONG = rPCONG | (0x2<<pos);
			break;
		case EINT:
			rPCONG = rPCONG | (0X3 << pos);

			break;
		default:
			return -1;
	}

	return 0;
}

int portG_eint_trig(int pin, enum trigger trig)
{
	int pos = pin*4;

	if (pin < 0 || pin > 7)
		return -1;

switch (trig) {
		case LLOW:
			// COMPLETAR: poner en rEXTINT a partir de la posici�n pos tres bits
			// a 000, para configurar interrupciones externas por nivel bajo
			rEXTINT =rEXTINT & ~(0x7<<pos);
			break;
		case LHIGH:
			// COMPLETAR: poner en rEXTINT a partir de la posici�n pos tres bits
			// a 001, para configurar interrupciones externas por nivel alto
			rEXTINT = rEXTINT & ~(0x7<<pos);
			rEXTINT = rEXTINT | (0x1<<pos);
			break;
		case FALLING:
			// COMPLETAR: poner en rEXTINT a partir de la posici�n pos tres bits
			// a 010, para configurar interrupciones externas por flanco de
			// bajada
			rEXTINT = rEXTINT & ~(0x7<<pos);
			rEXTINT = rEXTINT | (0x2<<pos);
			break;
		case RISING:
			// COMPLETAR: poner en rEXTINT a partir de la posici�n pos tres bits
			// a 100, para configurar interrupciones externas por flanco de
			// subida
			rEXTINT = rEXTINT & ~(0x7<<pos);
			rEXTINT = rEXTINT | (0x4<<pos);
			break;
		case EDGE:
			// COMPLETAR: poner en rEXTINT a partir de la posici�n pos tres bits
			// a 110, para configurar interrupciones externas por cualquier
			// flanco
			rEXTINT = rEXTINT & ~(0x7<<pos);
			rEXTINT = rEXTINT | (0x6<<pos);
			break;
		default:
			return -1;
	}
	return 0;
}

int portG_write(int pin, enum digital val)
{
	int pos = pin*2;

	if (pin < 0 || pin > 7)
		return -1; // indica error

	if (val < 0 || val > 1)
		return -1; // indica error

	if ((rPCONG & (0x3 << pos)) != (0x1 << pos))
		return -1; // indica error

	if (val)

		rPDATG |= (0x1 << pin);
	else

		rPDATG = rPDATG & ~(0x1 << pin);

	return 0;
}

int portG_read(int pin, enum digital* val)
{
	int pos = pin*2;

	if (pin < 0 || pin > 7)
		return -1; // indica error

	if (rPCONG & (0x3 << pos))
		return -1; // indica error

	if (rPDATG&(0x1 << pin))/*COMPLETAR: true si a 1 en rPDATG el pin indicado por el metro pin*/
		*val = HIGH;
	else
		*val = LOW;

	return 0;
}

int portG_conf_pup(int pin, enum enable st)
{
	if (pin < 0 || pin > 7)
		return -1; // indica error

	if (st != ENABLE && st != DISABLE)
		return -1; // indica error

	if (st == ENABLE)
		// COMPLETAR: poner el pin de rPUPG indicado por el parametro pin al valor adecuado,
		// para activar la resistencia de pull-up
		rPUPG = rPUPG & ~(0x1 << pin);
	else
		// COMPLETAR: poner el pin de rPUPG indicado por el parametro pin al valor adecuado,
		// para desactivar la resistencia de pull-up
		rPUPG = rPUPG | (0x1 << pin);
	return 0;
}


