#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

//
#include "Edge.h"
#include "Vertex.h"

//
class Neighborhood
{
public:
	//
	Neighborhood();

	//
	Neighborhood
	(
		const std::shared_ptr<Vertex>& center, 
		
		const std::vector<std::shared_ptr<Edge>>& outerEdges
	);

	Neighborhood(const Neighborhood& original);

	~Neighborhood();

	//
	bool operator== (const Neighborhood& leftNeighborhood) const;

	bool operator< (const Neighborhood& leftNeighborhood) const;

	//
	std::shared_ptr<Vertex> Center() const;

	char CenterElement() const;

	//
	static std::function<std::shared_ptr<Vertex>(std::shared_ptr<Neighborhood>)>

		GetCenter;

	//
	std::vector<std::shared_ptr<Edge>> Edges() const;

	std::vector<std::shared_ptr<Edge>> Edges(COLOR color);

	EDGES_BY_COLOR EdgesByColor();

	//
	int EdgeCount() const;

	//
	std::set<char> Neighbors();

	std::set<char> Vertices() const;

	//
	int VertexCount() const;

	//
	std::set<COLOR> EdgeColors();

	int EdgeColorCount();

	//
	std::vector<std::shared_ptr<Edge>> InnerEdges() const;

	std::vector<std::shared_ptr<Edge>> InnerEdges(COLOR color);

	//
	int InnerEdgeCount() const;

	//
	std::set<COLOR> InnerEdgeColors();

	int InnerEdgeColorCount() const;

	//
	std::vector<std::shared_ptr<Edge>> OuterEdges() const;

	std::vector<std::shared_ptr<Edge>> OuterEdges(COLOR color);

	//
	int OuterEdgeCount() const;

	//
	std::set<COLOR> OuterEdgeColors();

	int OuterEdgeColorCount() const;

	//
	std::multiset<DEGREES_BY_COLOR> DegreeSequences();

	//
	bool IsSimilar(const std::shared_ptr<Neighborhood>& otherNeighborhood);

	//
	INNER_DIFFERENCE InnerForwardDifference(const std::shared_ptr<Neighborhood>& otherNeighborhood) const;

	INNER_DIFFERENCE InnerReverseDifference(const std::shared_ptr<Neighborhood>& otherNeighborhood) const;

	//
	static std::map<char, char>	CentralMap
	(
		const std::shared_ptr<Neighborhood>& firstNeighborhood,
		const std::shared_ptr<Neighborhood>& secondNeighborhood
	);

	//																													
	std::vector<Edge> OuterForwardDifference
	(
		const std::shared_ptr<Neighborhood>& rightNeighborhood,

		const std::map<char, char>& centralMap
	)
	const;

	std::vector<Edge> OuterReverseDifference
	(
		const std::shared_ptr<Neighborhood>& rightNeighborhood,

		const std::map<char, char>& centralMap
	)
	const;

private:
	//
	std::shared_ptr<Vertex> center;

	char centerElement;

	//
	std::set<char> neighbors;
	
	std::set<char> vertices;

	//
	std::vector<std::shared_ptr<Edge>> edges;

	//
	std::multiset<DEGREES_BY_COLOR> LazyDegreeSequences();

	std::optional<std::multiset<DEGREES_BY_COLOR>> degreeSequences;

	//
	EDGES_BY_COLOR	LazyEdgesByColor();

	std::optional<EDGES_BY_COLOR> edgesByColor;

	//
	int edgeCount;

	//
	std::set<COLOR> LazyEdgeColors();

	std::optional<std::set<COLOR>> edgeColors;

	//
	int LazyEdgeColorCount();

	std::optional<int> edgeColorCount;

	//
	std::vector<std::shared_ptr<Edge>> innerEdges;

	//
	EDGES_BY_COLOR	LazyInnerEdgesByColor();

	std::optional<EDGES_BY_COLOR> innerEdgesByColor;

	//
	int innerEdgeCount;

	//
	std::set<COLOR> innerEdgeColors;

	int innerEdgeColorCount;

	//
	std::vector<std::shared_ptr<Edge>> outerEdges;

	//
	EDGES_BY_COLOR	LazyOuterEdgesByColor();

	std::optional<EDGES_BY_COLOR> outerEdgesByColor;

	//
	int outerEdgeCount;

	//
	std::set<COLOR> outerEdgeColors;

	int outerEdgeColorCount;
};

#endif //NEIGHBORHOOD_H
