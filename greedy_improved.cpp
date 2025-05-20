#include <iostream>
#include <vector>
#include <algorithm>

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

// k日先のスコアを求める
ll evaluate(const Input& input, const vector<int>& out, int k) {
    ll score = 0ll;
    vector<int> last(CONTEST_TYPES, 0);
    for (int d = 0; d < out.size(); d++) {
        last[out[d]] = d + 1;
        for (int i = 0; i < CONTEST_TYPES; i++) {
            score -= (ll)(d + 1 - last[i]) * input.c[i];
        }
        score += input.s[d][out[d]];
    }
    for (int d = out.size(); d < min((int)out.size() + k, input.D); d++) {
        for (int i = 0; i < CONTEST_TYPES; i++) {
            score -= (ll)(d + 1 - last[i]) * input.c[i];
        }
    }   

    return score;
}

// k日先のスコアによる貪欲法
vector<int> solve(const Input& input, int k) {
    vector<int> out;
    for (int d = 0; d < input.D; d++) {
        ll max_score = -INF;
        ll best_i = 0;
        for (int i = 0; i < CONTEST_TYPES; i++) {
            out.push_back(i);
            ll score = evaluate(input, out, k);
            if (max_score < score) {
                max_score = score;
                best_i = i;
            }
            out.pop_back();
        }
        out.push_back(best_i);
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

    ll max_score = -INF;
    int best_k = 0;
    vector<int> best_out;
    for (int k = 0; k <= CONTEST_TYPES; k++) {
        vector<int> out = solve(input, k);
        ll score = compute_score(input, out);
        cerr << "k = " << k << ", score = " << score << endl;
        if (max_score < score) {
            max_score = score;
            best_k = k;
            best_out = out;
        }
    }

    cerr << "best: k = " << best_k << ", score = " << max_score << endl;
    for (auto x : best_out) cout << x + 1 << endl;

    return 0;
}

