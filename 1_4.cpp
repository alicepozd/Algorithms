#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <set>

class Graph {
public:
    explicit Graph(int VertexCount) {
        nextdoors.resize(VertexCount);
    }

    void add_vertex() {
        std::vector<int> NewVertex(0);
        nextdoors.push_back(NewVertex);
    }

    void add_edge(int from, int to) {
        nextdoors[from].push_back(to);
    }

    bool has_edge(int from, int to) const {
        for (auto v : nextdoors[from - 1]) {
            if (v == to - 1) {
                return true;
            }
        }
        return false;
    }

    const std::vector<int>& get_next_vertexes(int from) const {
        return nextdoors[from];
    }

    int vertex_count() const {
        return nextdoors.size();
    }
private:
    std::vector<std::vector<int>> nextdoors;
};

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
    for (int i = 0; i < graph.vertex_count(); i++) {
        const std::vector<int> nextdoors = graph.get_next_vertexes(i);
        for (auto v : nextdoors){
            out << v;
        }
        out << "\n";
    }
    return out;
}

void add_edges(const Graph& graph, Graph& NewGraph, std::vector<int>& CompTime, std::vector<int>& used, int a){ // добавление в новый граф рёбер из а
    std::vector<int> next = graph.get_next_vertexes(a);                                                           // ведущих в уже посещённые вершины
    for (auto v : next) {                                                                             // при условии, что вершины не разделены мостом
        if (used[v] == 2 and CompTime[v] == CompTime[a]) {                                     // CompTime нужен чтобы определять посещена ли вершина
            NewGraph.add_edge(a, v);
        }
    }
}

void time_DFS(const Graph& graph, Graph& NewGraph, int top) { // алгоритм основан на обходе DFS, ищет мосты
    int n = graph.vertex_count();
    std::vector<int> used (n, 0);
    std::stack<int> s;
    std::vector<int> Time (n, 0); // время захода в вершину
    std::vector<int> CompTime (n, 0); // после выполнения алгоритма, в этом массиве окажутся компоненты связности графа, если убрать мосты
    s.push(top);
    int time = 0;
    while (!s.empty()){
        const int vertex = s.top();
        if (!s.empty() and used[vertex] == 2) {
            continue;
        }
        used[vertex] = 1;
        std::vector<int> next = graph.get_next_vertexes(vertex);
        bool is_leaf = true;
        for (int i = next.size() - 1; i >= 0; i--) {
            if (used[next[i]] == 0) {
                s.push(next[i]);
                Time[next[i]] = Time[vertex] + 1;
                CompTime[next[i]] = Time[vertex] + 1;
                time++;
                is_leaf = false;
            }
            else if (used[next[i]] == 1) {
                CompTime[vertex] = std::min(CompTime[vertex], CompTime[next[i]]);
            }
        }
        if (is_leaf) {
            used[vertex] = 2;
            s.pop();
            for (auto v : next) {
                if (Time[v] != Time[vertex] - 1) {
                    if (used[v] == 1) {
                        CompTime[vertex] = std::min(CompTime[vertex], Time[v]);
                    }
                    else {
                        CompTime[vertex] = std::min(CompTime[vertex], CompTime[v]);
                    }
                }
            }
            add_edges(graph, NewGraph, CompTime, used, vertex);
        }
    }
}

void simple(const Graph& graph, Graph& NewGraph){ // упрощение графа, убирает мосты
    Graph TempGraph(graph.vertex_count());
    time_DFS(graph, TempGraph, 0);
    time_DFS(TempGraph, NewGraph, 0);
}

void edges_faces_update(std::vector<int>& Way, int a, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces) {
    // обновление граней рёбер при укладке нового пути
    std::vector<int> temp(0);
    bool is_way_continue = false;
    for (int i = 0; i < Way.size(); i++) {
        if (!is_way_continue){
            if (Way[i] == a) {
                is_way_continue = true;
                temp.push_back(a);
            }
            else {
                for (int g = 0; g < EdgesFaces[Way[i]].size(); g++) {
                    if (EdgesFaces[Way[i]][g].first == Way[i + 1]){
                        EdgesFaces[Way[i]][g].second.first = 0;
                        break;
                    }
                }
                for (int g = 0; g < EdgesFaces[Way[i + 1]].size(); g++) {
                    if (EdgesFaces[Way[i + 1]][g].first == Way[i]){
                        EdgesFaces[Way[i + 1]][g].second.first = 0;
                        break;
                    }
                }
            }
        }
        else {
            temp.push_back(Way[i]);
        }
    }
    Way.resize(temp.size());
    for (int i = 0; i < temp.size(); i++){
        Way[i] = temp[i];
    }
}

