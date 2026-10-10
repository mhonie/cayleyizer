//
#include "Cayley.h"
#include "Graph.h"
#include "Homogeneous.h"

//
Cayley::Cayley() = default;

Cayley::Cayley(Cayley& original) = default;

Cayley::~Cayley() = default;

//
std::optional<bool> Cayley::Test(Graph& graph)
{
    const std::optional<bool> isConnected = graph.IsConnected();

    const std::optional<bool> isRegular = graph.IsRegular();

    if (isConnected == false || isRegular == false)

        return false;

    const std::optional<bool> isHomogeneous = Homogeneous::Test(graph);

    if (isHomogeneous == false)

        return false;

    if (isConnected == true && isRegular == true && isHomogeneous == true)

        return true;

    return std::nullopt;
}