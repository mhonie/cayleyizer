//
#include <vector>

//
#include "Edge.h"
#include "Vertex.h"
#include "Graph.h"
#include "GraphBuilder.h"
#include "GraphSummary.h"
#include "GraphUpdater.h"

//
int main()
{	
	const auto edge1 = Edge('0', '1', COLOR::RED);
	const auto edge2 = Edge('1', '2', COLOR::RED);
	const auto edge3 = Edge('2', '3', COLOR::RED);
	const auto edge4 = Edge('3', '4', COLOR::RED);
	const auto edge5 = Edge('4', '0', COLOR::RED);
	
	const auto edge6 = Edge('5', '6', COLOR::RED);
	const auto edge7 = Edge('6', '7', COLOR::RED);
	const auto edge8 = Edge('7', '8', COLOR::RED);
	const auto edge9 = Edge('8', '9', COLOR::RED);
	const auto edge10 = Edge('9', '5', COLOR::RED);
	
	const auto edge11 = Edge('0', '5', COLOR::YELLOW);
	const auto edge12 = Edge('1', '6', COLOR::YELLOW);
	const auto edge13 = Edge('2', '7', COLOR::YELLOW);
	const auto edge14 = Edge('3', '8', COLOR::YELLOW);
	const auto edge15 = Edge('4', '9', COLOR::YELLOW);
	
	const auto edge16 = Edge('5', '0', COLOR::YELLOW);
	const auto edge17 = Edge('6', '1', COLOR::YELLOW);
	const auto edge18 = Edge('7', '2', COLOR::YELLOW);
	const auto edge19 = Edge('8', '3', COLOR::YELLOW);
	const auto edge20 = Edge('9', '4', COLOR::YELLOW);
	
	// const auto edge21 = Edge('a', 'b', COLOR::RED);
	// const auto edge22 = Edge('b', 'c', COLOR::RED);
	// const auto edge23 = Edge('c', 'd', COLOR::RED);
	// const auto edge24 = Edge('d', 'e', COLOR::RED);
	// const auto edge25 = Edge('e', 'a', COLOR::RED);
	// 
	// const auto edge26 = Edge('f', 'g', COLOR::RED);
	// const auto edge27 = Edge('g', 'h', COLOR::RED);
	// const auto edge28 = Edge('h', 'i', COLOR::RED);
	// const auto edge29 = Edge('i', 'j', COLOR::RED);
	// const auto edge30 = Edge('j', 'f', COLOR::RED);
	// 
	// const auto edge31 = Edge('a', 'f', COLOR::GREEN);
	// const auto edge32 = Edge('b', 'g', COLOR::GREEN);
	// const auto edge33 = Edge('c', 'h', COLOR::GREEN);
	// const auto edge34 = Edge('d', 'i', COLOR::GREEN);
	// const auto edge35 = Edge('e', 'j', COLOR::GREEN);
	// 
	// const auto edge36 = Edge('f', 'a', COLOR::GREEN);
	// const auto edge37 = Edge('g', 'b', COLOR::GREEN);
	// const auto edge38 = Edge('h', 'c', COLOR::GREEN);
	// const auto edge39 = Edge('i', 'd', COLOR::GREEN);
	// const auto edge40 = Edge('j', 'e', COLOR::GREEN);

	const std::vector<Edge> edges
	{
		edge1, edge2, edge3, edge4, edge5,
		edge6, edge7, edge8, edge9, edge10,
		edge11, edge12, edge13, edge14, edge15,
		edge16, edge17, edge18, edge19, edge20,
	};
	
	// const std::vector<Edge> edges
	// {
	// 	edge1, edge2, edge3, edge4, edge5,
	// 	edge6, edge7, edge8, edge9, edge10,
	// 	edge11, edge12, edge13, edge14, edge15,
	// 	edge16, edge17, edge18, edge19, edge20, 
	// 	edge21,	edge22,	edge23,	edge24,	edge25,			  
	// 	edge26, edge27,	edge28,	edge29,	edge30,			  
	// 	edge31,	edge32,	edge33,	edge34,	edge35,
	//  edge36, edge37, edge38,	edge39,	edge40
	// };
	
	// const auto edge1 = Edge('0', '1', COLOR::RED);
	// const auto edge2 = Edge('0', '2', COLOR::RED);
	// const auto edge3 = Edge('0', '3', COLOR::RED);
	// const auto edge4 = Edge('0', '4', COLOR::RED);
	// const auto edge5 = Edge('1', '4', COLOR::RED);
	// const auto edge6 = Edge('2', '3', COLOR::RED);
	// 
	// const auto edge7 = Edge('a', 'b', COLOR::RED);
	// const auto edge8 = Edge('a', 'c', COLOR::RED);
	// const auto edge9 = Edge('a', 'd', COLOR::RED);
	// const auto edge10 = Edge('a', 'e', COLOR::RED);
	// const auto edge11 = Edge('b', 'c', COLOR::RED);
	// const auto edge12 = Edge('d', 'e', COLOR::RED);
	// 
	// const std::vector<Edge> edges
	// {
	// 	edge1, edge2, edge3, edge4, edge5,
	// 	edge6, edge7, edge8, edge9, edge10,
	// 	edge11, edge12
	// };
	
	Graph graph;
	
	std::set<std::shared_ptr<Vertex>> vertices;
	
	std::set<std::shared_ptr<Vertex>> disconnectedVertices;
	
	IRREGULARITIES irregularities;
	
	INNER_DIFFERENCES innerForwardDifferences;
	INNER_DIFFERENCES innerReverseDifferences;
	
	OUTER_DIFFERENCES outerForwardDifferences;
	OUTER_DIFFERENCES outerReverseDifferences;
	
	GraphUpdater graphUpdater;
	
	Graph updated;
	
	graph = GraphBuilder::Build(edges);
	
	vertices = graph.Vertices();
	
	auto testVertex = *vertices.begin();
	
	disconnectedVertices = graph.DisconnectedVertices();
	
	irregularities = graph.Irregularities();
	
	innerForwardDifferences = graph.InnerForwardDifferences();
	innerReverseDifferences = graph.InnerReverseDifferences();
	
	outerForwardDifferences = graph.OuterForwardDifferences();
	outerReverseDifferences = graph.OuterReverseDifferences();
	
	// GraphSummary::Print(graph);
	
	graphUpdater = GraphUpdater(graph);
	
	updated = graphUpdater.AddEdge(Edge('4', '3', COLOR::BLUE));
	
	disconnectedVertices = updated.DisconnectedVertices();
	
	irregularities = updated.Irregularities();
	
	innerForwardDifferences = updated.InnerForwardDifferences();
	innerReverseDifferences = updated.InnerReverseDifferences();
	
	outerForwardDifferences = updated.OuterForwardDifferences();
	outerReverseDifferences = updated.OuterReverseDifferences();
	
	// GraphSummary::Print(updated);
	
	graphUpdater = GraphUpdater(updated);
	
	updated = graphUpdater.AddVertex('q');
	
	disconnectedVertices = updated.DisconnectedVertices();
	
	irregularities = updated.Irregularities();
	
	innerForwardDifferences = updated.InnerForwardDifferences();
	innerReverseDifferences = updated.InnerReverseDifferences();
	
	outerForwardDifferences = updated.OuterForwardDifferences();
	outerReverseDifferences = updated.OuterReverseDifferences();
	
	// GraphSummary::Print(updated);	
	
	graphUpdater = GraphUpdater(updated);
	
	auto verticesToDelete = std::vector<std::shared_ptr<Vertex>>
	(
		disconnectedVertices.begin(), 
		disconnectedVertices.end()
	);
	
	updated = graphUpdater.DeleteVertices(verticesToDelete);
	
	disconnectedVertices = updated.DisconnectedVertices();
	
	irregularities = updated.Irregularities();
	
	innerForwardDifferences = updated.InnerForwardDifferences();
	innerReverseDifferences = updated.InnerReverseDifferences();
	
	outerForwardDifferences = updated.OuterForwardDifferences();
	outerReverseDifferences = updated.OuterReverseDifferences();
	
	// GraphSummary::Print(updated);
	
	graphUpdater = GraphUpdater(updated);
	
	auto edgeToDelete = *updated.Edges().begin();
	
	updated = graphUpdater.DeleteEdge(edgeToDelete);
	
	disconnectedVertices = updated.DisconnectedVertices();
	
	irregularities = updated.Irregularities();
	
	innerForwardDifferences = updated.InnerForwardDifferences();
	innerReverseDifferences = updated.InnerReverseDifferences();
	
	outerForwardDifferences = updated.OuterForwardDifferences();
	outerReverseDifferences = updated.OuterReverseDifferences();
	
	// GraphSummary::Print(updated);
}