#include <iostream>
#include <vector>

struct segment_tree {
public:
    explicit segment_tree(const std::vector<int>& tickets_number) {
        buffer.resize(find_next_power_of_two(tickets_number.size()) * 2);
        for (int i = buffer.size() / 2; i < buffer.size(); i++) {
            if (i < buffer.size() / 2 + tickets_number.size()) {
                buffer[i] = tickets_number[i - buffer.size() / 2];
            }
            else {
                buffer[i] = -1;
            }
        }
        for (int i = buffer.size() / 2 - 1; i > 0; i--) {
            buffer[i] = std::max(buffer[2 * i], buffer[2 * i + 1]);
        }
        buffer[0] = -1;
    }

    int get_max(int first, int last){
        return get(0, buffer.size() / 2 - 1, first, last, 1);
    }

    void add_numb(int first, int last, int number) {
        add(0, buffer.size() / 2 - 1, first, last, 1, number);
    }
private:
    std::vector<int> buffer;

    int add(int L, int R, int l, int r, int v, int number) {
        if (R < l or r < L) {
            return buffer[v];
        }
        if (L == R) {
            if (l <= L and L <= r) {
                buffer[v] += number;
            }
            return buffer[v];
        }
        buffer[v] = std::max(add(L, (L + R) / 2, l, r, v * 2, number), add((L + R) / 2 + 1, R, l, r, v * 2 + 1, number));
        return buffer[v];
    }

    int get(int L, int R, int l, int r, int v){
        if (L > r || R < l){
            return -1;
        }
        if (l <= L && R <= r) {
            return buffer[v];
        }
        int ans = std::max(get(L, (L + R) / 2, l, r, v * 2), get((L + R) / 2 + 1, R, l, r, v * 2 + 1));
        return ans;
    }

    int find_next_power_of_two(int nubmer) const {
        int pow = 1;
        while (pow < nubmer) {
            pow *= 2;
        }
        return pow;
    }
};

bool request_processing(segment_tree& seg_tree, int capacity, int first, int last, int number) {
    if (seg_tree.get_max(first, last) + number <= capacity) {
        seg_tree.add_numb(first, last, number);
        return true;
    }
    return false;
}

int main() {
    int n;
    std::cin >> n;
    n--;
    std::vector<int> tickets_number(0);
    for (int i = 0; i < n; i++) {
        int tickets;
        std::cin >> tickets;
        tickets_number.push_back(tickets);
    }
    int capacity, m;
    std::cin >> capacity >> m;
    segment_tree seg_tree = segment_tree(tickets_number);
    for (int i = 0; i < m; i++) {
        int first, last, number;
        std::cin >> first >> last >> number;
        if (first == last) {
            continue;
        }
        if (n == 0 or !request_processing(seg_tree, capacity, first, last - 1, number)) {
            std::cout << i << "\n";
        }
    }
}