#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <set>

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
        nextdoors[to].push_back(from);
    }

    void sortedges() {
        for (int i = 0; i < nextdoors.size(); i++) {
            sort(nextdoors[i].begin(), nextdoors[i].end());
        }
    }

    bool HasEdge(int from, int to) {
        for (int i = 0; i < nextdoors[from].size(); i++) {
            if (nextdoors[from][i] == to) {
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
    int VortexNum;
};

void DelEdges (const Graph& graph, Graph& NewGraph, std::vector<int>& CompTime, std::vector<int>& used, int a){
    std::vector<int> next = graph.GetNextVertexes(a);
    for (int i = 0; i < next.size(); i++) {
        if (used[next[i]] == 2 and CompTime[next[i]] == CompTime[a]) {
            NewGraph.AddEdge(a, next[i]);
        }
    }
}

void timeDFS(const Graph& graph, Graph& NewGraph, int top, void (*visit)(const Graph& graph, Graph& NewGraph, std::vector<int>& CompTime, std::vector<int>& used, int i)) {
    int n = graph.VertexCount();
    std::vector<int> used (n, 0);
    std::stack<int> s;
    std::vector<int> Time (n, 0);
    std::vector<int> CompTime (n, 0);
    s.push(top);
    int time = 0;
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
                Time[next[i]] = Time[vertex] + 1;
                CompTime[next[i]] = Time[vertex] + 1;
                time++;
                p = 1;
            }
            else if (used[next[i]] == 1) {
                CompTime[vertex] = std::min(CompTime[vertex], CompTime[next[i]]);
            }
        }
        if (!p) {
            used[vertex] = 2;
            s.pop();
            for (int i = 0; i < next.size(); i++) {
                if (Time[next[i]] != Time[vertex] - 1) {
                    if (used[next[i]] == 1) {
                        CompTime[vertex] = std::min(CompTime[vertex], Time[next[i]]);
                    }
                    else {
                        CompTime[vertex] = std::min(CompTime[vertex], CompTime[next[i]]);
                    }
                }
            }
            visit(graph, NewGraph, CompTime, used, vertex);
        }
    }
}

void simple(const Graph& graph, Graph& NewGraph){
    timeDFS(graph, NewGraph, 0, DelEdges);
}

