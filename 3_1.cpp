#include <iostream>
#include <vector>
#include <algorithm>

class Graph {
public:
    explicit Graph(int VertexCount) {
        nextdoors.resize(VertexCount);
    }

    void AddVertex() {
        std::vector<std::pair<int, int>> NewVertex(0);
        nextdoors.push_back(NewVertex);
    }

    void AddEdge(int from, int to, int w) {
        nextdoors[from].push_back(std::make_pair(to, w));
    }

    bool HasEdge(int from, int to) const {
        for (int i = 0; i < nextdoors[from - 1].size(); i++) {
            if (nextdoors[from - 1][i].first == to - 1) {
                return true;
            }
        }
        return false;
    }

    const std::vector<std::pair<int, int>>& GetNextVertexes(int from) const {
        return nextdoors[from];
    }

    int VertexCount() const {
        return nextdoors.size();
    }

    void sortedges() {
        for (int i = 0; i < nextdoors.size(); i++) {
            sort(nextdoors[i].begin(), nextdoors[i].end());
        }
    }
private:
    std::vector<std::vector<std::pair<int, int>>> nextdoors;
};

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
    for (int i = 0; i < graph.VertexCount(); i++) {
        std::vector<std::pair<int, int>> nextdoors = graph.GetNextVertexes(i);
        for (int j = 0; j < nextdoors.size(); j++){
            out << "(" << nextdoors[j].first << ", " << nextdoors[j].second << ") ";
        }
        out << "\n";
    }
    return out;
}

void recount(const Graph& graph, std::vector<int>& min_distance, int vertex){
    min_distance[vertex] = -1;
    std::vector<std::pair<int, int>> next = graph.GetNextVertexes(vertex);
    for (int i = 0; i < next.size(); i++){
        min_distance[next[i].first] = std::min(min_distance[next[i].first], next[i].second);
    }
}

int find_next_edge_dist(const Graph& graph, std::vector<int>& min_distance){
    std::pair<int, int> next_eage = std::make_pair(-1, INT32_MAX);
    for (int i = 0; i < min_distance.size(); i++) {
        if (min_distance[i] != -1 && (min_distance[i] < next_eage.second)){
            next_eage.first = i;
            next_eage.second = min_distance[i];
        }
    }
    recount(graph, min_distance, next_eage.first);
    return next_eage.second;
}

int prim_weight(const Graph& graph){
    std::vector<int> min_distance(graph.VertexCount(), INT32_MAX);
    recount(graph, min_distance, 0);
    int weight = 0;
    for (int i = 1; i < graph.VertexCount(); i++){
        weight += find_next_edge_dist(graph, min_distance);
    }
    return weight;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; i++) {
        int b, e, w;
        std::cin >> b >> e >> w;
        b--;
        e--;
        graph.AddEdge(b, e, w);
        graph.AddEdge(e, b, w);
    }
    std::cout << prim_weight(graph);
}