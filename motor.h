#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void motion_pin_config (void);
void motion_set (unsigned char Direction);

void forward (void);
void back (void);
void left (void);
void right (void); 
void soft_left (void);
void soft_right (void);
void soft_left_2 (void);
void soft_right_2 (void);
void stop (void);
