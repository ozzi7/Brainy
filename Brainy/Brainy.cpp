// Brainy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Axon.h"
#include "Neuron.h"
#include "Brain.h"

using namespace std;
void Get_Input_Output_Identity(vector<float>& input, vector<float>& result, int length);
float Get_Accuracy_Identity(vector<float>& output, vector<float>& result);

int main()
{
    const int input_length = 200;
    const int nof_input_neurons = 3;
    const int nof_output_neurons = 3;
    const int nof_main_neurons = 4;
    const float step_size_ms = 100;
    const float sigma = 0.01f;
    vector<float> input, result;


    vector<float> output(input_length * nof_output_neurons, 0.0f);

    Get_Input_Output_Identity(input, result, input_length * nof_input_neurons);

    Brain * brain = new Brain(nof_input_neurons, nof_output_neurons, nof_main_neurons, step_size_ms);
    brain->Run(input, output, input_length);
}
void Get_Input_Output_Identity(vector<float> & input, vector<float> & result, int length)
{
    for (int i = 0; i < length; ++i)
    {
        int bit = rand() % 2;
        input.push_back(bit);
        result.push_back(bit);
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