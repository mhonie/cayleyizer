#ifndef EDGE_H
#define EDGE_H

//
#include "Color.h"

//
#include <map>
#include <memory>
#include <functional>

//
class Edge
{
public:
	//
	Edge();

	Edge(char tail, char head, COLOR color);

	Edge(char tail, char head);

	Edge(const Edge& originalEdge);

	//
	~Edge();

	//
	static std::function<std::shared_ptr<Edge>(const Edge&)> CreatePtr;

	static std::function<Edge(const std::shared_ptr<Edge>&)> CopyPtr;

	//
	static Edge Flip(const std::shared_ptr<Edge>& edge);

	//
	static std::function<bool(std::shared_ptr<Edge>)>

		GetEquals(const std::shared_ptr<Edge>& otherEdge);

	static std::function<bool(std::shared_ptr<Edge> edge)>

		GetIsEquivalent(char, char, COLOR color);

	//
	char Head() const;

	//
	static std::function<char(const Edge& edge)> GetHead;

	static std::function<char(std::shared_ptr<Edge> edge)> GetPtrHead;

	//
	char Tail() const;

	//
	static std::function<char(const Edge& edge)> GetTail;

	static std::function<char(std::shared_ptr<Edge> edge)> GetPtrTail;

	//
	COLOR Color() const;

	//
	static std::function<COLOR(const Edge& edge)> GetColor;

	static std::function<COLOR(std::shared_ptr<Edge>)> GePtrColor;

	//
	bool IsInEdge(char element) const;

	static std::function<bool(std::shared_ptr<Edge>)> GetIsInEdge(char element);

	//
	bool IsOutEdge(char element) const;

	static std::function<bool(std::shared_ptr<Edge>)> GetIsOutEdge(char element);

	//
	bool IsIncident(char element) const;

	static std::function<bool(std::shared_ptr<Edge>)> GetIsIncident(char element);

	static std::function<bool(std::shared_ptr<Edge>)> GetNotIncident(char element);

	//
	static std::vector<std::shared_ptr<Edge>>

		Incidents(char element, const std::vector<std::shared_ptr<Edge>>& edges);

	//
	static std::vector<std::shared_ptr<Edge>>

		Incidents(std::set<char> elements, const std::vector<std::shared_ptr<Edge>>& edges);

	//
	static std::vector<std::shared_ptr<Edge>>

		IncidentsIn(char element, const std::vector<std::shared_ptr<Edge>>& edges);

	//
	static std::vector<std::shared_ptr<Edge>>

		IncidentsOut(char element, const std::vector<std::shared_ptr<Edge>>& edges);

	//
	static std::set<char> GetElements(std::vector<Edge> edges);

	static std::set<char> GetElementsPtr(std::vector<std::shared_ptr<Edge>> edges);

	//
	bool IsColor(COLOR otherColor) const;

	//
	static std::set<COLOR>

		DistinctColors(const std::vector<std::shared_ptr<Edge>>& edges);

	//
	static std::map<COLOR, std::vector<std::shared_ptr<Edge>>>

		GroupByColor(const std::vector<std::shared_ptr<Edge>>& edges);

private:
	//
	char tail;
	char head;

	//
	COLOR color;

	//
	static std::function<std::vector<std::shared_ptr<Edge>>(std::vector<std::shared_ptr<Edge>>, char)>

		UpdateIncidents(const std::vector<std::shared_ptr<Edge>>& edges);
};

#endif //EDGE_H
