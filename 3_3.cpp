#include <iostream>
#include <vector>
#include <algorithm>

long long find_set(std::vector<long long>& comp, long long vertex){
    while (comp[vertex] != -1){
        vertex = comp[vertex];
    }
    return vertex;
}

void union_set(long long second_set, long long first_set, std::vector<long long>& comp, std::vector<long long>& comp_size){
    if (first_set > second_set) {
        std::swap(first_set, second_set);
    }
    comp[first_set] = second_set;
    comp_size[second_set] += comp_size[first_set];
    comp_size[first_set] = 0;
}

bool edges_compare(const std::pair<std::pair<long long, long long>, long long>& first, const std::pair<std::pair<long long, long long>, long long>& second){
    return first.second < second.second;
}

long long kruskal_weight(long long n, std::vector<std::pair<std::pair<long long, long long>, long long>>& edges){
    std::sort(edges.begin(), edges.end(), edges_compare);
    std::vector<long long> comp(n, -1);
    std::vector<long long> comp_size(n, 1);
    long long eadges_in_MST = 0;
    long long weight = 0;
    long long eadge_num = 0;
    while (eadges_in_MST < n - 1){
        long long first_set = find_set(comp, edges[eadge_num].first.first);
        long long second_set = find_set(comp, edges[eadge_num].first.second);
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
    long long n, m;
    std::cin >> n >> m;
    std::vector<std::pair<std::pair<long long, long long>, long long>> edges(0);
    std::vector<long long> cost(n);
    long long min_cost_vertex = -1;
    long long min_cost = -1;
    for(long long i = 0; i < n; i++) {
        std::cin >> cost[i];
        if (min_cost == -1 || cost[i] < min_cost){
            min_cost = cost[i];
            min_cost_vertex = i;
        }
    }
    for (long long i = 0; i < n; i++) {
        if (min_cost_vertex != i) {
            edges.push_back(std::make_pair(std::make_pair(min_cost_vertex, i), cost[i] + min_cost));
        }
    }
    for (long long i = 0; i < m; i++) {
        long long b, e, w;
        std::cin >> b >> e >> w;
        b--;
        e--;
        edges.push_back(std::make_pair(std::make_pair(e, b), w));
    }
    std::cout << kruskal_weight(n, edges);
}