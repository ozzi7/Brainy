#pragma once

#include <tuple>

using namespace std;

class Util
{
public:
	static float Get_Euclidean_distance(tuple<float, float, float> a, tuple<float, float, float> b)
	{
		return (float)sqrt(pow(get<0>(b) - get<0>(a), 2) +
			pow(get<1>(b) - get<1>(a), 2) +
			pow(get<2>(b) - get<2>(a), 2));
	}
};