void find_way(const Graph& graph, std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, std::vector<int>& Way, int i, int NextWayBegin) {
    // нахождение следующего пути начинающегося в вершине i, проходящего через некоторое количество не уложенных вешин, первая из которых NextWayBegin, и заканчивающийся в уже уложенной
    std::vector<int> used(graph.vertex_count(), 0);
    Way.push_back(i);
    used[i] = 1;
    int prev = -1;
    int now = i;
    if (NextWayBegin != -1) {
        Way.push_back(NextWayBegin);
        prev = now;
        now = NextWayBegin;
        used[NextWayBegin] = 1;
    }
    bool is_begin = false; // нужен чтобы просто зайти в цикл, в самом начале из уже уложенной вершины
    while (!is_begin or VertexFaces[now].empty()) { // по сути проверка уложена ли уже вершина
        is_begin = true;
        if (prev != -1 and !VertexFaces[now].empty()) { // случай, когда при входе в цикл в пути уже две вершины
            break;
        }
        for (int j = 0; j < EdgesFaces[now].size(); j++) {
            if (!EdgesFaces[now][j].second.first and EdgesFaces[now][j].first != prev) {
                EdgesFaces[now][j].second.first = -1;
                for (int g = 0; g < EdgesFaces[EdgesFaces[now][j].first].size(); g++) {
                    if (EdgesFaces[EdgesFaces[now][j].first][g].first == now){
                        EdgesFaces[EdgesFaces[now][j].first][g].second.first = -1;
                        break;
                    }
                }
                prev = now;
                if (used[EdgesFaces[now][j].first] == 1) {
                    Way.push_back(EdgesFaces[now][j].first);
                    edges_faces_update(Way, EdgesFaces[now][j].first, EdgesFaces);
                    return;
                }
                used[EdgesFaces[now][j].first] = 1;
                now = EdgesFaces[now][j].first;
                Way.push_back(now);
                break;
            }
        }
    }
}

void edges_BFS(std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, std::vector<std::vector<bool>>& used, std::set<int>& M, int a) {
    std::queue<int> q;
    q.push(a);
    while (!q.empty()) {
        int now = q.front();
        q.pop();
        if (!VertexFaces[now].empty()){
            std::set<int> N;
            for (auto elem = M.begin(); elem != M.end(); ++elem) {
                if (!VertexFaces[now].count(*elem)) {
                    N.insert(*elem);
                }
            }
            for (auto elem : N) {
                M.erase(elem);
            }
            continue;
        }
        for (int i = 0; i < EdgesFaces[now].size(); i++) {
            if (!used[now][i]) {
                used[now][i] = true;
                q.push(EdgesFaces[now][i].first);
            }
        }
    }
}

std::pair<bool, int> update_possible_way_begin_for_has_no_faces_edge(std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, std::vector<std::vector<bool>>& used, std::stack<std::pair<int, std::pair<int, int>>>& WayBegin, std::pair<int, int> edge_numb){
    std::set<int> M;
    int element = -1;
    for (auto elem : VertexFaces[edge_numb.first]) {
        M.insert(elem);
    }
    used[edge_numb.first][edge_numb.second] = true;
    edges_BFS(VertexFaces, EdgesFaces, used, M, EdgesFaces[edge_numb.first][edge_numb.second].first);
    for (auto elem : M) {
        element = elem;
    }
    if (M.empty()) {
        return std::make_pair(false, 0); // если невозможно выбрать путь, а значит граф непланарен
    }
    WayBegin.push(std::make_pair(M.size(), std::make_pair(edge_numb.first, EdgesFaces[edge_numb.first][edge_numb.second].first)));
    return std::make_pair(true, element);
}

std::pair<bool, int> update_possible_way_begin_for_has_one_face_edge(std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, std::stack<std::pair<int, std::pair<int, int>>>& WayBegin, std::pair<int, int> edge_numb){
    std::set<int> M;
    int element = -1;
    for (auto elem : VertexFaces[edge_numb.first]) {
        if (VertexFaces[EdgesFaces[edge_numb.first][edge_numb.second].first].count(elem)){
            M.insert(elem);
        }
    }
    for (auto elem : M) {
        element = elem;
    }
    if (M.empty()) {
        return std::make_pair(false, 0); // если невозможно выбрать путь, а значит граф непланарен
    }
    WayBegin.push(std::make_pair(M.size(), std::make_pair(edge_numb.first, EdgesFaces[edge_numb.first][edge_numb.second].first)));
    return std::make_pair(true, element);
}

