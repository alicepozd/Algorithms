#include <iostream>
#include <vector>
#include <algorithm>

class Graph {
public:
    explicit Graph(int VertexCount) {
        nextdoors.resize(VertexCount);
    }

    void AddVertex() {
        std::vector<std::pair<int, std::pair<double, double>>> NewVertex(0);
        nextdoors.push_back(NewVertex);
    }

    void AddEdge(int from, int to, std::pair<double, double> w) {
        nextdoors[from].push_back(std::pair<int, std::pair<double, double>>(to, w));
    }

    bool HasEdge(int from, int to) const {
        for (int i = 0; i < nextdoors[from - 1].size(); i++) {
            if (nextdoors[from - 1][i].first == to - 1) {
                return true;
            }
        }
        return false;
    }

    const std::vector<std::pair<int, std::pair<double, double>>>& GetNextVertexes(int from) const {
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
    std::vector<std::vector<std::pair<int, std::pair<double, double>>>> nextdoors;
};

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
    for (int i = 0; i < graph.VertexCount(); i++) {
        std::vector<std::pair<int, std::pair<double, double>>> nextdoors = graph.GetNextVertexes(i);
        for (int j = 0; j < nextdoors.size(); j++){
            out << "(" << nextdoors[j].first << ", (" << nextdoors[j].second.first << ", " << nextdoors[j].second.second << ")) ";
        }
        out << "\n";
    }
    return out;
}

void upd(std::vector<int>& first, const std::vector<int>& second){
    for (int i = 0; i < second.size(); i++) {
        first[i] = second[i];
    }
}

void Ford_Belman(const Graph& graph, std::vector<double>& depth, int from){
    for (int i = 0; i < graph.VertexCount() - 1; i++){
        for (int j = 0; j < graph.VertexCount(); j++){
            std::vector<std::pair<int, std::pair<double, double>>> next = graph.GetNextVertexes(j);
            for (int g = 0; g < next.size(); g++) {
                if ((depth[j] != INT32_MIN) && (depth[j] - next[g].second.second > 0)){
                    depth[next[g].first] = std::max(depth[next[g].first], (depth[j] - next[g].second.second) * next[g].second.first);
                }
            }
        }
    }
}

bool Is_Max(const Graph& graph, std::vector<double>& depth){
    for (int j = 0; j < graph.VertexCount(); j++){
        std::vector<std::pair<int, std::pair<double, double>>> next = graph.GetNextVertexes(j);
        for (int g = 0; g < next.size(); g++) {
            if ((depth[j] != INT32_MIN) && (depth[j] - next[g].second.second > 0) && (depth[j] - next[g].second.second) * next[g].second.first > depth[next[g].first]){
                return false;
            }
        }
    }
    return true;
}

int main(){
    int n, m, s;
    double v;
    std::cin >> n >> m >> s >> v;
    Graph graph(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        double rab, cab, rba, cba;
        std::cin >> a >> b >> rab >> cab >> rba >> cba;
        graph.AddEdge(a - 1, b - 1, std::pair<double, double>(rab, cab));
        graph.AddEdge(b - 1, a - 1, std::pair<double, double>(rba, cba));
    }
    std::vector<double> MaxMoney (n, INT32_MIN);
    MaxMoney[s - 1] = v;
    Ford_Belman(graph, MaxMoney, s - 1);
    if (!Is_Max(graph, MaxMoney)){
        std::cout << "YES";
    }
    else {
        std::cout << "NO";
    }
}
