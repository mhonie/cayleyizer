//
#include "Vertex.h"

//
#include <algorithm>
#include <iterator>
#include <numeric>
#include <stdexcept>

//
Vertex::Vertex()
{
	element = 0;

	edges = {};

	degree = 0;

	isConnected = false;
}

//
Vertex::Vertex(const char element)
{
	this->element = element;

	edges = {};

	degree = 0;

	isConnected = false;
}

// 
Vertex::Vertex(const char element, const std::vector<std::shared_ptr<Edge>>& edges)
{
	this->element = element;

	this->edges = edges;

	degree = static_cast<int>(edges.size());

	isConnected = degree > 0;
}

//
Vertex::Vertex(const Vertex& original) = default;

//
std::function<Vertex(const std::shared_ptr<Vertex>&)>Vertex::CopyPtr = 

	[](const std::shared_ptr<Vertex>& vertex)
	{
		return Vertex(vertex->Element(), vertex->Edges());
	};

//
std::function<std::shared_ptr<Vertex>(const Vertex&)> Vertex::CreatePtr =

	[](const Vertex& vertex)
	{
		return std::make_shared<Vertex>(vertex.Element(), vertex.Edges());
	};

//
std::function<Vertex(char)> Vertex::CreateDisconnected =

	[](const char element) { return Vertex(element); };

//
Vertex::~Vertex() = default;

//
bool Vertex::operator== (const Vertex& otherVertex) const
{
	if (element != otherVertex.element)

		return false;

	std::vector<std::shared_ptr<Edge>> secondEdges = otherVertex.edges;

	if (edges.size() != secondEdges.size()) 

		return false;

	const std::multiset<std::shared_ptr<Edge>> edgeSet(edges.begin(), edges.end());

	const std::multiset<std::shared_ptr<Edge>> secondEdgeSet(secondEdges.begin(), secondEdges.end());

	const bool sameEdges = edgeSet == secondEdgeSet;

	return sameEdges;
}

//
bool Vertex::operator< (const Vertex& otherVertex) const
{
	return element < otherVertex.element;
}

//
auto Vertex::SameDegrees(std::shared_ptr<Vertex>& otherVertex)
{
	return [this, otherVertex](const COLOR color)
	{
		if (this->InDegree(color) != otherVertex->InDegree(color))

			return false;

		if (this->OutDegree(color) != otherVertex->OutDegree(color))

			return false;

		return true;
	};
}

//
bool Vertex::IsEquivalent(std::shared_ptr<Vertex> otherVertex)
{
	if (Degree() != otherVertex->Degree())

		return false;

	if (InDegree() != otherVertex->InDegree())

		return false;

	if (OutDegree() != otherVertex->OutDegree())

		return false;

	if (EdgeColorCount() != otherVertex->EdgeColorCount())

		return false;

	if (InColorCount() != otherVertex->InColorCount())
		
		return false;

	if (OutColorCount() != otherVertex->OutColorCount())

		return false;

	const std::set<COLOR> tempEdgeColors = EdgeColors();

	const std::set<COLOR> secondEdgeColors = otherVertex->EdgeColors();

	if (tempEdgeColors != secondEdgeColors)

		return false;

	const bool isEquivalent =

		std::all_of
		(
			tempEdgeColors.begin(),
			tempEdgeColors.end(),
			SameDegrees(otherVertex)
		);

	return isEquivalent;
}

//
std::function<bool(std::shared_ptr<Vertex>)>

	Vertex::GetIsEquivalent(const std::shared_ptr<Vertex>& vertex)
	{
		return [vertex](const std::shared_ptr<Vertex>& currentVertex)
		{
			return currentVertex->IsEquivalent(vertex);
		};
	}

//
char Vertex::Element() const
{
	return element;
}

//
std::function<char(const Vertex& vertex)> Vertex::GetElement =

	[](const Vertex& vertex) { return vertex.Element(); };

