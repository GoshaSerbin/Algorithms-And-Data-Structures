#include <cassert>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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

int findNumberOfShortestPaths(const IGraph& graph, int from, int to) {
    std::vector<int> distances(graph.VerticesCount(), INT_MAX);
    std::vector<int> parents(graph.VerticesCount(), -1);

    std::vector<int> shortestPathsCounts(graph.VerticesCount(), 0);
    shortestPathsCounts[from] = 1;

    std::queue<int> queue;
    queue.push(from);
    distances[from] = 0;

    while (!queue.empty()) {
        int vertex = queue.front();
        queue.pop();
        for (auto adjacent : graph.GetNextVertices(vertex)) {
            if (distances[adjacent] > distances[vertex] + 1) {
                distances[adjacent] = distances[vertex] + 1;
                parents[adjacent] = vertex;
                queue.push(adjacent);

                shortestPathsCounts[adjacent] = shortestPathsCounts[vertex];
            } else if (distances[adjacent] == distances[vertex] + 1) {
                shortestPathsCounts[adjacent] += shortestPathsCounts[vertex];
            }
        }
    }

    return shortestPathsCounts[to];
}

int main() {
    int size{};
    std::cin >> size;
    ListGraph graph(size);

    int v{};
    std::cin >> v;

    int from{}, to{};
    for (int i = 0; i < v; ++i) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }

    std::cin >> from >> to;
    std::cout << findNumberOfShortestPaths(graph, from, to) << std::endl;

    return 0;
}
