#include "Neuron.h"

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