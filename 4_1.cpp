#include <iostream>
#include <vector>
#include <cmath>

struct sparse_table{
    std::vector<int> array_numb;
    std::vector<std::vector<int>> table;
    std::vector<int> fl;

    explicit sparse_table(const std::vector<int>& array) {
        fl.resize(0);
        fl.push_back(0);
        for (int i = 1; i < array.size(); i++) {
            fl.push_back(fl[(i - 1) / 2] + 1);
        }
        array_numb.resize(0);
        for (int i = 0; i < array.size(); i++){
            array_numb.push_back(array[i]);
        }
        table.resize(0);
        int len = 0;
        for (int i = 0; i < fl[array.size() - 1] + 1; i++) {
            std::vector<int> new_line(0);
            len *= 2;
            if (i == 1){
                len = 1;
            }
            for (int j = 0; j < array.size(); j++) {
                if (j + len * 2 > array.size()){
                    break;
                }
                if (i == 0) {
                    new_line.push_back(j);
                }
                else {
                    if (array[table[i - 1][j]] < array[table[i - 1][j + len]]){
                        new_line.push_back(table[i - 1][j]);
                    }
                    else {
                        new_line.push_back(table[i - 1][j + len]);
                    }
                }
            }
            table.push_back(new_line);
        }
    }

    int get_min(int a, int b) const{
        int j = fl[b - a];
        if (array_numb[table[j][a]] <= array_numb[table[j][b - pow(2, j) + 1]]) {
            return table[j][a];
        }
        return table[j][b - pow(2, j) + 1];
    }
};

int get_second_statistics(const sparse_table& sp, const std::vector<int>& array, int a, int b){
    int min_numb = sp.get_min(a, b);
//    std::cout << "(" << min_numb << ") ";
    if (a == min_numb){
        return sp.get_min(a + 1, b);
    }
    else if (b == min_numb){
        return sp.get_min(a, b - 1);
    }
    int first = sp.get_min(a, min_numb - 1);
    int second = sp.get_min(min_numb + 1, b);
    if (array[first] <= array[second]) {
        return first;
    }
    return second;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> array(0);
    for (int i = 0; i < n; i++) {
        int elem;
        std::cin >> elem;
        array.push_back(elem);
    }
    sparse_table sp = sparse_table(array);
    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;
        std::cout << array[get_second_statistics(sp, array, a - 1, b - 1)] << "\n";
    }
}
