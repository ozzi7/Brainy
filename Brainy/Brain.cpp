#include "Brain.h"
#include "Util.h"


Brain::Brain(int _nof_input_neurons, int _nof_output_neurons, int _nof_main_neurons, float _step_size_ms)
{
	pq = new priority_queue<tuple<float, Axon*>, vector<tuple<float, Axon*>>, AxonFiringComparator>();

	nof_input_neurons = _nof_input_neurons;
	nof_output_neurons = _nof_output_neurons;
	nof_main_neurons = _nof_main_neurons;
	step_size_ms = _step_size_ms;

	// currently constructs a fully connected graph
	for (int i = 0; i < nof_input_neurons; ++i)
	{
		input_neurons.push_back(new Neuron(pq));
	}	
	for (int i = 0; i < nof_output_neurons; ++i)
	{
		output_neurons.push_back(new Neuron(pq, i));
	}
	for (int i = 0; i < nof_main_neurons; ++i)
	{
		main_neurons.push_back(new Neuron(pq));
	}

	// create axons between all neurons
	all_neurons.reserve(input_neurons.size() + output_neurons.size() + main_neurons.size());
	all_neurons.insert(all_neurons.end(), input_neurons.begin(), input_neurons.end());
	all_neurons.insert(all_neurons.end(), output_neurons.begin(), output_neurons.end());
	all_neurons.insert(all_neurons.end(), main_neurons.begin(), main_neurons.end());

	for (int i = 0; i < all_neurons.size(); ++i)
	{
		for (int j = 0; j < all_neurons.size(); ++j)
		{
			if (i != j) // disallow self-loops for now
			{
				Axon* axon = new Axon(0.1f);

				axon->connecting = make_tuple(all_neurons[i], all_neurons[j]);

				all_neurons[i]->Add_Output(axon);
				all_axons.push_back(axon);
			}
		}
	}
}
/// <summary>
/// Runs the brain for <max_steps> steps with the given input and records the output
/// </summary>
/// <param name="input"></param>
/// <param name="output">Output vector must already be zeroed out</param>
/// <param name="max_steps">Total number of steps the brain should run</param>
void Brain::Run(vector<float> &input, vector<float> &output, int max_steps)
{
	reset();

	for (int i = 0; i < max_steps; ++i)
	{
		float maximum_ms = current_timestamp_ms + step_size_ms;

		for (int j = 0; j < nof_input_neurons; j++)
		{
			// apply new input to input neurons
			input_neurons[j]->Activate(current_timestamp_ms, input[i*nof_input_neurons + j]);
		}
		
		while (!pq->empty())
		{
			// process all neuron firings until the next input is ready
			tuple<float, Axon*> firing = pq->top();
			if (get<0>(firing) <= maximum_ms)
			{
				// process firing if it happened in this interval
				get<1>(firing)->Activate(get<0>(firing));

				pq->pop();
			}
			else
				break;
		}

		/* record output neuron firings */
		for (int j = 0; j < nof_output_neurons; ++j)
		{
			output[i * nof_output_neurons + output_neurons[j]->output_id] = output_neurons[j]->nof_firings;
			total_firings += output_neurons[j]->nof_firings;
			output_neurons[j]->nof_firings = 0.0f;
		}
		current_timestamp_ms += step_size_ms;
	}

	for (int i = 0; i < all_neurons.size(); ++i)
	{
		total_firings += all_neurons[i]->nof_firings;
	}
}
void Brain::reset()
{
	current_timestamp_ms = 0.0f;
	total_firings = 0;

	// clear the queue for next Run call 
	while (!pq->empty())
	{
		pq->pop();
	}

	for (auto neuron : all_neurons)
	{
		neuron->Reset();
	}
}
void Brain::Get_Params(vector<float>& params)
{
	for (int i = 0; i < all_axons.size(); i+=1)
	{
		params[i*2] = all_axons[i]->length;
		params[i*2+1] = all_axons[i]->weight;
	}
}
void Brain::Set_Params(vector<float>& params)
{
	for (int i = 0; i < all_axons.size(); i += 1)
	{
		all_axons[i]->length = min(1.0f, max(0.0f, params[i * 2])); // [0,1]
		all_axons[i]->weight = min(1.0f, max(-1.0f, params[i * 2 + 1])); // [-1,1]
	}
}