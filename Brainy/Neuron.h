#pragma once

#include <tuple>
#include <limits>
#include <vector>
#include <algorithm>
#include <queue> 

#include "Axon.h"


using namespace std;

class Neuron
{
public:
	Neuron(priority_queue<tuple<float, Axon*>, vector<tuple<float, Axon*>>, AxonFiringComparator>* _pq, 
		int _output_id = -1);
	void Activate(float timestamp, float value);
	void Add_Output(Axon * axon);
	void Reset();

	tuple<float, float, float> position;
	int output_id = -1; // != -1 for output neurons
	float nof_firings = 0;

private:
	float last_activation_TS = numeric_limits<float>::lowest();
	vector<Axon*> outgoing_axons;

	float activation_threshold = 0.2f;
	float current_potential = -0.2f;
	float potential_floor = -0.2f;
	float refractory_period = 10.0f;

	priority_queue<tuple<float, Axon*>, vector<tuple<float, Axon*>>, AxonFiringComparator> * pq;
};