//
#include "Color.h"
#include "Graph.h"
#include "Vertex.h"
#include "GraphSummary.h"

//
#include "Cyclic.h"
#include "Cayley.h"
#include "Homogeneous.h"

//
#include <iomanip>
#include <iostream>
#include <ostream>
#include <variant>

//
typedef std::pair<COLOR, std::pair<int, int>> EXCESS_EDGES_BY_COLOR;

//
enum GENERAL_OUTPUT_WIDTHS
{
	BANNER_WIDTH = 24,
	HEADER_WIDTH = 13,

	COLOR_WIDTH = 7,

	Irregularity_WIDTH = 3,
	DIFFERENCE_WIDTH = 3
};

//
enum GRAPH_PROPERTY_OUTPUT_WIDTHS
{
	FIELD_NAME = 16,
	FIELD_VALUE = 6
};

//
static const char* INDETERMINATE = "maybe";

//
GraphSummary::GraphSummary() = default;

//
GraphSummary::GraphSummary(const GraphSummary& original) = default;

//
GraphSummary::~GraphSummary() = default;

//
namespace
{
	//
	void printFullBanner()
	{
		const std::string banner(2 * BANNER_WIDTH + HEADER_WIDTH + 2, '=');

		std::cout << banner << '\n' << '\n';
	}

	//
	void printHeader(const char* str)
	{
		const std::string banner(BANNER_WIDTH, '=');

		std::cout << banner << " " << std::left << std::setw(HEADER_WIDTH)

			<< str << " " << banner << '\n' << '\n';
	}

	//
	void printFieldName(const char* str)
	{
		std::cout << '\t' << std::left << std::setw(FIELD_NAME) << str;
	}

	//
	void printBooleanFieldValue(const bool value)
	{
		std::cout << std::left << std::boolalpha

			<< std::setw(FIELD_VALUE) << value << '\n';
	}

	//
	void printStringFieldValue(const char* str)
	{
		std::cout << std::left << std::setw(FIELD_VALUE) << str << '\n';
	}

	//
	void printDisconnected(Graph graph)
	{
		printFieldName("DISCONNECTED: ");

		std::cout << '\n' << '\n';

		const std::set<std::shared_ptr<Vertex>>

			disconnectedVertices = graph.DisconnectedVertices();

		for (const std::shared_ptr<Vertex>& vertex : disconnectedVertices)

			std::cout << '\t' << vertex->Element() << '\n';

		std::cout << '\n';
	}

	//
	void printIrregularity(const EXCESS_EDGES_BY_COLOR& excessEdges)
	{
		const std::string color = Colors::ToString(excessEdges.first);

		const int excessInEdges = excessEdges.second.first;

		const int excessOutEdges = excessEdges.second.second;

		std::cout << '\t' << '\t' << std::left << std::setw(COLOR_WIDTH) << color << ": "

			<< std::right << std::setw(Irregularity_WIDTH) << excessInEdges << " "

			<< std::right << std::setw(Irregularity_WIDTH) << excessOutEdges << '\n';
	}

	//
	void printIrregularities(Graph graph)
	{
		printFieldName("IRREGULARITIES: ");

		std::cout << '\n' << '\n';

		const IRREGULARITIES irregularities = graph.Irregularities();

		for (const auto& [element, colorExcesses] : irregularities)
		{
			for (const auto& excessEdges : colorExcesses)
			{
				std::cout << '\t' << element << '\n';

				printIrregularity(excessEdges);
			}
		}

		std::cout << '\n';
	}

	//
	void printInnerDifference(const EXCESS_EDGES_BY_COLOR& difference)
	{
		const std::string color = Colors::ToString(difference.first);

		const int excessInEdges = difference.second.first;

		const int excessOutEdges = difference.second.second;

		std::cout << '\t' << '\t' << std::left << std::setw(COLOR_WIDTH) << color << ": "

			<< std::right << std::setw(DIFFERENCE_WIDTH) << excessInEdges << " "

			<< std::right << std::setw(DIFFERENCE_WIDTH) << excessOutEdges << '\n';
	}

	//
	void printInnerForwardDifferences(Graph graph)
	{
		INNER_DIFFERENCES forwardDifferences = graph.InnerForwardDifferences();

		for (const auto& [elements, diffByColor] : forwardDifferences)
		{
			std::cout << "\t" << elements.first << " - " << elements.second << '\n';

			for (const auto& difference : diffByColor)

				printInnerDifference(difference);
		}

		std::cout << '\n';
	}

	//
	void printInnerReverseDifferences(Graph graph)
	{
		INNER_DIFFERENCES reverseDifferences = graph.InnerReverseDifferences();

		for (const auto& [elements, diffByColor] : reverseDifferences)
		{
			std::cout << "\t" << elements.second << " - " << elements.first << '\n';

			for (const auto& difference : diffByColor)

				printInnerDifference(difference);
		}

		std::cout << '\n';
	}

