/*
sudo c++ -O3 -Wall -shared -std=c++17 -fPIC \
  -I../include \
  $(python3 -m pybind11 --includes) \
  Color.cpp Edge.cpp Vertex.cpp Neighborhood.cpp \
  Graph.cpp GraphBuilder.cpp \
  GraphUpdater.cpp GraphSummary.cpp ../python/Bindings.cpp \
  -o cayleyizer.so
*/

//
#include "Color.h"
#include "Edge.h"
#include "Vertex.h"
#include "Graph.h"
#include "GraphBuilder.h"
#include "GraphUpdater.h"
#include "GraphSummary.h"

//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>

//
PYBIND11_MODULE(cayleyizer, mod)
{
    pybind11::enum_<COLOR>(mod, "COLOR")
        .value("RED", COLOR::RED)
        .value("BLUE", COLOR::BLUE)
        .value("GREEN", COLOR::GREEN)
        .value("ORANGE", COLOR::ORANGE)
        .value("VIOLET", COLOR::VIOLET)
        .value("YELLOW", COLOR::YELLOW)
        .export_values();

    pybind11::class_<Edge, std::shared_ptr<Edge>>(mod, "Edge")
        .def(pybind11::init<const char, const char, const COLOR>())
        .def("head", &Edge::Head)
        .def("tail", &Edge::Tail)
        .def("color", &Edge::Color);
    
    pybind11::class_<Vertex, std::shared_ptr<Vertex>>(mod, "Vertex")
        .def(pybind11::init<const char>())
        .def("element", &Vertex::Element);
    
    pybind11::class_<Graph>(mod, "Graph")
        .def("vertices", &Graph::Vertices)
        .def("vertexCount", &Graph::VertexCount)
        .def("edges", &Graph::Edges)
        .def("edgeCount", &Graph::EdgeCount)
        .def("disconnectedVertices", &Graph::DisconnectedVertices)
        .def("irregularities", &Graph::Irregularities)
        .def("innerForwardDifferences", &Graph::InnerForwardDifferences)
        .def("innerReverseDifferences", &Graph::InnerReverseDifferences)
        .def("outerForwardDifferences", &Graph::OuterForwardDifferences)
        .def("outerReverseDifferences", &Graph::OuterReverseDifferences);
    
    pybind11::class_<GraphBuilder>(mod, "GraphBuilder")
        .def(pybind11::init<>())
        .def_static
        (
            "build",
    
            pybind11::overload_cast<const std::vector<Edge>&>
            (
                &GraphBuilder::Build
            )
        );
    
    pybind11::class_<GraphUpdater>(mod, "GraphUpdater")
        .def(pybind11::init<const Graph&>())
        .def("addEdge", &GraphUpdater::AddEdge)
        .def("addEdges", &GraphUpdater::AddEdges)
        .def("deleteEdge", &GraphUpdater::DeleteEdge)
        .def("deleteEdges", &GraphUpdater::DeleteEdges)
        .def("flipEdge", &GraphUpdater::FlipEdge)
        .def("flipEdges", &GraphUpdater::FlipEdges)
        .def("addVertex", &GraphUpdater::AddVertex)
        .def("addVertices", &GraphUpdater::AddVertices)
        .def("deleteVertex", &GraphUpdater::DeleteVertex)
        .def("deleteVertices", &GraphUpdater::DeleteVertices);
    
    pybind11::class_<GraphSummary>(mod, "GraphSummary")
        .def(pybind11::init<>())
        .def_static("print", &GraphSummary::Print);
}

