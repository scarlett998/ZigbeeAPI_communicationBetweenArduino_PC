/*
 * wirelesslab4Tem.c
 *
 * Created: 10/31/2019 5:41:15 PM
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
#define DHT11_PIN 9

char sendPacketTerm[] = {0x7e, 0x00, 0x0f, 0x10, 0x01, 0x00, 0x13, 0xa2, 0x00, 0x40, 0x7c, 0x43, 0x5a, 0xff, 0xfe, 0x00, 0x00, 0x23, 0xc0};
char *reciveData;
int position = 0;
char lenght = 3;
char sendData = 0x42;
char checkBit = 0;
char dataWait = 0;
uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

void serialInit(void);

void Request()				/* Microcontroller send start pulse/request */
{
	DDRB |= (1<<DHT11_PIN);
	PORTB &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTB |= (1<<DHT11_PIN);	/* set to high pin */
}

void Response()				/* receive response from DHT11 */
{
	DDRB &= ~(1<<DHT11_PIN);
	while(PINB & (1<<DHT11_PIN));
	while((PINB & (1<<DHT11_PIN))==0);
	while(PINB & (1<<DHT11_PIN));
}

uint8_t Receive_data()			/* receive data */
{
	for (int q=0; q<8; q++)
	{
		while((PINB & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PINB & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PINB & (1<<DHT11_PIN));
	}
	return c;
}

int term(){
	char data[5];
	Request();		/* send start pulse */
	Response();		/* receive response */
	I_RH=Receive_data();	/* store first eight bit in I_RH */
	D_RH=Receive_data();	/* store next eight bit in D_RH */
	I_Temp=Receive_data();	/* store next eight bit in I_Temp */
	D_Temp=Receive_data();	/* store next eight bit in D_Temp */
	CheckSum=Receive_data();/* store next eight bit in CheckSum */
	//itoa(I_Temp,data,10);
	//itoa(D_Temp,data,10);
	int temperature=itoa(I_Temp,data,10);
	return temperature;
}

int main(void)
{
    /* Replace with your application code */
	reciveData = calloc(RECIVEMAX, sizeof(char));
	//Set Serial Transmit pin to output
	DDRD = 0b00000010;
	//initialize serial communication
	serialInit();
	//Turn on global interrupts
	sei();
    while (1) 
    {
		if(dataWait){
			_delay_ms(200);
			dataWait = 0;
			sei();
			}
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
				
				if(data == 'C'){
					UDR0 =  sendPacketTerm[0];
				}
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

if(position == 0){
	term();
}

	position++;
	if(position == 17){
		char data = sendPacketTerm[17]-sendData;
		UDR0 = sendData;
		checkBit = sendPacketTerm[18]+data;
		return;
		}else if(position == 18){
		UDR0 = checkBit;
		cli();
		dataWait = 1;
		position = 0;
		return;
	}
	
	UDR0 = sendPacketTerm[position];
}

