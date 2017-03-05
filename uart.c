#include "uart.h"

char data_in[10];
char ready_data[10];
int data_count=0;
int command_ready=0;

//Function To Initialize UART2
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled
void uart2_init(void)
{
 UCSR2B = 0x00; //disable while setting baud rate
 UCSR2A = 0x00;
 UCSR2C = 0x06;
 //UBRR2L = 0x5F; //set baud rate lo
 UBRR2L = 0x07;
 UBRR2H = 0x00; //set baud rate hi
 UCSR2B = 0x98;
}


void send_char(char c)
{
	while ((UCSR2A & (1 << UDRE2)) == 0) {};
	UDR2 = c;
}


void uart2_puts(char s[])
{
	int i =0;
	
	while (s[i] != 0x00)
	{
		send_char(s[i]);
		i++;
	}
}


SIGNAL(SIG_USART2_RECV)
 {
	   // Get data from the USART in register
	   data_in[data_count] = UDR2;

	   if (data_in[data_count] == '\n')
	    {
		   
		   
		   
		   command_ready = 1;
		   // Reset to 0, ready to go again

		   data_count = 0;
		} 
		   else 
		   {
		   data_count++;
	       }
 }
 
 
char *get_data()
{
	if(command_ready==1)
	{

		command_ready=0;
		return data_in;		

	}
	else
		return '\0';

}        

