#include "lcd.h"
#include "adc.h"
#include  "motor.h"
#include  "posencoder.h"

void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();
	posencoder_init();
 
}
void init_devices (void)
{
 cli(); //Clears the global interrupts
 port_init();
 sei();   //Enables the global interrupts
}		


void motor_test()
{

		forward(); //both wheels forward
		_delay_ms(1000);

		stop();						
		_delay_ms(500);
	
		back(); //bpth wheels backward						
		_delay_ms(1000);

		stop();

}
int main(void)
{
	
	init_devices();
	lcd_set_4bit();
	lcd_init();
	adc_init();
	//motor_test();
	unsigned char sharp, distance, adc_reading;
	unsigned int value;
	while(1)
	{
		sharp = ADC_Conversion(11);						//Stores the Analog value of front sharp connected to ADC channel 11 into variable "sharp"
		//value = Sharp_GP2D12_estimation(sharp);				//Stores Distance calsulated in a variable "value".
		
		value= getPosLeftCount();
		lcd_print(2,14,value,3); 
	}
}
