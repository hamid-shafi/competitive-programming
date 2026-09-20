/* _In The Name Of God_ */

#include <bits/stdc++.h>
using namespace std;

#define maxs(a, b)			a = max(a, b)
#define mins(a, b)			a = min(a, b)
#define pb						push_back
#define F						first
#define S						second
#define lc						id << 1
#define rc						lc|1
#define mid						((l + r)/2)
// #define int                     long long

typedef pair<int, int>     	pii;
typedef long long               	ll;

const ll  MOD    = 1e9  + 7; // 998244353;
const ll  INF    = 1e17;
const int MXN    = 5e5  + 1;
const int LOG    = 23;
const int SQ     = 1;

ll Pow(ll a, ll b) { return !b ? 1 : (Pow(a*a %MOD, b/2) * (b&1 ? a : 1)) %MOD; }

int n;
ll seg[MXN<<2][SQ+1], lz[MXN<<2], v[MXN];
char c[MXN];

void Merge(int id) {
    int p1= 1, p2 = 1;
    seg[id][0] = 0;
    for (int i = 1; i <= SQ; i++) {
        if (seg[lc][p1] < seg[rc][p2]) {
            seg[id][0] += (seg[rc][p2] > 0);
            seg[id][i] = seg[rc][p2];
            p2++;
        }
        else {
            seg[id][0] += (seg[lc][p1] > 0);
            seg[id][i] = seg[lc][p1];
            p1++;
        }
    }
}

void Build(int l = 1, int r = n + 1, int id = 1) {
    if (r - l < 2) {
        seg[id][0] = 0;
        for (int i = 1; i <= SQ; i++) seg[id][i] = -INF - (i > 1 ? INF : 0);
        return;
    }
    Build(l, mid, lc);
    Build(mid, r, rc);
    Merge(id);
}

void Put(ll x, int id) {
    lz[id] += x;
    seg[id][0] = 0;
    for (int i = 1; i <= SQ; i++) {
        seg[id][0] += ((seg[id][i] += x) > 0);
    }
}

void Shift(int l, int r, int id) {
    if (r - l > 1 && lz[id]) {
        Put(lz[id], lc);
        Put(lz[id], rc);
        lz[id] = 0;
    }
}
void Upd(int s, int e, ll x, int l = 1, int r = n + 1, int id = 1) {
    Shift(l, r, id);
    if (s <= l && r <= e) {
        Put(x, id);
        return;
    }
    if (s < mid) Upd(s,e,x, l, mid, lc);
    if (mid < e) Upd(s,e,x, mid, r, rc);
    Merge(id);
}

int Get(int l = 1, int r = n + 1, int id = 1) {
    if (seg[id][0] < SQ || r - l < 2) return seg[id][0];
    Shift(l, r, id);
    return Get(l, mid, lc) + Get(mid, r, rc);
}

void _solve() {
    int q; cin >> q;
    unordered_map<int, int> mp;
    map<int, int> pm, mx;
    for (int i = 1; i <= q; i++) {
        cin >> c[i] >> v[i];
        if (c[i] == '+') {
            mp[v[i]]++;
            maxs(mx[-v[i]], mp[v[i]]);
        }
        else {
            mp[v[i]]--;
        }
    }
    for (pii i : mx) {
        pm[i.F] = n;
        n += i.S;
    }
    Build();
    mp.clear();
    int k = 0;
    for (int i = 1; i <= q; i++) {
        k += (c[i] == '+') ? 1 : -1;
        if (c[i] == '+') {
            mp[v[i]]++;
            int I = pm[-v[i]] + mp[v[i]];
            Upd(I, I + 1, INF + v[i]);
            if (I != 1)
            Upd(1, I, -2ll*v[i]);
        }
        else {
            int I = pm[-v[i]] + mp[v[i]];
            Upd(I, I+1, -INF-v[i]);
            if (I != 1)
            Upd(1, I, 2ll*v[i]);
            mp[v[i]]--;
        }
        cout << k-Get() << '\n';
    }
}

int32_t main() {
    cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(0);
    int _ = 1;
    // cin >> _;
    while (_--) _solve();
    return 0.0;
}
