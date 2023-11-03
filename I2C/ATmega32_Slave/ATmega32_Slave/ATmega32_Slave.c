 /*
 * ATmega32_Slave.c
 * http://www.electronicwings.com
 *
 *
 *
 *Le Slave attend la reception du code 0x25 du Master
 *Pour lui envoyer l'état du bit PB0 du PORTB. (Mettre un Bouton Poussoir sur PB0)
 *le code recu est envoyé sur le PORTD ( mettre 8 led pour voir le code)
 */ 


#define F_CPU 8000000UL							/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>								/* Include AVR std. library file */
#include <util/delay.h>							/* Include inbuilt defined Delay header file */
#include <stdio.h>								/* Include standard I/O header file */
#include <string.h>								/* Include string header file */
#include "I2C_Slave_H_File.h"					/* Include I2C slave header file */

#define Slave_Address			0x20

int main(void)
{
	int drapeau=0;
	int8_t count = 0;
	uint8_t valeur=0b00000000;
	
	PORTC|=(1<<PC0);//Resistance de Rappel activé pour le Bouton Poussoir PORTE
	PORTC|=(1<<PC1);//Resistance de Rappel activé pour le Bouton Poussoir FENETRE 1
	PORTC|=(1<<PC2);//Resistance de Rappel activé pour le Bouton Poussoir  FENETRE 2
	
	I2C_Slave_Init(Slave_Address);
		
	while (1)
	{
		switch(I2C_Slave_Listen())				/* Check for any SLA+W or SLA+R */
		{
			case 0://receive
			{
				do
				{
	
					count = I2C_Slave_Receive();/* Receive data byte*/
					if(count==0x25) drapeau=1;  // vérifier si c'est 0x25 (code) alors autoriser la transmission
					PORTD=count;
				} while (count != -1);			/* Receive until STOP/REPEATED START received */
				
				break;
			}
			
			case 1://transmit
			{
				int8_t Ack_status;
		        if(drapeau==1){ //si bon code envoi l'état de PC2, PC1 et PC0
				do
					{   valeur=(PINC & 0b00000111);  //valeur de PC2, PC1 et PC0
						Ack_status = I2C_Slave_Transmit(valeur);	/* Send data byte */
				
					} while (Ack_status == 0);		/* Send until Acknowledgment is received */
				
					drapeau=0;//reset pour lire une autre donnée
				}
				break;
			}
			default:
				break;
		}
	}
}

