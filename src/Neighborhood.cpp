//
#include "Neighborhood.h"

//
#include <numeric>
#include <algorithm>
#include <iostream>

//
Neighborhood::Neighborhood()
{
	center = std::make_shared<Vertex>();

	outerEdges = {};

	centerElement = center->Element();

	innerEdges = {};

	neighbors = {};

	vertices = neighbors;

	vertices.insert(center->Element());

	edges = {};

	edgeCount = 0;

	innerEdgeCount = 0;
	outerEdgeCount = 0;

	innerEdgeColors = {};

	innerEdgeColorCount = 0;

	outerEdgeColors = {};

	outerEdgeColorCount = 0;
}

//
Neighborhood::Neighborhood
(
	const std::shared_ptr<Vertex>& center,

	const std::vector<std::shared_ptr<Edge>>& outerEdges
)
{
	this->center = center;
	
	this->outerEdges = outerEdges;

	centerElement = center->Element();

	innerEdges = center->Edges();

	neighbors = center->Neighbors();

	vertices = neighbors;

	vertices.insert(center->Element());

	edges = {};

	edges.insert(edges.end(), innerEdges.begin(), innerEdges.end());
	edges.insert(edges.end(), outerEdges.begin(), outerEdges.end());

	edgeCount = static_cast<int>(edges.size());

	innerEdgeCount = static_cast<int>(innerEdges.size());
	outerEdgeCount = static_cast<int>(outerEdges.size());

	innerEdgeColors = center->EdgeColors();

	innerEdgeColorCount = static_cast<int>(innerEdgeColors.size());

	outerEdgeColors = Edge::DistinctColors(outerEdges);

	outerEdgeColorCount = static_cast<int>(outerEdgeColors.size());
}

//
Neighborhood::Neighborhood(const Neighborhood& original) = default;

//
Neighborhood::~Neighborhood() = default;

//
bool Neighborhood::operator==(const Neighborhood& leftNeighborhood) const
{
	const bool areEqual = 
		
		centerElement == leftNeighborhood.centerElement

			&& *center == *leftNeighborhood.center

			&& outerEdges == leftNeighborhood.OuterEdges();

	return areEqual;
}

//
bool Neighborhood::operator<(const Neighborhood& leftNeighborhood) const
{
	return centerElement < leftNeighborhood.centerElement;
}

//
std::shared_ptr<Vertex> Neighborhood::Center() const
{
	return center;
}

//
char Neighborhood::CenterElement() const
{
	return centerElement;
}

//
std::function<std::shared_ptr<Vertex>(std::shared_ptr<Neighborhood>)>

	Neighborhood::GetCenter =

		[](const std::shared_ptr<Neighborhood>& neighborhood)
		{
			return neighborhood->center;
		};

//
std::vector<std::shared_ptr<Edge>> Neighborhood::Edges() const
{
	return edges;
}

//
std::vector<std::shared_ptr<Edge>> Neighborhood::Edges(const COLOR color)
{
	std::vector<std::shared_ptr<Edge>> edgesWithColor = {};

	std::vector<std::shared_ptr<Edge>>

		innerEdgesAtColor = LazyInnerEdgesByColor()[color];

	std::vector<std::shared_ptr<Edge>> outerEdgesAtColor = 
		
		LazyOuterEdgesByColor()[color];

	edgesWithColor.insert
	(
		edgesWithColor.end(), 
		innerEdgesAtColor.begin(), 
		innerEdgesAtColor.end()
	);

	edgesWithColor.insert
	(
		edgesWithColor.end(), 
		outerEdgesAtColor.begin(), 
		outerEdgesAtColor.end()
	);

	return edgesWithColor;
}

//
EDGES_BY_COLOR Neighborhood::EdgesByColor()
{
	return LazyEdgesByColor();
}

//
int Neighborhood::EdgeCount() const
{
	return edgeCount;
}

//
std::set<char> Neighborhood::Neighbors()
{
	return neighbors;
}

//
std::set<char> Neighborhood::Vertices() const
{
	return vertices;
}

