// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <string>
#include <sstream>

#include "Axon.h"
#include "Neuron.h"
#include "Brain.h"

using namespace std;

void Get_IO_Identity(vector<float>& input, vector<float>& result, int delay);
void Get_IO_Token_Recognition(vector<float>& input, vector<float>& result);
float Get_Accuracy(vector<float>& output, vector<float>& result);
void Neuroevolution_Algorithm_1(vector<int> & index, vector<Brain*>& brains, vector<float> & temporary_param_v, vector<vector<float>> & params);
void Neuroevolution_Algorithm_2(vector<int> & index, vector<Brain*>& brains, vector<float>& temporary_param_v, vector<vector<float>>& params, vector<float>&rewards);
string Get_String(vector<float> data);

const int population_size = 20;
const int nof_parents = 5;
const int training_iterations = 1000000;
const int input_length = 1000;
const int nof_input_neurons = 3;
const int nof_output_neurons = 3;
const int nof_main_neurons = 8;
const int total_neurons = nof_input_neurons + nof_output_neurons + nof_main_neurons;
const int nof_parameters = 2 * (total_neurons * (total_neurons - 1) - (nof_input_neurons)*(nof_input_neurons-1)/2); // 2 times the number of axons
const float step_size_ms = 100;
const float sigma = 0.001f; // higher neuron count => lower sigma
const float required_accuracy = 0.99f;

/* Random generator for gaussian samples */
random_device rd;
mt19937 gen(rd());
normal_distribution<float> distribution(0.0f, 1.0f);

