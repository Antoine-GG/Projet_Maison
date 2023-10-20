/*
 * UART_Clavier.cpp
 * Author: Mohand Chahed, Maxim Vezina & Wafaa Mokhtari
 * Lis les données du clavier
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#define BAUD 9600
#include <util/setbaud.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <util/delay.h>

void initUSART(void)
{
	UBRR0H = 0;											// baud rate
	UBRR0L = 51;										// 9600
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);				// Activer emission et reception  USART
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);			// 8 data bits, 1 stop bit, valeur au reset
	
}

void transmitByte(uint8_t data)
{
	UDR0 = data;										// envoyer la donnée
	while ( !( UCSR0A & (1<<UDRE0)) );					// Attendre que le buffer de transmission soit vide
	
}

uint8_t receiveByte(void)
{
	while ( !( UCSR0A & (1<<RXC0)) );					// Attendre que le buffer de réception soit plein
	return UDR0;										// retourner la valeur lue
	
}

void printString(const char myString[])
{
	uint8_t i = 0;
	while (myString[i]) 
	{
		transmitByte(myString[i]);
		i++;
		
	}
	
}

int main(void)
{
	DDRD |= (1 << PD6);
	DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
	DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
	DDRD &= ~(1 << PD1);
	PORTB |= ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
	PORTC |= ((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
	PORTD |= (1 << PD1);
	/*Inits*/
	initUSART();
	char result = 0x58;
	while (1)
	{
		PORTD |= (1 << PD6);
		_delay_ms(500);
		PORTD &= ~(1 << PD6);
		_delay_ms(500);
		
		if (PINB & (1<<PB0))
		{
			result = 0x33;
			
		}
		if (PINB & (1<<PB1))
		{
			result = 0x36;
			
		}
		if (PINB & (1<<PB2))
		{
			result = 0x39;
			
		}
		if (PINB & (1<<PB3))
		{
			result = 0x23;
			
		}
		if (PINB & (1<<PB4))
		{
			result = 0x32;
			
		}
		if (PINB & (1<<PB5))
		{
			result = 0x35;
			
		}
		if (PINC & (1<<PC0))
		{
			result = 0x38;
			
		}
		if (PINC & (1<<PC1))
		{
			result = 0x30;
			
		}
		if (PINC & (1<<PC2))
		{
			result = 0x31;
			
		}
		if (PINC & (1<<PC3))
		{
			result = 0x34;
			
		}
		if (PINC & (1<<PC4))
		{
			result = 0x37;
			
		}
		if (PINC & (1<<PC5))
		{
			result = 0x2A;
			
		}
		else
		{
			result = 0x58;
			
		}
		transmitByte(result);
		
	}
	return (0);
	
}