//
std::function<char(std::shared_ptr<Vertex> vertex)> Vertex::GetPtrElement =

	[](const std::shared_ptr<Vertex>& vertex) { return vertex->Element(); };

//
std::vector<std::shared_ptr<Edge>> Vertex::Edges() const
{
	return edges;
}

//
std::vector<std::shared_ptr<Edge>> Vertex::Edges(const char otherElement) const
{
	std::vector<std::shared_ptr<Edge>> adjacentEdges;

	std::copy_if
	(
		edges.begin(),
		edges.end(),
		back_inserter(adjacentEdges),
		Edge::GetIsIncident(otherElement)
	);

	return adjacentEdges;
}

//
EDGES_BY_COLOR Vertex::LazyEdgesByColor()
{
	if (edgesByColor.has_value())

		return edgesByColor.value();

	EDGES_BY_COLOR lazyEdgesByColor = Edge::GroupByColor(edges);

	edgesByColor = lazyEdgesByColor;

	return lazyEdgesByColor;
}

//
std::vector<std::shared_ptr<Edge>> Vertex::Edges(const COLOR color)
{
	const EDGES_BY_COLOR lazyEdgesByColor = LazyEdgesByColor();

	if (lazyEdgesByColor.count(color) == 0)

		return {};

	std::vector<std::shared_ptr<Edge>>

		edgesWithColor = lazyEdgesByColor.at(color);

	return edgesWithColor;
}

namespace 
{
	//
	auto getNeighbor(char element)
	{
		return [element](const std::shared_ptr<Edge>& edge)
		{
			if (edge->IsInEdge(element))

				return edge->Tail();

			if (edge->IsOutEdge(element))

				return edge->Head();

			throw std::logic_error("inconsistent vertex");
		};
	}
}

//
std::set<char> Vertex::LazyNeighbors()
{
	if (neighbors.has_value())

		return neighbors.value();

	std::set<char> lazyNeighbors{};

	std::transform
	(
		edges.begin(),
		edges.end(),
		std::inserter(lazyNeighbors, lazyNeighbors.end()),
		getNeighbor(element)
	);

	neighbors = lazyNeighbors;

	return lazyNeighbors;
}

//
std::set<char> Vertex::Neighbors()
{
	return LazyNeighbors();
}

//
int Vertex::Degree() const
{
	return degree;
}

//
namespace 
{
	//
	auto getDegreeAtColor =

		[](const std::pair<COLOR, std::vector<std::shared_ptr<Edge>>>& edgesAtColor)
		{
			int degree = static_cast<int>(edgesAtColor.second.size());

			const std::pair<COLOR, int> degreeAtColor = {edgesAtColor.first, degree};

			return degreeAtColor;
		};
}

//
std::map<COLOR, int> Vertex::LazyDegreeByColor()
{
	if (degreeByColor.has_value())

		return degreeByColor.value();

	EDGES_BY_COLOR lazyEdgesByColor = LazyEdgesByColor();

	std::map<COLOR, int> lazyDegreeByColor;

	std::transform
	(
		lazyEdgesByColor.begin(),
		lazyEdgesByColor.end(),
		inserter(lazyDegreeByColor, lazyDegreeByColor.end()),
		getDegreeAtColor

	);

	degreeByColor = lazyDegreeByColor;

	return lazyDegreeByColor;
}

//
int Vertex::Degree(const COLOR color)
{
	const std::map<COLOR, int> tempDegreeByColor = LazyDegreeByColor();

	if (tempDegreeByColor.count(color) == 0)

		return {};

	const int degreeAtColor = tempDegreeByColor.at(color);

	return degreeAtColor;
}

//
namespace 
{
	auto combineDegreesByColor
	(
		const std::map<COLOR, int>& inDegreesByColor,
		const std::map<COLOR, int>& outDegreesByColor
	)
	{
		return [inDegreesByColor, outDegreesByColor]

		(DEGREES_BY_COLOR& degreesByColor, const COLOR color)
		{
			auto colorDegrees = std::pair
			{
				inDegreesByColor.at(color),
				outDegreesByColor.at(color)
			};

			if (colorDegrees != std::pair{0, 0})
			
				degreesByColor.insert({ color, colorDegrees });

			return degreesByColor;
		};
	};
}

