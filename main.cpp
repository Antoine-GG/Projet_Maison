/* Réception UART a partir du microcontrolleur (qui controle le clavier)*/
#define F_CPU 8000000UL
#include <avr/io.h>
#define BAUD 9600 
#include <util/setbaud.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <util/delay.h>



void initUSART(void) {
	UBRR0H = 0;              /* baud rate  */
	UBRR0L = 103;           /* 9600 */
	UCSR0A |= (1 << U2X0);      /* mode asynchrone double vitesse */
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);    /* Activer emission et reception  USART */
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);   /* 8 data bits, 1 stop bit, valeur au reset*/
}
void transmitByte(uint8_t data) {
	while ( !( UCSR0A & (1<<UDRE0)) ); /* Attendre que le buffer de transmission soit vide */
	UDR0 = data;                      /* envoyer la donnée */
}
uint8_t receiveByte(void) {
	while ( !( UCSR0A & (1<<RXC0)) ); /* Attendre que le buffer de réception soit plein */
	return UDR0;                                /* retourner la valeur lue */
}
void printString(const char myString[]) {
	uint8_t i = 0;
	while (myString[i]) {
		transmitByte(myString[i]);
		i++;
	}
}

int main(void) {
	DDRB &|= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
	DDRC &|= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
	PORTB |= ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5));
	PORTC |= ((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
	
	char serialCharacter;
	
	// -------- Inits --------- //
	initUSART();
		while (1) {
			serialCharacter = receiveByte();  // Lire un caractère
            if(serialCharacter=='5'){ //code definit a 5
				PORTD|=(1<<PD4);//allumer la led 5(bouton 5)
				
			}
			else {
				PORTD &=~(1<<PD4);//ÉTEINT LA LED
			}
		}
	return (0);
}