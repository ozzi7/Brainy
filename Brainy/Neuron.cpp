#include "Neuron.h"

Neuron::Neuron(int _output_id)
{
	position = make_tuple(
		-5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10.0f))),
		-5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10.0f))),
		-5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10.0f))));
	output_id = _output_id;
}
void Neuron::Activate(float timestamp, float value)
{
	if (last_activation_TS + refractory_period <= timestamp)
	{
		// allow re-activation if refractory period has passed

		current_potential = max(current_potential + value, potential_floor);
		if (current_potential >= activation_threshold)
		{
			// the neuron fires
			last_activation_TS = timestamp;

			for (auto& axon : outgoing_axons)
			{
				axon->Activate(timestamp);
			}
		}
	}
}
void Neuron::Add_Output(Axon* axon)
{
	outgoing_axons.push_back(axon);
}