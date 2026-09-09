#ifndef GRAPH_SUMMARY_H
#define GRAPH_SUMMARY_H

//
#include "Graph.h"

//
class GraphSummary
{
public:
	//
	GraphSummary();

	GraphSummary(const GraphSummary& original);

	~GraphSummary();

	//
	static void Print(Graph& graph);
};

#endif //GRAPH_SUMMARY_H
