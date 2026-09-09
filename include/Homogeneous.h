#ifndef HOMOGENEOUS_H
#define HOMOGENEOUS_H

//
#include "Graph.h"
#include "Neighborhood.h"

//
typedef std::map<std::shared_ptr<Neighborhood>, std::vector<std::shared_ptr<Neighborhood>>>

	REGIONS;

//
typedef std::pair<std::shared_ptr<Neighborhood>, std::shared_ptr<Neighborhood>> 

	NEIGHBORHOOD_PAIR;

//
typedef std::map<NEIGHBORHOOD_PAIR, bool> 

	ISOMORPHISM_TESTS;

//
class Homogeneous
{
public:
	//
	Homogeneous();

	Homogeneous(Homogeneous& original);

	~Homogeneous();

	//
	static std::optional<bool> Test(Graph& graph);

private:
	//
	static bool Compare
	(
		const std::shared_ptr<Neighborhood>& neighborhood,

		const std::shared_ptr<Neighborhood>& otherNeighborhood,

		ISOMORPHISM_TESTS& isomorphismTests
	);

	static std::function<bool(const std::shared_ptr<Neighborhood>&)>

		IsHomogeneous
		(
			const std::shared_ptr<Neighborhood>& neighborhood,

			ISOMORPHISM_TESTS& isomorphismTests
		);

	//
	static bool Compare
	(
		std::shared_ptr<Neighborhood>& neighborhood,

		std::vector<std::shared_ptr<Neighborhood>>& otherNeighborhoods,

		ISOMORPHISM_TESTS& isomorphismTests
	);

	//
	static std::function<bool(std::shared_ptr<Neighborhood>&)>

		AreHomogeneous(REGIONS& regions, ISOMORPHISM_TESTS& isomorphismTests);
};

#endif //HOMOGENEOUS_H
