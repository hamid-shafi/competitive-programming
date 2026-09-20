# Codeforces 710F — String Set Queries

[Problem](https://codeforces.com/problemset/problem/710/F) · [Original submission](https://codeforces.com/contest/710/submission/295351677) · Rating **2400**

Maintain an online set of lowercase strings. An operation inserts or deletes a pattern, or provides a text and asks for the total number of occurrences of all active patterns. Overlapping occurrences are counted, and the sum of all input lengths is at most `300,000`.

## Core idea

Treat [Aho–Corasick](https://cp-algorithms.com/string/aho_corasick.html) as a static black box: after building it from a group of weighted patterns, it counts all their occurrences in a text in linear time.

To support online updates, maintain several independent automatons whose trie sizes grow geometrically. A new update begins as a one-pattern automaton. Whenever two automatons have comparable sizes, merge their pattern lists and rebuild one automaton. This leaves only `O(log S)` active automatons, where `S` is the total input length, so a text query takes `O(|text| log S)` time.

An insertion has terminal weight `+1`, while a deletion has weight `-1`. These weights are accumulated through failure links when an automaton is built. Querying every active automaton and summing the results then makes deleted patterns cancel automatically.

This is the same logarithmic-rebuilding idea as the [official editorial](https://codeforces.com/blog/entry/46761). The editorial uses power-of-two groups and separate structures for inserted and deleted patterns; this implementation combines both using signed weights and balances groups by trie size.

## Files

- [`original.cpp`](original.cpp): the exact original accepted submission, in 1515 ms using C++20.
- [`readable.cpp`](readable.cpp): the same approach with descriptive names and structure.