//
DEGREES_BY_COLOR Vertex::LazyDegreesByColor()
{
	if (degreesByColor.has_value())

		return degreesByColor.value();

	const std::map<COLOR, int> tempInDegreesByColor = InDegreeByColor();
	const std::map<COLOR, int> tempOutDegreesByColor = OutDegreeByColor();

	std::set<COLOR> tempEdgeColors = EdgeColors();

	DEGREES_BY_COLOR lazyDegreesByColor =

		std::accumulate
		(
			std::begin(tempEdgeColors),
			std::end(tempEdgeColors),
			DEGREES_BY_COLOR{},
			combineDegreesByColor
			(
				tempInDegreesByColor,
				tempOutDegreesByColor
			)
		);

	degreesByColor = lazyDegreesByColor;

	return lazyDegreesByColor;
}

//
DEGREES_BY_COLOR Vertex::DegreesByColor()
{
	return LazyDegreesByColor();
}

//
std::pair<int, int> Vertex::Degrees(const COLOR color)
{
	return std::pair{ InDegree(color), OutDegree(color) };
}

//
std::vector<std::shared_ptr<Edge>> Vertex::LazyInEdges()
{
	if (inEdges.has_value())

		return inEdges.value();

	std::vector<std::shared_ptr<Edge>> lazyInEdges;

	std::copy_if
	(
		edges.begin(),
		edges.end(),
		back_inserter(lazyInEdges),
		Edge::GetIsInEdge(element)
	);

	inEdges = lazyInEdges;

	return lazyInEdges;
}

//
std::vector<std::shared_ptr<Edge>> Vertex::InEdges()
{
	return LazyInEdges();
}

//
std::vector<std::shared_ptr<Edge>> Vertex::InEdges(const char otherElement)
{
	std::vector<std::shared_ptr<Edge>> adjacentEdges;

	std::vector<std::shared_ptr<Edge>> tempInEdges = InEdges();

	std::copy_if
	(
		tempInEdges.begin(),
		tempInEdges.end(),
		back_inserter(adjacentEdges),
		Edge::GetIsOutEdge(otherElement)
	);

	return adjacentEdges;
}

//
namespace
{
	//
	auto inEdgesAtColor(char element)
	{
		return [element]
		(
			const std::pair<COLOR, std::vector<std::shared_ptr<Edge>>>& edgesAtColor
		)
		{
			std::vector<std::shared_ptr<Edge>> allEdgesAtColor = 
				
				edgesAtColor.second;

			std::vector<std::shared_ptr<Edge>> inEdgesAtColor;

			std::copy_if
			(
				allEdgesAtColor.begin(),
				allEdgesAtColor.end(),
				back_inserter(inEdgesAtColor),
				Edge::GetIsInEdge(element)
			);

			std::pair<COLOR, std::vector<std::shared_ptr<Edge>>> edgeGroup = 
			{
				edgesAtColor.first,

				inEdgesAtColor
			};

			return edgeGroup;
		};
	}
}

//
EDGES_BY_COLOR Vertex::LazyInEdgesByColor()
{
	if (inEdgesByColor.has_value())

		return inEdgesByColor.value();

	EDGES_BY_COLOR lazyEdgesByColor = LazyEdgesByColor();

	EDGES_BY_COLOR lazyInEdgesByColor;

	std::transform
	(
		lazyEdgesByColor.begin(),
		lazyEdgesByColor.end(),
		inserter(lazyInEdgesByColor, lazyInEdgesByColor.end()),
		inEdgesAtColor(element)
	);

	inEdgesByColor = lazyInEdgesByColor;

	return lazyInEdgesByColor;
}

//
std::vector<std::shared_ptr<Edge>> Vertex::InEdges(const COLOR color)
{
	EDGES_BY_COLOR lazyInEdgesByColor = LazyInEdgesByColor();

	if (lazyInEdgesByColor.count(color) == 0)

		return {};

	return lazyInEdgesByColor.at(color);
}

