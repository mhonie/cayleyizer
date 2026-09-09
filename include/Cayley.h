#ifndef CAYLEY_H
#define CAYLEY_H

//
#include "Graph.h"

//
class Cayley
{
public:
	//
	Cayley();

	Cayley(Cayley& original);

	~Cayley();

	//
	static std::optional<bool> Test(Graph& graph);

};

#endif //CAYLEY_H