//
int Neighborhood::VertexCount() const
{
	return static_cast<int>(vertices.size());
}

//
std::set<COLOR> Neighborhood::LazyEdgeColors()
{
	if (edgeColors.has_value())

		return edgeColors.value();

	std::set<COLOR> innerColors = innerEdgeColors;
	std::set<COLOR> outerColors = outerEdgeColors;

	std::set<COLOR> allColors;

	std::set_union
	(
		std::begin(innerColors),
		std::end(innerColors),
		std::begin(outerColors),
		std::end(outerColors),
		std::inserter(allColors, std::begin(allColors))
	);

	edgeColors = allColors;

	return allColors;
}

//
std::set<COLOR> Neighborhood::EdgeColors()
{
	return LazyEdgeColors();
}

//
int Neighborhood::LazyEdgeColorCount()
{
	if (edgeColorCount.has_value())

		return edgeColorCount.value();

	int colorCount = static_cast<int>(EdgeColors().size());

	edgeColorCount = colorCount;

	return colorCount;
}

//
int Neighborhood::EdgeColorCount()
{
	return LazyEdgeColorCount();
}

//
EDGES_BY_COLOR Neighborhood::LazyEdgesByColor()
{
	if (edgesByColor.has_value())

		return edgesByColor.value();

	EDGES_BY_COLOR lazyEdgesByColor = Edge::GroupByColor(edges);

	edgesByColor = lazyEdgesByColor;

	return lazyEdgesByColor;
}

//
std::vector<std::shared_ptr<Edge>> Neighborhood::InnerEdges() const
{
	return innerEdges;
}

//
EDGES_BY_COLOR Neighborhood::LazyInnerEdgesByColor()
{
	if (innerEdgesByColor.has_value())

		return innerEdgesByColor.value();

	std::map<COLOR,	std::vector<std::shared_ptr<Edge>>>

		lazyInnerEdgesByColor = Edge::GroupByColor(innerEdges);

	innerEdgesByColor = lazyInnerEdgesByColor;

	return lazyInnerEdgesByColor;
}

//
std::vector<std::shared_ptr<Edge>> Neighborhood::InnerEdges(const COLOR color)
{
	std::map<COLOR,	std::vector<std::shared_ptr<Edge>>>

		lazyInnerEdgesByColor = LazyInnerEdgesByColor();

	if (lazyInnerEdgesByColor.count(color) == 0)

		return {};

	return lazyInnerEdgesByColor.at(color);
}

//
int Neighborhood::InnerEdgeCount() const
{
	return innerEdgeCount;
}

//
std::set<COLOR> Neighborhood::InnerEdgeColors()
{
	return innerEdgeColors;
}

//
int Neighborhood::InnerEdgeColorCount() const
{
	return innerEdgeColorCount;
}

//
std::vector<std::shared_ptr<Edge>> Neighborhood::OuterEdges() const
{
	return outerEdges;
}

//
EDGES_BY_COLOR Neighborhood::LazyOuterEdgesByColor()
{
	if (outerEdgesByColor.has_value())

		return outerEdgesByColor.value();

	EDGES_BY_COLOR lazyOuterEdgesByColor = Edge::GroupByColor(outerEdges);

	outerEdgesByColor = lazyOuterEdgesByColor;

	return lazyOuterEdgesByColor;
}

//
std::vector<std::shared_ptr<Edge>> Neighborhood::OuterEdges(const COLOR color)
{
	EDGES_BY_COLOR lazyOuterEdgesByColor = LazyOuterEdgesByColor();

	if (lazyOuterEdgesByColor.count(color) == 0)

		return {};

	return lazyOuterEdgesByColor.at(color);
}

//
int Neighborhood::OuterEdgeCount() const
{
	return outerEdgeCount;
}

//
std::set<COLOR> Neighborhood::OuterEdgeColors()
{
	return outerEdgeColors;
}

//
int Neighborhood::OuterEdgeColorCount() const
{
	return outerEdgeColorCount;
}

