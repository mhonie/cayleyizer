#ifndef ISOMORPHIC_H
#define ISOMORPHIC_H

//
#include "Graph.h"
#include "Neighborhood.h"

//
typedef std::map<std::shared_ptr<Neighborhood>, std::vector<std::shared_ptr<Neighborhood>>>

	REGIONS;

//
typedef std::map<std::pair<std::shared_ptr<Neighborhood>, std::shared_ptr<Neighborhood>>, bool>

	ISOMORPHISM_TESTS;

//
class Isomorphic
{
public:
	//
	Isomorphic();

	Isomorphic(Isomorphic& original);

	~Isomorphic();

	//
	static bool Test
	(
		std::vector<std::shared_ptr<Edge>> edges, 
		
		std::vector<std::shared_ptr<Edge>> otherEdges
	);

private:
	
};

#endif //ISOMORPHIC_H
