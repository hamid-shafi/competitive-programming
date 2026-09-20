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
const ll  INF    = 1e9  + 1;
const int MXN    = 3e5  + 5;
const int LOG    = 23;

ll Pow(ll a, ll b) { return !b ? 1 : (Pow(a*a %MOD, b/2) * (b&1 ? a : 1)) %MOD; }

int q, N[LOG];
vector<int> nxt[LOG][26], ch[LOG][26], f[LOG], w[LOG];
set<pii> Ahos;
vector<pair<string, int>> qs[LOG];
vector<vector<int>> adj[LOG];
bool haveAho[LOG];

void creatNode(int p, int c, int id, bool o = 1) {
    if (o) {
        N[id]++;
        ch[id][c][p] = N[id];
    }
    for (int i = 0; i < 26; i++) {
        ch[id][i].push_back(0);
        nxt[id][i].push_back(0);
    }
    f[id].push_back(0);
    w[id].push_back(0);
    adj[id].push_back({});
}

void addString(string s, int W, int id) {
    int v = 0;
    qs[id].push_back({s, W});
    for (int i = 0; i < s.size(); i++) {
        if (!ch[id][s[i] - 'a'][v]) creatNode(v, s[i] - 'a', id);
        v = ch[id][s[i] - 'a'][v];
    }
    w[id][v] += W;
}

void bfsOnAho(int id) {
    queue<int> qu;
    qu.push(0);
    while (qu.size()) {
        int v = qu.front();
        qu.pop();
        for (int i = 0; i < 26; i++)
            if (ch[id][i][v]) {
                f[id][ch[id][i][v]] = nxt[id][i][f[id][v]];
                qu.push(ch[id][i][v]);
                adj[id][f[id][ch[id][i][v]]].push_back(ch[id][i][v]);
                nxt[id][i][v] = ch[id][i][v];
            }
            else nxt[id][i][v] = nxt[id][i][f[id][v]];
    }
}

void dfsOnAho(int id, int v = 0) {
    for (int u : adj[id][v]) {
        w[id][u] += w[id][v];
        dfsOnAho(id, u);
    }
}

int Get(string t, int id) {
    int ans = 0, v = 0;
    for (int i = 0; i < t.size(); i++) {
        v = nxt[id][t[i] - 'a'][v];
        ans += w[id][v];
    }
    return ans;
}

void clearAho(int id) {
    adj[id].clear();
    f[id].clear();
    w[id].clear();
    for (int i = 0; i < 26; i++) {
        nxt[id][i].clear();
        ch[id][i].clear();
    }
    N[id] = 0;
    qs[id].clear();
}

void Merge(int i1, int i2) {
    vector<pair<string, int>> st;
    for (auto u : qs[i1]) st.push_back(u);
    for (auto u : qs[i2]) st.push_back(u);
    clearAho(i1);
    clearAho(i2);
    creatNode(0, 0, i1, 0);
    for (auto u : st) {
        addString(u.F, u.S, i1);
    }
    bfsOnAho(i1);
    dfsOnAho(i1);
    haveAho[i2] = 0;
}

void addQuery(string s, int W) {
    for (int i = 0; i < LOG; i++) {
        if (!haveAho[i]) {
            creatNode(0, 0, i, 0);
            addString(s, W, i);
            bfsOnAho(i);
            dfsOnAho(i);
            Ahos.insert({N[i], i});
            haveAho[i] = 1;
            break;
        }
    }
    while (1) {
        bool ok = 0;
        auto q = Ahos.begin();
        for (int i = 0; i < Ahos.size() - 1; i++) {
            if ((*q).F * 2 >= (*(next(q))).F) {
                ok = 1;
                break;
            }
            q = next(q);
        }
        if (!ok) break;
        else {
            int p = (*q).S;
            Merge((*q).S, (*(next(q))).S);
            Ahos.erase(next(q));
            Ahos.erase(q);
            Ahos.insert({N[p], p});
        }
    }
}

void _solve() {
    cin >> q;
    for (int i = 1; i <= q; i++) {
        string s;
        int e;
        cin >> e >> s;
        if (e == 3) {
            int ans = 0;
            for (pii i : Ahos) {
                ans += Get(s, i.S);
            }
            cout << ans << endl;
        }
        else {
            addQuery(s, (e == 1 ? 1 : -1));
        }
    }   
}

int32_t main() {
    cin.tie(0); ios_base::sync_with_stdio(0);
    int _ = 1;
    // cin >> _;
    while (_--) _solve();
    return 0.0;
}
