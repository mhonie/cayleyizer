//
#include "Edge.h"
#include "Graph.h"
#include "Vertex.h"
#include "GraphBuilder.h"

//
#include <algorithm>
#include <iterator>
#include <stdexcept>

//
GraphBuilder::GraphBuilder() = default;

//
GraphBuilder::GraphBuilder(const GraphBuilder& original) = default;

//
GraphBuilder::~GraphBuilder() = default;

//
namespace 
{
	//
	auto createVertex(const std::vector<std::shared_ptr<Edge>>& edges)
	{
		return [edges](const char element)
		{
			const std::vector<std::shared_ptr<Edge>> incidents = 
				
				Edge::Incidents(element, edges);

			return Vertex(element, incidents);
		};
	}

	//
	std::vector<Vertex> getVertices
	(
		const std::vector<std::shared_ptr<Edge>>& edges
	)
	{
		std::set<char> elements = Edge::GetElementsPtr(edges);

		std::vector<Vertex> vertices;

		std::transform
		(
			std::begin(elements),
			std::end(elements),
			std::inserter(vertices, vertices.end()),
			createVertex(edges)
		);

		return vertices;
	}

	// 
	auto isOuterEdge(const std::set<char>& neighbors)
	{
		return [neighbors](const std::shared_ptr<Edge>& edge)
		{
			const char head = edge->Head();
			const char tail = edge->Tail();

			const bool isOuterEdge =

				neighbors.count(head) > 0 && neighbors.count(tail) > 0;

			return isOuterEdge;
		};
	}

	//
	auto createNeighborhood(const std::vector<std::shared_ptr<Edge>>& edges)
	{
		return [edges](const std::shared_ptr<Vertex>& vertex)
		{
			const std::set<char> neighbors = vertex->Neighbors();

			std::vector<std::shared_ptr<Edge>> outerEdges;

			std::copy_if
			(
				edges.begin(),
				edges.end(),
				back_inserter(outerEdges),
				isOuterEdge(neighbors)
			);

			return std::make_shared<Neighborhood>(vertex, outerEdges);
		};
	}
}

//
Graph GraphBuilder::Build(const std::vector<Edge>& edges)
{
	std::vector<std::shared_ptr<Edge>> sharedEdges;

	std::transform
	(
		edges.begin(),
		edges.end(),
		std::back_inserter(sharedEdges),
		Edge::CreatePtr
	);

	std::vector<Vertex> vertices = getVertices(sharedEdges);

	std::vector<std::shared_ptr<Vertex>> sharedVertices;

	std::transform
	(
		vertices.begin(),
		vertices.end(),
		std::back_inserter(sharedVertices),
		Vertex::CreatePtr
	);

	std::vector<std::shared_ptr<Neighborhood>> neighborhoods;

	std::transform
	(
		sharedVertices.begin(),
		sharedVertices.end(),
		std::back_inserter(neighborhoods),
		createNeighborhood(sharedEdges)
	);

	return Graph(neighborhoods);
}

//
namespace
{
	//
	bool consistencyCheck
	(
		const std::vector<Vertex>& connectedVertices,

		const std::vector<Vertex>& disconnectedVertices
	)
	{
		std::vector<Vertex> inconsistent;

		std::set_intersection
		(
			connectedVertices.begin(),
			connectedVertices.end(),
			disconnectedVertices.begin(),
			disconnectedVertices.end(),
			std::back_inserter(inconsistent)
		);

		if (!inconsistent.empty())

			return false;

		const bool misconnected =

			std::any_of
			(
				disconnectedVertices.begin(),
				disconnectedVertices.end(),
				Vertex::GetIsConnected
			);

		return !misconnected;
	}
}

//
Graph GraphBuilder::Build
(
	const std::vector<Edge>& edges,

	const std::set<char>& disconnected
)
{
	std::vector<std::shared_ptr<Edge>> sharedEdges;

	std::transform
	(
		edges.begin(),
		edges.end(),
		std::back_inserter(sharedEdges),
		Edge::CreatePtr
	);

	std::vector<Vertex> connectedVertices = getVertices(sharedEdges);

	std::vector<Vertex> disconnectedVertices;

	std::transform
	(
		std::begin(disconnected),
		std::end(disconnected),
		std::back_inserter(disconnectedVertices),
		Vertex::CreateDisconnected
	);

	const bool consistent = 
		
		consistencyCheck(connectedVertices, disconnectedVertices);

	if (!consistent) 
		
		throw std::logic_error("inconsistent graph");

	std::vector<Vertex> vertices;

	std::set_union
	(
		connectedVertices.begin(),
		connectedVertices.end(),
		disconnectedVertices.begin(),
		disconnectedVertices.end(),
		std::back_inserter(vertices)
	);

	std::vector<std::shared_ptr<Vertex>> sharedVertices;

	std::transform
	(
		vertices.begin(),
		vertices.end(),
		std::back_inserter(sharedVertices),
		Vertex::CreatePtr
	);

	std::vector<std::shared_ptr<Neighborhood>> neighborhoods;

	std::transform
	(
		sharedVertices.begin(),
		sharedVertices.end(),
		std::back_inserter(neighborhoods),
		createNeighborhood(sharedEdges)
	);

	auto graph = Graph(neighborhoods);

	return graph;
}
