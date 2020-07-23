#include "Brain.h"
#include "Util.h"


Brain::Brain()
{
	// currently constructs a fully connected graph
	for (int i = 0; i < nof_input_neurons; ++i)
	{
		input_neurons.push_back(new Neuron());
	}	
	for (int i = 0; i < nof_output_neurons; ++i)
	{
		output_neurons.push_back(new Neuron(i));
	}
	for (int i = 0; i < nof_main_neurons; ++i)
	{
		main_neurons.push_back(new Neuron());
	}

	// create axons between all neurons
	vector<Neuron*> all_neurons;
	all_neurons.reserve(input_neurons.size() + output_neurons.size() + main_neurons.size());
	all_neurons.insert(all_neurons.end(), input_neurons.begin(), input_neurons.end());
	all_neurons.insert(all_neurons.end(), output_neurons.begin(), output_neurons.end());
	all_neurons.insert(all_neurons.end(), main_neurons.begin(), main_neurons.end());

	for (int i = 0; i < all_neurons.size(); ++i)
	{
		for (int j = 0; j < all_neurons.size(); ++j)
		{
			Axon* axon = new Axon(Util::Get_Euclidean_distance(all_neurons[i]->position, all_neurons[j]->position));
		}
	}
}
vector<float> Brain::Run(vector<float> input)
{
	for (int i = 0; i < input.size() + maximum_post_steps; ++i)
	{
		float maximum_ms = current_timestamp_ms + step_size_ms;

		for (int j = 0; j < nof_input_neurons; j++)
		{
			// apply new input to input neurons
			input_neurons[j]->Activate(current_timestamp_ms, input[i + j]);
		}
		
		while (!pq.empty())
		{
			// process all neuron firings until the next input is ready
			tuple<float, Axon*> firing = pq.top();
			if (get<0>(firing) <= maximum_ms)
			{
				// process firing if it happened in this interval
				get<1>(firing)->Activate(get<0>(firing));
				pq.pop();
			}
			else
				break;
		}
	}
}