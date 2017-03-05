#include "lcd.h"
#include "adc.h"
#include  "motor.h"
#include  "posencoder.h"
#include "particlefilter.h"
#include "uart.h"


unsigned int range_points[3]={0,0,0};
unsigned int odo_points[2]={0,0};



unsigned long int ms;

unsigned long int current_time=0;



void Timer4_Init(void)
{
	TCCR4B = 0x00; //stop
 	TCNT4H = 0x1F; //Counter higher 8 bit value
 	TCNT4L = 0x01; //Counter lower 8 bit value
 	OCR4AH = 0x00; //Output Compair Register (OCR)- Not used
 	OCR4AL = 0x00; //Output Compair Register (OCR)- Not used
 	OCR4BH = 0x00; //Output Compair Register (OCR)- Not used
 	OCR4BL = 0x00; //Output Compair Register (OCR)- Not used
 	OCR4CH = 0x00; //Output Compair Register (OCR)- Not used
 	OCR4CL = 0x00; //Output Compair Register (OCR)- Not used
 	ICR4H  = 0x00; //Input Capture Register (ICR)- Not used
 	ICR4L  = 0x00; //Input Capture Register (ICR)- Not used
 	TCCR4A = 0x00; 
 	TCCR4C = 0x00;
 	TCCR4B = 0x04; //start Timer
     TIMSK4 = 0x01; //timer4 overflow interrupt enable


}

void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();
	posencoder_init();
	Timer4_Init();
	
 
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


ISR(TIMER4_OVF_vect)
{
	 TCNT4H = 0x1F; //reload counter high value
 	 TCNT4L = 0x01; //reload counter low value

	ms++;
}

unsigned long int millis(void) 
{ 
    return ms; 
} 


void send_data_over_serial()

{

		char output[65];
		sprintf(output, "%u;%u;%u;%u;%u;%u;\n",range_points[0],range_points[1],range_points[2],odo_points[0],odo_points[1],current_time );
		uart2_puts(output);





}

unsigned int sense_obstacles(){

    unsigned char sharp; 
	unsigned int value;
    sharp = ADC_Conversion(11);						//Stores the Analog value of front sharp connected to ADC channel 11 into variable "sharp"
    value = Sharp_GP2D12_estimation(sharp);
	return value;


}


void sense_objects()
{
	
	
	left();
	_delay_ms(150);
	range_points[0]=sense_obstacles();
	right();
	_delay_ms(150);
	range_points[1]=sense_obstacles();
	right();
    _delay_ms(150);
	range_points[2]=sense_obstacles();
	left();
	_delay_ms(150);
	stop();
	clearPosCounters();



		
}



int test_partcle_filter()
{
	int i;
	float total_weight = 0;
	float measurement_ref[REFERENCE_POINTS];
	create_particles();
	memset(measurement_ref, 0.0, sizeof(measurement_ref));
//	sense(1, 1, 1, measurement_ref,sense_points);
	int h=0;
	char output[10];
	//printf("sensed value is %f, %f, %f\n", measurement_ref[0], measurement_ref[1], measurement_ref[2]);
	for(i=0; i<NUM_PARTICLE; i++){
		float weight = calculate_weight(particles[i], measurement_ref);
		particles[i]->weight = weight;
		total_weight += weight;
		
		if(weight > 0.01)
		{
			lcd_print(2,14,i,3);
			h=h+1;
			
			
            
			_delay_ms(100);


		}
	//send_particle_over_serial(particles[i]->x,particles[i]->y,particles[i]->vx,particles[i]->vy,particles[i]->direction,particles[i]->weight);
		//	printf("main: particles[%d]->weight=%f\n", i, particles[i]->weight);	
	}
	lcd_print(2,1,h,3);
//	printf("main: total_weight=%f\n", total_weight);
	
}


int command_data(char *str)
{

	char data[6];
		
	data[0]=str[1];
	data[1]=str[2];
	data[2]=str[3];
	data[3]=str[4];
	data[4]=str[5];
	data[5]=0;
	

	return atoi(data);

}

void reset_odometry()
{

	odo_points[0]=0;
	odo_points[1]=0;
	clearPosCounters();

		
}

int exec_motion_cmd(char cmd, int duration)
{
	int no_command=0;
	lcd_home();
	lcd_wr_char(cmd);
	unsigned long int mills=millis();
	switch(cmd)
	{
		case 'F':
			forward();
			break;
		case 'B':
			back();
			break;
		case 'L':
			left();
			break;
		case 'R':
			right();
			break;
		case 'O':
			reset_odometry();
			no_command=1;
			break;
			
		case 'N':
			no_command=1;
			break;
		default:
			no_command=1;
			break;


	}
	if(no_command==0){
		_delay_ms(duration);
		  current_time= current_time+millis()-mills; 
	}
     

}

int main(void)
{ 
	
	init_devices();
	lcd_set_4bit();
	lcd_init();
	adc_init();
	 uart2_init();
	//motor_test();
	unsigned char sharp, distance, adc_reading;
	unsigned int value;
	current_time=0;

	
	while(1)
	{
	
	
		char * cin=get_data();
		int duration=command_data(cin);
		exec_motion_cmd(cin[0],duration);

 
		odo_points[0]= odo_points[0]+getPosLeftCount();
		odo_points[1]= odo_points[1]+getPosRightCount();
		sense_objects();
		
		
		send_data_over_serial();
		
		lcd_print(2,2,  current_time,5);

	 
	}
}
 
