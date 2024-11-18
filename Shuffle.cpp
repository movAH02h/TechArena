#include <bits/stdc++.h>

using namespace std;

#define pb emplace_back
#define sz(x) (int)x.size()
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define fastio() ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr)

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int N = 2e2 + 10;
const int TIMES = 200;
const int INF = 1e9;

mt19937 rnd(time(nullptr));
mt19937_64 rndll(time(nullptr));

struct Q {
    int x;
    int y;
    ld s;
};

vector<Q> q;
int n, m, F;
vector<int> ans, order, par, siz;
vector<ld> c;
ld M, mn = 1e100, TIME = (5 - (ld)TIMES / 800) / TIMES;
vector<int> used;

int get(int x) {
    return x == par[x] ? x : par[x] = get(par[x]);
}

void uni(int l, int r) {
    l = get(l), r = get(r);
    if (siz[l] < siz[r]) {
        swap(l, r);
    }
    par[r] = l;
    siz[l] += siz[r];
}

ld penalty(ld res) {
    return res <= M ? 0 : (res - M) * F;
}

ld calc() {
    ld cost = 0;
    auto cur = c;
    siz.assign(n, 1);
    iota(all(par), 0);

    for (int i : order) {
        auto [x, y, s] = q[i];
        x = get(x), y = get(y);

        if (x == y) {
            ld res = cur[x] * s;
            cost += cur[x] + res + penalty(res);
            cur[x] = res;
        } else {
            ld res = cur[x] * cur[y] * s;
            cost += cur[x] + cur[y] + res + penalty(res);
            cur[x] = cur[y] = res;
            uni(x, y);
        }
    }

    return cost;
}

void calc_for_order(string str = "") {
    for (int &i : order) {
        cin >> i;
        --i;
    }
    cout << str << ": " << fixed << setprecision(15) << calc() << '\n';
}

void print(vector<int> &mas) {
    for (int i : mas) {
        cout << i + 1 << ' ';
    }
    cout << '\n';
}

int rand(int l, int r, int wout = -1) {
    int ret;
    do {
        ret = rnd() % (r - l);
    } while (ret == wout);
    return ret;
}

ld ver() {
    return (ld)rnd() / RAND_MAX;
}

void annealing() {
    iota(all(order), 0);
    shuffle(all(order), rnd);

    

    long double t = 1;
    auto start = clock();
    while ((ld)(clock() - start) / CLOCKS_PER_SEC < TIME) {
        t *= 0.99;
        int i = rand(0, m);
        int j = rand(0, m, i);

        swap(order[i], order[j]);
        ld cost = calc();

        if (cost < mn) { // точно лучше
            ans = order;
            mn = cost;
        } else
        { // хуже и вероятность не прокнула, откатываем
            swap(order[i], order[j]);
        } // хуже, но оставляем
    }

    // cout << "temp: " << t << '\n';
}

signed main() {
#ifdef local:
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#else
#endif

    auto start = clock();

    cin >> n >> m >> M >> F;

    q.resize(m);
    c.resize(n);
    used.resize(m);
    par.resize(n);
    siz.resize(n);

    for (auto &x : c) {
        cin >> x;
    }
    for (auto &[x, y, s] : q) {
        cin >> x >> y >> s;
        if (x > y) {
            swap(x, y);
        }
        y--;
        x--;
    }

    ans.resize(m);
    iota(all(ans), 0);
    order = ans;
    mn = calc();

    for (int it = 0; it < TIMES; ++it) {
        annealing();
    }

    print(ans);

    // order[0] = 11;
    // order[1] = 12;
    // order[2] = 8;
    // order[3] = 13;
    //
    // used[11] = 1;
    // used[12] = 1;
    // used[8] = 1;
    // used[13] = 1;
    //
    // rec(4);
    //
    // print(ans);
    // cout << fixed << setprecision(15) << mn << '\n';
    // order = ans;
    // calc();

    // cout << "cur ans: " << fixed << setprecision(15) << mn << '\n';
    // calc_for_order("best ans");
    // cout << fixed << setprecision(15) << "diff (more is better): " << calc() - mn << '\n';
    // cout << "time: " << fixed << setprecision(3) << (ld)(clock() - start) / CLOCKS_PER_SEC << '\n';
}