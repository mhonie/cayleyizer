#ifndef GRAPH_H
#define GRAPH_H

//
#include "Neighborhood.h"

//
typedef std::map<char, IRREGULARITY> IRREGULARITIES;

//
typedef std::map<std::pair<char, char>, INNER_DIFFERENCE> INNER_DIFFERENCES;

//
typedef std::map<std::pair<char, char>, std::vector<Edge>> OUTER_DIFFERENCES;

//
class Graph
{
public:
	//
	Graph();

	explicit Graph(const std::vector<std::shared_ptr<Neighborhood>>& neighborhoods);

	Graph(const Graph& otherGraph);

	~Graph();

	//
	std::set<std::shared_ptr<Vertex>> Vertices();

	int VertexCount();

	//
	std::set<std::shared_ptr<Edge>> Edges();

	int EdgeCount();

	//
	std::set<COLOR> EdgeColors();

	int EdgeColorCount();

	//
	std::vector<std::shared_ptr<Neighborhood>> Neighborhoods() const;

	std::shared_ptr<Neighborhood> NeighborhoodOf(char element) const;

	//
	static std::function<std::shared_ptr<Neighborhood>(char element)>

		GetNeighborhoodsOf(const Graph& graph);

	//
	int NeighborhoodCount() const;

	//
	bool IsEmpty() const;

	bool IsSingleton();

	//
	std::set<std::shared_ptr<Vertex>> ConnectedVertices();

	std::set<std::shared_ptr<Vertex>> DisconnectedVertices();

	//
	std::optional<bool> IsConnected();

	//
	std::optional<bool> IsRegular();

	//
	IRREGULARITIES	Irregularities();

	//
	std::set<std::shared_ptr<Vertex>> RegularVertices();

	std::set<std::shared_ptr<Vertex>> IrregularVertices();

	//
	INNER_DIFFERENCES InnerForwardDifferences();

	//
	INNER_DIFFERENCES InnerReverseDifferences();

	//
	OUTER_DIFFERENCES OuterForwardDifferences();

	//
	OUTER_DIFFERENCES OuterReverseDifferences();

private:
	//
	std::set<std::shared_ptr<Vertex>> LazyVertices();

	std::optional<std::set<std::shared_ptr<Vertex>>> vertices;

	//
	int LazyVertexCount();

	std::optional<int> vertexCount;

	//
	std::set<std::shared_ptr<Edge>> LazyEdges();

	std::optional<std::set<std::shared_ptr<Edge>>> edges;

	//
	int LazyEdgeCount();

	std::optional<int> edgeCount;

	//
	std::set<COLOR> LazyEdgeColors();

	std::optional<std::set<COLOR>> edgeColors;

	//
	int LazyEdgeColorCount();

	std::optional<int> colorCount;

	//
	std::vector<std::shared_ptr<Neighborhood>> neighborhoods;

	int neighborhoodCount;

	//
	std::map<std::shared_ptr<Vertex>, bool> LazyConnectivities();

	std::optional<std::map<std::shared_ptr<Vertex>, bool>> connectivities;

	//
	std::set<std::shared_ptr<Vertex>> LazyConnectedVertices();

	std::optional<std::set<std::shared_ptr<Vertex>>> connectedVertices;

	//
	std::set<std::shared_ptr<Vertex>> LazyDisconnectedVertices();

	std::optional<std::set<std::shared_ptr<Vertex>>> disconnectedVertices;

	//
	std::optional<bool> LazyIsConnected();

	std::optional<bool> isConnected;

	//
	std::map<std::shared_ptr<Vertex>, bool> LazyRegularities();

	std::optional<std::map<std::shared_ptr<Vertex>, bool>> regularities;

	//
	std::set<std::shared_ptr<Vertex>> LazyRegularVertices();

	std::optional<std::set<std::shared_ptr<Vertex>>> regularVertices;

	//
	std::set<std::shared_ptr<Vertex>> LazyIrregularVertices();

	std::optional<std::set<std::shared_ptr<Vertex>>> irregularVertices;

	//
	std::optional<bool> LazyIsRegular();

	std::optional<bool> isRegular;

	//
	IRREGULARITIES	LazyIrregularities();

	std::optional<IRREGULARITIES> irregularities;

	//
	bool isEmpty;

	//
	bool LazyIsSingleton();

	std::optional<bool> isSingleton;

	//
	INNER_DIFFERENCES LazyInnerForwardDifferences();

	std::optional<INNER_DIFFERENCES> innerForwardDifferences;

	//
	INNER_DIFFERENCES LazyInnerReverseDifferences();

	std::optional<INNER_DIFFERENCES> innerReverseDifferences;

	//
	OUTER_DIFFERENCES LazyOuterForwardDifferences();

	std::optional<OUTER_DIFFERENCES> outerForwardDifferences;

	//
	OUTER_DIFFERENCES LazyOuterReverseDifferences();

	std::optional<OUTER_DIFFERENCES> outerReverseDifferences;

	//
	OUTER_DIFFERENCES LazyOuterDifferences();

	std::optional<OUTER_DIFFERENCES> outerDifferences;
};

#endif //GRAPH_H
