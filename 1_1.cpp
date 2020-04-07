#include <iostream>
#include <vector>
#include <queue>
#include <stack>

class Graph {
public:
    Graph(int VertexCount) {
        nextdoors.resize(VertexCount);
    }

    void AddVertex() {
        std::vector<int> NewVertex(0);
        nextdoors.push_back(NewVertex);
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
        return nextdoors.size();
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
};

void BFS(Graph& graph, std::vector<int>& H, int from, void (*visit)(std::vector<int>&, std::pair<int, int>)){// для одной компоненты связности [O(V + E) for List; O(V**2) for Matrix]
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
        visit(H, vertex);
    }
}

void changeH(std::vector<int>& H, std::pair<int, int> vertex){ // visit
    H[vertex.first] = vertex.second;
}

void addH(const std::vector<int>& H, std::vector<int>& AddDist){
    for (int i = 0; i < AddDist.size(); i++) {
        AddDist[i] += H[i];
    }
}

void ZeroH(std::vector<int>& H){
    for (int i = 0; i < H.size(); i++){
        H[i] = 0;
    }
}

int Find_Dist(int n, int m, int leon, int matilda, int milk){
    std::vector <int> SummDist(n, 0);
    std::vector<int> H(n, 0);
    Graph graph(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;
        graph.AddEdge(a, b);
    }
    BFS(graph, H, leon, changeH);
    addH(H, SummDist);
    ZeroH(H);
    BFS(graph, H, matilda, changeH);
    addH(H, SummDist);
    ZeroH(H);
    BFS(graph, H, milk, changeH);
    addH(H, SummDist);
    ZeroH(H);
    int ans = SummDist[0];
    for (int i = 1; i < SummDist.size(); i++) {
        ans = std::min(ans, SummDist[i]);
    }
    return ans;
}

int main() {
    int n, m, leon, matilda, milk;
    std::cin >> n >> m >> leon >> matilda >> milk;
    leon--;
    matilda--;
    milk--;
    int ans = Find_Dist(n, m, leon, matilda, milk);
    std::cout << ans;
    return 0;
}