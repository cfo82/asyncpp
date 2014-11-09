
#ifndef MAX_H
#define MAX_H

template <int V0, int V1>
struct Max
{
	static const int Value = V0 > V1 ? V0 : V1;
};

#endif // MAX_H
