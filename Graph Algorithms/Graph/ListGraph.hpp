#pragma once

#include "IGraph.hpp"

#include <cassert>

struct ListGraph final : public IGraph {
 public:
    ListGraph(int size) : adjacencyLists(size) {}

    ListGraph(const IGraph& graph) : adjacencyLists(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph() {}

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

    int VerticesCount() const override { return static_cast<int>(adjacencyLists.size()); }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices{};

        for (int from = 0; from < adjacencyLists.size(); from++) {
            for (int to : adjacencyLists[from]) {
                if (to == vertex) {
                    prevVertices.push_back(from);
                }
            }
        }
        return prevVertices;
    }

 private:
    std::vector<std::vector<int>> adjacencyLists;
};
