/*
 * SPI_Master.cpp
 * Author: Mohand Chahed, Maxim Vezina & Wafaa Mokhtari
 * Connecte aux atmegas qui controlent la serrure et la lumiere
 */

#define  F_CPU 8000000UL
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output SS*/
	DDRB |= (1<<PB3)|(1<<PB5);
	DDRB |= (1<<PB2);									//SS du SLAVE 1
	DDRB |= (1<<PB1);									//SS du SLAVE 2
	/* Enable SPI, Master, set clock rate fck/64 */
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1);
	
}

void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
}

int main(void)
{   
	uint8_t donnees;
	/*Bouton Poussoir sur PC0, Activer résistance de Rappel*/
	PORTC |=(1<<PC0);
	PORTC |=(1<<PC1);
	SPI_MasterInit();									//Initialisation SPI Master
	PORTB|=(1<<PB2);									//SS du SLAVE1 = 1
	PORTB|=(1<<PB1);									//SS du SLAVE2 = 1
    while (1)
    { 
		/*lire l'etat du bouton poussoir*/
		/*donnees pour la lumière*/
		if (bit_is_clear(PINC,PC0))
		donnees=0x01;									//si bouton appuyé envoyé 01
		else
		donnees=0x00;									//si non envoyé 00
		/*controle du SLAVE1*/
		PORTB &= ~(1<<PB2);								//Activé Slave 1 SS=0
		_delay_us(5);
		SPI_MasterTransmit(donnees);					//Transmettre la donnee
		_delay_us(5);
		PORTB|=(1<<PB2);								//Désactivé Slave1 SS=1
		/*controle du SLAVE2*/
		if (bit_is_clear(PINC,PC1))
		{
			donnees=0x01;								//si bouton appuyé envoyé 01
			
		}
		else
		{
			donnees=0x00;								//si non envoyé 00
			
		}
		PORTB &= ~(1<<PB1);								//Activé Slave2 SS=0
		_delay_us(5);
		/*donnnes pour le verrou*/
		SPI_MasterTransmit(donnees);					//Transmettre la donnee
		_delay_us(5);
		PORTB|=(1<<PB1);								//Désactivé Slave 2   SS=1
		
    }  
	
}