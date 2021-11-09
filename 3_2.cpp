#include <iostream>
#include <vector>
#include <algorithm>

class DSU{
public:
    DSU(int n) {
        comp.resize(n, -1);
        comp_size.resize(n, 1);
    }

    int find_set(int vertex){
        while (comp[vertex] != -1){
            vertex = comp[vertex];
        }
        return vertex;
    }

    void union_set(int second_set, int first_set){
        if (first_set > second_set) {
            std::swap(first_set, second_set);
        }
        comp[first_set] = second_set;
        comp_size[second_set] += comp_size[first_set];
        comp_size[first_set] = 0;
    }
private:
    std::vector<int> comp;
    std::vector<int> comp_size;
};

bool edges_compare(const std::pair<std::pair<int, int>, int>& first, const std::pair<std::pair<int, int>, int>& second){
    return first.second < second.second;
}

int kruskal_weight(int n, std::vector<std::pair<std::pair<int, int>, int>>& edges){
    std::sort(edges.begin(), edges.end(), edges_compare);
    DSU set_system = DSU(n);
    int eadges_in_MST = 0;
    int weight = 0;
    int eadge_num = 0;
    while (eadges_in_MST < n - 1){
        int first_set = set_system.find_set(edges[eadge_num].first.first);
        int second_set = set_system.find_set(edges[eadge_num].first.second);
        if (first_set != second_set){
            set_system.union_set(second_set, first_set);
            weight += edges[eadge_num].second;
            eadges_in_MST++;
        }
        eadge_num++;
    }
    return weight;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::pair<std::pair<int, int>, int>> edges(0);
    for (int i = 0; i < m; i++) {
        int b, e, w;
        std::cin >> b >> e >> w;
        b--;
        e--;
        edges.emplace_back(std::make_pair(std::make_pair(e, b), w));
    }
    std::cout << kruskal_weight(n, edges);
}