void modif(std::vector<int>& Way, int a, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EagesFaces) {
    std::vector<int> temp(0);
    int p = 0;
    for (int i = 0; i < Way.size(); i++) {
        if (!p){
            if (Way[i] == a) {
                p = 1;
                temp.push_back(a);
            }
            else {
                for (int g = 0; g < EagesFaces[Way[i]].size(); g++) {
                    if (EagesFaces[Way[i]][g].first == Way[i + 1]){
                        EagesFaces[Way[i]][g].second.first = 0;
                        break;
                    }
                }
                for (int g = 0; g < EagesFaces[Way[i + 1]].size(); g++) {
                    if (EagesFaces[Way[i + 1]][g].first == Way[i]){
                        EagesFaces[Way[i + 1]][g].second.first = 0;
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

void FindWay(const Graph& graph, std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EagesFaces, std::vector<int>& Way, int i, int NextWayBegin) {
    std::vector<int> used(graph.VertexCount(), 0);
    Way.push_back(i);
    int prev = -1;
    int now = i;
    used[i] = 1;
    if (NextWayBegin != -1) {
        Way.push_back(NextWayBegin);
        prev = now;
        now = NextWayBegin;
        used[NextWayBegin] = 1;
    }
    bool p = false;
    while (!p or VertexFaces[now].empty()) {
        p = true;
        if (prev != -1 and !VertexFaces[now].empty()) {
            break;
        }
        for (int j = 0; j < EagesFaces[now].size(); j++) {
            if (!EagesFaces[now][j].second.first and EagesFaces[now][j].first != prev) {
                EagesFaces[now][j].second.first = -1;
                for (int g = 0; g < EagesFaces[EagesFaces[now][j].first].size(); g++) {
                    if (EagesFaces[EagesFaces[now][j].first][g].first == now){
                        EagesFaces[EagesFaces[now][j].first][g].second.first = -1;
                        break;
                    }
                }
                prev = now;
                if (used[EagesFaces[now][j].first] == 1) {
                    Way.push_back(EagesFaces[now][j].first);
                    modif(Way, EagesFaces[now][j].first, EagesFaces);
                    return;
                }
                used[EagesFaces[now][j].first] = 1;
                now = EagesFaces[now][j].first;
                Way.push_back(now);
                break;
            }
        }
    }
}

void EagesBFS(std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EagesFaces, std::vector<std::vector<bool>>& used, std::set<int>& M, int a) {
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
        for (int i = 0; i < EagesFaces[now].size(); i++) {
            if (!used[now][i]) {
                used[now][i] = true;
                q.push(EagesFaces[now][i].first);
            }
        }
    }
}

std::pair<int, int> NextWayBegin(std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EagesFaces, int& BreakComp){
    std::stack<std::pair<int, std::pair<int, int>>> WayBegin;
    std::stack<int> Elements;
    std::vector<std::vector<bool>> used(EagesFaces.size(), std::vector<bool>(0));
    for (int i = 0; i < EagesFaces.size(); i++) {
        for (int j = 0; j < EagesFaces[i].size(); j++) {
            used[i].push_back(false);
        }
    }
    int element = -1;
    for (int i = 0; i < VertexFaces.size(); i++) {
        if (!VertexFaces[i].empty()) {
            for (int j = 0; j < EagesFaces[i].size(); j++) {
                if (VertexFaces[EagesFaces[i][j].first].empty()) {
                    std::set<int> M;
                    for (auto elem : VertexFaces[i]) {
                        M.insert(elem);
                    }
                    used[i][j] = true;
                    EagesBFS(VertexFaces, EagesFaces, used, M, EagesFaces[i][j].first);
                    for (auto elem : M) {
                        element = elem;
                    }
                    Elements.push(element);
                    if (M.empty()) {
                        return std::pair<int, int>(-2, -2);
                    }
                    WayBegin.push(std::pair<int, std::pair<int, int>>(M.size(),std::pair<int, int>(i, EagesFaces[i][j].first)));
                }
                else if (EagesFaces[i][j].second.first == 0) {
                    std::set<int> M;
                    for (auto elem : VertexFaces[i]) {
                        if (VertexFaces[EagesFaces[i][j].first].count(elem)){
                            M.insert(elem);
                        }
                    }
                    for (auto elem : M) {
                        element = elem;
                    }
                    Elements.push(element);
                    if (M.empty()) {
                        return std::pair<int, int>(-2, -2);
                    }
                    WayBegin.push(std::pair<int, std::pair<int, int>>(M.size(),std::pair<int, int>(i, EagesFaces[i][j].first)));
                }
            }
        }
    }
    if (WayBegin.empty()) {
        return std::pair<int, int>(-1, -1);
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

bool NewComp(const std::vector<int>& Way,std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EagesFaces, int& CompNumb, int& BreakComp){
    int prev = -1;
    int now = Way[0];
    while (now != Way[Way.size() - 1]) {
        for (int j = 0; j < EagesFaces[now].size(); j++) {
            if ((EagesFaces[now][j].second.first == BreakComp or EagesFaces[now][j].second.second == BreakComp) and EagesFaces[now][j].first != prev) {
                if (EagesFaces[now][j].second.first == BreakComp) {
                    EagesFaces[now][j].second.first = CompNumb + 1;
                    for (int g = 0; g < EagesFaces[EagesFaces[now][j].first].size(); g++) {
                        if (EagesFaces[EagesFaces[now][j].first][g].first == now) {
                            if (EagesFaces[EagesFaces[now][j].first][g].second.first == BreakComp) {
                                EagesFaces[EagesFaces[now][j].first][g].second.first = CompNumb + 1;
                            }
                            else {
                                EagesFaces[EagesFaces[now][j].first][g].second.second = CompNumb + 1;
                            }
                        }
                    }
                }
                else {
                    EagesFaces[now][j].second.second = CompNumb + 1;
                    for (int g = 0; g < EagesFaces[EagesFaces[now][j].first].size(); g++) {
                        if (EagesFaces[EagesFaces[now][j].first][g].first == now) {
                            if (EagesFaces[EagesFaces[now][j].first][g].second.first == BreakComp) {
                                EagesFaces[EagesFaces[now][j].first][g].second.first = CompNumb + 1;
                            }
                            else {
                                EagesFaces[EagesFaces[now][j].first][g].second.second = CompNumb + 1;
                            }
                        }
                    }
                }
                prev = now;
                now = EagesFaces[now][j].first;
                VertexFaces[now].insert(CompNumb + 1);
                VertexFaces[now].erase(BreakComp);
                break;
            }
        }
    }
    for (int i = 0; i < Way.size(); i++) {
        if (i == Way.size() - 1 and Way[0] == Way[Way.size() - 1]) {
            for (int j = 0; j < EagesFaces[Way[i]].size(); j++) {
                if (EagesFaces[Way[i]][j].first == Way[i - 1]){
                    EagesFaces[Way[i]][j].second.first = BreakComp;
                    EagesFaces[Way[i]][j].second.second = CompNumb + 1;
                }
            }
            for (int j = 0; j < EagesFaces[Way[i - 1]].size(); j++) {
                if (EagesFaces[Way[i - 1]][j].first == Way[i]){
                    EagesFaces[Way[i - 1]][j].second.second = CompNumb + 1;
                    EagesFaces[Way[i - 1]][j].second.first = BreakComp;
                }
            }
            break;
        }
        if (i != 0){
            VertexFaces[Way[i]].insert(BreakComp);
            for (int j = 0; j < EagesFaces[Way[i]].size(); j++) {
                if (EagesFaces[Way[i]][j].first == Way[i - 1]){
                    EagesFaces[Way[i]][j].second.first = BreakComp;
                    EagesFaces[Way[i]][j].second.second = CompNumb + 1;
                }
            }
            for (int j = 0; j < EagesFaces[Way[i - 1]].size(); j++) {
                if (EagesFaces[Way[i - 1]][j].first == Way[i]){
                    EagesFaces[Way[i - 1]][j].second.second = CompNumb + 1;
                    EagesFaces[Way[i - 1]][j].second.first = BreakComp;
                }
            }
        }
        VertexFaces[Way[i]].insert(CompNumb + 1);
        for (int j = 0; j < EagesFaces[Way[i]].size(); j++) {
            if (EagesFaces[Way[i]][j].second.first == -1) {
                EagesFaces[Way[i]][j].second.first = BreakComp;
                EagesFaces[Way[i]][j].second.second = CompNumb + 1;
            }
        }
    }
    CompNumb++;
    return true;
}

bool Gamma(const Graph& graph, std::vector<std::set<int>>& VertexFaces, std::vector<std::vector<std::pair<int, std::pair<int, int>>>>& EagesFaces, int i){
    std::vector<int> MainWay(0);
    int CompNumb = 1;
    int BreakComp = 1;
    int NextBegin = -1;
    FindWay(graph, VertexFaces, EagesFaces, MainWay, i, NextBegin);
    if (MainWay.size() == 1){
        return true;
    }
    VertexFaces[MainWay[0]].insert(1);
    NewComp(MainWay, VertexFaces, EagesFaces, CompNumb, BreakComp);
    while (true) {
        std::vector<int> NextWay(0);
        std::pair<int, int> NextWayBegining = NextWayBegin(VertexFaces, EagesFaces, BreakComp);
        if (NextWayBegining.second == -1) {
            break;
        }
        else if (NextWayBegining.second == -2) {
            return false;
        }
        FindWay(graph, VertexFaces, EagesFaces, NextWay, NextWayBegining.first, NextWayBegining.second);
        if (NextWay.size() == 1){
            break;
        }
        if (!NewComp(NextWay, VertexFaces, EagesFaces, CompNumb, BreakComp)){
            return false;
        }

    }
    return true;
}

bool InitialiseGamma(const Graph& graph) {
    std::vector<std::set<int>> VertexFaces(graph.VertexCount(), std::set<int>());
    std::vector<std::vector<std::pair<int, std::pair<int, int>>>> EagesFaces(graph.VertexCount(), std::vector<std::pair<int, std::pair<int, int>>>(0));
    for (int i = 0; i < graph.VertexCount(); i++){
        std::vector<int> next = graph.GetNextVertexes(i);
        for (int j = 0; j < next.size(); j++) {
            EagesFaces[i].push_back(std::pair<int, std::pair<int, int>>(next[j], std::pair<int, int>(0, 0)));
        }
    }
    for (int i = 0; i < graph.VertexCount(); i++) {
        if (VertexFaces[i].empty()) {
            if (!Gamma(graph, VertexFaces, EagesFaces, i)) {
                return false;
            }
        }
    }
    return true;
}

bool planary(Graph& graph){
    int n = graph.VertexCount();
    Graph NewGraph1(n);
    simple(graph, NewGraph1);
    Graph NewGraph(n);
    simple(NewGraph1, NewGraph);
    NewGraph.sortedges();
    return InitialiseGamma(NewGraph);
}

int main() {
    int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;
        if (a != b) {
            if (!graph.HasEdge(a, b)){
                graph.AddEdge(b, a);
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