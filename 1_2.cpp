#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

enum Colors {WHITE, GRAY, BLACK};

class Graph {
public:
    explicit Graph(int VertexCount) {
        nextdoors.resize(VertexCount);
    }

    void AddVertex() {
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

    void sort_edges() {
        for (int i = 0; i < nextdoors.size(); i++) {
            sort(nextdoors[i].begin(), nextdoors[i].end());
        }
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

bool top_sort(const Graph& graph, std::vector<int>& used, std::vector<int>& vertices, int top) {
    std::stack<int> s;
    s.push(top);
    bool has_smth_add = false;
    while (!s.empty()){
        const int vertex = s.top();
        if (!s.empty() and used[vertex] == Colors(BLACK)) {
            continue;
        }
        used[vertex] = Colors(GRAY);
        std::vector<int> next = graph.get_next_vertexes(vertex);
        bool add_in_stack = false;
        for (int i = next.size() - 1; i >= 0; i--) {
            if (used[next[i]] == Colors(GRAY)) {
                return true;
            }
            else if (used[next[i]] == Colors(WHITE)) {
                s.push(next[i]);
                add_in_stack = true;
            }
        }
        if (!add_in_stack) {
            used[vertex] = Colors(BLACK);
            s.pop();
            vertices.push_back(vertex);
            has_smth_add = true;
        }
    }
    return !has_smth_add;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    std::vector<bool> has_no_input_edges (n , true);
    for (int i = 0; i < m; i++){
        int from, to;
        std::cin >> from >> to;
        graph.add_edge(from, to);
        has_no_input_edges[to] = false;
    }
    graph.sort_edges();
    std::vector<int> vertices (n, 0);
    std::vector<int> used (n, 0);
    bool has_cycle;
    for (int i = 0; i < n; i++) {
        if (has_no_input_edges[i] == true) {
            has_cycle = top_sort(graph, used, vertices, i);
            if (has_cycle) {
                break;
            }
        }
    }
    if (has_cycle) {
        std::cout << "NO";
    }
    else{
        std::cout << "YES\n";
        for (int i = n - 1; i >= 0; i++){
            std::cout << vertices[i] << " ";
        }
    }
    return 0;
}
