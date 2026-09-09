//
#include "Graph.h"
#include "Homogeneous.h"

//
#include <algorithm>
#include <iostream>
#include <numeric>

//
Graph::Graph()
{
	neighborhoods = {};

	neighborhoodCount = 0;

	isEmpty = true;

	isSingleton = false;
}

//
Graph::Graph(const std::vector<std::shared_ptr<Neighborhood>>& neighborhoods)
{
	this->neighborhoods = neighborhoods;

	neighborhoodCount = static_cast<int>(neighborhoods.size());

	isEmpty = neighborhoodCount == 0;
}

//
Graph::Graph(const Graph& otherGraph) = default;

//
Graph::~Graph() = default;

//
std::set<std::shared_ptr<Vertex>> Graph::LazyVertices()
{
	if (vertices.has_value())

		return vertices.value();

	std::set<std::shared_ptr<Vertex>> lazyVertices;

	std::transform
	(
		neighborhoods.begin(),
		neighborhoods.end(),
		inserter(lazyVertices, lazyVertices.end()),
		Neighborhood::GetCenter
	);

	vertices = lazyVertices;

	return lazyVertices;
}

//
std::set<std::shared_ptr<Vertex>> Graph::Vertices()
{
	return LazyVertices();
}

//
int Graph::LazyVertexCount()
{
	if (vertexCount.has_value())

		return vertexCount.value();

	int lazyVertexCount = static_cast<int>(Vertices().size());

	vertexCount = lazyVertexCount;

	return lazyVertexCount;
}

//
int Graph::VertexCount()
{
	return LazyVertexCount();
}

//
namespace 
{
	//
	auto updateGraphEdges = []
	(
		std::set<std::shared_ptr<Edge>>& graphEdges,

		const std::shared_ptr<Neighborhood>& neighborhood
	)
	{
		std::vector<std::shared_ptr<Edge>>

			neighborhoodEdges = neighborhood->Edges();

		graphEdges.insert
		(
			neighborhoodEdges.begin(),
			neighborhoodEdges.end()
		);

		return graphEdges;
	};
}

//
std::set<std::shared_ptr<Edge>> Graph::LazyEdges()
{
	if (edges.has_value())

		return edges.value();

	std::set<std::shared_ptr<Edge>> lazyEdges =

		std::accumulate
		(
			neighborhoods.begin(),
			neighborhoods.end(),
			std::set<std::shared_ptr<Edge>>(),
			updateGraphEdges
		);

	edges = lazyEdges;

	return lazyEdges;
}

//
std::set<std::shared_ptr<Edge>> Graph::Edges()
{
	return LazyEdges();
}

//
int Graph::LazyEdgeCount()
{
    if (edgeCount.has_value())

        return edgeCount.value();

    int lazyEdgeCount = static_cast<int>(Edges().size());

    edgeCount = lazyEdgeCount;

    return lazyEdgeCount;
}

//
int Graph::EdgeCount()
{
	return LazyEdgeCount();
}

//
namespace
{
	//
	auto updateGraphColors = []
	(
		const std::set<COLOR>& current,

		const std::shared_ptr<Neighborhood>& neighborhood
	)
	{
		return Colors::Distinct(current, neighborhood->EdgeColors());
	};
}

//
std::set<COLOR> Graph::LazyEdgeColors()
{
	if (edgeColors.has_value())

		return edgeColors.value();

	std::set<COLOR> lazyColors = 
		
		std::accumulate
		(
			neighborhoods.begin(),
			neighborhoods.end(),
			std::set<COLOR>(),
			updateGraphColors
		);

	edgeColors = lazyColors;

	return lazyColors;
}

//
std::set<COLOR> Graph::EdgeColors()
{
	return LazyEdgeColors();
}

//
int Graph::LazyEdgeColorCount()
{
	if (colorCount.has_value())

		return colorCount.value();

	int lazyColorCount = static_cast<int>(EdgeColors().size());

	colorCount = lazyColorCount;

	return lazyColorCount;
}

//
int Graph::EdgeColorCount()
{
	return LazyEdgeColorCount();
}

//
std::vector<std::shared_ptr<Neighborhood>> Graph::Neighborhoods() const
{
	return neighborhoods;
}

