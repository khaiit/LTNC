#if 1
#include <iostream>
#include "myneuralnet.h"
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
using namespace std;
#define PATTERN_COUNT 100
#define PATTERN_SIZE 2
#define NETWORK_INPUTNEURONS 100
#define NETWORK_OUTPUT 1
#define HIDDEN_LAYERS 0
#define EPOCHS 20000
#define PATTERN_TEST 101

float value_x[PATTERN_COUNT];
float value_y[PATTERN_COUNT];
float value_class[PATTERN_COUNT];

int find_N_line(string filename)
{
	int i = 1;
	string line;

	ifstream myfile(filename.c_str());
	if (myfile.is_open())
	{
		for (i; getline(myfile, line); i++);
	}
	myfile.close();
	return i-1;
}
void readfile(string filename) {

	string line;
	ifstream myfile(filename.c_str());

	if (myfile.is_open())
	{



			int temp = 0;
			while (getline(myfile, line))
		{
			istringstream iss(line);
			if (!(iss >> value_x[temp] >> value_y[temp] >>value_class[temp])) { cout << "Unable to read x t"; break; } // error
			temp++;
			

		}

	}
	myfile.close();

}


int main()
{

	float pattern[PATTERN_COUNT][PATTERN_SIZE];
	float desiredout[PATTERN_COUNT][NETWORK_OUTPUT];
	int numberofrow; // dung de tim so du olieu trong tap test va ket qua
	float resultoutput[PATTERN_TEST][NETWORK_OUTPUT]; // kich thuoc tuy vao so lu lieu test
 

	readfile("inputforclassification.txt");
	for (int i = 0; i < PATTERN_COUNT; i++)
	{
													// pattern[x][y] can than PATTERN_SIZE khi sua
		pattern[i][0] = value_x[i];
		pattern[i][1] = value_y[i];
				
		desiredout[i][0] = value_class[i];
	}
	

	

//--------------------------------------------------------------
	myneuralnet net;//Ten mang neuron
    int i,j;
    float error;

    //Khoi tao mang voi cac thong so DEFINE o tren
    net.create(PATTERN_SIZE,NETWORK_INPUTNEURONS,NETWORK_OUTPUT,HIDDEN_LAYERS,HIDDEN_LAYERS);

    //Bat dau train theo so luong PATTERN_COUNT
    for(i=0;i<EPOCHS;i++)
    {
        error=0;
        for(j=0;j<PATTERN_COUNT;j++)
        {
            error+=net.train(desiredout[j],pattern[j],0.2f,0.1f);
        }
        error/=PATTERN_COUNT;
        // Tra ve neu loi
        cout << "ERROR:" << error << "\r";

    }

  
	ofstream result("resultforclassification.txt");
	string line;
	
	

	ifstream myfile("testclassification.txt");
	if (myfile.is_open())
	{
		
		
		float tempX = 0;
		float tempY = 0;
		int i = 0;
		while (getline(myfile, line))
		{
			istringstream iss(line);
			if (!(iss >> tempX >>tempY )) { cout << "Unable to read x t"; break; } // error
			resultoutput[i][0]= tempX;
			resultoutput[i][1] = tempY;
			i++;
		}

	}
	myfile.close();
	/////////
	
	for (i = 0; i<PATTERN_TEST; i++)
    {

		net.propagate(resultoutput[i]);
		if (result.is_open())
		{
			float temp = 0;
			//Xuat ket qua 
			temp = net.getOutput().neurons[0]->output;
			if (temp <= 0.5) { temp = 0; }
			else { temp = 1; }
			result << value_x[i] <<"\t" << value_y[i] << "\t" << temp  << endl;

		}
		else
		{

			cout << "Something went wrong with opening the file!";
		}

    }

	result.close();
		


		
	
    return 0;
}

#endif