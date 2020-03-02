#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

class Graph {
public:
    Graph(int VortexCount) {
        nextdoors.resize(VortexCount);
        VortexNum = VortexCount;
    }

    void AddVortex() {
        std::vector<int> NewVortex(0);
        nextdoors.push_back(NewVortex);
        VortexNum++;
    }

    void AddEdge(int from, int to) {
        nextdoors[from].push_back(to);
    }

    bool HasEdge(int from, int to) {
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
        return VortexNum;
    }

    void sortedges() {
        for (int i = 0; i < nextdoors.size(); i++) {
            sort(nextdoors[i].begin(), nextdoors[i].end());
        }
    }

    void Print() const {
        for (int i = 0; i < nextdoors.size(); i++) {
            for (int j = 0; j < nextdoors[i].size(); j++){
                std::cout << nextdoors[i][j] << " ";
            }
        }
        std::cout << "\n";
    }
private:
    std::vector<std::vector<int>> nextdoors;
    int VortexNum;
};

int TopSort(const Graph& graph, std::vector<int>& used, std::vector<int>& vertices, int InRes, int top) {
    std::stack<int> s;
    s.push(top);
    while (!s.empty()){
        int vertex = s.top();
        while (!s.empty() and used[vertex] == 2) {
            s.pop();
            vertex = s.top();
        }
        used[vertex] = 1;
        std::vector<int> next = graph.GetNextVertexes(vertex);
        int p = 0;
        for (int i = next.size() - 1; i >= 0; i--) {
            if (used[next[i]] == 1) {
                return 0;
            }
            else if (!used[next[i]]) {
                s.push(next[i]);
                p = 1;
            }
        }
        if (!p) {
            used[vertex] = 2;
            s.pop();
            vertices[vertices.size() - 1 - InRes] = vertex;
            InRes++;
        }
    }
    return InRes;
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
    int InRes = 0, AddInRes = 0;
    for (int i = 0; i < n; i++) {
        if (sources[i] == 1) {
            AddInRes = TopSort(graph, used, vertices, InRes, i);
            if (!AddInRes) {
                break;
            }
            InRes = AddInRes;
        }
    }
    if (!AddInRes) {
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