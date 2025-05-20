# Introduction to Heuristics Contest in C++

このリポジトリは、AtCoder の [Introduction to Heuristics Contest](https://atcoder.jp/contests/intro-heuristics) の C++ による解法をまとめたものです。

## 問題概要

- 26種類のコンテストをD日間で開催するスケジュールを最適化する問題
- 各コンテストタイプには満足度とペナルティが設定されている
- 目標は、満足度の合計からペナルティの合計を引いた値を最大化すること

## 解法

| ファイル名 | 解法 | 概要 | 点数 |
|------------|------|------|------|
| `greedy.cpp` | 貪欲法 | 1日ごとに、その日のスコアが最も良くなるコンテストを選ぶ | 62,634,806 |
| `greedy_improved.cpp` | 貪欲法の改善版 | 1日ごとに、k日先のスコアが最も良くなるコンテストを選ぶ | 104,195,466 |
| `local_search.cpp` | 局所探索法（山登り法） | ランダムな初期解からはじめ、ランダムに選んだ1日のコンテストをランダムに変える | 78,512,496 |
| `local_search_improved.cpp` | 局所探索法（山登り法） | `local_search.cpp`に加え、ランダムに選んだ2日のコンテストを交換する操作も行う | 112,474,082 |
| `local_search_fast.cpp` | 局所探索法の高速化版 | スコア計算を差分更新で高速化 | 112,429,611 |

これらの解法は[公式の解説](https://img.atcoder.jp/intro-heuristics/editorial.pdf)をもとにしています。

## ビルド方法

```bash
g++ -std=c++17 greedy.cpp -o greedy
```

## 実行方法

```bash
./greedy < input.txt
```

## TODO

- 局所探索の高速化
- 焼きなまし法
- ビームサーチ
- ローカルで複数のテストケースを回せるようにする