#include "Brain.h"

Brain::Brain()
{

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