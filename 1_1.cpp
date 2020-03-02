#include <iostream>
#include <vector>
#include <queue>
#include <stack>

class Graph {
public:
    Graph(int VortexCount) {
        nextdoors.resize(VortexCount);
        H.resize(VortexCount);
        VortexNum = VortexCount;
    }

    void AddVortex() {
        std::vector<int> NewVortex(0);
        nextdoors.push_back(NewVortex);
        VortexNum++;
    }

    void AddEdge(int from, int to) {
        nextdoors[from - 1].push_back(to - 1);
        nextdoors[to - 1].push_back(from - 1);
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

    void ChangeH(int vertex, int h){
        H[vertex] = h;
    }

    int GetH(int vertex) const{
        return H[vertex];
    }

    void ZeroH() {
        for (int i = 0; i < H.size(); i++) {
            H[i] = 0;
        }
    }

    void Print() const {
        for (int i = 0; i < nextdoors.size(); i++) {
            for (int j = 0; j < nextdoors[i].size(); j++){
                std::cout << nextdoors[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
private:
    std::vector<std::vector<int>> nextdoors;
    std::vector<int> H;
    int VortexNum;
};

void BFS(Graph& graph, int from, void (*visit)(Graph&, std::pair<int, int>)){// для одной компоненты связности [O(V + E) for List; O(V**2) for Matrix]
    std::queue<std::pair<int, int>> qu;
    std::vector<bool> color (graph.VertexCount(), false);
    qu.push(std::pair<int, int>(from, 0));
    int h = 0;
    while (!qu.empty()) {
        std::pair<int, int> vertex = qu.front();
        h = vertex.second + 1;
        qu.pop();
        color[vertex.first] = true;
        const std::vector<int>& next = graph.GetNextVertexes(vertex.first);
        for (int i = 0; i < next.size(); i++){
            if(!color[next[i]]) {
                qu.push(std::pair<int, int>(next[i], h));
                color[next[i]] = true;
            }
        }
        visit(graph, vertex);
    }
}

void changeH(Graph& graph, std::pair<int, int> vertex){ // visit
    graph.ChangeH(vertex.first, vertex.second);
}

void addH(const Graph& graph, std::vector<int>& AddDist){
    for (int i = 0; i < AddDist.size(); i++) {
        AddDist[i] += graph.GetH(i);
    }
}

int main() {
    int n, m, leon, matilda, milk;
    std::cin >> n >> m >> leon >> matilda >> milk;
    leon--;
    matilda--;
    milk--;
    std::vector <int> SummDist(n, 0);
    Graph graph(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;
        graph.AddEdge(a, b);
    }
    BFS(graph, leon, changeH);
    addH(graph, SummDist);
    graph.ZeroH();
    BFS(graph, matilda, changeH);
    addH(graph, SummDist);
    graph.ZeroH();
    BFS(graph, milk, changeH);
    addH(graph, SummDist);
    graph.ZeroH();
    int ans = SummDist[0];
    for (int i = 1; i < SummDist.size(); i++) {
        ans = std::min(ans, SummDist[i]);
    }
    std::cout << ans;
    return 0;
}