#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

void HasWay(std::vector<std::bitset<1000>>& Matrix){
    for (int k = 0; k < Matrix.size(); k++){
        for (int i = 0; i < Matrix.size(); i++){
            if (Matrix[i].test(999 - k)) {
                Matrix[i] |= (Matrix[k]);
            }
        }
    }
}

int main(){
    int n = 0;
    std::cin >> n;
    std::vector<std::bitset<1000>> Matrix (0);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        std::bitset<1000> line;
        for (int j = 0; j < s.size(); j++){
            if (s[j] == '1') {
                line.set(999 - j, true);
            }
        }
        Matrix.push_back(line);
    }
    HasWay(Matrix);
    for (int i = 0; i < n; i++){
        std::string line = Matrix[i].to_string();
        for (int j = 0; j < n; j++){
            std::cout << line[j];
        }
        std::cout << "\n";
    }
}