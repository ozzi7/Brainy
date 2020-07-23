#include "Axon.h"

Axon::Axon()
{

}
void Axon::Activate(float timestamp)
{
	get<0>(connecting)->Activate(timestamp, weight);
}