//
int Vertex::LazyInDegree()
{
	if (inDegree.has_value())

		return inDegree.value();

	const std::vector<std::shared_ptr<Edge>> tempInEdges = InEdges();

	int lazyInDegree = static_cast<int>(tempInEdges.size());

	inDegree = lazyInDegree;

	return lazyInDegree;
}

//
int Vertex::InDegree()
{
	return LazyInDegree();
}

//
std::map<COLOR, int> Vertex::LazyInDegreeByColor()
{
	if (inDegreeByColor.has_value())

		return inDegreeByColor.value();

	EDGES_BY_COLOR lazyInEdgesByColor = LazyInEdgesByColor();

	std::map<COLOR, int> lazyInDegreesByColor;

	std::transform
	(
		lazyInEdgesByColor.begin(),
		lazyInEdgesByColor.end(),
		inserter
		(
			lazyInDegreesByColor, 
			lazyInDegreesByColor.end()
		),
		getDegreeAtColor
	);

	inDegreeByColor = lazyInDegreesByColor;

	return lazyInDegreesByColor;
}

//
int Vertex::InDegree(const COLOR color)
{
	const std::map<COLOR, int>

		lazyInDegreesByColor = LazyInDegreeByColor();

	if (lazyInDegreesByColor.count(color) == 0)

		return 0;

	return lazyInDegreesByColor.at(color);
}

//
std::map<COLOR, int> Vertex::InDegreeByColor()
{
	return LazyInDegreeByColor();
}

//
std::vector<std::shared_ptr<Edge>> Vertex::LazyOutEdges()
{
	if (outEdges.has_value())

		return outEdges.value();

	std::vector<std::shared_ptr<Edge>> outEdgesTemp;

	std::copy_if
	(
		edges.begin(),
		edges.end(),
		back_inserter(outEdgesTemp),
		Edge::GetIsOutEdge(element)
	);

	outEdges = outEdgesTemp;

	return outEdgesTemp;
}

//
std::vector<std::shared_ptr<Edge>> Vertex::OutEdges()
{
	return LazyOutEdges();
}

//
std::vector<std::shared_ptr<Edge>> Vertex::OutEdges(const char otherElement)
{
	std::vector<std::shared_ptr<Edge>> adjacentOutEdges = {};

	std::vector<std::shared_ptr<Edge>> tempOutEdges = OutEdges();

	std::copy_if
	(
		tempOutEdges.begin(),
		tempOutEdges.end(),
		back_inserter(adjacentOutEdges),
		Edge::GetIsInEdge(otherElement)
	);

	return adjacentOutEdges;
}

//
namespace
{
	//
	auto outEdgesAtColor(char element)
	{
		return [element]
		(
			const std::pair<COLOR, std::vector<std::shared_ptr<Edge>>>& edgesAtColor
		)
		{
			std::vector<std::shared_ptr<Edge>>

				allEdgesAtColor = edgesAtColor.second;

			std::vector<std::shared_ptr<Edge>> outEdgesAtColor;

			std::copy_if
			(
				allEdgesAtColor.begin(),
				allEdgesAtColor.end(),
				back_inserter(outEdgesAtColor),
				Edge::GetIsOutEdge(element)
			);

			std::pair<COLOR, std::vector<std::shared_ptr<Edge>>> edgeGroup = 
			{
				edgesAtColor.first,

				outEdgesAtColor
			};

			return edgeGroup;
		};
	}
}

//
EDGES_BY_COLOR Vertex::LazyOutEdgesByColor()
{
	if (outEdgesByColor.has_value())

		return outEdgesByColor.value();

	EDGES_BY_COLOR lazyEdgesByColor = LazyEdgesByColor();

	EDGES_BY_COLOR lazyOutEdgesByColor;

	std::transform
	(
		lazyEdgesByColor.begin(),
		lazyEdgesByColor.end(),
		inserter(lazyOutEdgesByColor, lazyOutEdgesByColor.end()),
		outEdgesAtColor(element)
	);

	outEdgesByColor = lazyOutEdgesByColor;

	return lazyOutEdgesByColor;
}