std::pair<int, int> next_way_begin(std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, int& BreakComp){
    //находим начало следующего пути, который будем укладывать
    //укладываем тот, у которого меньше допустимых компонент
    std::stack<std::pair<int, std::pair<int, int>>> WayBegin;
    std::stack<int> Elements;
    std::vector<std::vector<bool>> used(EdgesFaces.size(), std::vector<bool>(0));
    for (int i = 0; i < EdgesFaces.size(); i++) {
        for (int j = 0; j < EdgesFaces[i].size(); j++) {
            used[i].push_back(false);
        }
    }
    for (int i = 0; i < VertexFaces.size(); i++) {
        if (!VertexFaces[i].empty()) {
            for (int j = 0; j < EdgesFaces[i].size(); j++) {
                if (VertexFaces[EdgesFaces[i][j].first].empty()) {
                    std::pair<bool, int> element = update_possible_way_begin_for_has_no_faces_edge(VertexFaces, EdgesFaces, used, WayBegin, std::make_pair(i, j));
                    if (!element.first) {
                        return std::make_pair(-2, -2);
                    }
                    Elements.push(element.second);
                }
                else if (EdgesFaces[i][j].second.first == 0) {
                    std::pair<bool, int> element = update_possible_way_begin_for_has_one_face_edge(VertexFaces, EdgesFaces, WayBegin, std::make_pair(i, j));
                    if (!element.first) {
                        return std::make_pair(-2, -2);
                    }
                    Elements.push(element.second);
                }
            }
        }
    }
    if (WayBegin.empty()) {
        return std::make_pair(-1, -1);
    }
    std::pair<int, std::pair<int, int> > ans = WayBegin.top();
    BreakComp = Elements.top();
    while (!WayBegin.empty()) {
        if (WayBegin.top().first < ans.first){
            ans = WayBegin.top();
            BreakComp = Elements.top();
        }
        WayBegin.pop();
        Elements.pop();
    }
    return ans.second;
}

void change_faces(const std::vector<int>& Way,std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, int& CompNumb, int& BreakComp) {
    //изменение граней в некоторой ранее уложенной части графа
    int prev = -1;
    int now = Way[0];
    while (now != Way[Way.size() - 1]) {
        for (int j = 0; j < EdgesFaces[now].size(); j++) {
            if ((EdgesFaces[now][j].second.first == BreakComp or EdgesFaces[now][j].second.second == BreakComp) and EdgesFaces[now][j].first != prev) {
                if (EdgesFaces[now][j].second.first == BreakComp) {
                    EdgesFaces[now][j].second.first = CompNumb + 1;
                }
                else {
                    EdgesFaces[now][j].second.second = CompNumb + 1;
                }
                for (int g = 0; g < EdgesFaces[EdgesFaces[now][j].first].size(); g++) {
                    if (EdgesFaces[EdgesFaces[now][j].first][g].first == now) {
                        if (EdgesFaces[EdgesFaces[now][j].first][g].second.first == BreakComp) {
                            EdgesFaces[EdgesFaces[now][j].first][g].second.first = CompNumb + 1;
                        }
                        else {
                            EdgesFaces[EdgesFaces[now][j].first][g].second.second = CompNumb + 1;
                        }
                    }
                }
                prev = now;
                now = EdgesFaces[now][j].first;
                VertexFaces[now].insert(CompNumb + 1);
                VertexFaces[now].erase(BreakComp);
                break;
            }
        }
    }
}

