#pragma once

#include "IGraph.hpp"

#include <cassert>

struct MatrixGraph final : public IGraph {
 public:
    MatrixGraph(int size) { resizeMatrix(size); }

    MatrixGraph(const IGraph& graph) {
        resizeMatrix(graph.VerticesCount());
        for (int i = 0; i < graph.VerticesCount(); i++) {
            const auto nextVertices = graph.GetNextVertices(i);
            for (const auto vertex : nextVertices) {
                adjacencyMatrix[i][vertex] = 1;
            }
        }
    }

    ~MatrixGraph() {}

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < adjacencyMatrix.size());
        assert(0 <= to && to < adjacencyMatrix.size());
        adjacencyMatrix[from][to] = 1;
    }

    int VerticesCount() const override { return static_cast<int>(adjacencyMatrix.size()); }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyMatrix.size());
        std::vector<int> nextVertices{};
        for (int to = 0; to < adjacencyMatrix.size(); to++) {
            if (adjacencyMatrix[vertex][to]) {
                nextVertices.push_back(to);
            }
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyMatrix.size());
        std::vector<int> prevVertices{};

        for (int from = 0; from < adjacencyMatrix.size(); from++) {
            if (adjacencyMatrix[from][vertex]) {
                prevVertices.push_back(from);
            }
        }
        return prevVertices;
    }

 private:
    void resizeMatrix(int size) {
        adjacencyMatrix.resize(size);
        for (auto& vertex : adjacencyMatrix) {
            vertex.resize(size);
        }
    }

    std::vector<std::vector<int>> adjacencyMatrix;
};
