#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f877a.h>

#define _XTAL_FREQ 3276800		// Frekvence krystalu

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

void USART_Init(void)
{
	TRISCbits.TRISC6 = 0;		// 0 = vystup
	TRISCbits.TRISC7 = 1;
	TXSTA = 0b00100100;
	RCSTA = 0b10010000;
	SPBRG = 21;					// BRGH = 0: BaudRate = FOSC/(64(X+1)) 
								//			=> X = FOSC/(64 BaudRate) - 1
								// BRGH = 1: BaudRate = FOSC/(16(X+1))
								//			=> X = FOSC/(16 BaudRate) - 1
	RCIF = TXIF = 0;
}
char USART_ReceiveData(void)
{
	while(RCIF != 1)
		;
	return RCREG;
}
void USART_SendData(char data)
{
	while(TXIF != 1)
		;
	TXREG = data;
}

void main(void) 
{
	USART_Init();
	while(1)
		USART_SendData(USART_ReceiveData());

	return;
}
