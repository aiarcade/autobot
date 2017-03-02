#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <math.h> //included to support power function
void adc_init();
unsigned char ADC_Conversion(unsigned char);
