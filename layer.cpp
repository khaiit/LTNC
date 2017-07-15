
#include "myneuralnet.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



layer::layer() :neurons(0), neuroncount(0), layerinput(0), inputcount(0)
{

}

layer::~layer()
{
	if (neurons)
	{
		for (int i = 0; i<neuroncount; i++)
		{
			delete neurons[i];
		}
		delete[] neurons;
	}
	if (layerinput)
	{
		delete[] layerinput;
	}
}

void layer::create(int inputsize, int _neuroncount)
{
	assert(inputsize && _neuroncount);//kiem tra loi
	int i;
	neurons = new neuron*[_neuroncount];
	for (i = 0; i<_neuroncount; i++)
	{
		neurons[i] = new neuron;
		neurons[i]->create(inputsize);
	}

	layerinput = new float[inputsize];
	neuroncount = _neuroncount;
	inputcount = inputsize;
}
// Tinh toan ket qua cua lop dung sigmoid (Co the thay doi sigmoid bang ham khac)
void layer::calculate()
{
	int i, j;
	float sum;
	//Ap cong thuc vao cac neural
	for (i = 0; i<neuroncount; i++)
	{
		sum = 0;//Tong cac gia tri
		for (j = 0; j<inputcount; j++)
		{
			
			sum += neurons[i]->weights[j] * layerinput[j]; //ap trong so : input * weight
		}
		sum += neurons[i]->wgain * neurons[i]->gain; //ap gain hoac theta nhan cho trong so gain.
		//dua vao sigmoid
		neurons[i]->output = 1.f / (1.f + exp(-sum));//tinh sigmoid
		// neurons[i]->output=-1 + 2*(1.f + exp(-sum));
	}
}

