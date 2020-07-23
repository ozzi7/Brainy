#pragma once

#include "Neuron.h"

using namespace std; 

class Brain
{
public:
	Brain();
	vector<float> Run(vector<float> input);

private:

	priority_queue<Axon> pq;
};