//
std::vector<std::shared_ptr<Edge>> Vertex::OutEdges(const COLOR color)
{
	const EDGES_BY_COLOR lazyOutEdgesByColor = LazyOutEdgesByColor();

	if (lazyOutEdgesByColor.count(color) == 0)

		return {};

	std::vector<std::shared_ptr<Edge>>

		edgesWithColor = lazyOutEdgesByColor.at(color);

	return edgesWithColor;
}

//
std::map<COLOR, int> Vertex::LazyOutDegreeByColor()
{
	if (outDegreeByColor.has_value())

		return outDegreeByColor.value();

	EDGES_BY_COLOR lazyOutEdgesByColor = LazyOutEdgesByColor();

	std::map<COLOR, int> lazyOutDegreesByColor;

	std::transform
	(
		lazyOutEdgesByColor.begin(),
		lazyOutEdgesByColor.end(),
		inserter(lazyOutDegreesByColor, lazyOutDegreesByColor.end()),
		getDegreeAtColor
	);

	outDegreeByColor = lazyOutDegreesByColor;

	return lazyOutDegreesByColor;
}

//
int Vertex::OutDegree(const COLOR color)
{
	const std::map<COLOR, int>

		lazyOutDegreesByColor = LazyOutDegreeByColor();

	if (lazyOutDegreesByColor.count(color) == 0)

		return 0;

	const int outColorDegree = lazyOutDegreesByColor.at(color);

	return outColorDegree;
}

//
std::map<COLOR, int> Vertex::OutDegreeByColor()
{
	return LazyOutDegreeByColor();
}

//
int Vertex::LazyOutDegree()
{
	if (outDegree.has_value())

		return outDegree.value();

	const std::vector<std::shared_ptr<Edge>> tempOutEdges = OutEdges();

	int lazyOutDegree = static_cast<int>(tempOutEdges.size());

	outDegree = lazyOutDegree;

	return lazyOutDegree;
}

//
int Vertex::OutDegree()
{
	return LazyOutDegree();
}

//
std::set<COLOR> Vertex::LazyEdgeColors()
{
	if (edgeColors.has_value())

		return edgeColors.value();

	EDGES_BY_COLOR lazyEdgesByColor = LazyEdgesByColor();

	std::set<COLOR> lazyEdgeColors;

	std::transform
	(
		lazyEdgesByColor.begin(),
		lazyEdgesByColor.end(),
		std::inserter
		(
			lazyEdgeColors, 
			lazyEdgeColors.end()
		),
		[](const auto& pair) { return pair.first; }
	);

	edgeColors = lazyEdgeColors;

	return lazyEdgeColors;
}

//
std::set<COLOR> Vertex::EdgeColors()
{
	return LazyEdgeColors();
}

//
int Vertex::LazyColorCount()
{
	if (colorCount.has_value())

		return colorCount.value();

	const std::set<COLOR> tempEdgeColors = EdgeColors();

	const int lazyColorCount = 
		
		static_cast<int>(tempEdgeColors.size());

	colorCount = lazyColorCount;

	return lazyColorCount;
}

//
int Vertex::EdgeColorCount()
{
	return LazyColorCount();
}

//
int Vertex::LazyInColorCount()
{
	if (inColorCount.has_value())

		return inColorCount.value();

	const EDGES_BY_COLOR tempInEdgesByColor = LazyInEdgesByColor();

	const int lazyInColorCount =

		static_cast<int>
		(
			std::count_if
			(
				tempInEdgesByColor.begin(),
				tempInEdgesByColor.end(),
				[]
				(
					const std::pair<COLOR, std::vector<std::shared_ptr<Edge>>>& edgesAtColor
				)
				{
					return !edgesAtColor.second.empty();
				}
			)
		);

	inColorCount = lazyInColorCount;

	return lazyInColorCount;
}

