//
#include "Homogeneous.h"
#include "Isomorphic.h"

//
#include <algorithm>
#include <iterator>
#include <numeric>
#include <optional>

//
Homogeneous::Homogeneous() = default;

//
Homogeneous::Homogeneous(Homogeneous& original) = default;

//
Homogeneous::~Homogeneous() = default;

//
bool Homogeneous::Compare
(
	const std::shared_ptr<Neighborhood>& neighborhood,

	const std::shared_ptr<Neighborhood>& otherNeighborhood,

	ISOMORPHISM_TESTS& isomorphismTests
)
{
	auto testIterator = isomorphismTests.find({ neighborhood, otherNeighborhood });

	if (testIterator != isomorphismTests.end())
	
		return isomorphismTests[{ neighborhood, otherNeighborhood}];	

	testIterator = isomorphismTests.find({ otherNeighborhood, neighborhood });

	if (testIterator != isomorphismTests.end())
		
		return isomorphismTests[{ otherNeighborhood, neighborhood }];
	
	// TODO: should this be in isSimilarTo?
	const std::multiset<DEGREES_BY_COLOR> degreeSequence = neighborhood->DegreeSequences();

	const std::multiset<DEGREES_BY_COLOR> otherDegreeSequence = otherNeighborhood->DegreeSequences();

	bool areIsomorphic;

	if (degreeSequence == otherDegreeSequence)
	{
		std::vector<std::shared_ptr<Edge>> edges = neighborhood->Edges();
 
		std::vector<std::shared_ptr<Edge>> otherEdges = neighborhood->Edges();

		// TODO: compare unique edges
		
		// std::vector<std::shared_ptr<Edge>> uniqueEdges;
		// 
		// std::set_difference
		// (
		// 	edges.begin(),
		// 	edges.end(),
		// 	otherEdges.begin(),
		// 	otherEdges.end(),
		// 	std::back_inserter(uniqueEdges)
		// );
		// 
		// std::vector<std::shared_ptr<Edge>> otherUniqueEdges;
		// 
		// std::set_difference
		// (
		// 	otherEdges.begin(),
		// 	otherEdges.end(), 
		// 	edges.begin(),
		// 	edges.end(),
		// 	std::back_inserter(otherUniqueEdges)
		// );

		areIsomorphic = Isomorphic::Test(edges, otherEdges);
	}

	areIsomorphic = false;

	isomorphismTests.insert(std::pair<NEIGHBORHOOD_PAIR, bool>
	{
		{ otherNeighborhood, neighborhood },

		areIsomorphic
	});

	return areIsomorphic;
}

//
namespace
{
	//
	auto isSimilarTo(std::shared_ptr<Neighborhood>& neighborhood)
	{
		return [neighborhood]
		(
			const std::shared_ptr<Neighborhood>& otherNeighborhood
		)
		{
			return neighborhood->IsSimilar(otherNeighborhood);
		};
	}
}

//
std::function<bool(const std::shared_ptr<Neighborhood>&)>

	Homogeneous::IsHomogeneous
	(
		const std::shared_ptr<Neighborhood>& neighborhood,

		ISOMORPHISM_TESTS& isomorphismTests
	)
	{
		return [neighborhood, &isomorphismTests]
		(
			const std::shared_ptr<Neighborhood>& otherNeighborhood
		)
		{
			const bool areIsomorphic = 
				
				Compare(neighborhood, otherNeighborhood, isomorphismTests);

			return areIsomorphic;
		};
	}

//
bool Homogeneous::Compare
(
	std::shared_ptr<Neighborhood>& neighborhood,

	std::vector<std::shared_ptr<Neighborhood>>& otherNeighborhoods,

	ISOMORPHISM_TESTS& isomorphismTests
)
{
	const bool areSimilar =

		std::all_of
		(
			std::begin(otherNeighborhoods),
			std::end(otherNeighborhoods),
			isSimilarTo(neighborhood)
		);

	if (!areSimilar)

		return false;

	const bool areIsomorphic =

		std::all_of
		(
			std::begin(otherNeighborhoods),
			std::end(otherNeighborhoods),
			IsHomogeneous(neighborhood, isomorphismTests)
		);

	return areIsomorphic;
}

//
namespace
{
	auto updateRegions(Graph& graph)
	{
		return[graph]
		(
			REGIONS& regions,

			const std::shared_ptr<Neighborhood>& neighborhood
		)
		{
			std::set<char> neighbors = neighborhood->Neighbors();

			std::vector<std::shared_ptr<Neighborhood>> otherNeighborhoods;

			std::transform
			(
				std::begin(neighbors),
				std::end(neighbors),
				std::inserter
				(
					otherNeighborhoods,
					std::begin(otherNeighborhoods)
				),
				Graph::GetNeighborhoodsOf(graph)
			);

			auto region = 
			
				std::pair<std::shared_ptr<Neighborhood>, std::vector<std::shared_ptr<Neighborhood>>>
			
					{ neighborhood, otherNeighborhoods };

			regions.insert(region);

			return regions;
		};
	}
}

//
std::function<bool(std::shared_ptr<Neighborhood>&)>

	Homogeneous::AreHomogeneous
	(
		REGIONS& regions, 
		
		ISOMORPHISM_TESTS& isomorphismTests
	)
	{
		return [regions, &isomorphismTests]
		(
			std::shared_ptr<Neighborhood>& neighborhood
		)
		{
			std::vector<std::shared_ptr<Neighborhood>> otherNeighborhoods = 
				
				regions.at(neighborhood);

			const bool isHomogeneous = 
				
				Compare(neighborhood, otherNeighborhoods, isomorphismTests);

			return isHomogeneous;
		};
	}

//
std::optional<bool> Homogeneous::Test(Graph& graph)
{
	std::vector<std::shared_ptr<Neighborhood>> neighborhoods = 
		
		graph.Neighborhoods();

	REGIONS regions = 

		std::accumulate
		(
			neighborhoods.begin(),
			neighborhoods.end(),
			REGIONS { },
			updateRegions(graph)
		);

	ISOMORPHISM_TESTS isomorphismTests = {};

	const bool isHomogeneous =

		std::all_of
		(
			neighborhoods.begin(),
			neighborhoods.end(),
			AreHomogeneous(regions, isomorphismTests)
		);

	if (isHomogeneous)

		return std::nullopt;

	return true;
}
