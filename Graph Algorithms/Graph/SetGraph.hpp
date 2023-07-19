#pragma once

#include "IGraph.hpp"

#include <cassert>
#include <unordered_set>

struct SetGraph final : public IGraph {
 public:
    SetGraph(int size) : adjacencySets(size) {}

    SetGraph(const IGraph& graph) : adjacencySets(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++) {
            const auto nextVertices = graph.GetNextVertices(i);
            for (const auto vertex : nextVertices) {
                adjacencySets[i].insert(vertex);
            }
        }
    }

    ~SetGraph() {}

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < adjacencySets.size());
        assert(0 <= to && to < adjacencySets.size());
        adjacencySets[from].insert(to);
    }

    int VerticesCount() const override { return static_cast<int>(adjacencySets.size()); }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencySets.size());
        std::vector<int> nextVertices;
        nextVertices.reserve(adjacencySets[vertex].size());
        for (const auto to : adjacencySets[vertex]) {
            nextVertices.push_back(to);
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < adjacencySets.size());
        std::vector<int> prevVertices{};

        for (int from = 0; from < adjacencySets.size(); from++) {
            for (int to : adjacencySets[from]) {
                if (to == vertex) {
                    prevVertices.push_back(from);
                }
            }
        }
        return prevVertices;
    }

 private:
    std::vector<std::unordered_set<int>> adjacencySets;
};