int main()
{
    vector<float> input(input_length * nof_input_neurons, 0.0f);
    vector<float> result(input_length * nof_output_neurons, 0.0f);
    vector<float> output(input_length * nof_output_neurons, 0.0f);

    vector<float> temporary_param_v(nof_parameters, 0.0f);
    vector<vector<float>> params(population_size, vector<float>(nof_parameters, 0.0f));
    vector<float> rewards(population_size, 0.0f);    
    vector<int> index(rewards.size(), 0);

    vector<Brain*> brains;
    
    int iters_needed = 0;
    int spikes_needed = 0;

    const int nof_test_repetitions = 10;

    for(int test_iter = 0; test_iter < nof_test_repetitions; ++test_iter)
    {
        brains.clear();
        for (int i = 0; i < population_size; ++i)
        {
            brains.push_back(new Brain(nof_input_neurons, nof_output_neurons, nof_main_neurons, step_size_ms));
            brains[i]->Get_Params(params[i]);
        }

        for (int iter = 0; iter < training_iterations; ++iter)
        {
            /* Create a new input sample */
            Get_IO_Identity(input, result, 3);
            //Get_IO_Token_Recognition(input, result);

            /* Create an index for later sorting */
            for (int i = 0; i != index.size(); i++) {
                index[i] = i;
            }

            /* Evaluate all individuals on the new input */
            for (int b = 0; b < brains.size(); ++b)
            {
                fill(output.begin(), output.end(), 0.0f);
                brains[b]->Run(input, output, result, input_length);
                rewards[b] = Get_Accuracy(output, result);
            }

            /* Sort the individuals by performance */
            sort(index.begin(), index.end(),
                [&](const int& a, const int& b) {
                    return (rewards[a] > rewards[b]);
                }
            );

            /* read params out again they can be changed by STDP*/
            for (int i = 0; i < population_size; ++i)
                brains[i]->Get_Params(params[i]);

            Neuroevolution_Algorithm_1(index, brains, temporary_param_v, params);
            //Neuroevolution_Algorithm_2(index, brains, temporary_param_v, params, rewards);

            float sum_rewards = 0.0f;
            for (auto& n : rewards)
                sum_rewards += n;

            // stop and output statistics if accuracy is reached
            if (rewards[index[0]] >= required_accuracy)
            {
                cout << (required_accuracy*100) << "% accuracy at iteration: " << iter << " with " << brains[index[0]]->total_firings << " spikes" << endl;
                cout << "Input:  " << Get_String(input) << endl;
                cout << "Output: " << Get_String(output) << endl;
                cout << "Result: " << Get_String(result) << endl;
                iters_needed += iter;
                spikes_needed += brains[index[0]]->total_firings;
                break;
            }

            // regularly output some statistics
            if (iter % 1 == 0) {
                cout << "Iter: " << iter << ", Best fitness: " << rewards[index[0]] << ", Avg fitness: " << (sum_rewards / population_size) <<
                    ", Spikes: " << brains[index[0]]->total_firings << endl;
            }
        }
    }
    cout << "Avg iterations needed: " << (iters_needed / nof_test_repetitions) << ", Avg spikes produced " << (spikes_needed / nof_test_repetitions) << endl;
}
void Get_IO_Identity(vector<float> & input, vector<float> & result, int delay)
{
    for (int i = 0; i < input.size(); ++i)
    {
        int bit = rand() % 2;
        input[i] = bit;
        if(i + delay < result.size())
            result[i + delay] = bit;
    }
}
void Get_IO_Token_Recognition(vector<float>& input, vector<float>& result)
{
    vector<int> pattern = vector<int>{ 1,1 };
    for (int i = 0; i < input.size(); ++i)
    {
        int bit = rand() % 2;
        input[i] = bit;
        result[i] = 0;
    }
    for (int i = 0; i < input.size() - pattern.size(); ++i)
    {
        int matches = 0;
        for (int j = 0; j < pattern.size(); ++j)
        {
            if (input[i + j] == pattern[j])
                matches++;
        }
        if (matches == pattern.size() && i + pattern.size() < result.size())
        {
            result[i + pattern.size()] = 1;
        }
    }
}
float Get_Accuracy(vector<float>& output, vector<float>& result)
{
    float accuracy = 0.0f;

    for (int i = 0; i < result.size(); ++i)
    {
        if (output[i] == result[i])
            accuracy += 1.0f;
    }
    accuracy /= result.size();

    return accuracy;
}
string Get_String(vector<float> data)
{
    stringstream result;
    std::copy(data.begin(), data.begin()+60, ostream_iterator<float>(result, ""));
    return result.str();
}
void Neuroevolution_Algorithm_1(vector<int>& index, vector<Brain*>& brains, vector<float>& temporary_param_v, vector<vector<float>>& params)
{
    /* Create slightly permutated children from the top individuals */
    // one individual gets the best paramaters again
    brains[index[0]]->Set_Params(params[index[0]]);

    for (int i = 1; i < population_size; ++i)
    {
        for (int j = 0; j < nof_parameters; ++j)
        {
            // permutate params
            temporary_param_v[j] = params[index[(i - 1) % nof_parents]][j] + sigma * distribution(gen);
        }
        // apply params
        brains[index[i]]->Set_Params(temporary_param_v);
    }

    // read params again into param vector
    for (int i = 0; i < population_size; ++i)
    {
        brains[i]->Get_Params(params[i]);
    }
}
void Neuroevolution_Algorithm_2(vector<int>& index, vector<Brain*>& brains, vector<float>& temporary_param_v, vector<vector<float>>& params, vector<float>& rewards)
{
    /*create a new parameter set by weighing the params by their rewards */
    /*only use top 50% of individuals */

    float sum_reward = 0.0f;
    vector<float> rewards_normalized = vector<float>(population_size, 0.0f);
    vector<float> temporary_param_v_2(nof_parameters, 0.0f);

    for (int i = 0; i < population_size; ++i)
    {
        sum_reward += rewards[index[i]];
    }
    for (int i = 0; i < population_size/2; ++i)
    {
        if(sum_reward != 0.0f)
            rewards_normalized[index[i]] = rewards[index[i]]/sum_reward;
    }

    fill(temporary_param_v.begin(), temporary_param_v.end(), 0.0f);

    for (int i = 0; i < population_size; ++i)
    {
        for (int j = 0; j < nof_parameters; ++j)
        {
            // permutate params
            temporary_param_v[j] += params[index[i]][j] * rewards_normalized[index[i]];
        }
    }
    for (int i = 0; i < population_size; ++i)
    {
        for (int j = 0; j < nof_parameters; ++j)
        {
            temporary_param_v_2[j] = temporary_param_v[j] + sigma * distribution(gen);
        }
        // apply params
        brains[index[i]]->Set_Params(temporary_param_v_2);
    }
}
