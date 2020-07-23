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
	Neuron(int _output_id = -1);
	void Activate(float timestamp, float value);
	void Add_Output(Axon * axon);

	tuple<float, float, float> position;

private:
	int output_id = -1; // != -1 for output neurons
	float last_activation_TS = numeric_limits<float>::lowest();
	vector<Axon*> outgoing_axons;

	float activation_threshold = 0.2f;
	float current_potential = -0.2f;
	float potential_floor = -0.2f;
	float refractory_period = 10.0f;
};