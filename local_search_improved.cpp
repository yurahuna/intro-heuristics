#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
using ll = long long;

const int CONTEST_TYPES = 26;
const ll INF = 1ll << 60;

struct Input {
    int D;
    vector<vector<ll>> s;
    vector<ll> c;
};

// 現時点でのスコアを求める
ll compute_score(const Input& input, const vector<int>& out) {
    ll score = 0ll;
    vector<int> last(CONTEST_TYPES, 0);
    for (int d = 0; d < input.D; d++) {
        last[out[d]] = d + 1;
        score += input.s[d][out[d]];
        for (int i = 0; i < CONTEST_TYPES; i++) {
            score -= (ll)(d + 1 - last[i]) * input.c[i];
        }
    }
    return score;
}

// 経過時間を取得
double get_time() {
    static chrono::system_clock::time_point start = chrono::system_clock::now();
    chrono::system_clock::time_point now = chrono::system_clock::now();
    chrono::duration<double> diff = now - start;
    return diff.count();
}


// 1点変更と2点スワップによる山登り法
vector<int> solve_local_search(const Input& input) {
    const double TL = 1.9;

    // 乱数生成器
    mt19937 rng(890482);
    
    // ランダムな初期解を生成
    vector<int> out(input.D);
    for (int i = 0; i < input.D; i++) {
        out[i] = rng() % CONTEST_TYPES;
    }
    
    ll score = compute_score(input, out);
    
    while (get_time() < TL) {
        if (rng() % 2 == 0) {
            // 1点変更
            int d = rng() % input.D;
            int q = rng() % CONTEST_TYPES;
            int old = out[d];
            out[d] = q;
            
            ll new_score = compute_score(input, out);
            if (score > new_score) {
                out[d] = old;
            } else {
                score = new_score;
            }
        } else {
            // 2点スワップ
            int d1 = rng() % (input.D - 1);
            int d2 = min<int>(d1 + 1 + rng() % 16, input.D);
            swap(out[d1], out[d2]);
            
            ll new_score = compute_score(input, out);
            if (score > new_score) {
                swap(out[d1], out[d2]);
            } else {
                score = new_score;
            }
        }

    }
    
    return out;
}

int main() {
    int D;
    cin >> D;
    vector<ll> c(CONTEST_TYPES);
    for (int i = 0; i < CONTEST_TYPES; i++) {
        cin >> c[i];
    }
    vector<vector<ll>> s(D, vector<ll>(CONTEST_TYPES));
    for (int i = 0; i < D; i++) {
        for (int j = 0; j < CONTEST_TYPES; j++) {
            cin >> s[i][j];
        }
    }

    Input input = {D, s, c};
    vector<int> out = solve_local_search(input);
    cerr << "score = " << compute_score(input, out) << endl;

    for (auto x : out) cout << x + 1 << endl;

    return 0;
}

