//
#include "Edge.h"

//
#include <algorithm>
#include <iterator>
#include <numeric>

//
Edge::Edge()
{
	tail = 0;
	head = 0;

	color = COLOR::NONE;
}

//
Edge::Edge(const char tail, const char head, const COLOR color)
{
	this->tail = tail;
	this->head = head;

	this->color = color;
}

//
Edge::Edge(const char tail, const char head)
{
	this->tail = tail;
	this->head = head;

	color = COLOR::NONE;
}

//
Edge::Edge(const Edge& originalEdge) = default;

//
Edge::~Edge() = default;

//
std::function<std::shared_ptr<Edge>(const Edge&)> Edge::CreatePtr =

	[](const Edge& edge) { return std::make_shared<Edge>(edge); };

//
std::function<Edge(const std::shared_ptr<Edge>&)> Edge::CopyPtr =

	[](const std::shared_ptr<Edge>& edge) { return Edge(*edge); };

//
Edge Edge::Flip(const std::shared_ptr<Edge>& edge)
{
	return
	{
		edge->Head(),
		edge->Tail(),
		edge->Color()
	};
}

//
std::function<bool(std::shared_ptr<Edge>)>

	Edge::GetEquals(const std::shared_ptr<Edge>& otherEdge)
	{
		return [otherEdge](const std::shared_ptr<Edge>& edge) 
		{ 
			return edge == otherEdge; 
		};
	}

//
std::function<bool(std::shared_ptr<Edge> edge)>

	Edge::GetIsEquivalent(const char tail, const char head, const COLOR color)
	{
		return [tail, head, color](const std::shared_ptr<Edge>& edge)
		{
			return edge->tail == tail
				&& edge->head == head
				&& edge->color == color;
		};
	}

//
char Edge::Head() const
{
	return head;
}

//
std::function<char(const Edge& edge)> Edge::GetHead =

	[](const Edge& edge) { return edge.Head(); };

//
std::function<char(std::shared_ptr<Edge> edge)> Edge::GetPtrHead = 

	[](const std::shared_ptr<Edge>& edge) { return edge->Head(); };

//
char Edge::Tail() const
{
	return tail;
}

//
std::function<char(const Edge& edge)> Edge::GetTail =

	[](const Edge& edge) { return edge.Tail(); };

//
std::function<char(std::shared_ptr<Edge> edge)> Edge::GetPtrTail = 

	[](const std::shared_ptr<Edge>& edge) { return edge->Tail(); };

//
COLOR Edge::Color() const
{
	return color;
}

//
std::function<COLOR(const Edge& edge)> Edge::GetColor =

	[](const Edge& edge) { return edge.Color(); };

//
std::function<COLOR(std::shared_ptr<Edge>)> Edge::GePtrColor = 

	[](const std::shared_ptr<Edge>& edge) { return edge->Color(); };

//
bool Edge::IsInEdge(const char element) const
{
	return head == element;
}

//
std::function<bool(std::shared_ptr<Edge>)> Edge::GetIsInEdge(char element)
{
	return [element](const std::shared_ptr<Edge>& edge)
	{
		return edge->IsInEdge(element);
	};
}

//
bool Edge::IsOutEdge(const char element) const
{
	return tail == element;
}

//
std::function<bool(std::shared_ptr<Edge>)> Edge::GetIsOutEdge(char element)
{
	return [element](const std::shared_ptr<Edge>& edge)
	{
		return edge->IsOutEdge(element);
	};
}

//
bool Edge::IsIncident(const char element) const
{
	return IsInEdge(element) || IsOutEdge(element);
}

//
std::function<bool(std::shared_ptr<Edge>)> Edge::GetIsIncident(char element)
{
	return [element](const std::shared_ptr<Edge>& edge)
	{
		return edge->IsIncident(element);
	};
}

//
std::function<bool(std::shared_ptr<Edge>)> Edge::GetNotIncident(char element)
{
	return [element](const std::shared_ptr<Edge>& edge)
	{
		return !edge->IsIncident(element);
	};
}

//
std::vector<std::shared_ptr<Edge>>

	Edge::Incidents(const char element, const std::vector<std::shared_ptr<Edge>>& edges)
	{
		std::vector<std::shared_ptr<Edge>> incidents;

		std::copy_if
		(
			edges.begin(),
			edges.end(),
			back_inserter(incidents),
			GetIsIncident(element)
		);

		return incidents;
	}

//
//
std::function<std::vector<std::shared_ptr<Edge>>(std::vector<std::shared_ptr<Edge>>, char)>

	Edge::UpdateIncidents(const std::vector<std::shared_ptr<Edge>>& edges)
	{
		return [edges]
		(
			std::vector<std::shared_ptr<Edge>> incidents,
			
			const char element
		)
		{
			const std::vector<std::shared_ptr<Edge>>

				newIncidents = Incidents(element, edges);

			for (const std::shared_ptr<Edge>& newIncident : newIncidents)
			{
				if
				(
					std::find
					(
						incidents.begin(),
						incidents.end(),
						newIncident
					) 
						== incidents.end()
				)
				{
					incidents.push_back(newIncident);
				}
			}

			return incidents;
		};
	}
	
