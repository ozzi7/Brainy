#include "Neuron.h"

Neuron::Neuron()
{

}
void Neuron::Activate(float timestamp, float value)
{
	if (last_activation_TS + refractory_period <= timestamp)
	{
		// allow re-activation if refractory period has passed

		current_potential = max(current_potential + value, potential_floor);
		if (current_potential > activation_threshold)
		{
			// the neuron fires
			last_activation_TS = timestamp;

			for (auto& axon : outgoing_axons)
			{
				axon.Activate(timestamp);
			}
		}
	}
}
