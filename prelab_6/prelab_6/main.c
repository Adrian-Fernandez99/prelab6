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
void write_char(char caracter);
void write_str(char* texto);

uint16_t ADC_read(uint8_t PIN);

uint16_t ADC_in = 0;

// MAIN LOOP
int main(void)
{
	setup();
	sei();
	while (1)
	{	
		ADC_in = ADC_read(5);
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
	ADMUX = (1 << REFS0);  // 5V de referencia
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);  // prescaler = 64
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

uint16_t ADC_read(uint8_t PIN)
{
	ADMUX = (ADMUX & 0xF0) | (PIN & 0x0F);   // Selecciona canal (0–7)
	ADCSRA |= (1 << ADSC);                   // Inicia conversión
	while (ADCSRA & (1 << ADSC));            // Espera a que termine
	return ADC;                              // Devuelve valor (10 bits)
}

void write_char(char caracter)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = caracter;
}

void write_str(char* texto)
{
	for(uint8_t i = 0; *(texto+i) != "\0"; i++)
	{
		write_char(*(texto+i));
	}
}


// Interrupt routines
ISR(USART_RX_vect)
{
	char temporal = UDR0;
	PORTB = temporal;
}