#include "ArcGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"

#include <deque>
#include <functional>
#include <iostream>
#include <queue>

void BFS(const IGraph& graph, int vertex, std::vector<bool>& visited, std::function<void(int)>& func) {
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty()) {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);

        for (int nextVertex : graph.GetNextVertices(currentVertex)) {
            if (!visited[nextVertex]) {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void BFS(const IGraph& graph, std::function<void(int)> func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) BFS(graph, i, visited, func);
    }
}

void DFS(const IGraph& graph, int vertex, std::vector<bool>& visited, std::function<void(int)>& func) {
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) DFS(graph, nextVertex, visited, func);
    }
}

void DFS(const IGraph& graph, std::function<void(int)> func) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) DFS(graph, i, visited, func);
    }
}

void topologicalSort(const IGraph& graph, int vertex, std::vector<bool>& visited, std::deque<int>& sorted) {
    visited[vertex] = true;

    for (int nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) topologicalSort(graph, nextVertex, visited, sorted);
    }

    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph& graph) {
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) topologicalSort(graph, i, visited, sorted);
    }

    return sorted;
}

void testGraph(const IGraph& graph) {
    std::cout << "BFS:  ";
    BFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << "\nDFS:  ";
    DFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << "\nSORT: ";
    for (int vertex : topologicalSort(graph)) {
        std::cout << vertex << " ";
    }
    std::cout << "\n\n";
}

int main() {
    ListGraph listGraph(9);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);
    listGraph.AddEdge(7, 8);

    std::cout << "ListGraph:\n";
    testGraph(listGraph);

    ArcGraph arcGraph(listGraph);
    std::cout << "ArcGraph:\n";
    testGraph(arcGraph);

    MatrixGraph matrixGraph(arcGraph);
    std::cout << "MatrixGraph:\n";
    testGraph(matrixGraph);

    SetGraph setGraph(matrixGraph);
    std::cout << "SetGraph:\n";
    testGraph(setGraph);

    return 0;
}
