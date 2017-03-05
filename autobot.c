#include "lcd.h"
#include "adc.h"
#include  "motor.h"
#include  "posencoder.h"
#include "particlefilter.h"
#include "uart.h"


unsigned int range_points[3]={0,0,0};
unsigned int odo_points[2]={0,0};


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

void send_data_over_serial()

{

		char output[65];
		sprintf(output, "%u;%u;%u;%u;%u;\n",range_points[0],range_points[1],range_points[2],odo_points[0],odo_points[1] );
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

void getOdometry()
{
	

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

int exec_motion_cmd(char cmd, int duration)
{
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
		default:
			break;


	}
	_delay_ms(duration);

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


	
	while(1)
	{
	
	
		char * cin=get_data();
		int duration=command_data(cin);
		exec_motion_cmd(cin[0],duration);


		odo_points[0]= getPosLeftCount();
		odo_points[1]= getPosRightCount();
		sense_objects();
		
		
		send_data_over_serial();
		//lcd_home();
		//lcd_string(cin);
		//lcd_print(2,2,command_data(cin),5);

	 
	}
}
 
