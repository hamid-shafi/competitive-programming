#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::greater;
using std::int64_t;
using std::map;
using std::max;
using std::pair;
using std::unordered_map;
using std::vector;

class LazyMaxSegmentTree {
public:
    explicit LazyMaxSegmentTree(int size)
        : size_(size), maximum_(4 * size, -kInfinity), lazy_(4 * size, 0) {}

    void add(int query_left, int query_right, int64_t delta) {
        add(query_left, query_right, delta, 1, 0, size_);
    }

    int count_positive() {
        return count_positive(1, 0, size_);
    }

private:
    static constexpr int64_t kInfinity = int64_t{1} << 60;

    int size_;
    vector<int64_t> maximum_;
    vector<int64_t> lazy_;

    void apply(int node, int64_t delta) {
        maximum_[node] += delta;
        lazy_[node] += delta;
    }

    void push(int node) {
        if (lazy_[node] == 0) {
            return;
        }
        apply(node * 2, lazy_[node]);
        apply(node * 2 + 1, lazy_[node]);
        lazy_[node] = 0;
    }

    void add(int query_left, int query_right, int64_t delta,
             int node, int segment_left, int segment_right) {
        if (query_right <= segment_left || segment_right <= query_left) {
            return;
        }
        if (query_left <= segment_left && segment_right <= query_right) {
            apply(node, delta);
            return;
        }

        push(node);
        const int middle = (segment_left + segment_right) / 2;
        add(query_left, query_right, delta, node * 2, segment_left, middle);
        add(query_left, query_right, delta, node * 2 + 1, middle, segment_right);
        maximum_[node] = max(maximum_[node * 2], maximum_[node * 2 + 1]);
    }

    int count_positive(int node, int segment_left, int segment_right) {
        if (maximum_[node] <= 0) {
            return 0;
        }
        if (segment_right - segment_left == 1) {
            return 1;
        }

        push(node);
        const int middle = (segment_left + segment_right) / 2;
        return count_positive(node * 2, segment_left, middle)
             + count_positive(node * 2 + 1, middle, segment_right);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int query_count;
    cin >> query_count;

    vector<pair<char, int>> operations(query_count);
    unordered_map<int, int> frequency;
    map<int, int, greater<int>> maximum_copies;
    frequency.reserve(2 * query_count);

    for (auto &[type, weight] : operations) {
        cin >> type >> weight;
        if (type == '+') {
            const int copies = ++frequency[weight];
            maximum_copies[weight] = max(maximum_copies[weight], copies);
        } else {
            --frequency[weight];
        }
    }

    unordered_map<int, int> first_slot;
    first_slot.reserve(2 * maximum_copies.size());
    int slot_count = 0;
    for (const auto &[weight, copies] : maximum_copies) {
        first_slot[weight] = slot_count;
        slot_count += copies;
    }

    constexpr int64_t kInfinity = int64_t{1} << 60;
    LazyMaxSegmentTree tree(slot_count);
    frequency.clear();

    int eel_count = 0;
    for (const auto &[type, weight] : operations) {
        if (type == '+') {
            ++eel_count;
            const int slot = first_slot[weight] + frequency[weight]++;
            tree.add(slot, slot + 1, kInfinity + weight);
            tree.add(0, slot, -2LL * weight);
        } else {
            --eel_count;
            const int slot = first_slot[weight] + frequency[weight] - 1;
            tree.add(slot, slot + 1, -kInfinity - weight);
            tree.add(0, slot, 2LL * weight);
            --frequency[weight];
        }

        cout << eel_count - tree.count_positive() << '\n';
    }
}
