#include <iostream>
#include <vector>
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
        nextdoors[from].push_back(std::pair<int, int>(to, w));
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

    void sortedges() {
        for (int i = 0; i < nextdoors.size(); i++) {
            sort(nextdoors[i].begin(), nextdoors[i].end());
        }
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

void upd(std::vector<int>& first, const std::vector<int>& second){
    for (int i = 0; i < second.size(); i++) {
        first[i] = second[i];
    }
}

void Ford_Belman(const Graph& graph, std::vector<int>& depth, int Max_Numb_E, int from){
    depth[from] = 0;
    for (int i = 0; i < Max_Numb_E; i++){
        std::vector<int> new_depth(graph.VertexCount());
        upd(new_depth, depth);
        for (int j = 0; j < graph.VertexCount(); j++){
            std::vector<std::pair<int, int>> next = graph.GetNextVertexes(j);
            for (int g = 0; g < next.size(); g++) {
                if (depth[j] != INT32_MAX){
                    new_depth[next[g].first] = std::min(new_depth[next[g].first], depth[j] + next[g].second);
                }
            }
        }
        upd(depth, new_depth);
    }
}

int main(){
    int n, m, k, s, f;
    std::cin >> n >> m >> k >> s >> f;
    Graph graph(n);
    for (int i = 0; i < m; i++) {
        int from, to, w;
        std::cin >> from >> to >> w;
        graph.AddEdge(from - 1, to - 1, w);
    }
    std::vector<int> depth (n, INT32_MAX);
    Ford_Belman(graph, depth, k, s - 1);
    if (depth[f - 1] == INT32_MAX) {
        std::cout << "-1";
    }
    else{
        std::cout << depth[f - 1];
    }
}
