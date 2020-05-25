#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

#define MAX_MATRIX_SIZE 1000

void HasWay(std::vector<std::bitset<MAX_MATRIX_SIZE>>& Matrix){
    for (int k = 0; k < Matrix.size(); k++){
        for (int i = 0; i < Matrix.size(); i++){
            if (Matrix[i].test(MAX_MATRIX_SIZE - 1 - k)) {
                Matrix[i] |= (Matrix[k]);
            }
        }
    }
}

int main(){
    int n = 0;
    std::cin >> n;
    std::vector<std::bitset<MAX_MATRIX_SIZE>> Matrix (0);
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        std::bitset<MAX_MATRIX_SIZE> line;
        for (int j = 0; j < s.size(); j++){
            if (s[j] == '1') {
                line.set(MAX_MATRIX_SIZE - 1 - j, true);
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
