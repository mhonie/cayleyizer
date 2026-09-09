//
#include "Color.h"
#include "Edge.h"
#include "Vertex.h"
#include "Graph.h"
#include "Cyclic.h"

//
#include <numeric>
#include <iostream>

//
Cyclic::Cyclic() = default;

Cyclic::Cyclic(Cyclic& original) = default;

Cyclic::~Cyclic() = default;

//
std::function<Traversal(Traversal isCyclic, std::shared_ptr<Vertex> vertex)>

	Cyclic::UpdateIsCyclic
	(
		COLOR color,

		const std::set<std::shared_ptr<Vertex>>::iterator& first,
		const std::set<std::shared_ptr<Vertex>>::iterator& last,

		int total
	)
	{
		return [color, first, last, total]
		(
			Traversal state,

			const std::shared_ptr<Vertex>& vertex
		)
		{
			if (state.IsCyclic == false)

				return state;

			const std::vector<std::shared_ptr<Edge>> outEdges = 
				
				vertex->OutEdges(color);

			if (static_cast<int>(outEdges.size()) != 1)
			{
				state.IsCyclic = false;
				
				return state;
			}

			const char nextElement = outEdges[0]->Head();

			const int visitedCount = state.VisitedCount();

			if (vertex == *last)
			{
				if (visitedCount != total - 1)
				{
					state.IsCyclic = false;

					return state;
				}

				if (nextElement != (*first)->Element())
				{
					state.IsCyclic = false;

					return state;
				}

				state.IsCyclic = true;

				return state;
			}

			if (state.Visited.count(nextElement) > 0)
			{
				state.IsCyclic = false;

				return state;
			}

			state.Visited.insert(nextElement);

			return state;
		};
	}

//
bool Cyclic::Test(Graph & graph)
{
	if (graph.IsSingleton())

		return true;

	if (graph.EdgeColorCount() != 1)

		return false;

	const int tempVertexCount = graph.VertexCount();

	if (graph.EdgeCount() != tempVertexCount - 1)
	
		return false;

	const COLOR edgeColor = *(graph.EdgeColors().begin());

	std::set<std::shared_ptr<Vertex>> vertices = graph.Vertices();

	const auto firstVertex = std::begin(vertices);

	const auto endOfVertices = std::end(vertices);

	const auto lastVertex = std::prev(endOfVertices);

	const auto updateIsCyclic =

		UpdateIsCyclic
		(
			edgeColor, 
			firstVertex, 
			lastVertex, 
			tempVertexCount
		);

	const auto [isCyclic, _] =

		std::accumulate
		(
			firstVertex,
			endOfVertices,
			Traversal{},
			updateIsCyclic
		);

	if (isCyclic.has_value())

		return isCyclic.value();

	throw std::logic_error("cyclic graph test failed.");
}
