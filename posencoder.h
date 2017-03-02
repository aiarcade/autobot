#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void left_encoder_pin_config (void);
void right_encoder_pin_config (void);
void posencoder_init();
void left_position_encoder_interrupt_init (void) ;
void right_position_encoder_interrupt_init (void);
ISR(INT5_vect);
ISR(INT4_vect);
unsigned long int getPosLeftCount();
unsigned long int getPosRightCount();
void  clearPosCounters();
