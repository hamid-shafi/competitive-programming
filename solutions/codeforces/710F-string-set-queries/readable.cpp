#include <array>
#include <iostream>
#include <iterator>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::optional;
using std::pair;
using std::queue;
using std::set;
using std::string;
using std::vector;

struct WeightedPattern {
    string text;
    int weight;
};

class AhoCorasick {
public:
    explicit AhoCorasick(const vector<WeightedPattern>& patterns) {
        for (const auto& pattern : patterns) {
            add_pattern(pattern);
        }
        build();
    }

    int trie_size() const {
        return static_cast<int>(nodes_.size()) - 1;
    }

    long long count_occurrences(const string& text) const {
        long long answer = 0;
        int state = 0;

        for (char character : text) {
            state = nodes_[state].transition[character - 'a'];
            answer += nodes_[state].matched_weight;
        }
        return answer;
    }

private:
    struct Node {
        array<int, 26> child;
        array<int, 26> transition;
        int failure = 0;
        int matched_weight = 0;

        Node() {
            child.fill(-1);
            transition.fill(0);
        }
    };

    vector<Node> nodes_{1};

    void add_pattern(const WeightedPattern& pattern) {
        int state = 0;
        for (char character : pattern.text) {
            const int letter = character - 'a';
            if (nodes_[state].child[letter] == -1) {
                nodes_[state].child[letter] = static_cast<int>(nodes_.size());
                nodes_.emplace_back();
            }
            state = nodes_[state].child[letter];
        }
        nodes_[state].matched_weight += pattern.weight;
    }

    void build() {
        queue<int> pending;

        for (int letter = 0; letter < 26; ++letter) {
            const int child = nodes_[0].child[letter];
            if (child != -1) {
                nodes_[0].transition[letter] = child;
                pending.push(child);
            }
        }

        while (!pending.empty()) {
            const int state = pending.front();
            pending.pop();

            nodes_[state].matched_weight +=
                nodes_[nodes_[state].failure].matched_weight;

            for (int letter = 0; letter < 26; ++letter) {
                const int child = nodes_[state].child[letter];
                if (child == -1) {
                    nodes_[state].transition[letter] =
                        nodes_[nodes_[state].failure].transition[letter];
                    continue;
                }

                nodes_[state].transition[letter] = child;
                nodes_[child].failure =
                    nodes_[nodes_[state].failure].transition[letter];
                pending.push(child);
            }
        }
    }
};

class AutomatonBlock {
public:
    explicit AutomatonBlock(vector<WeightedPattern> patterns)
        : patterns_(std::move(patterns)), automaton_(patterns_) {}

    const vector<WeightedPattern>& patterns() const {
        return patterns_;
    }

    int trie_size() const {
        return automaton_.trie_size();
    }

    long long count_occurrences(const string& text) const {
        return automaton_.count_occurrences(text);
    }

private:
    vector<WeightedPattern> patterns_;
    AhoCorasick automaton_;
};

class DynamicPatternSet {
public:
    void apply_update(const string& pattern, int weight) {
        const int slot = first_free_slot();
        blocks_[slot].emplace(vector<WeightedPattern>{{pattern, weight}});
        active_blocks_.insert({blocks_[slot]->trie_size(), slot});
        merge_comparable_blocks();
    }

    long long count_occurrences(const string& text) const {
        long long answer = 0;
        for (const auto& [size, slot] : active_blocks_) {
            answer += blocks_[slot]->count_occurrences(text);
        }
        return answer;
    }

private:
    static constexpr int kMaximumBlocks = 23;

    array<optional<AutomatonBlock>, kMaximumBlocks> blocks_;
    set<pair<int, int>> active_blocks_;

    int first_free_slot() const {
        for (int slot = 0; slot < kMaximumBlocks; ++slot) {
            if (!blocks_[slot].has_value()) {
                return slot;
            }
        }
        return -1;
    }

    void merge_comparable_blocks() {
        while (true) {
            bool merged = false;

            for (auto left = active_blocks_.begin(); left != active_blocks_.end();) {
                const auto right = std::next(left);
                if (right == active_blocks_.end()) {
                    break;
                }
                if (2LL * left->first < right->first) {
                    left = right;
                    continue;
                }

                const pair<int, int> left_entry = *left;
                const pair<int, int> right_entry = *right;
                const int destination = left_entry.second;
                const int source = right_entry.second;

                vector<WeightedPattern> combined = blocks_[destination]->patterns();
                const auto& source_patterns = blocks_[source]->patterns();
                combined.insert(combined.end(), source_patterns.begin(),
                                source_patterns.end());

                active_blocks_.erase(left_entry);
                active_blocks_.erase(right_entry);
                blocks_[destination].emplace(std::move(combined));
                blocks_[source].reset();
                active_blocks_.insert(
                    {blocks_[destination]->trie_size(), destination});

                merged = true;
                break;
            }

            if (!merged) {
                return;
            }
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int query_count;
    cin >> query_count;

    DynamicPatternSet patterns;
    while (query_count--) {
        int type;
        string text;
        cin >> type >> text;

        if (type == 1) {
            patterns.apply_update(text, 1);
        } else if (type == 2) {
            patterns.apply_update(text, -1);
        } else {
            cout << patterns.count_occurrences(text) << endl;
        }
    }
}