void change_faces_in_new_way(const std::vector<int>& Way,std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, int& CompNumb, int& BreakComp) {
    //изменение граней в новой части графа
    for (int i = 0; i < Way.size(); i++) {
        if ((i == Way.size() - 1 and Way[0] == Way[Way.size() - 1]) or i != 0) {
            if (!(i == Way.size() - 1 and Way[0] == Way[Way.size() - 1])) {
                VertexFaces[Way[i]].insert(BreakComp);
            }
            for (int j = 0; j < EdgesFaces[Way[i]].size(); j++) {
                if (EdgesFaces[Way[i]][j].first == Way[i - 1]){
                    EdgesFaces[Way[i]][j].second.first = BreakComp;
                    EdgesFaces[Way[i]][j].second.second = CompNumb + 1;
                }
            }
            for (int j = 0; j < EdgesFaces[Way[i - 1]].size(); j++) {
                if (EdgesFaces[Way[i - 1]][j].first == Way[i]){
                    EdgesFaces[Way[i - 1]][j].second.second = CompNumb + 1;
                    EdgesFaces[Way[i - 1]][j].second.first = BreakComp;
                }
            }
            if (i == Way.size() - 1 and Way[0] == Way[Way.size() - 1]){
                break;
            }
        }
        VertexFaces[Way[i]].insert(CompNumb + 1);
        for (int j = 0; j < EdgesFaces[Way[i]].size(); j++) {
            if (EdgesFaces[Way[i]][j].second.first == -1) {
                EdgesFaces[Way[i]][j].second.first = BreakComp;
                EdgesFaces[Way[i]][j].second.second = CompNumb + 1;
            }
        }
    }
}

bool laying_down_new_component(const std::vector<int>& Way,std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, int& CompNumb, int& BreakComp){
    change_faces(Way, VertexFaces, EdgesFaces,  CompNumb, BreakComp);
    change_faces_in_new_way(Way, VertexFaces, EdgesFaces,  CompNumb, BreakComp);
    CompNumb++;
    return true;
}

bool one_component_gamma(const Graph& graph, std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EdgesFaces, int i){ // гамма алгоритм для одной компоненты связности
    std::vector<int> MainWay(0);                                                                                                                                     // - представитель компоненты
    int CompNumb = 1;
    int BreakComp = 1;
    int NextBegin = -1;
    find_way(graph, VertexFaces, EdgesFaces, MainWay, i, NextBegin);
    if (MainWay.size() == 1){
        return true;
    }
    VertexFaces[MainWay[0]].insert(1);
    laying_down_new_component(MainWay, VertexFaces, EdgesFaces, CompNumb, BreakComp);
    while (true) {
        std::vector<int> NextWay(0);
        std::pair<int, int> NextWayBegining = next_way_begin(VertexFaces, EdgesFaces, BreakComp);
        if (NextWayBegining.second == -1) {
            break;
        }
        else if (NextWayBegining.second == -2) {
            return false;
        }
        find_way(graph, VertexFaces, EdgesFaces, NextWay, NextWayBegining.first, NextWayBegining.second);
        if (NextWay.size() == 1){
            break;
        }
        if (!laying_down_new_component(NextWay, VertexFaces, EdgesFaces, CompNumb, BreakComp)){
            return false;
        }

    }
    return true;
}

bool gamma(const Graph& graph) {
    std::vector<std::set<int>> VertexFaces(graph.vertex_count(), std::set<int>());
    std::vector<std::vector<std::pair<int, std::pair<int, int>>>> EdgesFaces(graph.vertex_count(), std::vector<std::pair<int, std::pair<int, int>>>(0));
    for (int i = 0; i < graph.vertex_count(); i++){
        std::vector<int> next = graph.get_next_vertexes(i);
        for (auto v : next) {
            EdgesFaces[i].push_back(std::pair<int, std::pair<int, int>>(v, std::pair<int, int>(0, 0)));
        }
    }
    for (int i = 0; i < graph.vertex_count(); i++) { // запуск гамма алгоритма тля всех компонент связности
        if (VertexFaces[i].empty()) {
            if (!one_component_gamma(graph, VertexFaces, EdgesFaces, i)) {
                return false;
            }
        }
    }
    return true;
}

void sort_edges(const Graph& graph, Graph& SortEdgesGraph) {
    for (int i = 0; i < graph.vertex_count(); i++){
        std::vector<int> next = graph.get_next_vertexes(i);
        sort(next.begin(), next.end());
        for (int j = 0; j < next.size(); j++) {
            SortEdgesGraph.add_edge(i, j);
        }
    }
}

bool planary(const Graph& graph){ // проверка является ли граф планарным
    int n = graph.vertex_count();
    Graph NewGraph(n);
    simple(graph, NewGraph);
    Graph SortEdgesGraph(n);
    sort_edges(NewGraph, SortEdgesGraph);
    return gamma(NewGraph);
}

int main() {
    int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;
        if (a != b) {
            if (!graph.has_edge(a, b)){
                graph.add_edge(b, a);
            }
        }
    }
    if (planary(graph)) {
        std::cout << "YES";
    }
    else {
        std::cout << "NO";
    }
    return 0;
}