//
int Vertex::InColorCount()
{
	return LazyInColorCount();
}

//
int Vertex::LazyOutColorCount()
{
	if (outColorCount.has_value())

		return outColorCount.value();

	const EDGES_BY_COLOR tempOutEdgesByColor = LazyOutEdgesByColor();

	const int lazyOutColorCount =

		static_cast<int>
		(
			std::count_if
			(
				tempOutEdgesByColor.begin(),
				tempOutEdgesByColor.end(),
				[]
				(
					const std::pair<COLOR, std::vector<std::shared_ptr<Edge>>>& edgesAtColor
				)
				{
					return !edgesAtColor.second.empty();
				}
			)
		);

	outColorCount = lazyOutColorCount;

	return lazyOutColorCount;
}

//
int Vertex::OutColorCount()
{
	return LazyOutColorCount();
}

//
bool Vertex::IsConnected() const
{
	return isConnected;
}

//
bool Vertex::IsDisconnected() const
{
	return !isConnected;
}

//
std::function<bool(Vertex)> Vertex::GetIsConnected =

	[](const Vertex& vertex)
	{
		return vertex.IsConnected();
	};

//
std::function<bool(std::shared_ptr<Vertex>)> Vertex::GetPtrIsConnected =

	[](const std::shared_ptr<Vertex>& vertex)
	{
		return vertex->IsConnected();
	};

//
std::function<bool(Vertex)> Vertex::GetIsDisconnected =

	[](const Vertex& vertex)
	{
		return vertex.IsDisconnected();
	};

//
std::function<bool(std::shared_ptr<Vertex>)> Vertex::GetPtrIsDisconnected =

	[](const std::shared_ptr<Vertex>& vertex)
	{
		return vertex->IsDisconnected();
	};

//
std::function<bool(COLOR)> Vertex::UpdateRegularity()
{
	return [this](const COLOR color)
	{
		const int tempInDegree = this->InDegree(color);
		const int tempOutDegree = this->OutDegree(color);

		const bool tempIsRegular = tempInDegree == 1 && tempOutDegree == 1;

		return tempIsRegular;
	};
}

//
bool Vertex::LazyIsRegular()
{
	if (isRegular.has_value())

		return isRegular.value();

	const int tempInDegree = InDegree();

	const int tempOutDegree = OutDegree();

	if (tempInDegree != tempOutDegree)
	{
		isRegular = false;

		return false;
	}

	const int tempInColorCount = InColorCount();

	const int tempColorCount = OutColorCount();

	if (tempInColorCount != tempColorCount)
	{
		isRegular = false;

		return false;
	}

	std::set<COLOR> tempEdgeColors = EdgeColors();

	const bool lazyIsRegular = 
		
		std::all_of
		(
			tempEdgeColors.begin(),
			tempEdgeColors.end(),
			UpdateRegularity()
		);

	isRegular = lazyIsRegular;

	return lazyIsRegular;
}

//
bool Vertex::IsRegular()
{
	return LazyIsRegular();
}

//
std::function<IRREGULARITY(IRREGULARITY, COLOR)> Vertex::UpdateIrregularity()
{
	return [this](IRREGULARITY tempIrregularity, const COLOR color)
	{
		const int inColorDegree = this->InDegree(color);
		const int outColorDegree = this->OutDegree(color);

		const int inIrregularity = static_cast<int>(inColorDegree) - 1;
		const int outIrregularity = static_cast<int>(outColorDegree) - 1;

		if (inIrregularity != 0 || outIrregularity != 0)
		{
			const std::pair<COLOR, std::pair<int, int>>

				colorExcess
				{
					color, {inIrregularity, outIrregularity}
				};

			tempIrregularity.insert(colorExcess);
		}

		return tempIrregularity;
	};
}