//
namespace 
{
	//
	auto hasCenter(char element)
	{
		return [element](const std::shared_ptr<Neighborhood>& neighborhood)
		{
			return neighborhood->Center()->Element() == element;
		};
	}
}

//
std::shared_ptr<Neighborhood> Graph::NeighborhoodOf(const char element) const
{
	const auto iterator =

		std::find_if
		(
			neighborhoods.begin(),
			neighborhoods.end(),
			hasCenter(element)
		);

	if (iterator == neighborhoods.end())

		throw std::logic_error("element does not exist");

	std::shared_ptr<Neighborhood> neighborhood = *iterator;

	return neighborhood;
}
//
std::function<std::shared_ptr<Neighborhood>(char element)>

	Graph::GetNeighborhoodsOf(const Graph& graph)
	{
		return [graph](const char element)
		{
			return graph.NeighborhoodOf(element);
		};
	}

//
int Graph::NeighborhoodCount() const
{
	return neighborhoodCount;
}

//
bool Graph::IsEmpty() const
{
	return isEmpty;
}

//
bool Graph::LazyIsSingleton()
{
	if (isSingleton.has_value())

		return isSingleton.value();

	bool lazyIsSingleton;

	if (neighborhoodCount == 1)
	{
		if ((*neighborhoods.begin())->EdgeCount() == 0)

			lazyIsSingleton = true;

		else

			lazyIsSingleton = false;
	}
	else

		lazyIsSingleton = false;

	isSingleton = lazyIsSingleton;

	return lazyIsSingleton;
}

//
bool Graph::IsSingleton()
{
	return LazyIsSingleton();
}

//
namespace
{
	//
	auto updateConnectivities =

		[](const std::shared_ptr<Neighborhood>& neighborhood)
		{
			std::shared_ptr<Vertex> vertex = neighborhood->Center();

			bool connectivity = vertex->IsConnected();

			return std::pair{ vertex, connectivity };
		};
}

//
std::map<std::shared_ptr<Vertex>, bool> Graph::LazyConnectivities()
{
	if (connectivities.has_value())

		return connectivities.value();

	std::map<std::shared_ptr<Vertex>, bool> lazyConnectivities;

	std::transform
	(
		neighborhoods.begin(),
		neighborhoods.end(),
		inserter(lazyConnectivities, lazyConnectivities.end()),
		updateConnectivities
	);

	connectivities = lazyConnectivities;

	return lazyConnectivities;
}

//
namespace 
{
	//
	auto updateConnected = []
	(
		std::set<std::shared_ptr<Vertex>> connected,

		const std::pair<std::shared_ptr<Vertex>, bool>& connectivity
	)
	{
		if (connectivity.second)

			connected.insert(connectivity.first);

		return connected;
	};

	auto updateDisconnected = []
	(
		std::set<std::shared_ptr<Vertex>> disconnected,

		const std::pair<std::shared_ptr<Vertex>, bool>& connectivity
	)
	{
		if (!connectivity.second)

			disconnected.insert(connectivity.first);

		return disconnected;
	};
}

//
std::set<std::shared_ptr<Vertex>> Graph::LazyConnectedVertices()
{
	if (connectedVertices.has_value())

		return connectedVertices.value();

	std::map<std::shared_ptr<Vertex>, bool>

		tempConnectivities = LazyConnectivities();

	std::set<std::shared_ptr<Vertex>> lazyConnectedVertices =

		std::accumulate
		(
			tempConnectivities.begin(),
			tempConnectivities.end(),
			std::set<std::shared_ptr<Vertex>>{},
			updateConnected
		);

	connectedVertices = lazyConnectedVertices;

	return lazyConnectedVertices;
}

//
std::set<std::shared_ptr<Vertex>> Graph::ConnectedVertices()
{
	return LazyConnectedVertices();
}

//
std::set<std::shared_ptr<Vertex>> Graph::LazyDisconnectedVertices()
{
	if (disconnectedVertices.has_value())

		return disconnectedVertices.value();

	std::map<std::shared_ptr<Vertex>, bool>

		tempConnectivities = LazyConnectivities();

	std::set<std::shared_ptr<Vertex>> lazyDisconnected =

		std::accumulate
		(
			tempConnectivities.begin(),
			tempConnectivities.end(),
			std::set<std::shared_ptr<Vertex>>{},
			updateDisconnected
		);

	disconnectedVertices = lazyDisconnected;

	return lazyDisconnected;
}

