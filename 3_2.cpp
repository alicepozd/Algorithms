#include <iostream>
#include <vector>
#include <algorithm>

int find_set(std::vector<int>& comp, int vertex){
    while (comp[vertex] != -1){
        vertex = comp[vertex];
    }
    return vertex;
}

void union_set(int second_set, int first_set, std::vector<int>& comp, std::vector<int>& comp_size){
    if (first_set > second_set) {
        std::swap(first_set, second_set);
    }
    comp[first_set] = second_set;
    comp_size[second_set] += comp_size[first_set];
    comp_size[first_set] = 0;
}

bool edges_compare(const std::pair<std::pair<int, int>, int>& first, const std::pair<std::pair<int, int>, int>& second){
    return first.second < second.second;
}

int kruskal_weight(int n, std::vector<std::pair<std::pair<int, int>, int>>& edges){
    std::sort(edges.begin(), edges.end(), edges_compare);
    std::vector<int> comp(n, -1);
    std::vector<int> comp_size(n, 1);
    int eadges_in_MST = 0;
    int weight = 0;
    int eadge_num = 0;
    while (eadges_in_MST < n - 1){
        int first_set = find_set(comp, edges[eadge_num].first.first);
        int second_set = find_set(comp, edges[eadge_num].first.second);
        if (first_set != second_set){
            union_set(second_set, first_set, comp, comp_size);
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
        edges.push_back(std::make_pair(std::make_pair(e, b), w));
    }
    std::cout << kruskal_weight(n, edges);
}