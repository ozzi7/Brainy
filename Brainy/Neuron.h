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
	Neuron();
	void Activate(float timestamp, float value);

	tuple<float, float, float> position;

private:
	float id = -1;
	float last_activation_TS = numeric_limits<float>::lowest();
	vector<Axon> outgoing_axons;

	float activation_threshold = 0.2f;
	float current_potential = -0.2f;
	float potential_floor = -0.2f;
	float refractory_period = 10.0f;
};