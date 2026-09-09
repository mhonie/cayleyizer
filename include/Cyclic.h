#ifndef CYCLIC_H
#define CYCLIC_H

//
#include "Graph.h"

//
struct Traversal
{
	std::optional<bool> IsCyclic = std::nullopt;

	std::set<char> Visited;

	int VisitedCount() const
	{
		return static_cast<int>(Visited.size());
	}
};

//
class Cyclic
{
public:
	//
	Cyclic();

	Cyclic(Cyclic& original);

	~Cyclic();

	static bool Test(Graph& graph);

private:

	//
	static std::function<Traversal(Traversal isCyclic, std::shared_ptr<Vertex> vertex)>

		UpdateIsCyclic
		(
			COLOR color,

			const std::set<std::shared_ptr<Vertex>>::iterator& first,
			const std::set<std::shared_ptr<Vertex>>::iterator& last,

			int total
		);
};

#endif //CYCLIC_H
