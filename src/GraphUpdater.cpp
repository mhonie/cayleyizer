//
#include "GraphBuilder.h"
#include "GraphUpdater.h"

//
#include <algorithm>
#include <iostream>
#include <iterator>

//
GraphUpdater::GraphUpdater() : graph(Graph()) { }

//
GraphUpdater::GraphUpdater(const Graph& graph) : graph(graph) { }

//
GraphUpdater::GraphUpdater(const GraphUpdater& original) = default;

//
GraphUpdater::~GraphUpdater() = default;

namespace 
{
	//
	std::set<char> getDisconnectedElements
	(
		std::vector<Vertex> vertices,

		const std::vector<Edge>& edges
	)
	{
		std::set<char> connectedElements = Edge::GetElements(edges);

		std::set<char> allElements;

		std::transform
		(
			vertices.begin(),
			vertices.end(),
			std::inserter(allElements, std::begin(allElements)),
			Vertex::GetElement
		);

		std::set<char> disconnectedElements;

		std::set_difference
		(
			std::begin(allElements),
			std::end(allElements),
			std::begin(connectedElements),
			std::end(connectedElements),
			inserter
			(
				disconnectedElements, 
				std::end(disconnectedElements)
			)
		);

		return disconnectedElements;
	}
}

//
Graph GraphUpdater::AddEdge(const Edge& edge)
{
	std::set<std::shared_ptr<Edge>> oldEdges = graph.Edges();

	std::vector<Edge> newEdges;

	std::transform
	(
		std::begin(oldEdges),
		std::end(oldEdges),
		std::back_inserter(newEdges),
		Edge::CopyPtr
	);

	newEdges.insert(newEdges.end(), edge);

	const std::set<std::shared_ptr<Vertex>>	oldVertices = graph.Vertices();

	std::vector<Vertex> oldVerticesCopy;

	std::transform
	(
		std::begin(oldVertices),
		std::end(oldVertices),
		std::back_inserter(oldVerticesCopy),
		Vertex::CopyPtr
	);

	const std::set<char> disconnected = 
		
		getDisconnectedElements(oldVerticesCopy, newEdges);

	const Graph newGraph = 
		
		GraphBuilder::Build(newEdges, disconnected);

	return newGraph;
}

//
Graph GraphUpdater::AddEdges(const std::vector<Edge>& edges)
{
    Graph newGraph = graph;

    for(const Edge& edge : edges)
	
        newGraph = GraphUpdater(newGraph).AddEdge(edge);

    return newGraph;
}

//
namespace 
{
	//
	auto removeIncident(const std::shared_ptr<Edge>& edge)
	{
		return [edge](const std::shared_ptr<Vertex>& oldVertex)
		{
			const char element = oldVertex->Element();

			if (edge->IsIncident(element))
			{
				std::vector<std::shared_ptr<Edge>> edges = oldVertex->Edges();

				const auto iterator =

					std::find_if(edges.begin(), edges.end(), Edge::GetEquals(edge));

				if (iterator != edges.end())
				{
					edges.erase(iterator);

					auto newVertex = std::make_shared<Vertex>(element, edges);

					return newVertex;
				}

				return oldVertex;
			}

			return oldVertex;
		};
	}
}

//
Graph GraphUpdater::DeleteEdge(const std::shared_ptr<Edge>& edge)
{
	std::set<std::shared_ptr<Edge>> oldEdges = graph.Edges();

	const auto iterator =

		std::find_if
		(
			std::begin(oldEdges),
			std::end(oldEdges),
			Edge::GetEquals(edge)
		);

	if (iterator == oldEdges.end())

		throw std::logic_error("edge does not exist.");

	oldEdges.erase(iterator);

	std::vector<Edge> newEdges;

	std::transform
	(
		std::begin(oldEdges),
		std::end(oldEdges),
		std::back_inserter(newEdges),
		Edge::CopyPtr
	);

	const std::set<std::shared_ptr<Vertex>>	oldVertices = graph.Vertices();

	std::vector<std::shared_ptr<Vertex>> newVertices;

	std::transform
	(
		std::begin(oldVertices),
		std::end(oldVertices),
		std::back_inserter(newVertices),
		removeIncident(edge)
	);

	std::vector<Vertex> newVerticesCopy;

	std::transform
	(
		newVertices.begin(),
		newVertices.end(),
		std::back_inserter(newVerticesCopy),
		Vertex::CopyPtr
	);

	const std::set<char> disconnectedElements = 
		
		getDisconnectedElements(newVerticesCopy, newEdges);

	const Graph newGraph = 
		
		GraphBuilder::Build(newEdges, disconnectedElements);

	return newGraph;
}

//
Graph GraphUpdater::DeleteEdges(const std::vector<std::shared_ptr<Edge>>& edges)
{
	Graph newGraph = graph;

	for (const std::shared_ptr<Edge>& edge : edges)

		newGraph = GraphUpdater(newGraph).DeleteEdge(edge);

	return newGraph;
}

