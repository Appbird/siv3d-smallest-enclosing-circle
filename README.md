# Smallest Enclosing Circle
このリポジトリでは、C++フレームワークOpenSiv3Dの機能として追加される、最小包含円問題を解く関数`SmallestEnclosingCircle()`を実装・テストします。

Authors (敬称略): Nachia, Luke256, ラクラムシ, polyster, あぷりばーど

## 要件

平面上の $N$ 点の集合 $S =  \{(X_0, Y_0), (X_1, Y_1), \cdots, (X_{N-1}, Y_{N-1})\}$ が与えたとします。

$S$ の最小包含円 $C$ 、すなわち、 $S \subset C$ であるような面積最小の円 $C$ を求めてください。

### 制約
- $0 \leq N \leq 2 \times 10^5$
- $X_i, Y_i$ は小数で、C++の`double`型で表現できる。  ($0 \leq i < N$)
- $X_i, Y_i \leq 10^{18}$ ( $0 \leq i < N$ )
- $(X_i, Y_i) \neq (X_j, Y_j)$とは**限らない** ($0 \leq i < j < N$)

## 実装
この要件を満たす関数`SmallestEnclosingCircle`の実装は`src/SmallestEnclosingCircle.hpp`に格納されています。

アルゴリズムは以下の論文を参照しています。時間計算量は期待線形時間です。
- Emo Welzl. "Smallest enclosing disks (balls and ellipsoids)." New Results and New Trends in Computer Science 555 (1991): 359-370.

thanks: Nachiaさん

## このOpenSiv3Dプロジェクトについて
このOpenSiv3D Projectで動作するアプリケーションには三つのモードがあります。

- `FullTest`
  - フォルダ`./App/input-auto`, `./App/input-handmade`に格納された入力において、関数`SmallestEnclosingCircle`の出力が期待値と合致するかを調べる。
  - 全てのテストケースで期待値と合致すれば、`Accepted`とみなします。
  - テストし終わると...
    1.  テストケースに対してどのような出力が行われたかを示すビジュアライザが起動します。
      - リストボックスから問題例を選択することができます。
    2. `./App/fulltest.log`にテスト結果に関するログが残されます。
- `RandomTest`
  - その場でランダムに問題例を一秒ごとに生成します。
  - 関数`SmallestEnclosingCircle`の解答が期待値と合致しているかどうかをビジュアライザとともに確認できます。
- `GenerateInput`
  - このモードで起動すると、フォルダ`./App/input-auto`にテストケースの入力例を生成します。
  - `FullTest`では手で作られた入力と自動生成された入力の二つを用いてチェックしますが、このモードではその後者の入力を生成します。
  - **生成される入力データは固定です。**
  - **このリモートリポジトリには、入力が巨大なデータ（ $N \geq 10^4$ ）は格納されていません。**
    - そのため、ローカルで`FullTest`を走らせる場合には、まず一度`GenerateInput`モードを実行する必要があります。

このモードは、`Main.cpp`で定義されている変数`constexpr ApplicationMode mode`の値を変更することによって切り替えられます。


### 入力
テスト入力は以下の形式で`App/input-auto/*.txt`, `App/input-handmade/*.txt`に格納されています。1ファイルが1問題例に相当します。

> $N$
> 
> $X_1\quad Y_1$
> 
> $X_2\quad Y_2$
> 
> $\vdots$
> 
> $X_n\quad Y_n$

### テスト期待値の生成方法
ナイーブなアルゴリズム`SmallestEnclosingCircleNaive`によって求解したものを期待値とします。具体的には、3点の組み合わせを全て試しその3点から構成される円が $S$ の最小包含円になっているかを判定することによって求めます。ただし、 $S$ の凸包の頂点以外からなる3点は候補になりえないことを考慮することで高速化します。

（implemented by ラクラムシさん）

### 出力
関数`SmallestEnclosingCircle`が、 $C$ の中心 $(X, Y)$ 、半径 $R$ を出力したとき、
期待値との絶対誤差が $10^{-8}$ 以下であれば、正答とみなします。

### テストケースの構成
テストケースは全部で73個あり、8つのパートから構成されています。

#### `input-handmade`に格納されているもの

1) `few`: $N$ が小さいケース; 14ケース
   -  `few-0`: $N = 0$ ; 1ケース
   -  `few-1`: $N = 1$ ; 1ケース
   -  `few-2`: $N = 2$ ; 1ケース
   -  `few-3`: $N = 3$ ; 4ケース
      -  `few-3-equilateral`: 正三角形をなすケース
      -  `few-3-obtuse`: 鈍角三角形をなすケース
      -  `few-3-acute`: 鋭角三角形をなすケース 
      -  `few-3-rand`: ランダムなケース
   -  `few-4`: $N = 4$ ; 7ケース
      - `few-4-a2`: 鋭角三角形二つからなる四角形
      - `few-4-o2`: 鈍角三角形二つからなる四角形
      - `few-4-not-convex`: 凸でない四角形
      - `few-4-oa`: 鋭角三角形・鈍角三角形1つずつからなる四角形
      - `few-4-rect`: 長方形をなすケース
      - `few-4-rect`: 正方形をなすケース
      - `few-4-rand`: ランダムなケース
2) `line`: 全ての点が一直線上にあるケース （ $N = 8$ ）; 4ケース
   - `line-on`: 一直線上に全てあるケース
   - `line-expect1`: 1点を除いて全ての点が一直線上に並んでいるケース
   - `line-expect1`: 2点を除いて全ての点が一直線上に並んでいるケース
   - `line-each`: 各々の4点が異なる同一直線上に存在するケース
3) `circle`: 4点以上が同一円上に存在するケース（ $N = 12$ ); 4ケース
    - `circle-all`: 全ての点が同一円に存在する
    - `circle-except1`: 1点を除いて全ての点が同一円に存在する
    - `circle-except2`: 2点を除いて全ての点が同一円に存在する
    - `circle-each`: 各々の4点が異なる円上に存在するケース
4) `same`: すべてが同一点にあるようなケース; 2ケース
   - `same-2`: $N = 2$ 
   - `same-12`: $N = 12$
5) `partial-same`: 一部が同一点であるようなケース（ $N = 8$ ）; 3ケース
   - `partial-same-2`: $2$ 点が同一点に存在し、 $6$ 点がランダムに存在するケース
   - `partial-same-2-2`: $2$ 点が同一点に存在し、それとは異なる $2$ 点が一点に存在し、 $6$ 点がランダムに存在するケース
   - `partial-same-4`: $4$ 点が同一点に存在し、 $4$ 点がランダムに存在するケース

#### `input-auto`に格納されているもの

6) `huge`: 座標値が巨大なケース（ $N = 100$ ）; 10ケース
   - `huge-small`: 原点からの距離が $R \simeq 1 \times 10^{12}$ であるような領域中の点を中心とする、辺長 $5 \times 10^4$ の正方形上に分布する点群のケース; 5ケース
   - `huge-large`: 原点中心・半径 $R \simeq 1 \times 10^{17}$ の円上に分布する点のケース; 5ケース
7) `shuffled`: シャッフルしないと計算量が爆発するケース( $N = 300$ ); 1ケース
8) `many`: 数が多いケース; 35ケース
   - `many-1e4-*`: $N = 1 \times 10^4$ ; 10ケース
   - `many-2e5-*`: $N = 2 \times 10^5$ ; 25ケース
