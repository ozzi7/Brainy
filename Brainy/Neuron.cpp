#include "Neuron.h"

Neuron::Neuron(priority_queue<tuple<float, Axon*>, vector<tuple<float, Axon*>>, AxonFiringComparator> * _pq,
	bool _is_dacaying, bool _is_adaptive, int _output_id)
{
	position = make_tuple(
		-5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10.0f))),
		-5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10.0f))),
		-5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10.0f))));
	output_id = _output_id;
	pq = _pq;
	is_adaptive = _is_adaptive;
	is_decaying = _is_dacaying;
}
void Neuron::Activate(Axon* activated_axon, float timestamp, float value)
{
	/* decay neuron over time */
	if(is_decaying)
		current_potential = max(starting_potential, current_potential - (timestamp - last_decay_timestamp) * 0.000001f);

	/* create an adaptive volt threshold */
	if (is_adaptive)
	{
		float beta = 1.0f;
		float adaption_time_constant = 30.0f; // should be around the memory time needed (with different values per neuron)
		float passed_time = (timestamp - last_decay_timestamp);
		a_t = (exp((-passed_time) / adaption_time_constant) * a_t + (1 - exp((-passed_time) / adaption_time_constant)) * has_fired * 1);
		activation_threshold = initial_activation_threshold + beta * a_t;
	}
	has_fired = false;
	last_decay_timestamp = timestamp;

	if (last_activation_TS + refractory_period <= timestamp)
	{
		// allow re-activation if refractory period has passed

		current_potential = max(current_potential + value, potential_floor);
		if (current_potential >= activation_threshold)
		{
			// the neuron fires
			neuron_firings.push_back(timestamp);
			last_activation_TS = timestamp;

			for (auto& axon : outgoing_axons)
			{
				pq->push(make_tuple(timestamp + axon->length, axon));
			}
			current_potential = starting_potential;
			nof_firings++;
			has_fired = true;
		}
	}
	if (activated_axon != nullptr)
		incoming_axon_activations.push_back(make_tuple(timestamp, activated_axon));
}
void Neuron::Add_Output_Axon(Axon* axon)
{
	outgoing_axons.push_back(axon);
}
void Neuron::Add_Input_Axon(Axon* axon)
{	
	incoming_axons.push_back(axon);
}
void Neuron::Reset()
{
	last_activation_TS = numeric_limits<float>::lowest();
	last_decay_timestamp = 0.0f;
	current_potential = starting_potential;
	nof_firings = 0.0f;
	activation_threshold = initial_activation_threshold;
	a_t = 0.0f;
	has_fired = false;
	neuron_firings.clear();
	incoming_axon_activations.clear();
}
void Neuron::Update_Weights_STDP(float timestamp)
{
	if (!is_stdp_enabled)
		return;

	/* updates the weights for all incoming axons, unless the activations are not old enough then we postpone the update*/
	/* update the output neurons based on */
	int i;
	for (i = 0; i < incoming_axon_activations.size(); ++i)
	{
		if (get<0>(incoming_axon_activations[i]) <= timestamp - 100)
		{
			float weight_change = 0.0f;
			float Aplus = (1.0f - get<1>(incoming_axon_activations[i])->weight) *nu_pos;
			float Aminus = (get<1>(incoming_axon_activations[i])->weight - (-1.0f)) * nu_neg;
			for (int j = neuron_firings.size()-1; j >= 0; --j)
			{
				float time_diff = neuron_firings[j] - get<0>(incoming_axon_activations[i]);
				if (time_diff > 100.0f || time_diff < -100.0f)
					break;

				if (time_diff > 0.0f)
				{
					// x > 0
					weight_change += Aplus * exp(-time_diff / 3.0f);
				}
				else if (time_diff < 0.0f)
				{
					// x < 0
					weight_change += Aminus * exp(time_diff / 15.0f);
				}
			}
			/*if(weight_change < -0.01f || weight_change > 0.01f)
			cout << "Update weight: " << get<1>(incoming_axon_activations[i])->weight << " to " <<
				get<1>(incoming_axon_activations[i])->weight + weight_change << endl;*/

			get<1>(incoming_axon_activations[i])->SetWeight(get<1>(incoming_axon_activations[i])->weight + weight_change);
		}
		else break;
	}
	// removes the first and already updated axon activations
	incoming_axon_activations.erase(incoming_axon_activations.begin(), incoming_axon_activations.begin() + i);

	////neuron_firings.clear(); // later should be deleted

	// weight adjustment test
	//for (float weight = -1.0f; weight <= 1.0f; weight = weight + 0.1f)
	//{
	//	for (float diff = -100.0f; diff <= 100.0f; diff = diff + 10.0f)
	//	{
	//		float weight_change = 0.0f;
	//		float Aplus = (1.0f - weight) * nu_pos;
	//		float Aminus = (weight - (-1.0f)) * nu_neg;


	//		if (diff > 0.0f)
	//		{
	//			// x > 0
	//			weight_change += Aplus * exp(-diff / 3.0f);
	//		}
	//		else if (diff < 0.0f)
	//		{
	//			// x < 0
	//			weight_change += Aminus * exp(diff / 15.0f);
	//		}
	//		cout << "Weight: " << weight << ", time diff: " << diff << ", weight update: " << weight_change << endl;
	//	}
	//}
}