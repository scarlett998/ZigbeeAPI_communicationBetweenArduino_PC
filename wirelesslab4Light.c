/*
 * wirelesslab4Light.c
 *
 * Created: 10/30/2019 10:59:53 AM
 * Author : Group
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#define F_CPU 16000000 // 16 MHz oscillator.
#define BaudRate 9600
#define MYUBRR (F_CPU/ 16 /BaudRate) - 1
#define RECIVEMAX 23


char sendPacketLight[] = {0x7e, 0x00, 0x0f, 0x10, 0x01, 0x00, 0x13, 0xa2, 0x00, 0x40, 0x7c, 0x43, 0x5a, 0xff, 0xfe, 0x00, 0x00, 0x41, 0xa2};
char *reciveData;
int position = 0;
char lenght = 3;
char sendData = 0x42;
char checkBit = 0;
char dataWait = 0;
void serialInit(void);


int main(void)
{
	reciveData = calloc(RECIVEMAX, sizeof(char));	
	//Set PORT B to output light
	DDRB = 0xff;
	//Set Serial Transmit pin to output
	DDRD = 0b00000010;
	//Set PORTB 0 turn off light
	PORTB = 0;
	//initialize serial communication
	serialInit();
	//Turn on global interrupts
	sei();

	while(1){
		if(dataWait){
			_delay_ms(200);
			dataWait = 0;
			sei();
		};
	}
}


void serialInit() {
	/* Set the baud rate */
	UBRR0H = (unsigned char) ((MYUBRR)>>8);
	UBRR0L = (unsigned char) MYUBRR;
	/* Frame format: 8data, No parity, 1stop bit */
	UCSR0C = (3 << UCSZ00);
	/* Enable receiver and transmitter to engage in communication! */
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1<<TXEN0) | (1<<TXCIE0);
	
}


//Receive interrupt
ISR(USART_RX_vect){
	char reciveByte = UDR0;
	if(position<3){
		reciveData[position] = reciveByte;
		}else if(position == 3){
		lenght = 3 + reciveData[2] + 1;
		}else if(position>3){
		if(position < lenght-1){
			reciveData[position] = reciveByte;
			}else if(position == lenght-1){
			if(reciveData[2] == 19){
				char data = reciveData[RECIVEMAX-2];
				if(data == 'A'){
					PORTB = 0xff;
					sendData = 'A';
					}else if(data == 'B'){
					PORTB = 0;
					sendData = 'B';
				}
				UDR0 =  sendPacketLight[0];
				position = 0;
				return;
				}else{
				position = 0;
				return;
			}
		}
	}
	position++;
	
}

ISR(USART_TX_vect){
	position++;
	if(position == 17){
		char data = sendPacketLight[17];
		
		if(data != sendData){
			UDR0 = 'B';
			checkBit = sendPacketLight[18]-1;
			}else{
			checkBit = sendPacketLight[18];
			UDR0  = 'A';
		}
		return;
		}else if(position == 18){
		UDR0 = checkBit;
		cli();
		dataWait = 1;
		position = 0;
		return;
	}

	UDR0 = sendPacketLight[position];
}