//
namespace
{
	//
	auto updateDegreeSequence
	(
		const EDGES_BY_COLOR& edgesByColor,

		char element
	)
	{
		return [edgesByColor, element]
		(
			DEGREES_BY_COLOR& degreeSequence,

			const COLOR color
		)
		{
			const std::vector<std::shared_ptr<Edge>>&

				edgesAtColor = edgesByColor.at(color);

			const std::vector<std::shared_ptr<Edge>> inEdges = 
				
				Edge::IncidentsIn(element, edgesAtColor);

			const std::vector<std::shared_ptr<Edge>> outEdges =

				Edge::IncidentsOut(element, edgesAtColor);
			
			int inDegreeAtColor = static_cast<int>(inEdges.size());
			int outDegreeAtColor = static_cast<int>(outEdges.size());

			std::pair<int, int> colorDegrees = {inDegreeAtColor, outDegreeAtColor};

			if (colorDegrees != std::pair{ 0, 0 })
			{
				const std::pair<COLOR, std::pair<int, int>>

					sequenceElement
					{
						color, colorDegrees
					};

				degreeSequence.insert(sequenceElement);
			}

			return degreeSequence;
		};
	}

	//
	auto updateDegreeSequences
	(
		const EDGES_BY_COLOR& edgesByColor,

		const std::set<COLOR>& edgeColors
	)
	{
		return [edgesByColor, edgeColors]
		(
			std::multiset<DEGREES_BY_COLOR> degreeSequences,

			const char element
		)
		{
			const DEGREES_BY_COLOR degreeSequence =

				accumulate
				(
					edgeColors.begin(),
					edgeColors.end(),
					DEGREES_BY_COLOR{},
					updateDegreeSequence(edgesByColor, element)
				);

			degreeSequences.insert(degreeSequence);

			return degreeSequences;
		};
	}
}

//
std::multiset<DEGREES_BY_COLOR> Neighborhood::LazyDegreeSequences()
{
	if (degreeSequences.has_value())

		return degreeSequences.value();

	const std::set<COLOR> tempEdgeColors = EdgeColors();

	const EDGES_BY_COLOR tempEdgesByColor = EdgesByColor();

	const DEGREES_BY_COLOR centralSequence =
	{
		center->DegreesByColor()
	};

	std::multiset<DEGREES_BY_COLOR> lazyDegreeSequences =

		accumulate
		(
			neighbors.begin(),
			neighbors.end(),
			std::multiset<DEGREES_BY_COLOR>{},
			updateDegreeSequences
			(
				tempEdgesByColor,
				tempEdgeColors
			)
		);

	lazyDegreeSequences.insert(centralSequence);

	degreeSequences = lazyDegreeSequences;

	return lazyDegreeSequences;
}

//
std::multiset<DEGREES_BY_COLOR> Neighborhood::DegreeSequences()
{
	return LazyDegreeSequences();
}

//
bool Neighborhood::IsSimilar(const std::shared_ptr<Neighborhood>& otherNeighborhood)
{
	const std::shared_ptr<Vertex> thisCenter = center;

	std::shared_ptr<Vertex> otherCenter = otherNeighborhood->center;

	if (!thisCenter->IsEquivalent(otherCenter))

		return false;

	if (OuterEdgeCount() != otherNeighborhood->OuterEdgeCount())

		return false;

	if (OuterEdgeColors() != otherNeighborhood->OuterEdgeColors())

		return false;

	return true;
}

//
INNER_DIFFERENCE

	Neighborhood::InnerForwardDifference
	(
		const std::shared_ptr<Neighborhood>& otherNeighborhood
	)
		const
	{
		const std::shared_ptr<Vertex> thisCenter = center;

		std::shared_ptr<Vertex> otherCenter = otherNeighborhood->center;

		INNER_DIFFERENCE forwardDifference =

			thisCenter->ForwardDifference(otherCenter);

		return forwardDifference;
	}

