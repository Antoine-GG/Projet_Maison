/* Réception UART a partir du microcontrolleur (qui controle le clavier)*/
#define F_CPU 8000000UL
#include <avr/io.h>
#define BAUD 9600 
#include <util/setbaud.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <util/delay.h>

void initUSART(void)
{
	UBRR0H = 0;              /* baud rate  */
	UBRR0L = 103;           /* 9600 */
	UCSR0A |= (1 << U2X0);      /* mode asynchrone double vitesse */
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);    /* Activer emission et reception  USART */
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);   /* 8 data bits, 1 stop bit, valeur au reset*/
	
}
void transmitByte(uint8_t data)
{
	while ( !( UCSR0A & (1<<UDRE0)) ); /* Attendre que le buffer de transmission soit vide */
	UDR0 = data;                      /* envoyer la donnée */
	
}
uint8_t receiveByte(void) 
{
	while ( !( UCSR0A & (1<<RXC0)) ); /* Attendre que le buffer de réception soit plein */
	return UDR0;                                /* retourner la valeur lue */
	
}
void printString(const char myString[]) 
{
	uint8_t i = 0;
	while (myString[i]) {
		transmitByte(myString[i]);
		i++;
		
	}
	
}

int main(void) {
	DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
	DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
	PORTB |= ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
	PORTC |= ((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
	// -------- Inits --------- //
	initUSART();
	char result = 'X';
	while (1) 
	{
		uint8_t pinState = (PINB & 0x3F) | ((PINC & 0x0F) << 6);
		// Convert the binary pinState to a letter
		switch (pinState) {
			case 0x3F: result = '1'; break;
			case 0x3E: result = '2'; break;
			case 0x3D: result = '3'; break;
			case 0x3B: result = '4'; break;
			case 0x37: result = '5'; break;
			case 0x2F: result = '6'; break;
			case 0x1F: result = '7'; break;
			case 0x3C: result = '8'; break;
			case 0x3A: result = '9'; break;
			case 0x36: result = '*'; break;
			case 0x2E: result = '0'; break;
			case 0x1E: result = '#'; break;
			default:   result = 'X'; break;  // X for unknown or no button pressed
				
			}
			// Transmit the result over UART
			transmitByte(result);
			
		}
	return (0);
	
}