//
std::set<std::shared_ptr<Vertex>> Graph::DisconnectedVertices()
{
	return LazyDisconnectedVertices();
}

//
namespace
{
	//
	auto centerIsConnected = []
	(
		const std::shared_ptr<Neighborhood>& neighborhood
	)
	{
		return neighborhood->Center()->IsConnected();
	};
}

//
std::optional<bool> Graph::LazyIsConnected()
{
	if (isConnected.has_value())

		return isConnected;

	const bool lazyIsConnected =

		std::all_of
		(
			neighborhoods.begin(),
			neighborhoods.end(),
			centerIsConnected
		);

	if (lazyIsConnected == false)

		isConnected = lazyIsConnected;

	return isConnected;
}

//
std::optional<bool> Graph::IsConnected()
{
	return LazyIsConnected();
}

//
namespace
{
	//
	auto updateRegularities =

		[](const std::shared_ptr<Neighborhood>& neighborhood)
		{
			std::shared_ptr<Vertex> vertex = neighborhood->Center();

			bool regularity = vertex->IsRegular();

			return std::pair{ vertex, regularity };
		};
}

//
std::map<std::shared_ptr<Vertex>, bool> Graph::LazyRegularities()
{
	if (regularities.has_value())

		return regularities.value();

	std::map<std::shared_ptr<Vertex>, bool> lazyRegularities;

	std::transform
	(
		neighborhoods.begin(),
		neighborhoods.end(),
		inserter(lazyRegularities, lazyRegularities.end()),
		updateRegularities
	);

	connectivities = lazyRegularities;

	return lazyRegularities;
}

//
namespace
{
	//
	auto updateRegular = []
	(
		std::set<std::shared_ptr<Vertex>> regular,

		const std::pair<std::shared_ptr<Vertex>, bool>& regularity
		)
		{
			if (regularity.second)

				regular.insert(regularity.first);

			return regular;
		};

	auto updateIrregular = []
	(
		std::set<std::shared_ptr<Vertex>> irregular,

		const std::pair<std::shared_ptr<Vertex>, bool>& regularity
		)
		{
			if (!regularity.second)

				irregular.insert(regularity.first);

			return irregular;
		};
}

//
std::set<std::shared_ptr<Vertex>> Graph::LazyRegularVertices()
{
	if (regularVertices.has_value())

		return regularVertices.value();

	std::map<std::shared_ptr<Vertex>, bool>

		tempRegularities = LazyRegularities();

	std::set<std::shared_ptr<Vertex>> lazyRegularVertices =

		std::accumulate
		(
			tempRegularities.begin(),
			tempRegularities.end(),
			std::set<std::shared_ptr<Vertex>>{},
			updateRegular
		);

	regularVertices = lazyRegularVertices;

	return lazyRegularVertices;
}

//
std::set<std::shared_ptr<Vertex>> Graph::RegularVertices()
{
	return LazyRegularVertices();
}

//
std::set<std::shared_ptr<Vertex>> Graph::LazyIrregularVertices()
{
	if (irregularVertices.has_value())

		return irregularVertices.value();

	std::map<std::shared_ptr<Vertex>, bool>

		tempRegularities = LazyRegularities();

	std::set<std::shared_ptr<Vertex>> lazyIrregularVertices =

		std::accumulate
		(
			tempRegularities.begin(),
			tempRegularities.end(),
			std::set<std::shared_ptr<Vertex>>{},
			updateIrregular
		);

	irregularVertices = lazyIrregularVertices;

	return lazyIrregularVertices;
}

//
std::set<std::shared_ptr<Vertex>> Graph::IrregularVertices()
{
	return LazyIrregularVertices();
}

//
namespace
{
	//
	auto centerIsRegular = []
	(
		const std::shared_ptr<Neighborhood>& neighborhood
	)
	{
		return neighborhood->Center()->IsRegular();
	};
}

