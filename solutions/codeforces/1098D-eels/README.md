# Codeforces 1098D — Eels

[Problem](https://codeforces.com/problemset/problem/1098/D) · [Original submission](https://codeforces.com/contest/1098/submission/324185125) · [Readable submission](https://codeforces.com/contest/1098/submission/391354249) · Rating **2800**

Eels fight until only one remains. If two eels of weights `a <= b` fight, one survives with weight `a + b`; the fight is **dangerous** when `b <= 2a`.

Starting from an empty multiset, process up to `500,000` insertions and deletions of weights up to `10^9`, with duplicates allowed. After each update, output the maximum possible number of dangerous fights over all orders of fights.

## Core idea

### Reduction to a range-add problem

First consider the following independent data-structure problem:

> Maintain an integer array of length `N` under `Q` range-add updates. After every update, report how many entries are at least zero. It is guaranteed that this count never exceeds `T`.

Build a lazy segment tree that stores the maximum value in every segment. A range addition takes `O(log N)`. To count the non-negative entries, discard a node as soon as its maximum is negative; otherwise, descend into its children. Since at most `T` leaves qualify, every visited node lies on one of those root-to-leaf paths or is a sibling pruned from such a path. One count therefore takes `O((T + 1) log N)` time.

For `Q` update-and-count pairs, the total complexity is

```text
O(Q (T + 1) log N) time, O(N) memory.
```

### Mapping Eels to the array

Let the active weights be sorted as `a[0] <= ... <= a[m-1]`, and define

```text
slack[i] = a[i] - 2 * sum(a[0 ... i-1]).
```

Call an eel *fat* when `slack[i] > 0`. The useful characterization is

```text
maximum dangerous fights = m - number of fat eels.
```

The first eel is always fat. Every additional fat eel creates one unavoidable non-dangerous fight, and there is an order of fights that attains this bound. Because all values are integers, counting `slack[i] > 0` is exactly the same as counting `slack[i] - 1 >= 0`, so this is the range-add problem above. The code keeps the unshifted slack and tests for strict positivity directly.

All operations are read first so that enough leaves can be reserved for the maximum simultaneous multiplicity of every weight. The leaves are ordered by decreasing weight, and an inactive leaf starts near negative infinity. Prefix updates still affect inactive leaves, so each one accumulates the contribution of smaller active weights before it is needed.

Inserting `x` activates the next reserved leaf by adding `INF + x`. It also subtracts `2x` from the slack of every eel placed after this copy in nondecreasing order: all larger weights and the relevant existing copies of `x`. In the reversed leaf order, those entries form one prefix. Deleting `x` reverses both additions. Thus every eel operation becomes two range additions followed by one count query.

Fat weights grow at least as `1, 3, 9, ...`, so `T <= 1 + floor(log_3 W)`, where `W` is the maximum weight. Here `W <= 10^9`, hence `T <= 19`. Since `N <= Q`, the accepted implementation runs in `O(Q T log Q) = O(Q log W log Q)` time and uses `O(Q)` memory.

The standard solution instead groups weights into powers-of-two buckets and maintains each bucket's sum and minimum; see the [official editorial](https://codeforces.com/blog/entry/64331).

## Files

- [`original.cpp`](original.cpp): the exact original accepted submission.
- [`readable.cpp`](readable.cpp): the same algorithm with descriptive names and structure; accepted in C++20 in 1546 ms.
