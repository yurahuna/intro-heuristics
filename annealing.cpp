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

ll cost(int a, int b) {
    ll d = b - a;
    return d * (d - 1ll) / 2ll;
}

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

struct State {
    vector<int> out;
    ll score;
    vector<vector<ll>> ds; // ds[i] = {d | out[d] = i}

    State(const Input& input, const vector<int>& out) : out(out) {
        ds.resize(CONTEST_TYPES);
        for (int d = 0; d < input.D; d++) {
            ds[out[d]].push_back(d + 1);
        }
        score = compute_score(input, out);
    }

    // Change the contest type at day d to new_i
    void change(const Input& input, int d, int new_i) {
        int old_i = out[d];
        auto p = find(ds[old_i].begin(), ds[old_i].end(), d + 1) - ds[old_i].begin();
        int prev = p > 0 ? ds[old_i][p - 1] : 0;
        int next = p + 1 < ds[old_i].size() ? ds[old_i][p + 1] : input.D + 1;
        ds[old_i].erase(ds[old_i].begin() + p);
        score += (cost(prev, d + 1) + cost(d + 1, next) - cost(prev, next)) * input.c[old_i];

        auto insert_pos = lower_bound(ds[new_i].begin(), ds[new_i].end(), d + 1);
        p = insert_pos - ds[new_i].begin();
        prev = p > 0 ? ds[new_i][p - 1] : 0;
        next = p < ds[new_i].size() ? ds[new_i][p] : input.D + 1;
        ds[new_i].insert(insert_pos, d + 1);
        score -= (cost(prev, d + 1) + cost(d + 1, next) - cost(prev, next)) * input.c[new_i];
        
        score += input.s[d][new_i] - input.s[d][old_i];
        out[d] = new_i;
    }

    void swap(const Input& input, int d1, int d2) {
        int old_i1 = out[d1];
        int old_i2 = out[d2];
        // cerr << "change day " << d1 << "'s contest type to " << old_i2 << endl;
        change(input, d1, old_i2);
        // cerr << "change day " << d2 << "'s contest type to " << old_i1 << endl;
        change(input, d2, old_i1);
    }
    
    void print() {
        cerr << "out = ";
        for (int i = 0; i < out.size(); i++) cerr << out[i] << " ";
        cerr << endl;
        cerr << "score = " << score << endl;
        for (int i = 0; i < CONTEST_TYPES; i++) {
            cerr << "contest type " << i << ": ";
            for (int d : ds[i]) cerr << d << " ";
            cerr << endl;
        }
    }
};

// 経過時間を取得
double get_time() {
    static chrono::system_clock::time_point start = chrono::system_clock::now();
    chrono::system_clock::time_point now = chrono::system_clock::now();
    chrono::duration<double> diff = now - start;
    return diff.count();
}

// 焼きなまし法
vector<int> solve_annealing(const Input& input) {
    const double T0 = 2e3;
    const double T1 = 6e2;
    const double TL = 1.9;

    // 乱数生成器
    mt19937 rng(890482);
    uniform_real_distribution<double> dist(0.0, 1.0);

    // ランダムな初期解を生成
    vector<int> out(input.D);
    for (int i = 0; i < input.D; i++) {
        out[i] = rng() % CONTEST_TYPES;
    }
    
    State state(input, out);
    ll best_score = state.score;
    vector<int> best_out = state.out;
    
    int cnt = 0;
    double T = T0;
    
    while (true) {
        cnt++;

        // 経過時間の取得と温度の更新は 100 反復に 1 回だけ行う
        if (cnt % 100 == 0) {
            double t = get_time() / TL;
            if (t >= 1.0) break;
            T = pow(T0, 1.0 - t) * pow(T1, t);
            // cerr << "time = " << t << ", T = " << T << endl;
            // cerr << "score = " << state.score << endl;
        }
        
        ll old_score = state.score;
        
        if (dist(rng) < 0.5) {
            // 1点変更
            int d = rng() % input.D;
            int old = state.out[d];
            int q = rng() % CONTEST_TYPES;
            state.change(input, d, q);
            
            if (old_score > state.score && 
                dist(rng) > exp((state.score - old_score) / T)) {
                state.change(input, d, old);
            }
        } else {
            // 2点スワップ
            int d1 = rng() % (input.D - 1);
            int d2 = min<int>(d1 + 1 + rng() % 16, input.D - 1);
            int old_i1 = state.out[d1];
            int old_i2 = state.out[d2];
            state.swap(input, d1, d2);
            
            if (old_score > state.score &&
                dist(rng) > exp((state.score - old_score) / T)) {
                state.swap(input, d1, d2);
            }
        }
        
        if (state.score > best_score) {
            best_score = state.score;
            best_out = state.out;
        }
    }
    
    return best_out;
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
    vector<int> out = solve_annealing(input);
    cerr << "score = " << compute_score(input, out) << endl;

    for (auto x : out) cout << x + 1 << endl;

    return 0;
}

