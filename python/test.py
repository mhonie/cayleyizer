import cayleyizer


def find_edge(graph, tail, head, color):
    for edge in graph.edges():
        if (
            edge.tail() == tail
            and edge.head() == head
            and edge.color() == color
        ):
            return edge

    raise Exception("edge not found")


def find_vertex(graph, element):
    for vertex in graph.vertices():
        if vertex.element() == element:
            return vertex

    raise Exception("vertex not found")


#
# Build original graph
#

edges = [
    cayleyizer.Edge('0', '1', cayleyizer.COLOR.RED),
    cayleyizer.Edge('1', '2', cayleyizer.COLOR.RED),
    cayleyizer.Edge('2', '3', cayleyizer.COLOR.RED),
    cayleyizer.Edge('3', '4', cayleyizer.COLOR.RED),
    cayleyizer.Edge('4', '0', cayleyizer.COLOR.RED),

    cayleyizer.Edge('5', '6', cayleyizer.COLOR.RED),
    cayleyizer.Edge('6', '7', cayleyizer.COLOR.RED),
    cayleyizer.Edge('7', '8', cayleyizer.COLOR.RED),
    cayleyizer.Edge('8', '9', cayleyizer.COLOR.RED),
    cayleyizer.Edge('9', '5', cayleyizer.COLOR.RED),

    cayleyizer.Edge('0', '5', cayleyizer.COLOR.YELLOW),
    cayleyizer.Edge('1', '6', cayleyizer.COLOR.YELLOW),
    cayleyizer.Edge('2', '7', cayleyizer.COLOR.YELLOW),
    cayleyizer.Edge('3', '8', cayleyizer.COLOR.YELLOW),
    cayleyizer.Edge('4', '9', cayleyizer.COLOR.YELLOW),

    cayleyizer.Edge('5', '0', cayleyizer.COLOR.YELLOW),
    cayleyizer.Edge('6', '1', cayleyizer.COLOR.YELLOW),
    cayleyizer.Edge('7', '2', cayleyizer.COLOR.YELLOW),
    cayleyizer.Edge('8', '3', cayleyizer.COLOR.YELLOW),
    cayleyizer.Edge('9', '4', cayleyizer.COLOR.YELLOW)
]

graph = cayleyizer.GraphBuilder.build(edges)



print("vertex count:", graph.vertexCount(), flush=True)
print("edge count:", graph.edgeCount(), flush=True)

assert graph.vertexCount() == 10
assert graph.edgeCount() == 20

print("\nORIGINAL GRAPH")
cayleyizer.GraphSummary.print(graph)


#
# Add edge
#

new_edge = cayleyizer.Edge(
    '0',
    '2',
    cayleyizer.COLOR.BLUE
)

graph = cayleyizer.GraphUpdater(graph).addEdge(new_edge)

assert graph.edgeCount() == 21

print("\nAFTER ADD EDGE")
cayleyizer.GraphSummary.print(graph)


#
# Flip edge
#

edge = find_edge(
    graph,
    '0',
    '2',
    cayleyizer.COLOR.BLUE
)

graph = cayleyizer.GraphUpdater(graph).flipEdge(edge)

assert find_edge(
    graph,
    '2',
    '0',
    cayleyizer.COLOR.BLUE
)

print("\nAFTER FLIP EDGE")
cayleyizer.GraphSummary.print(graph)


#
# Delete edge
#

edge = find_edge(
    graph,
    '2',
    '0',
    cayleyizer.COLOR.BLUE
)

graph = cayleyizer.GraphUpdater(graph).deleteEdge(edge)

assert graph.edgeCount() == 20

print("\nAFTER DELETE EDGE")
cayleyizer.GraphSummary.print(graph)


#
# Add disconnected vertex
#

graph = cayleyizer.GraphUpdater(graph).addVertex('a')

assert graph.vertexCount() == 11

vertex = find_vertex(graph, 'a')

print("\nAFTER ADD VERTEX")
cayleyizer.GraphSummary.print(graph)


#
# Delete disconnected vertex
#

graph = cayleyizer.GraphUpdater(graph).deleteVertex(vertex)

assert graph.vertexCount() == 10

print("\nAFTER DELETE VERTEX")
cayleyizer.GraphSummary.print(graph)


print("\nALL GRAPH UPDATER TESTS PASSED")