	//
	void printInnerDifferences(Graph& graph)
	{
		if (graph.InnerForwardDifferences().empty())
		{
			printFieldName("NO INNER FORWARD DIFFERENCES");

			std::cout << '\n' << '\n';
		}
		else
		{
			printFieldName("INNER FORWARD DIFFERENCES: ");

			std::cout << '\n' << '\n';

			printInnerForwardDifferences(graph);
		}

		if (graph.InnerReverseDifferences().empty())
		{
			printFieldName("NO INNER REVERSE DIFFERENCES");

			std::cout << '\n' << '\n';
		}
		else
		{
			printFieldName("INNER REVERSE DIFFERENCES: ");

			std::cout << '\n' << '\n';

			printInnerReverseDifferences(graph);
		}
	}

	//
	void printEdge(const Edge& edge)
	{
		std::cout << '\t' << '\t' << edge.Head() << " - " << edge.Tail() << '\n';
	}

	//
	void printOuterForwardDifferences(Graph& graph)
	{
		OUTER_DIFFERENCES differences = graph.OuterForwardDifferences();

		for (const auto& [elements, edgeDiff] : differences)
		{
			std::cout << "\t" << elements.first << " - " << elements.second << '\n';

			for (const Edge& edge : edgeDiff)

				printEdge(edge);
		}

		std::cout << '\n';
	}

	//
	void printOuterReverseDifferences(Graph& graph)
	{
		OUTER_DIFFERENCES differences = graph.OuterReverseDifferences();

		for (const auto& [elements, edgeDiff] : differences)
		{
			std::cout << "\t" << elements.second << " - " << elements.first << '\n';

			for (const Edge& edge : edgeDiff)

				printEdge(edge);
		}

		std::cout << '\n';
	}

	//
	void printOuterDifferences(Graph& graph)
	{
		if (graph.OuterForwardDifferences().empty())
		{
			printFieldName("NO OUTER FORWARD DIFFERENCES");

			std::cout << '\n' << '\n';
		}
		else
		{
			printFieldName("OUTER FORWARD DIFFERENCES: ");

			std::cout << '\n' << '\n';

			printOuterForwardDifferences(graph);
		}

		if (graph.OuterReverseDifferences().empty())
		{
			printFieldName("NO OUTER REVERSE DIFFERENCES");

			std::cout << '\n' << '\n';
		}
		else
		{
			printFieldName("OUTER REVERSE DIFFERENCES: ");

			std::cout << '\n' << '\n';

			printOuterReverseDifferences(graph);
		}
	}

	//
	void printGraphDiscrepancies(Graph& graph)
	{
		printHeader("DISCREPANCIES");

		printDisconnected(graph);

		printIrregularities(graph);

		printFieldName("DIFFERENCES: ");

		std::cout << '\n' << '\n';

		printInnerDifferences(graph);

		printOuterDifferences(graph);
	}

	//
	void printIsConnected(Graph graph)
	{
		printFieldName("IsConnected: ");

		std::optional<bool> isConnected = graph.IsConnected();

		if (isConnected.has_value())

			printBooleanFieldValue(isConnected.value());

		else

			printStringFieldValue(INDETERMINATE);
	}

	//
	void printIsRegular(Graph graph)
	{
		printFieldName("IsRegular: ");

		std::optional<bool> isRegular = graph.IsRegular();

		if (isRegular.has_value())

			printBooleanFieldValue(isRegular.value());

		else

			printBooleanFieldValue(false);
	}

	//
	void printIsHomogeneous(Graph& graph)
	{
		printFieldName("IsHomogeneous: ");

		std::optional<bool> isHomogeneous = Homogeneous::Test(graph);

		if (isHomogeneous.has_value())

			printBooleanFieldValue(isHomogeneous.value());

		else

			printStringFieldValue(INDETERMINATE);

		std::cout << '\n';
	}

	//
	void printIsCayley(Graph graph)
	{
		printFieldName("IsCayley: ");

		std::optional<bool> isCayley = Cayley::Test(graph);

		if (isCayley.has_value())

			printBooleanFieldValue(isCayley.value());

		else

			printStringFieldValue(INDETERMINATE);

		std::cout << '\n';
	}

	//
	void printIsEmpty(const Graph& graph)
	{
		printFieldName("IsEmpty: ");

		printBooleanFieldValue(graph.IsEmpty());

		std::cout << '\n';
	}

	//
	void printIsSingleton(Graph& graph)
	{
		printFieldName("IsSingleton: ");

		printBooleanFieldValue(graph.IsSingleton());

		std::cout << '\n';
	}

	//
	void printIsCyclic(Graph& graph)
	{
		printFieldName("IsCyclic: ");

		const bool isCyclic = Cyclic::Test(graph);

		printBooleanFieldValue(isCyclic);

		std::cout << '\n';
	}

	//
	void printGraphProperties(Graph& graph)
	{
		printHeader("GRAPH SUMMARY");

		printIsEmpty(graph);

		printIsSingleton(graph);

		printIsCyclic(graph);

		printIsConnected(graph);

		printIsRegular(graph);

		printIsHomogeneous(graph);

		printIsCayley(graph);
	}
}

//
void GraphSummary::Print(Graph& graph)
{
	printFullBanner();

	printGraphDiscrepancies(graph);

	printGraphProperties(graph);
}
