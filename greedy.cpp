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

ll compute_score(const Input& input, const vector<int>& out) {
    ll score = 0ll;
    vector<int> last(CONTEST_TYPES, 0);
    for (int d = 0; d < out.size(); d++) {
        last[out[d]] = d + 1;
        for (int i = 0; i < CONTEST_TYPES; i++) {
            score -= (ll)(d + 1 - last[i]) * input.c[i];
        }
        score += input.s[d][out[d]];
    }
    return score;
}

vector<int> solve(const Input& input) {
    vector<int> out;
    for (int d = 0; d < input.D; d++) {
        ll max_score = -INF;
        int best_i = 0;
        for (int i = 0; i < CONTEST_TYPES; i++) {
            out.push_back(i);
            ll current_score = compute_score(input, out);
            if (max_score < current_score) {
                max_score = current_score;
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
    vector<int> out = solve(input);
    cerr << "score = " << compute_score(input, out) << endl;

    for (auto x : out) cout << x + 1 << endl;

    return 0;
}

