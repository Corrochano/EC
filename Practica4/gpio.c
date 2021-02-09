#include "44b.h"
#include "gpio.h"

/* Port B interface implementation */

int portB_conf(int pin, enum port_mode mode)
{
	int ret = 0;
	if (pin < 0 || pin > 10)
		return -1;

	if (mode == SIGOUT){
		rPCONB |= (0x1 << pin);
		// COMPLETAR si es necesario

	}
	else if (mode == OUTPUT){
		rPCONB = rPCONB &~(0x1 << pin);
		// COMPLETAR: tomar la implementación prácticas anteriores

	}
	else
		ret = -1;

	return ret;
}

int portB_write(int pin, enum digital val)
{
	if (pin < 0 || pin > 10)
		return -1;

	if (val < 0 || val > 1)
		return -1;

	if (val){
		// COMPLETAR: tomar la implementación prácticas anteriores
		rPDATB = rPDATB | (0x1 << pin);

	}
	else{
		rPDATB = rPDATB & ~(0x1 << pin);
		// COMPLETAR: tomar la implementación prácticas anteriores


	}
	return 0;
}

/* Port G interface implementation */

int portG_conf(int pin, enum port_mode mode)
{
	//La variable pos indica el primer bit del registro de control del puerto G que corresponde al pin
	int pos  = pin*2;

	if (pin < 0 || pin > 7)
		return -1; // indica error

	switch (mode) {
		case INPUT:

			// COMPLETAR: tomar la implementación prácticas anteriores
			rPCONG = rPCONG &~(0X03 << pos);

			break;
		case OUTPUT:
			// COMPLETAR: tomar la implementación prácticas anteriores
			rPCONG &= ~(0x3<<pos);
			rPCONG |= (0x1<<pos);

			break;
		case SIGOUT:
			// COMPLETAR: tomar la implementación prácticas anteriores
			rPCONG &= ~(0x3<<pos);
			rPCONG|= (0x2<<pos);

			break;
		case EINT:
			// COMPLETAR: tomar la implementación prácticas anteriores
			rPCONG |= (0x3 << pos);

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
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 000, para configurar interrupciones externas por nivel bajo
			rEXTINT &= ~(0x7 << pos);
			break;
		case LHIGH:
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 001, para configurar interrupciones externas por nivel alto
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x1 << pos);

			break;
		case FALLING:
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 010, para configurar interrupciones externas por flanco de
			// bajada
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x2 << pos);

			break;
		case RISING:
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 100, para configurar interrupciones externas por flanco de
			// subida
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x4 << pos);

			break;
		case EDGE:
			// COMPLETAR: configurar el pin del puerto G para que el pin genere interrupciones externas por cualquier flanco
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x6 << pos);

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
		return -1;

	if (val < 0 || val > 1)
		return -1;

	if ((rPCONG & (0x3 << pos)) != (0x1 << pos))
		return -1;

	if (val)
		// COMPLETAR: tomar la implementación prácticas anteriores
		rPDATG |=(0x1 << pos);
	else
		// COMPLETAR: tomar la implementación prácticas anteriores
		rPDATG &=~(0x1 << pos);

	return 0;
}

int portG_read(int pin, enum digital* val)
{
	int pos = pin*2;

	if (pin < 0 || pin > 7)
		return -1;

	if (rPCONG & (0x3 << pos))
		return -1;

	if (rPDATG & (0x3 << pos))
		// COMPLETAR: tomar la implementación prácticas anteriores
		*val=HIGH;
	else
		// COMPLETAR: tomar la implementación prácticas anteriores
		*val=LOW;

	return 0;
}

int portG_conf_pup(int pin, enum enable st)
{
	if (pin < 0 || pin > 7)
		return -1;

	if (st != ENABLE && st != DISABLE)
		return -1;

	if (st == ENABLE){
		rPUPG &=~(0x1<<pin);
		//COMPLETAR: activar la resistencia de pull-up del pin del puerto G
		rPUPG &= ~(0x1 << pin);
	}
	else{
		rPUPG |=(0x1<<pin);
		// COMPLETAR: tomar la implementación prácticas anteriores
		rPUPG |= (0x1 << pin);
	}
	return 0;
}



