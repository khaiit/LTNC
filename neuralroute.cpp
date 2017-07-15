#include "myneuralnet.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


neuron::neuron() :weights(0), deltavalues(0), output(0), gain(0), wgain(0) //constructor
{

}
//Destructor
neuron::~neuron()
{
	if (weights)
		delete[] weights;
	if (deltavalues)
		delete[] deltavalues;
}

// Khoi tao cac trong so cua neuron
void neuron::create(int inputcount)
{
	assert(inputcount);
	float sign = -1;//Dung khi doi dau
	float random;//Lay so ngau nghien
	weights = new float[inputcount];
	deltavalues = new float[inputcount];

	//khoi tao cac trong so la cac gia tri khong dau -> thay doi neu can
	//delta = 0
	for (int i = 0; i<inputcount; i++)
	{
		//Lay so ngau nghien trong khoang -0.5 and 0.5
		random = (float(rand()) / float(RAND_MAX)) / 2.f; //min 0.5
		random *= sign;
		sign *= -1;
		weights[i] = random;
		deltavalues[i] = 0;
	}
	gain = 1;

	random = (float(rand()) / float(RAND_MAX)) / 2.f; //min 0.5
	random *= sign;
	sign *= -1;
	wgain = random;




}
