/*
 *SPI_Master.cpp
 *Author: Mohand Chahed, Maxim Vezina & Wafaa Mokhtari
 *Connecte aux autres ATmegas et au capteur de temperature
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#define BAUD 9600
#include <util/setbaud.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <util/delay.h>

void SPI_MasterInit(void)
{
	/*Set MOSI and SCK output SS*/
	DDRB |= (1<<PB3)|(1<<PB5);
	DDRB |= (1<<PB2);									//SS du SLAVE 1
	DDRB |= (1<<PB1);									//SS du SLAVE 2
	/*Enable SPI, Master, set clock rate fck/64*/
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1);
	
}

void SPI_MasterTransmit(char cData)
{
	/*Start transmission*/
	SPDR = cData;
	/*Wait for transmission complete*/
	while(!(SPSR & (1<<SPIF)));
	
}

void initUSART(void)
{
	UBRR0H = 0;											//baud rate
	UBRR0L = 51;										//9600
	UCSR0B |= (1 << RXEN0);								//Activer emission et reception  USART
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);			//8 data bits, 1 stop bit, valeur au reset
	
}

void transmitByte(uint8_t data)
{
	UDR0 = data;										//Envoyer la donnée
	while ( !( UCSR0A & (1<<UDRE0)) );					//Attendre que le buffer de transmission soit vide
	
}

uint8_t receiveByte(void)
{
	while ( !( UCSR0A & (1<<RXC0)) );					//Attendre que le buffer de réception soit plein
	return UDR0;										//Retourner la valeur lue
	
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
	uint8_t donnees;
	/*Bouton Poussoir sur PC0, Activer résistance de Rappel*/
	PORTD |=(1<<PD2);
	PORTD |=(1<<PD3);
	SPI_MasterInit();									//Initialisation SPI Master
	PORTB|=(1<<PB2);									//SS du SLAVE1 = 1
	PORTB|=(1<<PB1);									//SS du SLAVE2 = 1
	/*Set LED pin (for example, PB1) as an output*/
	DDRD |= (1 << PD4);
	/*Initialize UART*/
	initUSART();
    while (1)
    { 
		/*Lire l'etat du bouton poussoir*/
		/*Donnees pour la lumière*/
		if (bit_is_clear(PIND,PD2))
		donnees=0x01;									//Si bouton appuyé envoyé 01
		else
		donnees=0x00;									//Si non envoyé 00
		/*Controle du SLAVE 1*/
		PORTB &= ~(1<<PB2);								//Activé Slave 1 SS = 0
		_delay_us(5);
		SPI_MasterTransmit(donnees);					//Transmettre la donnee
		_delay_us(5);
		PORTB|=(1<<PB2);								//Désactivé Slave1 SS = 1
		/*Controle du SLAVE 2*/
		if (bit_is_clear(PIND,PD3))
		{
			donnees=0x01;								//Si bouton appuyé envoyé 01
			
		}
		else
		{
			donnees=0x00;								//Si non envoyé 00
			
		}
		PORTB &= ~(1<<PB1);								//Activé Slave2 SS=0
		_delay_us(5);
		/*Donnnes pour le verrou*/
		SPI_MasterTransmit(donnees);					//Transmettre la donnee
		_delay_us(5);
		PORTB|=(1<<PB1);								//Désactivé Slave2 SS = 1
		
    }  
	while(1)
	{
		unsigned char receivedData = receiveByte();
		if (receivedData == 0x58)
		{
			PORTB |= (1 << PD4);						//Turn on the LED
			_delay_ms(500);								//Wait for 500 ms
			PORTB &= ~(1 << PD4);						//Turn off the LED
			_delay_ms(500);								//Wait for 500 ms
			
		}
		
	}
	
}