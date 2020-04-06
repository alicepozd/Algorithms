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

    void AddEdge(int from, int to) {
        nextdoors[from].push_back(to);
    }

    bool HasEdge(int from, int to) const {
        for (int i = 0; i < nextdoors[from - 1].size(); i++) {
            if (nextdoors[from - 1][i] == to - 1) {
                return true;
            }
        }
        return false;
    }

    const std::vector<int>& GetNextVertexes(int from) const {
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

    std::ostream& operator<<(std::ostream& out) const {
        for (int i = 0; i < nextdoors.size(); i++) {
            for (int j = 0; j < nextdoors[i].size(); j++){
                out << nextdoors[i][j] << " ";
            }
        }
        out << "\n";
        return out;
    }
private:
    std::vector<std::vector<int>> nextdoors;
};

int TopSort(const Graph& graph, std::vector<int>& used, std::vector<int>& vertices, int& InRes, int top) {
    std::stack<int> s;
    s.push(top);
    while (!s.empty()){
        int vertex = s.top();
        while (!s.empty() and used[vertex] == Colors(BLACK)) {
            s.pop();
            vertex = s.top();
        }
        used[vertex] = Colors(GRAY);
        std::vector<int> next = graph.GetNextVertexes(vertex);
        bool add_in_stack = false;
        for (int i = next.size() - 1; i >= 0; i--) {
            if (used[next[i]] == Colors(GRAY)) {
                return 0;
            }
            else if (used[next[i]] == Colors(WHITE)) {
                s.push(next[i]);
                add_in_stack = true;
            }
        }
        if (!add_in_stack) {
            used[vertex] = Colors(BLACK);
            s.pop();
            vertices[vertices.size() - 1 - InRes] = vertex;
            InRes++;
        }
    }
    if (InRes){
        return true;
    }
    else {
        return false;
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    std::vector<int> sources (n , 1);
    for (int i = 0; i < m; i++){
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        sources[to] = 0;
    }
    graph.sortedges();
    std::vector<int> vertices (n, 0);
    std::vector<int> used (n, 0);
    int InRes = 0;
    bool Is_Sorted;
    for (int i = 0; i < n; i++) {
        if (sources[i] == 1) {
            Is_Sorted = TopSort(graph, used, vertices, InRes, i);
            if (!Is_Sorted) {
                break;
            }
        }
    }
    if (!Is_Sorted) {
        std::cout << "NO";
    }
    else{
        std::cout << "YES\n";
        for (int i = 0; i < n; i++){
            std::cout << vertices[i] << " ";
        }
    }
    return 0;
}
