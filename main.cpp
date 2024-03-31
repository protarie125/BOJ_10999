#include <algorithm>
#include <bit>
#include <chrono>
#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

using vb = vector<bool>;
using vi = vector<int>;
using vl = vector<ll>;
using vd = vector<double>;
using vii = vector<pii>;
using vll = vector<pll>;
using vs = vector<string>;

using vvb = vector<vb>;
using vvi = vector<vi>;
using vvl = vector<vl>;
using vvd = vector<vd>;

ll n, u, q;
vl Arr;
ll sz;
vl Tree;
vl Lazy;

constexpr auto kDefault = 0LL;

ll Le(ll k) {
  return 2 * k;
}
ll Ri(ll k) {
  return 2 * k + 1;
}
ll Op(const ll& lhs, const ll& rhs) {
  return lhs + rhs;
}
ll Pull(ll b, ll e, ll x) {
  return (e - b) * x;
}

void Init(ll k, ll l, ll r) {
  if (r <= l) {
    return;
  }

  if (l + 1 == r) {
    Tree[k] = Arr[l];
    return;
  }

  const auto& mid = (l + r) / 2;
  Init(Le(k), l, mid);
  Init(Ri(k), mid, r);

  Tree[k] = Op(Tree[Le(k)], Tree[Ri(k)]);
}
void Push(ll k, ll b, ll e) {
  if (0 == Lazy[k]) {
    return;
  }

  Tree[k] += Pull(b, e, Lazy[k]);

  if (b + 1 != e) {
    Lazy[Le(k)] += Lazy[k];
    Lazy[Ri(k)] += Lazy[k];
  }

  Lazy[k] = 0;
}
void Add(ll k, ll b, ll e, ll l, ll r, ll x) {
  Push(k, b, e);

  if (e <= b || e <= l || r <= b) {
    return;
  }

  if (l <= b && e <= r) {
    Tree[k] += Pull(b, e, x);

    if (b + 1 != e) {
      Lazy[Le(k)] += x;
      Lazy[Ri(k)] += x;
    }
    return;
  }

  const auto& mid = (b + e) / 2;
  Add(Le(k), b, mid, l, r, x);
  Add(Ri(k), mid, e, l, r, x);

  Tree[k] = Op(Tree[Le(k)], Tree[Ri(k)]);
}
ll Get(ll k, ll b, ll e, ll l, ll r) {
  Push(k, b, e);

  if (e <= b || e <= l || r <= b) {
    return kDefault;
  }

  if (l <= b && e <= r) {
    return Tree[k];
  }

  const auto& mid = (b + e) / 2;
  return Op(Get(Le(k), b, mid, l, r), Get(Ri(k), mid, e, l, r));
}

void Print() {
  auto p = 1LL;
  auto w = 1LL;
  while (p < 2 * sz) {
    for (auto i = 0; i < w; ++i) {
      cout << "| " << Tree[p + i] << ", " << Lazy[p + i] << ' ';
    }
    cout << endl;

    p += w;
    w *= 2;
  }
}

int main() {
  ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  cin >> n >> u >> q;

  sz = 1;
  while (sz < n) {
    sz *= 2;
  }

  Arr = vl(sz, kDefault);
  for (auto i = 0; i < n; ++i) {
    cin >> Arr[i];
  }
  Tree = vl(2 * sz, kDefault);
  Lazy = vl(2 * sz, kDefault);

  Init(1, 0, sz);
  //Print();

  for (auto i = 0; i < (u + q); ++i) {
    ll a, l, r, x;
    cin >> a;
    if (1 == a) {
      cin >> l >> r >> x;
      Add(1, 0, sz, l - 1, r, x);
    } else if (2 == a) {
      cin >> l >> r;
      cout << Get(1, 0, sz, l - 1, r) << '\n';
    }
    //Print();
  }

  return 0;
}