//
IRREGULARITY Vertex::LazyIrregularity()
{
	if (irregularity.has_value())

		return irregularity.value();

	std::set<COLOR> tempEdgeColors = EdgeColors();

	IRREGULARITY lazyIrregularity =

		accumulate
		(
			tempEdgeColors.begin(),
			tempEdgeColors.end(),
			IRREGULARITY{},
			UpdateIrregularity()
		);

	irregularity = lazyIrregularity;

	return lazyIrregularity;
}

//
IRREGULARITY Vertex::Irregularity()
{
	return LazyIrregularity();
}

//
auto Vertex::UpdateForwardDifference(const std::shared_ptr<Vertex>& rightVertex)
{
	return [rightVertex, this]
	(
		INNER_DIFFERENCE difference,

		const COLOR color
	)
	{
		const int firstInColorDegree = this->InDegree(color);
		const int secondInColorDegree = rightVertex->InDegree(color);

		int inColorDifference, outColorDifference;

		if (firstInColorDegree > secondInColorDegree)

			inColorDifference = firstInColorDegree - secondInColorDegree;

		else

			inColorDifference = 0;

		const int firstOutColorDegree = this->OutDegree(color);
		const int secondOutColorDegree = rightVertex->OutDegree(color);

		if (firstOutColorDegree > secondOutColorDegree)

			outColorDifference = firstOutColorDegree - secondOutColorDegree;

		else

			outColorDifference = 0;

		if (inColorDifference == 0 && outColorDifference == 0)
		
			return difference;

		difference.insert
		(
			std::pair
			{
				color,
				std::pair{ inColorDifference, outColorDifference }
			}
		);

		return difference;
	};
}

//
INNER_DIFFERENCE Vertex::ForwardDifference(const std::shared_ptr<Vertex>& rightVertex)
{
	const std::set<COLOR> leftEdgeColors = this->EdgeColors();
	const std::set<COLOR> rightEdgeColors = rightVertex->EdgeColors();

	const std::set<COLOR> edgeColors =

		Colors::Distinct(leftEdgeColors, rightEdgeColors);

	INNER_DIFFERENCE forwardDifference =

		accumulate
		(
			edgeColors.begin(),
			edgeColors.end(),
			INNER_DIFFERENCE{},
			UpdateForwardDifference(rightVertex)
		);

	return forwardDifference;
}

//
auto Vertex::UpdateReverseDifference(const std::shared_ptr<Vertex>& rightVertex)
{
	return [this, rightVertex]
	(
		INNER_DIFFERENCE difference,

		const COLOR color
	)
	{
		const int firstInColorDegree = rightVertex->InDegree(color);
		const int secondInColorDegree = this->InDegree(color);

		int inColorDifference, outColorDifference;

		if (firstInColorDegree > secondInColorDegree)

			inColorDifference = firstInColorDegree - secondInColorDegree;

		else

			inColorDifference = 0;

		const int firstOutColorDegree = rightVertex->OutDegree(color);
		const int secondOutColorDegree = this->OutDegree(color);

		if (firstOutColorDegree > secondOutColorDegree)

			outColorDifference = firstOutColorDegree - secondOutColorDegree;

		else

			outColorDifference = 0;

		if (inColorDifference == 0 && outColorDifference == 0)
		
			return difference;

		difference.insert
		(
			std::pair
			{
				color,
				std::pair{ inColorDifference, outColorDifference }
			}
		);

		return difference;
	};
}

//
INNER_DIFFERENCE Vertex::ReverseDifference(const std::shared_ptr<Vertex>& rightVertex)
{
	const std::set<COLOR> leftEdgeColors = this->EdgeColors();
	const std::set<COLOR> rightEdgeColors = rightVertex->EdgeColors();

	const std::set<COLOR> edgeColors =

		Colors::Distinct(leftEdgeColors, rightEdgeColors);

	INNER_DIFFERENCE reverseDifference =

		accumulate
		(
			edgeColors.begin(),
			edgeColors.end(),
			INNER_DIFFERENCE{},
			UpdateReverseDifference(rightVertex)
		);

	return reverseDifference;
}
