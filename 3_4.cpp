#include <iostream>
#include <vector>

using namespace std;

class Graph {
public:
    explicit Graph(int VertexCount) {
        nextdoors.resize(VertexCount);
    }

    void add_vertex() {
        std::vector<int> NewVertex(0);
        nextdoors.push_back(NewVertex);
    }

    void add_edge(int from, int to) {
        nextdoors[from].push_back(to);
    }

    bool has_edge(int from, int to) const {
        for (int i = 0; i < nextdoors[from - 1].size(); i++) {
            if (nextdoors[from - 1][i] == to - 1) {
                return true;
            }
        }
        return false;
    }

    const std::vector<int>& get_next_vertexes(int from) const {
        return nextdoors[from];
    }

    int vertex_count() const {
        return nextdoors.size();
    }
private:
    std::vector<std::vector<int>> nextdoors;
};

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
    for (int i = 0; i < graph.vertex_count(); i++) {
        const std::vector<int> nextdoors = graph.get_next_vertexes(i);
        for (int j = 0; j < nextdoors.size(); j++){
            out << nextdoors[j];
        }
        out << "\n";
    }
    return out;
}

bool recursion_DFS(const Graph& graph, std::vector<bool>& used, std::vector<int>& pairs, int vertex) {
    used[vertex] = true;
    std::vector<int> next = graph.get_next_vertexes(vertex);
    for (int i = 0; i < next.size(); i++) {
        if (pairs[next[i]] != -1 and (used[pairs[next[i]]] or !recursion_DFS(graph, used, pairs, pairs[next[i]]))) {
            continue;
        }
        pairs[next[i]] = vertex;
        return true;
    }
    return false;
}

int kun(const Graph& graph) {
    std::vector<int> pairs(graph.vertex_count(), -1);
    for (int i = 0; i < graph.vertex_count(); i++){
        std::vector<bool> used(graph.vertex_count(), false);
        recursion_DFS(graph, used, pairs, i);
    }
    int ans = 0;
    for (int i = 0; i < pairs.size(); i++) {
        if (pairs[i] != -1) {
            ans++;
        }
    }
    return ans / 2;
}

int make_graph(Graph& graph, const vector<vector<char>>& bridge) {
    int numb = 0;
    for (int i = 0; i < bridge.size(); ++i) {
        for (int j = 0; j < bridge[i].size(); ++j) {
            if (bridge[i][j] == '.') {
                continue;
            }
            if (i > 0 && bridge[i - 1][j] == '*') {
                graph.add_edge((i - 1) * bridge[i].size() + j, i * bridge[i].size() + j);
            }
            if (i < bridge.size() - 1 && bridge[i + 1][j] == '*') {
                graph.add_edge((i + 1) * bridge[i].size() + j, i * bridge[i].size() + j);

            }
            if (j > 0 && bridge[i][j - 1] == '*') {
                graph.add_edge(i * bridge[i].size() + j - 1, i * bridge[i].size() + j);
            }
            if (j < bridge[i].size() - 1 && bridge[i][j + 1] == '*') {
                graph.add_edge(i * bridge[i].size() + j + 1, i * bridge[i].size() + j);
            }
            numb++;
        }
    }
    return numb;
}

int main() {
    int n, m, a, b;
    std::cin >> n >> m >> a >> b;
    if (m == 0) {
        std::cout << 0;
    }
    else{
        vector<vector<char>> bridge (n, vector<char>(m));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                char elem;
                std::cin >> elem;
                bridge[i][j] = elem;
            }
        }
        Graph graph(m * n);
        int numb = make_graph(graph, bridge);
        if (b * 2 < a){
            std::cout << b * numb;
        }
        else {
            int max_pair = kun(graph);
            std::cout << a * max_pair + b * (numb - 2 * max_pair);
        }
    }
}
