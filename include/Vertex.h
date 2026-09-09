#ifndef VERTEX_H
#define VERTEX_H

//
#include "Edge.h"

//
#include <vector>
#include <optional>

//
typedef std::map<COLOR, std::pair<int, int>> DEGREES_BY_COLOR;

//
typedef std::map<COLOR, std::pair<int, int>> IRREGULARITY;

//
typedef std::map<COLOR, std::pair<int, int>> INNER_DIFFERENCE;

//
typedef std::map<COLOR, std::vector<std::shared_ptr<Edge>>> EDGES_BY_COLOR;

//
class Vertex
{
public:
	//
	Vertex();

	explicit Vertex(char element);

	Vertex(char element, const std::vector<std::shared_ptr<Edge>>& edges);

	//
	Vertex(const Vertex& original);

	static std::function<Vertex(const std::shared_ptr<Vertex>&)> CopyPtr;

	static std::function<std::shared_ptr<Vertex>(const Vertex&)> CreatePtr;

	//
	static std::function<Vertex(char)> CreateDisconnected;

	//
	~Vertex();

	//
	bool operator== (const Vertex& otherVertex) const;

	bool operator< (const Vertex& otherVertex) const;

	//
	bool IsEquivalent(std::shared_ptr<Vertex> otherVertex);

	static std::function<bool(std::shared_ptr<Vertex>)>

		GetIsEquivalent(const std::shared_ptr<Vertex>& vertex);

	//
	char Element() const;

	//
	static std::function<char(const Vertex& vertex)> GetElement;

	static std::function<char(std::shared_ptr<Vertex> vertex)> GetPtrElement;

	//
	std::vector<std::shared_ptr<Edge>> Edges() const;

	std::vector<std::shared_ptr<Edge>> Edges(char otherElement) const;

	std::vector<std::shared_ptr<Edge>> Edges(COLOR color);

	//
	std::set<char> Neighbors();

	//
	int Degree() const;

	//
	int Degree(COLOR color);

	//
	DEGREES_BY_COLOR DegreesByColor();

	std::pair<int, int> Degrees(COLOR color);

	//
	std::vector<std::shared_ptr<Edge>> InEdges();

	std::vector<std::shared_ptr<Edge>> InEdges(char otherElement);

	std::vector<std::shared_ptr<Edge>> InEdges(COLOR color);

	//
	int InDegree();

	int InDegree(COLOR color);

	//
	std::map<COLOR, int> InDegreeByColor();

	//
	std::vector<std::shared_ptr<Edge>> OutEdges();

	std::vector<std::shared_ptr<Edge>> OutEdges(char otherElement);

	std::vector<std::shared_ptr<Edge>> OutEdges(COLOR color);

	//
	int OutDegree();

	int OutDegree(COLOR color);

	//
	std::map<COLOR, int> OutDegreeByColor();

	//
	std::set<COLOR> EdgeColors();

	//
	int EdgeColorCount();

	int InColorCount();

	int OutColorCount();

	//
	bool IsConnected() const;

	bool IsDisconnected() const;

	//
	static std::function<bool(Vertex)> GetIsConnected;

	static std::function<bool(std::shared_ptr<Vertex>)>	GetPtrIsConnected;

	//
	static std::function<bool(Vertex)> GetIsDisconnected;

	static std::function<bool(std::shared_ptr<Vertex>)>	GetPtrIsDisconnected;

	//
	bool IsRegular();

	//
	IRREGULARITY Irregularity();

	//
	INNER_DIFFERENCE ForwardDifference(const std::shared_ptr<Vertex>& rightVertex);

	INNER_DIFFERENCE ReverseDifference(const std::shared_ptr<Vertex>& rightVertex);

private:
	//
	char element;

	//
	std::vector<std::shared_ptr<Edge>> edges;

	//
	auto SameDegrees(std::shared_ptr<Vertex>& otherVertex);

	//
	EDGES_BY_COLOR LazyEdgesByColor();

	std::optional<EDGES_BY_COLOR> edgesByColor;

	//
	std::set<char> LazyNeighbors();

	std::optional<std::set<char>> neighbors;

	//
	int degree;

	//
	std::map<COLOR, int> LazyDegreeByColor();

	std::optional<std::map<COLOR, int>> degreeByColor;

	//
	DEGREES_BY_COLOR LazyDegreesByColor();

	std::optional<DEGREES_BY_COLOR> degreesByColor;

	//
	std::vector<std::shared_ptr<Edge>> LazyInEdges();

	std::optional<std::vector<std::shared_ptr<Edge>>> inEdges;

	//
	EDGES_BY_COLOR	LazyInEdgesByColor();

	std::optional<EDGES_BY_COLOR> inEdgesByColor;

	//
	int LazyInDegree();

	std::optional<int> inDegree;

	//
	std::map<COLOR, int> LazyInDegreeByColor();

	std::optional<std::map<COLOR, int>> inDegreeByColor;

	//
	std::vector<std::shared_ptr<Edge>> LazyOutEdges();

	std::optional<std::vector<std::shared_ptr<Edge>>> outEdges;

	//
	EDGES_BY_COLOR	LazyOutEdgesByColor();

	std::optional<EDGES_BY_COLOR> outEdgesByColor;

	//
	std::map<COLOR, int> LazyOutDegreeByColor();

	std::optional<std::map<COLOR, int>> outDegreeByColor;

	//
	int LazyOutDegree();

	std::optional<int> outDegree;

	//
	std::set<COLOR> LazyEdgeColors();

	std::optional<std::set<COLOR>> edgeColors;

	//
	int LazyColorCount();

	std::optional<int> colorCount;

	//
	int LazyInColorCount();

	std::optional<int> inColorCount;

	//
	int LazyOutColorCount();

	std::optional<int> outColorCount;

	//
	bool isConnected;

	//
	std::function<bool(COLOR)> UpdateRegularity();

	bool LazyIsRegular();

	std::optional<bool> isRegular;

	//
	std::function<IRREGULARITY(IRREGULARITY, COLOR)> UpdateIrregularity();

	//
	IRREGULARITY LazyIrregularity();

	std::optional<IRREGULARITY> irregularity;

	//
    auto UpdateForwardDifference(const std::shared_ptr<Vertex>& rightVertex);

    auto UpdateReverseDifference(const std::shared_ptr<Vertex>& rightVertex);
};

#endif //VERTEX_H
