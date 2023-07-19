#pragma once

#include "IGraph.hpp"

#include <cassert>

using Edge = std::pair<int, int>;

struct ArcGraph final : public IGraph {
 public:
    ArcGraph(int size) : size(size){};

    ArcGraph(const IGraph& graph) {
        size = graph.VerticesCount();
        for (int from = 0; from < size; from++) {
            for (auto to : graph.GetNextVertices(from)) {
                edges.push_back({from, to});
            }
        }
    }

    ~ArcGraph() {}

    void AddEdge(int from, int to) override {
        assert(0 <= from && from < size);
        assert(0 <= to && to < size);
        edges.push_back({from, to});
    }

    int VerticesCount() const override { return size; }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < size);
        std::vector<int> nextVertices{};
        for (auto edge : edges) {
            if (edge.first == vertex) {
                nextVertices.push_back(edge.second);
            }
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < size);
        std::vector<int> prevVertices{};
        for (auto edge : edges) {
            if (edge.second == vertex) {
                prevVertices.push_back(edge.first);
            }
        }
        return prevVertices;
    }

 private:
    int size;
    std::vector<Edge> edges;
};
