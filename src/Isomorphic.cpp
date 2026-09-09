//  
#include "Isomorphic.h"  
  
//  
#include <igraph.h>  
#include <map>  
  
//  
Isomorphic::Isomorphic() = default;  
  
//  
Isomorphic::Isomorphic(Isomorphic& original) = default;  
  
//  
Isomorphic::~Isomorphic() = default;  
  
//  
igraph_vector_int_t Squash(std::vector<std::shared_ptr<Edge>> edges)  
{  
    std::map<char, igraph_integer_t> vertexMap;  
  
    igraph_integer_t nextVertex = 0;  
  
    for(const std::shared_ptr<Edge>& edge : edges)  
    {  
        if(vertexMap.find(edge->Tail()) == vertexMap.end())  
        {  
            vertexMap[edge->Tail()] = nextVertex;  
  
            nextVertex++;  
        }  
  
        if(vertexMap.find(edge->Head()) == vertexMap.end())  
        {  
            vertexMap[edge->Head()] = nextVertex;  
  
            nextVertex++;  
        }  
    }  
  
    igraph_vector_int_t squashedEdges;  
  
    igraph_vector_int_init  
    (  
        &squashedEdges,  
  
        edges.size() * 2  
    );  
  
    for(int i = 0; i < edges.size(); i++)  
    {  
        VECTOR(squashedEdges)[2 * i] =  
            vertexMap[edges[i]->Tail()];  
  
        VECTOR(squashedEdges)[2 * i + 1] =  
            vertexMap[edges[i]->Head()];  
    }  
  
    return squashedEdges;  
}  
  
bool Isomorphic::Test  
(  
    std::vector<std::shared_ptr<Edge>> edges,  
  
    std::vector<std::shared_ptr<Edge>> otherEdges  
)  
{  
    igraph_vector_int_t squashedEdges = Squash(edges);  
  
    igraph_vector_int_t otherSquashedEdges = Squash(otherEdges);  
  
    //  
    igraph_t igraph;  
  
    igraph_create  
    (  
        &igraph,  
  
        &squashedEdges,  
  
        0,  
  
        IGRAPH_DIRECTED  
    );  
  
    //  
    igraph_vector_int_t igraphEdgeColors;  
  
    igraph_vector_int_init  
    (  
        &igraphEdgeColors,  
  
        edges.size()  
    );  
  
    for(int i = 0; i < edges.size(); i++)  
    {  
        VECTOR(igraphEdgeColors)[i] =  
            static_cast<igraph_integer_t>(edges[i]->Color());  
    }  
  
    //  
    igraph_t otherIgraph;  
  
    igraph_create  
    (  
        &otherIgraph,  
  
        &otherSquashedEdges,  
  
        0,  
  
        IGRAPH_DIRECTED  
    );  
  
    //  
    igraph_vector_int_t otherIgraphEdgeColors;  
  
    igraph_vector_int_init  
    (  
        &otherIgraphEdgeColors,  
  
        otherEdges.size()  
    );  
  
    for(int i = 0; i < otherEdges.size(); i++)  
    {  
        VECTOR(otherIgraphEdgeColors)[i] =  
            static_cast<igraph_integer_t>(otherEdges[i]->Color());  
    }  
  
    //  
    igraph_bool_t isomorphic;  
  
    igraph_isomorphic_vf2  
    (  
        &igraph,  
        &otherIgraph,  
        NULL,  
        NULL,  
        &igraphEdgeColors,  
        &otherIgraphEdgeColors,  
        &isomorphic,  
        NULL,  
        NULL,  
        NULL,  
        NULL,  
        NULL  
    );  
  
    //  
    igraph_vector_int_destroy(&squashedEdges);  
    igraph_vector_int_destroy(&otherSquashedEdges);  
  
    igraph_vector_int_destroy(&igraphEdgeColors);  
    igraph_vector_int_destroy(&otherIgraphEdgeColors);  
  
    igraph_destroy(&igraph);  
    igraph_destroy(&otherIgraph);  
  
    return isomorphic;  
}  