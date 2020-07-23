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
    vector<float> input, result;
    vector<float> output(100 * 3, 0.0f);

    Get_Input_Output_Identity(input, result, 100 * 3);

    Brain * brain = new Brain();
    brain->Run(input, output, 100);
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
    return 0.0f;
}