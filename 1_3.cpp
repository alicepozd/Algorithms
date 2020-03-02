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
        std::cout << "(";
        for (int i = 0; i < nextdoors.size(); i++) {
            std::cout << "-";
            for (int j = 0; j < nextdoors[i].size(); j++){
                std::cout << nextdoors[i][j] << " ";
            }
        }
        std::cout << ")";
    }
private:
    std::vector<std::vector<int>> nextdoors;
    int VortexNum;
};

void DFS(const Graph& graph, std::vector<int>& vertices, std::vector<int>& used, int top, int& InRes) {
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
            if (!used[next[i]]) {
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
}

int FindComponents(const Graph& graph, const Graph& igraph, std::vector<int>& NumbComp){
    int n = NumbComp.size();
    std::vector<int> invDFSorder(n, 0);
    std::vector<int> used(n, 0);
    int NumbIn = 0;
    for (int i = 0; i < n; i++) { // DFS на инвертированном графе, результат в invDFSorder
        if (used[i] == 0) {
            DFS(igraph, invDFSorder, used, i, NumbIn);
        }
    }
    for (int i = 0; i < n; i++) {
        used[i] = 0;
    }
    std::vector<std::vector<int>> components(0);
    std::vector<int> VinComp(n, 0);
    for (int i = 0; i < n; i++) { // DFS на графе, в очерёдности вершин в invDFSorder, результат в виде компонент сильной связности
        NumbIn = 0;
        if (used[invDFSorder[i]] == 0) {
            DFS(graph, VinComp, used, invDFSorder[i], NumbIn);
            std::vector<int> newComp(0);
            for (int j = NumbIn - 1; j >= 0; j--) {
                newComp.push_back(VinComp[n - j - 1]);
            }
            components.push_back(newComp);
        }
    }
    for (int i = 0; i < components.size(); i++) { // номер компоненты связности для каждой вершины
        for (int j = 0; j < components[i].size(); j++) {
            NumbComp[components[i][j]] = i;
        }
    }
    int N = components.size();
    return N;
}

std::pair<int, int> FindSoursesSinks(const Graph& graph, const std::vector<int>& NumbComp, int N) {
    if (N == 1) {
        return std::pair<int, int>(0, 0);
    }
    int n = NumbComp.size();
    std::vector<std::pair<int, int>> Comp(N, std::pair<int, int>(1, 1));
    for(int i = 0; i < NumbComp.size(); i++){
        std::vector<int> next = graph.GetNextVertexes(i);
        int p = 0;
        for (int j = 0; j < next.size(); j++) {
            if (NumbComp[i] != NumbComp[next[j]]) {
                p = 1;
                Comp[NumbComp[next[j]]].first = 0;
            }
        }
        if (p) {
            Comp[NumbComp[i]].second = 0;
        }
    }
    std::pair<int, int> SoursesSinks(0, 0);
    for (int i = 0; i < Comp.size(); i++){
        if (Comp[i].first == 1){
            SoursesSinks.first++;
        }
        if (Comp[i].second == 1){
            SoursesSinks.second++;
        }
    }
    return SoursesSinks;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    Graph igraph(n);
    for (int i = 0; i < m; i++) { // считывание графа
        int from, to;
        std::cin >> from >> to;
        from--;
        to--;
        graph.AddEdge(from, to);
        if (from != to) {
            igraph.AddEdge(to, from);
        }
    }
    graph.sortedges();
    std::vector<int> NumbComp(n); // номер компоненты связности для каждой вершины
    int N = FindComponents(graph, igraph, NumbComp);
    std::pair<int, int> SoursesSinks = FindSoursesSinks(graph, NumbComp, N); // оличество стоков и истоков
    int ans = std::max(SoursesSinks.first, SoursesSinks.second);
    std::cout << ans;
    return 0;
}