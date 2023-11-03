/*
 * UART_Master.cpp
 * Author: Mohand Chahed, Maxim Vezina & Wafaa Mokhtari
 * Recoit les données du clavier par UART
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
	UCSR0B |= (1 << RXEN0);								// Activer emission et reception  USART
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
	while (myString[i]) {
		transmitByte(myString[i]);
		i++;
		
	}
	
}

int main(void)
{
	/*Set LED pin (for example, PB1) as an output*/
	DDRD |= (1 << PD4);
	/*Initialize UART*/
	initUSART();
	while(1)
	{
		unsigned char receivedData = receiveByte();
		if (receivedData == 0x58)
		{
			PORTB |= (1 << PD4);						// Turn on the LED
			_delay_ms(500);								// Wait for 500 ms
			PORTB &= ~(1 << PD4);						// Turn off the LED
			_delay_ms(500);								// Wait for 500 ms
			
		}
		
	}
	
}
