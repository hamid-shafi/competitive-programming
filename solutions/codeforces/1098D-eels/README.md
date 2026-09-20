# Codeforces 1098D — Eels

[Problem](https://codeforces.com/problemset/problem/1098/D) · [Original submission](https://codeforces.com/contest/1098/submission/324185125) · [Readable submission](https://codeforces.com/contest/1098/submission/391354249) · Rating **2800**

The task maintains a multiset of eel weights under insertions and deletions and asks for the maximum number of dangerous fights after every operation.

## Core idea

For sorted weights `a[0] <= ... <= a[n-1]`, call `a[i]` *fat* when

```text
a[i] > 2 * sum(a[0 ... i-1]).
```

Each fat eel forces one non-dangerous fight, so the answer is:

```text
number of eels - number of fat eels
```

My solution maintains the slack

```text
slack[i] = a[i] - 2 * sum(a[0 ... i-1]).
```

Adding a weight activates one preallocated leaf and subtracts `2*x` from every larger weight; deletion reverses both changes. A lazy segment tree stores the maximum slack in every interval. While counting positive slacks, an entire node is skipped as soon as its maximum is non-positive.

Fat weights grow at least like `1, 3, 9, ...`, so with weights at most `1e9` there are at most 19 of them. Each operation therefore takes `O(log W * log Q)` time in the worst case here, with `O(Q)` memory.

The standard solution instead groups weights into powers-of-two buckets and maintains each bucket's sum and minimum; see the [official editorial](https://codeforces.com/blog/entry/64331).

## Files

- [`original.cpp`](original.cpp): the exact original accepted submission.
- [`readable.cpp`](readable.cpp): the same algorithm with descriptive names and structure; accepted in C++20 in 1546 ms.
