#pragma once

#include <tuple>
#include <limits>
#include <vector>
#include <algorithm>
#include <queue> 
#include <math.h>
#include <list>
#include <iostream>

#include "Axon.h"


using namespace std;

class Neuron
{
public:
	Neuron(priority_queue<tuple<float, Axon*>, vector<tuple<float, Axon*>>, AxonFiringComparator>* _pq, 
		bool _is_dacaying, bool _is_adaptive, int _output_id = -1);
	void Activate(Axon* activated_axon, float timestamp, float value);
	void Add_Output_Axon(Axon * axon);
	void Add_Input_Axon(Axon* axon);
	void Update_Weights_STDP(float timestamp);
	void Reset();

	tuple<float, float, float> position;
	int output_id = -1; // != -1 for output neurons
	float nof_firings = 0;
	const float initial_activation_threshold = 0.2f;
	float activation_threshold = initial_activation_threshold;

private:
	float last_activation_TS = numeric_limits<float>::lowest();
	float last_decay_timestamp = 0.0f;
	bool has_fired = false;

	vector<Axon*> outgoing_axons;
	vector<Axon*> incoming_axons;
	
	vector<tuple<float, Axon*>> incoming_axon_activations;
	vector<float> neuron_firings;

	float current_potential = 0.0f;
	const float starting_potential = 0.0f;

	float a_t = 0.0f;
	const float potential_floor = -0.2f;
	const float refractory_period = 10.0f;

	/* STDP */
	const float nu_pos = 0.06;
	const float nu_neg = -0.09f;

	bool is_adaptive;
	bool is_decaying;
	bool is_stdp_enabled = false;
	priority_queue<tuple<float, Axon*>, vector<tuple<float, Axon*>>, AxonFiringComparator> * pq;
};