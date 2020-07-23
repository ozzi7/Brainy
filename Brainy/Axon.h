#pragma once

#include <tuple>

class Neuron;

using namespace std; 

class Axon
{
public:
	Axon();
	void Activate(float timestamp);

private:
	float weight;
	float length;
	tuple<Neuron*, Neuron*> connecting;
};

struct CompareAxonFiring {
    bool operator()(const tuple<float, Axon*> &a1, const tuple<float, Axon*> &a2)
    {
        // return "true" if "p1" is ordered  
        // before "p2", for example: 
        return get<0>(a1) < get<0>(a2);
    }
};