//
std::optional<bool> Graph::LazyIsRegular()
{
	if (isRegular.has_value())

		return isRegular;

	const bool lazyIsRegular =

		std::all_of
		(
			neighborhoods.begin(),
			neighborhoods.end(),
			centerIsRegular
		);

	if (lazyIsRegular == false)
	
		isRegular = lazyIsRegular;

	return lazyIsRegular;
}

//
std::optional<bool> Graph::IsRegular()
{
	return LazyIsRegular();
}

//
namespace 
{
	//
	auto updateIrregularities = []
	(
		const std::shared_ptr<Neighborhood>& neighborhood
	)
	{
		std::shared_ptr<Vertex> vertex = neighborhood->Center();

		char element = vertex->Element();

		IRREGULARITY irregularity = vertex->Irregularity();

		return std::pair<char, std::map<COLOR, std::pair<int, int>>>
		{
			element, irregularity
		};
	};
}

//
IRREGULARITIES

	Graph::LazyIrregularities()
	{
		if (irregularities.has_value())

			return irregularities.value();

		IRREGULARITIES lazyIrregularities;

		std::transform
		(
			neighborhoods.begin(),
			neighborhoods.end(),
			inserter(lazyIrregularities, lazyIrregularities.end()),
			updateIrregularities
		);

		irregularities = lazyIrregularities;

		return lazyIrregularities;
	}

//
IRREGULARITIES

	Graph::Irregularities()
	{
		return LazyIrregularities();
	}

//
INNER_DIFFERENCES Graph::LazyInnerForwardDifferences()
{
	if (innerForwardDifferences.has_value())

		return innerForwardDifferences.value();

	INNER_DIFFERENCES lazyInnerForwardDifferences;

	for 
	(
		auto firstNeighborhood = neighborhoods.begin(); 
		firstNeighborhood != neighborhoods.end(); 
		++firstNeighborhood
	)
	{
		for 
		(
			auto secondNeighborhood = std::next(firstNeighborhood); 
			secondNeighborhood != neighborhoods.end(); 
			++secondNeighborhood
		)
		{
			std::shared_ptr<Vertex> firstVertex = (*firstNeighborhood)->Center();
			std::shared_ptr<Vertex> secondVertex = (*secondNeighborhood)->Center();

			INNER_DIFFERENCE forwardDiff = 
				
				(*firstNeighborhood)->InnerForwardDifference(*secondNeighborhood);

			if (!forwardDiff.empty())

				lazyInnerForwardDifferences.insert
				(
					{
						{secondVertex->Element(), firstVertex->Element()},

						forwardDiff
					}
				);
		}
	}

	innerForwardDifferences = lazyInnerForwardDifferences;

	return lazyInnerForwardDifferences;
}

//
INNER_DIFFERENCES Graph::InnerForwardDifferences()
{
	return LazyInnerForwardDifferences();
}

//
INNER_DIFFERENCES Graph::LazyInnerReverseDifferences()
{
	if (innerReverseDifferences.has_value())

		return innerReverseDifferences.value();

	INNER_DIFFERENCES lazyInnerReverseDifferences;

	for
	(
		auto firstNeighborhood = neighborhoods.begin();
		firstNeighborhood != neighborhoods.end();
		++firstNeighborhood
	)
	{
		for
		(
			auto secondNeighborhood = std::next(firstNeighborhood);
			secondNeighborhood != neighborhoods.end();
			++secondNeighborhood
		)
		{
			std::shared_ptr<Vertex> firstVertex = (*firstNeighborhood)->Center();
			std::shared_ptr<Vertex> secondVertex = (*secondNeighborhood)->Center();

			INNER_DIFFERENCE reverseDiff =

				(*firstNeighborhood)->InnerReverseDifference(*secondNeighborhood);

			if (!reverseDiff.empty())

				lazyInnerReverseDifferences.insert
				(
					{
						{firstVertex->Element(), secondVertex->Element()},

						reverseDiff
					}
				);
		}
	}

	innerReverseDifferences = lazyInnerReverseDifferences;

	return lazyInnerReverseDifferences;
}

//
INNER_DIFFERENCES Graph::InnerReverseDifferences()
{
	return LazyInnerReverseDifferences();
}

