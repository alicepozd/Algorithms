#include <iostream>
#include <vector>
#include <set>
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

void recount(const Graph& graph, std::vector<int>& min_distance, std::set<std::pair<int, int>>& possible_edges, int vertex){
    min_distance[vertex] = 0;
    std::vector<std::pair<int, int>> next = graph.GetNextVertexes(vertex);
    for (int i = 0; i < next.size(); i++){
        if (min_distance[next[i].first] <= next[i].second){
            continue;
        }
        possible_edges.erase(possible_edges.find(std::make_pair(min_distance[next[i].first], next[i].first)));
        possible_edges.insert(std::make_pair(next[i].second, next[i].first));
        min_distance[next[i].first] = next[i].second;
    }
}

int find_next_edge_dist(const Graph& graph, std::vector<int>& min_distance, std::set<std::pair<int, int>>& possible_edges){
    std::pair<int, int> vertex = *(possible_edges.begin());
    possible_edges.erase(vertex);
    recount(graph, min_distance, possible_edges, vertex.second);
    return vertex.first;
}

int prim_weight(const Graph& graph){
    std::vector<int> min_distance(graph.VertexCount(), INT32_MAX);
    min_distance[0] = 0;
    std::set<std::pair<int, int>> possible_edges;
    possible_edges.insert(std::make_pair(0, 0));
    for (int i = 1; i < graph.VertexCount(); i++) {
        possible_edges.insert(std::make_pair(INT32_MAX, i));
    }
    int weight = 0;
    while (!possible_edges.empty()){
        weight += find_next_edge_dist(graph, min_distance, possible_edges);
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