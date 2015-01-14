/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
	 
#define CONFIDENCE_LEVEL 10000
	 
void USARTWriteChar(unsigned char data)
{
	//Wait until the transmitter is ready

	while(!(UCSRA & (1<<UDRE)))
	{
	//Do nothing
	}

	//Now write the data to USART buffer

	UDR=data;
}

 unsigned char USARTReadChar()
 {
    //Wait untill a data is available

    while(!(UCSRA & (1<<RXC)))
    {
       //Do nothing
    }

    //Now USART has got data from host
    //and is available is buffer

    return UDR;
 }


#include "common.h"
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
unsigned char ReceivedByte; 

ISR(USART_RXC_vect) 
{ 
	
	ReceivedByte = UDR; // Fetch the received byte value into the variable "ByteReceived" 
	UDR = ReceivedByte; // Echo back the received byte back to the computer 
	//if (ReceivedByte != 0xfe && ReceivedByte != 0xf8 && ReceivedByte != 0x0)
		//PORTB = ReceivedByte;
	PORTC = 0x00;
	//else
		//PORTB = 0;
}

// read
ISR(INT0_vect) 
{ 
	
	PORTB = ReceivedByte;

	
	
	
	
	
	PORTC = 0x01;
	
	
	
}

// write
ISR(INT1_vect) 
{ 
	
}

int main(void)
{
	int derp=0, stop=0, pressed=0, pressed_confidence_level=0, released_confidence_level=0;
	unsigned char c;
	
	cli();
	//DDRC = 0xff;
	DDRB = 0xff;
	DDRC = 0x01;
	DDRD = 0b00000000;	// PD2 PD3 inputs
	
	PORTB = 0xff;
	PORTC = 0xff;
	PORTD = 0b00001100;
	
	ASSR = 0;
	ACSR = (1 << ACD);	// disable analog comparator
	
	//MCUCR = 0;
	MCUCR = 0b10; //0b00000011; //0b00001011;
	//GICR = 0;
	GICR = 0b01000000;
	
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register 
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	
	UCSRA = 0;
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	
	UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC) 
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed 
	
    /* insert your hardware initialization here */
    for(;;)
	{
		
    }
    return 0;   /* never reached */
}
