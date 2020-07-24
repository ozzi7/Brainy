// Brainy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>

#include "Axon.h"
#include "Neuron.h"
#include "Brain.h"

using namespace std;

void Get_Input_Output_Identity(vector<float>& input, vector<float>& result);
float Get_Accuracy_Identity(vector<float>& output, vector<float>& result);

int main()
{
    const int population_size = 20;
    const int nof_parents = 5;
    const int training_iterations = 1000000;
    const int input_length = 200;
    const int nof_input_neurons = 3;
    const int nof_output_neurons = 3;
    const int nof_main_neurons = 14;
    const int total_neurons = nof_input_neurons + nof_output_neurons + nof_main_neurons;
    const float step_size_ms = 100;
    const float sigma = 0.01f;

    vector<float> input(input_length * nof_input_neurons, 0.0f);
    vector<float> result(input_length * nof_output_neurons, 0.0f);
    vector<float> output(input_length * nof_output_neurons, 0.0f);

    vector<vector<float>> params(population_size, vector<float>(2*total_neurons* total_neurons,0.0f));
    vector<float> rewards(population_size, 0.0f);    
    vector<int> index(rewards.size(), 0);
    vector<Brain*> brains;
    
    for (int i = 0; i < population_size; ++i)
    {
        brains.push_back(new Brain(nof_input_neurons, nof_output_neurons, nof_main_neurons, step_size_ms));
        brains[i]->Get_Params(params[i]);
    }

    /* Random generator for gaussian samples */
    std::random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> distribution(0.0f, 1.0f);

    vector<float> temporary_param_v(2 * total_neurons * total_neurons, 0.0f);

    for (int iter = 0; iter < training_iterations; ++iter)
    {
        /* Create a new input sample */
        Get_Input_Output_Identity(input, result);

        /* Create an index for later sorting */
        for (int i = 0; i != index.size(); i++) {
            index[i] = i;
        }

        /* Evaluate all individuals on the new input */
        for (int b = 0; b < brains.size(); ++b)
        {
            fill(output.begin(), output.end(), 0.0f);
            brains[b]->Run(input, output, input_length);
            rewards[b] = Get_Accuracy_Identity(output, result);
        }

        /* Sort the individuals by performance */
        sort(index.begin(), index.end(),
            [&](const float& a, const float& b) {
                return (rewards[a] > rewards[b]);
            }
        );

        /* Create slightly permutated children from the top individuals */
        // one individual gets the best paramaters again
        brains[0]->Set_Params(params[index[0]]);

        for (int i = 1; i < population_size; ++i)
        {
            for (int j = 0; j < params[index[(i-1) % nof_parents]].size(); ++j)
            {
                // permutate params
                temporary_param_v[j] = params[index[(i - 1) % nof_parents]][j] + sigma * distribution(gen);
            }
            // apply params
            brains[i]->Set_Params(temporary_param_v);
        }

        // read params again into param vector
        for (int i = 0; i < population_size; ++i)
        {
             brains[i]->Get_Params(params[i]);
        }

        if (iter % 100 == 0) {
            float sum_rewards = 0.0f;
            for (auto& n : rewards)
                sum_rewards += n;
            cout << "Iter: " << iter << ", Best fitness: " << rewards[index[0]] << ", Avg fitness: " << (sum_rewards / population_size) << endl;
        }
    }
}
void Get_Input_Output_Identity(vector<float> & input, vector<float> & result)
{
    for (int i = 0; i < input.size(); ++i)
    {
        int bit = rand() % 2;
        input[i] = bit;
        result[i] = bit;
    }
}
float Get_Accuracy_Identity(vector<float>& output, vector<float>& result)
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