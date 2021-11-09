#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

void HasWay(std::vector<std::vector<std::bitset<32>>>& Matrix){
    for (int k = 0; k < Matrix.size(); k++){
        for (int i = 0; i < Matrix.size(); i++){
            if (Matrix[i][k / 32].test(k % 32)) {
                for (int j = 0; j < Matrix.size() / 32 + 1; j++){
                    Matrix[i][j] |= (Matrix[k][j]);
                }
            }
        }
    }
}

int main(){
    int n = 0;
    std::cin >> n;
    std::vector<std::vector<std::bitset<32>>> Matrix (0);
    for (int i = 0; i < n; i++) {
        std::vector<std::bitset<32>> string(0);
        for (int j = 0; j < n / 32 + 1; j++) {
            if (j * 32 >= n){
                break;
            }
            std::bitset<32> now;
            for (int k = 0; k < 32; k++){
                if (k + j * 32 >= n){
                    break;
                }
                char s;
                std::cin >> s;
                if (s == '1') {
                    now[k] = true;
                }
            }
            string.push_back(now);
        }
        Matrix.push_back(string);
    }
    HasWay(Matrix);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n / 32 + 1; j++){
            for (int k = 0; k < 32; k++) {
                if (k + j * 32 < n) {
                    std::cout << Matrix[i][j][k];
                }
            }
        }
        std::cout << "\n";
    }
}
