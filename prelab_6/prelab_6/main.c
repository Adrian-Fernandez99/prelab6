/*
Universidad del Valle de Guatemala
IE2023 : Programación de Microcontroladores

prelab_6.c

Created: 4/25/2025 10:11:37 AM
Author : Adrián Fernández

Descripción: Controlador de dos servos haciendo 
uso de dos potenciometros ditstintos.
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Prototipos de función
void setup();
void ADC_init();
void UART_init();
void write_char(uint8_t direct, char caracter);
void write_str(char* texto);

// MAIN LOOP
int main(void)
{
	setup();
	sei();
	while (1)
	{	
	}
}

// NON-Interrupt subroutines
// Funciones de Seteo
void setup()
{
	cli();
	DDRB = 0xFF; 
	ADC_init();
	UART_init();
	sei();
}
void ADC_init()
{
	ADMUX = 0;
	ADMUX	|= (1 << REFS0);
	ADMUX	|= (1 << ADLAR);
	
	ADCSRA	= 0;
	ADCSRA	|= (1 << ADPS1) | (1 << ADPS0);
	ADCSRA	|= (1 << ADIE);
	ADCSRA	|= (1 << ADEN);
	
	ADCSRA	|= (1 << ADSC);
}

void UART_init()
{
	DDRD |= (1 << DDD1);	// Configuración de pines rx y tx
	DDRD &= ~(1 << DDD0);
	UCSR0A = 0;				// Configuración del serial
	UCSR0B |= (1 << RXCIE0) | (1<< RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	UBRR0 = 103;	// BAUD RATE a 9600
}

void write_char(uint8_t direct, char caracter)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	if (direct == 0)
	{
		PORTB = caracter;
	}
	if (direct == 1)
	{
		UDR0 = caracter;
	}
}
/*
void write_str(char* texto)
{
	for(uint8_t i = 0; *(texto+i) != "\0"; i++)
	{
		write_char(*(texto+i));
	}
}
*/

// Interrupt routines
ISR(USART_RX_vect)
{
	char temporal = UDR0;
	write_char(0, temporal);
}

ISR(ADC_vect)
{
	char ingreso = ADCH;
	write_char(1, ingreso);
	ADCSRA	|= (1 << ADSC);
}