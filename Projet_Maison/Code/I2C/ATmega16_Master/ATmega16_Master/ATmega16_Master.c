/*
 * ATmega16_Master.c
 * http://www.electronicwings.com
 *
 *Le Master envoi le code 0x25 au Slave pour lui dire qu'il doit lui envoyer l'état des Bouton poussoirs relié sur PC2, Pc1 et PC0 (capteur PORTE, FENETRE 1, FENETRE 2.
 *et il passe en mode lecture pour attendre la donnée.
 * En plus Le master va lire la température fournie par le DS1621
 */ 

#define F_CPU 8000000UL						/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <util/delay.h>						/* Include inbuilt defined Delay header file */
#include <stdio.h>							/* Include standard I/O header file */
#include <string.h>							/* Include string header file */
#include "I2C_Master_H_file.h"				/* Include I2C header file */

#define Slave_Write_Address		0x20
#define Slave_Read_Address		0x21
#define	count					10

int main()
{
   DDRC|=(1<<PC0)|(1<<PC1)|(1<<PC2);
   PORTC &=~(1<<PC0); //Temoin Porte
   PORTC &=~(1<<PC1); //Temoin Fenetre 1
   PORTC &=~(1<<PC2); // Temoin Fenetre 2
   char val;
	
	I2C_Init();								/* Initialize I2C */
	

	while (1)
	{
	
		I2C_Start_Wait(Slave_Write_Address);/* Start I2C communication with SLA+W */
		_delay_ms(5);
			
		I2C_Write(0x25);					/* Envoyer 0x25 code pour demander d'envoyer une données */
		_delay_ms(500);
		
	
		I2C_Repeated_Start(Slave_Read_Address);	/* Repeated Start I2C communication with SLA+R */
		_delay_ms(5);
		
		val=I2C_Read_Nack(); /*lire la donnée (1 seule) et envoyer sur le port D   vérifier l'état de PD0 */
		
		if ((val & 0b00000001)==0b00000000)  PORTC &=~(1<<PC0); else PORTC |=(1<<PC0);//Temoin Porte
		if ((val & 0b00000010)==0b00000000)  PORTC &=~(1<<PC1); else PORTC |=(1<<PC1);//Temoin Fenetre 1
		if ((val & 0b00000100)==0b00000000)  PORTC &=~(1<<PC2); else PORTC |=(1<<PC2);// Temoin Fenetre 2
		_delay_ms(500);
		
		I2C_Stop();   /* Stop I2C */
		
		//communication avec le capteur de temperature i2c ds1621
		// convertir une donnée,
		I2C_Start_Wait(TEMP_W);
		_delay_ms(5);
		I2C_Write(start_conversion);//ordre de conversion
		_delay_ms(5);
		I2C_Repeated_Start(TEMP_W);
		_delay_ms(5);
		I2C_Write(read_temperature);//code pour lire la temperature
		_delay_ms(5);
		I2C_Repeated_Start(TEMP_R);
		_delay_ms(5);
		tmp_msb=I2C_Read_Ack();//lecture de la temperature
		_delay_ms(5);
		tmp_lsb=I2C_Read_Nack();//lecture de la partie decimale de la temperature
		_delay_ms(5);
		
		I2C_Stop();
		
		
		Temperature=tmp_msb; //Température pas de 1oC
		//Temperature=tmp_msb+0.5*(tmp_lsb/128); //Température pas de 0.5oC
		
		//convertir la valeur de la pression en string et pret a etre envoyer au ESP
		dtostrf(Temperature, 6, 1, aff);
	};				
};