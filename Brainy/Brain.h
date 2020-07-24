#pragma once

#include <vector>

#include "Neuron.h"

using namespace std; 

class Brain
{
public:
	Brain(int _nof_input_neurons, int _nof_output_neurons, int _nof_main_neurons, float _step_size_ms);
	void Run(vector<float> & input, vector<float>& output, int max_steps);
	void Get_Params(vector<float> & params);
	void Set_Params(vector<float> & params);

private:
	void reset();

	float current_timestamp_ms = 0.0f;
	float step_size_ms = 100.0f;

	int nof_input_neurons = 3;
	int nof_output_neurons = 3;
	int nof_main_neurons = 4;
	vector<Neuron*> input_neurons;
	vector<Neuron*> output_neurons;
	vector<Neuron*> main_neurons;
	vector<Neuron*> all_neurons;

	vector<Axon*> all_axons;

	priority_queue<tuple<float, Axon*>, vector<tuple<float, Axon*>>, AxonFiringComparator> * pq;
};