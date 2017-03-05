/*-------------------------------------INCLUDES-------------------------------------*/

#include <math.h>
#include <stdlib.h>

#include "error.h"
/*-------------------------------------DEFINES-------------------------------------*/
#define REFERENCE_POINTS 3
#define NUM_PARTICLE 100
#define PI 3.141592654
void sense(float x, float y, float direction, float* measurement_ref,unsigned int **);

double gaussrand();

struct particle{
	float x;
	float y;
	float vx;
	float vy;
	float direction;
	float weight;
};


typedef struct particle particle;
particle** particles;
float calculate_weight(particle* p, float* measurement_ref);
int create_particles();
