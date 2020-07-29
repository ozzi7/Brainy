#include "Axon.h"
#include "Neuron.h"

Axon::Axon(float _length)
{
	length = _length;
	SetWeight(-0.01f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.02f))));
}
void Axon::Activate(float timestamp)
{
	get<1>(connecting)->Activate(this, timestamp, weight);
}
void Axon::SetWeight(float new_weight)
{
	weight = min(1.0f, max(-1.0f, new_weight));
}
void Axon::SetLength(float new_length)
{
	length = min(100.0f, max(0.0f, new_length));
}