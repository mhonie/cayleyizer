#ifndef GRAPH_UPDATER_H
#define GRAPH_UPDATER_H

//
#include "Graph.h"

//
class GraphUpdater
{
public:
	//
	GraphUpdater();

	explicit GraphUpdater(const Graph& graph);

	GraphUpdater(const GraphUpdater& original);

	~GraphUpdater();

	//
	Graph AddEdge(const Edge& edge);

	Graph AddEdges(const std::vector<Edge>& edges);

	Graph DeleteEdge(const std::shared_ptr<Edge>& edge);

	Graph DeleteEdges(const std::vector<std::shared_ptr<Edge>>& edges);

	Graph FlipEdge(const std::shared_ptr<Edge>& edge);

	Graph FlipEdges(const std::vector<std::shared_ptr<Edge>>& edges);

	//
	Graph AddVertex(char element);

	Graph AddVertices(const std::set<char>& elements);

	Graph DeleteVertex(const std::shared_ptr<Vertex>& vertex);

	Graph DeleteVertices(const std::vector<std::shared_ptr<Vertex>>& vertices);

private:

	Graph graph;	
};

#endif //GRAPH_UPDATER_H
