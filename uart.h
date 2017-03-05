#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


void uart2_init(void);
void send_char(char c);
void uart2_puts(char s[]);