//
Graph GraphUpdater::FlipEdge(const std::shared_ptr<Edge>& edge)
{
	std::set<std::shared_ptr<Edge>> oldEdges = graph.Edges();

	const auto iterator =

		std::find_if
		(
			std::begin(oldEdges),
			std::end(oldEdges),
			Edge::GetEquals(edge)
		);

	if (iterator == oldEdges.end())

		throw std::logic_error("edge does not exist.");

	oldEdges.erase(edge);

	std::vector<Edge> newEdges;

	std::transform
	(
		std::begin(oldEdges),
		std::end(oldEdges),
		std::back_inserter(newEdges),
		Edge::CopyPtr
	);

	const Edge flippedEdge = Edge::Flip(edge);

	newEdges.push_back(flippedEdge);

	const std::set<std::shared_ptr<Vertex>>	oldVertices = graph.Vertices();

	std::vector<Vertex> oldVerticesCopy;

	std::transform
	(
		std::begin(oldVertices),
		std::end(oldVertices),
		std::back_inserter(oldVerticesCopy),
		Vertex::CopyPtr
	);

	const std::set<char> disconnectedElements =
		
		getDisconnectedElements(oldVerticesCopy, newEdges);

	const Graph newGraph =

		GraphBuilder::Build(newEdges, disconnectedElements);

	return newGraph;
}

//
Graph GraphUpdater::FlipEdges(const std::vector<std::shared_ptr<Edge>>& edges)
{
	Graph newGraph = graph;

	for (const std::shared_ptr<Edge>& edge : edges)

		newGraph = GraphUpdater(newGraph).FlipEdge(edge);

	return newGraph;
}

//
namespace 
{
	//
	std::vector<std::shared_ptr<Vertex>> getDisconnectedVertices(Graph graph)
	{
		std::set<std::shared_ptr<Vertex>> oldVertices =
			
			graph.Vertices();

		std::vector<std::shared_ptr<Vertex>> disconnected;

		std::copy_if
		(
			std::begin(oldVertices),
			std::end(oldVertices),
			inserter(disconnected, disconnected.end()),
			Vertex::GetPtrIsDisconnected
		);

		return disconnected;
	}
}

//
Graph GraphUpdater::AddVertex(const char element)
{
	const std::set<std::shared_ptr<Edge>> oldEdges = graph.Edges();

	std::vector<Edge> oldEdgesCopy;

	std::transform
	(
		std::begin(oldEdges),
		std::end(oldEdges),
		std::back_inserter(oldEdgesCopy),
		Edge::CopyPtr
	);

	std::vector<std::shared_ptr<Vertex>>

		disconnectedVertices = getDisconnectedVertices(graph);

	std::set<char> disconnectedElements;

	std::transform
	(
		disconnectedVertices.begin(),
		disconnectedVertices.end(),
		inserter
		(
			disconnectedElements, 
			std::end(disconnectedElements)
		),
		Vertex::GetPtrElement
	);

	disconnectedElements.insert(element);

	const Graph newGraph =

		GraphBuilder::Build(oldEdgesCopy, disconnectedElements);

	return newGraph;
}

//
Graph GraphUpdater::AddVertices(const std::set<char>& elements)
{
    Graph newGraph = graph;

    for (const char element : elements)

        newGraph = GraphUpdater(newGraph).AddVertex(element);

    return newGraph;
}

//
Graph GraphUpdater::DeleteVertex(const std::shared_ptr<Vertex>& vertex)
{
	std::set<std::shared_ptr<Edge>> oldEdges = graph.Edges();

	std::vector<std::shared_ptr<Edge>> newEdges;

	const char element = vertex->Element();

	std::copy_if
	(
		std::begin(oldEdges),
		std::end(oldEdges),
		back_inserter(newEdges),
		Edge::GetNotIncident(element)
	);

	std::vector<Edge> newEdgesCopy;

	std::transform
	(
		newEdges.begin(),
		newEdges.end(),
		std::back_inserter(newEdgesCopy),
		Edge::CopyPtr
	);

	std::set<std::shared_ptr<Vertex>> oldVertices = graph.Vertices();

	const auto iterator = 
		
		std::find_if
		(
			std::begin(oldVertices),
			std::end(oldVertices),
			Vertex::GetIsEquivalent(vertex)
		);

	if (iterator != oldVertices.end())

		oldVertices.erase(iterator);

	std::vector<Vertex> newVertices;

	std::transform
	(
		std::begin(oldVertices),
		std::end(oldVertices),
		std::back_inserter(newVertices),
		Vertex::CopyPtr
	);

	const std::set<char> disconnectedElements =

		getDisconnectedElements(newVertices, newEdgesCopy);

	const Graph newGraph =

		GraphBuilder::Build(newEdgesCopy, disconnectedElements);

	return newGraph;
}

//
Graph GraphUpdater::DeleteVertices(const std::vector<std::shared_ptr<Vertex>>& vertices)
{
	Graph newGraph = graph;

	for (const std::shared_ptr<Vertex>& vertex : vertices)

		newGraph = GraphUpdater(newGraph).DeleteVertex(vertex);

	return newGraph;
}
