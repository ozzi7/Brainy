#include "Axon.h"
#include "Neuron.h"

Axon::Axon(float _length)
{
	length = _length;
	weight = -0.01f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.02f)));
}
void Axon::Activate(float timestamp)
{
	get<1>(connecting)->Activate(timestamp, weight);
}
