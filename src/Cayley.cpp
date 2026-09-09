//
#include "Cayley.h"
#include "Graph.h"
#include "Homogeneous.h"

//
Cayley::Cayley() = default;

Cayley::Cayley(Cayley& original) = default;

Cayley::~Cayley() = default;

//
std::optional<bool> Cayley::Test(Graph& graph)
{
	if (graph.IsConnected() && graph.IsRegular())
	{
		if (Homogeneous::Test(graph))

			return std::nullopt;

		return false;
	}
		
	return false;
}