//
INNER_DIFFERENCE

	Neighborhood::InnerReverseDifference
	(
		const std::shared_ptr<Neighborhood>& otherNeighborhood
	)
		const
	{
		const std::shared_ptr<Vertex> thisCenter = center;

		std::shared_ptr<Vertex> otherCenter = otherNeighborhood->center;

		INNER_DIFFERENCE reverseDifference = 
			
			thisCenter->ReverseDifference(otherCenter);

		return reverseDifference;
	}

//
namespace
{
	//
	auto identifyVertices =

		[](const char firstVertex, const char secondVertex)
		{
			return std::pair{ firstVertex, secondVertex };
		};

	//
	auto mapVertices
	(
		const std::vector<std::shared_ptr<Edge>>& firstEdges,
		const std::vector<std::shared_ptr<Edge>>& secondEdges,

		const std::function<char(std::shared_ptr<Edge> edge)>& edgePartner
	)
	{
		std::vector<char> firstVertices;
		std::vector<char> secondVertices;

		std::transform
		(
			firstEdges.begin(),
			firstEdges.end(),
			back_inserter(firstVertices),
			edgePartner
		);

		std::transform
		(
			secondEdges.begin(),
			secondEdges.end(),
			back_inserter(secondVertices),
			edgePartner
		);

		std::map<char, char> vertexMap{};

		transform
		(
			firstVertices.begin(),
			firstVertices.end(),
			secondVertices.begin(),
			inserter(vertexMap, vertexMap.end()),
			identifyVertices
		);

		return vertexMap;
	}

	//
	auto updateCentralMap
	(
		const std::shared_ptr<Vertex>& firstCenter,

		const std::shared_ptr<Vertex>& secondCenter
	)
	{
		return [firstCenter, secondCenter]
		(
			std::map<char, char> centralMap, 
			
			const COLOR color			
		)
		{
			const std::map<char, char> inMap =

				mapVertices
				(
					firstCenter->InEdges(color),
					secondCenter->InEdges(color),
					Edge::GetPtrTail
				);

			const std::map<char, char> outMap =

				mapVertices
				(
					firstCenter->OutEdges(color),
					secondCenter->OutEdges(color),
					Edge::GetPtrHead
				);

			centralMap.insert(inMap.begin(), inMap.end());
			centralMap.insert(outMap.begin(), outMap.end());

			return centralMap;
		};
	}
}

//
std::map<char, char> Neighborhood::CentralMap
(
	const std::shared_ptr<Neighborhood>& firstNeighborhood, 
	const std::shared_ptr<Neighborhood>& secondNeighborhood
)
{
	std::shared_ptr<Vertex> firstCenter = firstNeighborhood->center;
	std::shared_ptr<Vertex> secondCenter = secondNeighborhood->center;

	auto initialMap =

		std::map<char, char>
		{
			{ firstCenter->Element(), secondCenter->Element() }
		};

	if (!firstCenter->IsEquivalent(secondCenter))

		return initialMap;

	const std::set<COLOR> centerEdgeColors = firstCenter->EdgeColors();

	const std::map<char, char> centralMap =

		accumulate
		(
			centerEdgeColors.begin(),
			centerEdgeColors.end(),
			initialMap,
			updateCentralMap(firstCenter, secondCenter)
		);

	return centralMap;
}

//
namespace
{
	//
	struct DifferenceResult
	{
		bool Error = false;

		std::vector<Edge> Difference;
	};

	//
	auto updateOuterForwardDifference
	(
		const std::map<char, char>& centralMap,

		const std::vector<std::shared_ptr<Edge>>& rightOuterEdges
	)
	{
		return [centralMap, rightOuterEdges]
		(
			DifferenceResult diffResult,

			const std::shared_ptr<Edge>& leftOuterEdge
		)
		{
			if (diffResult.Error)
			{
				std::cerr << "inconsistent central map" << '\n';

				return diffResult;
			}

			const char leftHead = leftOuterEdge->Head();
			const char leftTail = leftOuterEdge->Tail();

			if
			(
				centralMap.count(leftHead) == 0 || 
				centralMap.count(leftTail) == 0
			)
			{
				diffResult.Error = true;

				return diffResult;
			}

			const char rightHead = centralMap.at(leftHead);
			const char rightTail = centralMap.at(leftTail);

			const COLOR firstColor = leftOuterEdge->Color();

			const auto iterator =

				std::find_if
				(
					rightOuterEdges.begin(),
					rightOuterEdges.end(),
					Edge::GetIsEquivalent(rightTail, rightHead, firstColor)
				);

			if (iterator == rightOuterEdges.end())
			{
				const auto newEdge = Edge(rightTail, rightHead, firstColor);

				diffResult.Difference.push_back(newEdge);
			}

			return diffResult;
		};
	}
}