//
std::vector<std::shared_ptr<Edge>> Edge::Incidents
(
	std::set<char> elements,

	const std::vector<std::shared_ptr<Edge>>& edges
)
{
	std::vector<std::shared_ptr<Edge>> incidents = 

		std::accumulate
		(
			std::begin(elements),
			std::end(elements),
			std::vector<std::shared_ptr<Edge>>(),
			UpdateIncidents(edges)
		);

	return incidents;
}

//
std::vector<std::shared_ptr<Edge>>

	Edge::IncidentsIn(const char element, const std::vector<std::shared_ptr<Edge>>& edges)
	{
		std::vector<std::shared_ptr<Edge>> inEdges;

		std::copy_if
		(
			edges.begin(),
			edges.end(),
			std::back_inserter(inEdges),
			GetIsInEdge(element)
		);

		return inEdges;
	}

//
std::vector<std::shared_ptr<Edge>>

	Edge::IncidentsOut(const char element, const std::vector<std::shared_ptr<Edge>>& edges)
	{
		std::vector<std::shared_ptr<Edge>> outEdges;

		std::copy_if
		(
			edges.begin(),
			edges.end(),
			std::back_inserter(outEdges),
			GetIsOutEdge(element)
		);

		return outEdges;
	}

//
std::set<char> Edge::GetElements(std::vector<Edge> edges)
{
	std::set<char> headElements;

	std::transform
	(
		edges.begin(),
		edges.end(),
		std::inserter(headElements, std::end(headElements)),
		GetHead
	);

	std::set<char> tailElements;

	std::transform
	(
		edges.begin(),
		edges.end(),
		std::inserter(tailElements, std::end(tailElements)),
		GetTail
	);

	std::set<char> elements;

	std::set_union
	(
		std::begin(headElements),
		std::end(headElements),
		std::begin(tailElements),
		std::end(tailElements),
		inserter(elements, std::end(elements))
	);

	return elements;
}

//
std::set<char> Edge::GetElementsPtr(std::vector<std::shared_ptr<Edge>> edges)
{
	std::set<char> headElements;

	std::transform
	(
		edges.begin(),
		edges.end(),
		std::inserter(headElements, std::end(headElements)),
		GetPtrHead
	);

	std::set<char> tailElements;

	std::transform
	(
		edges.begin(),
		edges.end(),
		std::inserter(tailElements, std::end(tailElements)),
		GetPtrTail
	);

	std::set<char> elements;

	std::set_union
	(
		std::begin(headElements),
		std::end(headElements),
		std::begin(tailElements),
		std::end(tailElements),
		inserter(elements, std::end(elements))
	);

	return elements;
}

//
bool Edge::IsColor(const COLOR otherColor) const
{
	return color == otherColor;
}

//
std::set<COLOR> Edge::DistinctColors(const std::vector<std::shared_ptr<Edge>>& edges)
{
	std::vector<COLOR> edgeColors{};

	std::transform
	(
		edges.begin(),
		edges.end(),
		std::back_inserter(edgeColors),
		GePtrColor
	);

	std::set<COLOR> distinctColors
	(
		std::make_move_iterator(edgeColors.begin()),
		std::make_move_iterator(edgeColors.end())
	);

	return distinctColors;
}

//
namespace
{
	//
	auto updateEdgesByColor = []
	(
		std::map<COLOR, std::vector<std::shared_ptr<Edge>>>& edgesByColor,

		const std::shared_ptr<Edge>& edge
	)
	{
		const COLOR color = edge->Color();
		if
		(
			const auto iterator = edgesByColor.find(color);

			iterator == edgesByColor.end()
		)
		{
			const std::pair<COLOR, std::vector<std::shared_ptr<Edge>>>

				colorEdge = { color, {edge} };

			edgesByColor.insert(colorEdge);
		}
		else
		{
			std::vector<std::shared_ptr<Edge>>

				edgesAtColor = edgesByColor[color];

			edgesByColor[color].push_back(edge);
		}

		return edgesByColor;
	};
}

//
std::map<COLOR, std::vector<std::shared_ptr<Edge>>>

	Edge::GroupByColor(const std::vector<std::shared_ptr<Edge>>& edges)
	{
		std::map<COLOR, std::vector<std::shared_ptr<Edge>>>

			edgesByColor =

				std::accumulate
				(
					edges.begin(),
					edges.end(),
					std::map<COLOR, std::vector<std::shared_ptr<Edge>>>(),
					updateEdgesByColor
				);

		return edgesByColor;
	}
