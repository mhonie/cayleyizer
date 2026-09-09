#ifndef GRAPH_BUILDER_H
#define GRAPH_BUILDER_H

//
#include "Graph.h"

//
class GraphBuilder
{
public:
	//
	GraphBuilder();

	GraphBuilder(const GraphBuilder& original);

	~GraphBuilder();

	//
	static Graph Build(const std::vector<Edge>& edges);

	static Graph Build
	(
		const std::vector<Edge>& edges,

		const std::set<char>& disconnected
	);
};

#endif //GRAPH_BUILDER_H