//
std::vector<Edge> Neighborhood::OuterForwardDifference
(
	const std::shared_ptr<Neighborhood>& rightNeighborhood,

	const std::map<char, char>& centralMap
)
const
{
	const std::vector<std::shared_ptr<Edge>> leftOuterEdges = OuterEdges();

	const std::vector<std::shared_ptr<Edge>>

		rightOuterEdges = rightNeighborhood->OuterEdges();

	const DifferenceResult initialDifference = { false, { } };

	auto differenceResult = 
		
		accumulate
		(
			leftOuterEdges.begin(),
			leftOuterEdges.end(),
			initialDifference,
			updateOuterForwardDifference(centralMap, rightOuterEdges)
		);

	if (differenceResult.Error)
	
		return { };	

	return differenceResult.Difference;
}

//
namespace
{
	//
	auto updateOuterReverseDifference
	(
		const std::map<char, char>& centralMap,

		const std::vector<std::shared_ptr<Edge>>& leftOuterEdges
	)
	{
		return [centralMap, leftOuterEdges]
		(
			DifferenceResult diffResult,

			const std::shared_ptr<Edge>& rightOuterEdge
		)
		{
			if (diffResult.Error)
			{
				std::cerr << "inconsistent system of neighborhoods" << '\n';

				return diffResult;
			}

			const char rightHead = rightOuterEdge->Head();
			const char rightTail = rightOuterEdge->Tail();

			if
			(
				centralMap.count(rightHead) == 0 || 
				centralMap.count(rightTail) == 0
			)
			{
				diffResult.Error = true;

				return diffResult;
			}				

			const char leftHead = centralMap.at(rightHead);
			const char leftTail = centralMap.at(rightTail);

			const COLOR rightColor = rightOuterEdge->Color();

			const auto iterator =

				std::find_if
				(
					leftOuterEdges.begin(),
					leftOuterEdges.end(),
					Edge::GetIsEquivalent(leftTail, leftHead, rightColor)
				);

			if (iterator == leftOuterEdges.end())
			{
				const auto newEdge = Edge(leftTail, leftHead, rightColor);

				diffResult.Difference.push_back(newEdge);
			}

			return diffResult;
		};
	}
}

//
std::vector<Edge> Neighborhood::OuterReverseDifference
(
	const std::shared_ptr<Neighborhood>& rightNeighborhood,

	const std::map<char, char>& centralMap
)
	const
{
	const std::vector<std::shared_ptr<Edge>> theseOuterEdges = OuterEdges();

	const std::vector<std::shared_ptr<Edge>>

		otherOuterEdges = rightNeighborhood->OuterEdges();

	std::map<char, char> reverseCentralMap;

	std::transform
	(
		centralMap.begin(),
		centralMap.end(),
		std::inserter(reverseCentralMap, reverseCentralMap.end()),
		[]
		(
			const std::pair<char, char>& mapping
		)
		{
			return std::pair{ mapping.second, mapping.first };
		}
	);

	const DifferenceResult initialResult = { false, { } };

	auto differenceResult =

		accumulate
		(
			otherOuterEdges.begin(),
			otherOuterEdges.end(),
			initialResult,
			updateOuterReverseDifference(reverseCentralMap, theseOuterEdges)
		);

	if (differenceResult.Error)
	
		return { };	

	return differenceResult.Difference;
}