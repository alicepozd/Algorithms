#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

class Graph {
public:
    explicit Graph(int VertexCount) {
        nextdoors.resize(VertexCount);
    }

    void add_vertex() {
        std::vector<std::pair<int, int>> NewVertex(0);
        nextdoors.push_back(NewVertex);
    }

    void add_edge(int from, int to, int w) {
        nextdoors[from].push_back(std::make_pair(to, w));
    }

    bool has_edge(int from, int to) const {
        for (int i = 0; i < nextdoors[from - 1].size(); i++) {
            if (nextdoors[from - 1][i].first == to - 1) {
                return true;
            }
        }
        return false;
    }

    const std::vector<std::pair<int, int>>& get_next_vertexes(int from) const {
        return nextdoors[from];
    }

    int vertex_count() const {
        return nextdoors.size();
    }

    void sort_edges() {
        for (int i = 0; i < nextdoors.size(); i++) {
            sort(nextdoors[i].begin(), nextdoors[i].end());
        }
    }
private:
    std::vector<std::vector<std::pair<int, int>>> nextdoors;
};

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
    for (int i = 0; i < graph.vertex_count(); i++) {
        const std::vector<std::pair<int, int>> nextdoors = graph.get_next_vertexes(i);
        for (int j = 0; j < nextdoors.size(); j++){
            out << "(" << nextdoors[j].first << ", " << nextdoors[i].second << ") ";
        }
        out << "\n";
    }
    return out;
}

int dijkstra(const Graph& graph, int x, int y){
    std::vector<int> value (graph.vertex_count(), INT32_MAX);
    std::set<std::pair<int, int>> s;
    s.insert(std::pair<int, int>(0, x));
    value[x] = 0;
    while (!s.empty()) {
        std::pair<int, int> now = (*s.begin());
        for (auto i : graph.get_next_vertexes(now.second)) {
            if (value[i.first] <= now.first + i.second) {
                continue;
            }
            if (s.count(std::pair<int, int>(value[i.first], i.first))) {
                s.erase(s.find(std::pair<int, int>(value[i.first], i.first)));
            }
            s.insert(std::pair<int, int>(now.first + i.second, i.first));
            value[i.first] = now.first + i.second;
        }
        s.erase(s.begin());
    }
    return  value[y];
}

void MakeGraph(Graph& graph, int m, int a, int b){ //строит граф, соответствующий условию задачи, с которым потом работает алгоритм Дейкстры
    for (int i = 0; i < m; i++) {
        long long temp = (static_cast<long long>(i) * i + 1) % m;
        if (temp != i) {
            graph.add_edge(i, temp, b);
        }
        graph.add_edge(i, (i + 1) % m, a);
    }
}

int main() {
    int a, b, m, x, y;
    std::cin >> a >> b >> m >> x >> y;
    Graph graph(m);
    MakeGraph(graph, m, a, b);
    int n = dijkstra(graph, x, y);
    std::cout << n;
    return 0;
}
