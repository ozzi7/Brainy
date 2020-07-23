#pragma once

#include <vector>

#include "Neuron.h"

using namespace std; 

class Brain
{
public:
	Brain();
	vector<float> Run(vector<float> input);

private:
	int maximum_post_steps = 10;
	float current_timestamp_ms = 0.0f;
	float step_size_ms = 100.0f;

	int nof_input_neurons = 3;
	int nof_output_neurons = 3;
	vector<Neuron*> input_neurons;
	vector<Neuron*> output_neurons;

	priority_queue<tuple<float, Axon*>, AxonFiringComparator> pq;
};