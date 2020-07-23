#pragma once

#include <tuple>

class Neuron;

using namespace std; 

class Axon
{
public:
	Axon(float _length);
	void Activate(float timestamp);

private:
	float weight;
	float length;
	tuple<Neuron*, Neuron*> connecting;
};

struct AxonFiringComparator {
    bool operator()(const tuple<float, Axon*> &a1, const tuple<float, Axon*> &a2)
    {
        return get<0>(a1) < get<0>(a2);
    }
};