//
OUTER_DIFFERENCES Graph::LazyOuterForwardDifferences()
{
	if (outerForwardDifferences.has_value())

		return outerForwardDifferences.value();

	OUTER_DIFFERENCES lazyOuterForwardDifferences = {};

	for
	(
		auto leftNeighborhood = neighborhoods.begin();
		leftNeighborhood != neighborhoods.end();
		++leftNeighborhood
	)
	{
		for
		(
			auto rightNeighborhood = std::next(leftNeighborhood);
			rightNeighborhood != neighborhoods.end();
			++rightNeighborhood
		)
		{
			std::shared_ptr<Vertex> leftVertex = (*leftNeighborhood)->Center();
			std::shared_ptr<Vertex> rightVertex = (*rightNeighborhood)->Center();

			if (leftVertex->IsEquivalent(rightVertex))
			{				
				std::map<char, char> forwardCentralMap =

					Neighborhood::CentralMap(*leftNeighborhood, *rightNeighborhood);

				std::vector<Edge> forwardDiff =

					(*leftNeighborhood)->OuterForwardDifference
					(
						*rightNeighborhood,

						forwardCentralMap
					);

				if (!forwardDiff.empty())

					lazyOuterForwardDifferences.insert
					(
						{
							{rightVertex->Element(), leftVertex->Element()},

							forwardDiff
						}
					);
			}
		}
	}

	outerForwardDifferences = lazyOuterForwardDifferences;

	return lazyOuterForwardDifferences;
}

//
OUTER_DIFFERENCES Graph::OuterForwardDifferences()
{
	return LazyOuterForwardDifferences();
}

//
OUTER_DIFFERENCES Graph::LazyOuterReverseDifferences()
{
	if (outerReverseDifferences.has_value())

		return outerReverseDifferences.value();

	OUTER_DIFFERENCES lazyOuterReverseDifferences = {};

	for
	(
		auto leftNeighborhood = neighborhoods.begin();
		leftNeighborhood != neighborhoods.end();
		++leftNeighborhood
	)
	{
		for
		(
			auto rightNeighborhood = std::next(leftNeighborhood);
			rightNeighborhood != neighborhoods.end();
			++rightNeighborhood
		)
		{
			std::shared_ptr<Vertex> leftVertex = (*leftNeighborhood)->Center();
			std::shared_ptr<Vertex> rightVertex = (*rightNeighborhood)->Center();

			if (leftVertex->IsEquivalent(rightVertex))
			{
				std::map<char, char> reverseCentralMap =

					Neighborhood::CentralMap(*rightNeighborhood, *leftNeighborhood);

				std::vector<Edge> reverseDiff =

					(*leftNeighborhood)->OuterReverseDifference
					(
						*rightNeighborhood,

						reverseCentralMap
					);

				if (!reverseDiff.empty())

					lazyOuterReverseDifferences.insert
					(
						{
							{leftVertex->Element(), rightVertex->Element()},

							reverseDiff
						}
					);
			}
		}
	}

	outerReverseDifferences = lazyOuterReverseDifferences;

	return lazyOuterReverseDifferences;
}

//
OUTER_DIFFERENCES Graph::OuterReverseDifferences()
{
	return LazyOuterReverseDifferences();
}

//
OUTER_DIFFERENCES Graph::LazyOuterDifferences()
{
	if (outerDifferences.has_value())

		return outerDifferences.value();

	auto tempOuterReverseDifferences = OuterReverseDifferences();
	auto tempOuterForwardDifferences = OuterForwardDifferences();

	OUTER_DIFFERENCES lazyOuterDifferences;

	std::transform
	(
		tempOuterReverseDifferences.begin(),
		tempOuterReverseDifferences.end(),
		inserter(lazyOuterDifferences, lazyOuterDifferences.end()),
		[](const auto& pair) { return pair; }
	);

	std::transform
	(
		tempOuterForwardDifferences.begin(),
		tempOuterForwardDifferences.end(),
		inserter(lazyOuterDifferences, lazyOuterDifferences.end()),
		[](const auto& pair) { return pair; }
	);

	outerDifferences = lazyOuterDifferences;

	return lazyOuterDifferences;
}
