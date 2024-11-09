# Smallest Enclosing Circle
このリポジトリでは、C++フレームワークOpenSiv3Dの機能として追加される、最小包含円問題を解く関数`SmallestEnclosingCircle()`を実装・テストします。

Authors (敬称略): Nachia, Luke256, ラクラムシ, polyster, あぷりばーど


## 要件

平面上の$N$点の集合$S =  \{(X_0, Y_0), (X_1, Y_1), \cdots, (X_{N-1}, Y_{N-1})\}$が与えたとします。

$S$の最小包含円$C$、すなわち、$S \sub C$であるような面積最小の円$C$を求めてください。

### 制約
- $1 \leq N \leq 2 \times 10^5$
- $X_i, Y_i$ は小数で、C++の`double`型で表現できる。  ($0 \leq i < N$)
- $X_i, Y_i \leq 10^{18}$ ($0 \leq i < N$)
- $(X_i, Y_i) \neq (X_j, Y_j)$とは**限らない** ($0 \leq i < j < N$)

## 実装
この要件を満たす関数`SmallestEnclosingCircle`の実装は`src/SmallestEnclosingCircle.hpp`に格納されています。

アルゴリズムは以下の論文を参照しています。時間計算量は期待線形時間です。
- Emo Welzl. "Smallest enclosing disks (balls and ellipsoids)." New Results and New Trends in Computer Science 555 (1991): 359-370.

thanks: Nachiaさん

## テスト
このOpenSiv3Dプロジェクトを走らせると、次のような入力を受け取って、結果を出力するプログラムが動作します。
OpenSiv3D上のエントリポイントである`Main()`は、以下の入力を読み取った後関数`SmallestEnclosingCircle`を実行し、返ってきた結果と期待値が合致しているかの確認を行います。
全てのテストケースで期待値と合致すれば、Acceptedとみなします。


### 入力
テスト入力は以下の形式で`App/input/*.txt`に格納されています。1ファイルが1問題例に相当します。

> $N \\$
> $X_1 \; Y_1 \\ X_2\; Y_2  \\ \vdots \\ X_n\; Y_n$

### テスト期待値の生成方法
ナイーブなアルゴリズムによって求解したものを期待値とします。具体的には、3点の組み合わせを全て試しその3点から構成される円が$S$の最小包含円になっているかを判定することによって求めます。ただし、$S$の凸包の頂点以外からなる3点は最小内包円にはなりえないため、事前に候補から外すことによって高速化します。

implemented by ラクラムシさん

### 出力
$C$の中心$(X, Y)$、半径$R$を出力したとき、
期待値との絶対誤差が$10^{-6}$以下であれば、正答とみなします。

### テストケースの構成
テストケースは全部で$N$個あり、四つのパートから構成されています。

1) $N$が小さいケース`few`
   -  $N = 0$: `few-0`
   -  $N = 1$: `few-1-rand*`
   -  $N = 2$: `few-2-rand*`
   -  $N = 3$:
      -  鈍角三角形をなすケース`few-3-obtuse`
      -  鋭角三角形をなすケース `few-3-acute`
      -  `few-3-rand*` x 2
   -  $N = 4$
      - 鋭角三角形二つからなる四角形`few-4-a2`
      - 鈍角三角形二つからなる四角形`few-4-o2`
      - 鋭角三角形・鈍角三角形1つずつからなる四角形`few-4-oa`
      - 凸でない四角形`few-4-not-convex`
      - `few-3-rand*` x 2
2) 全ての点が一直線上にあるケース `line`（$N = 8$）
   - 一直線上に全てあるケース `line-on`
   - 2点を除いて全ての点が一直線上に並んでいるケース`line-expect2`
   - 各々の4点が異なる同一直線上に存在するケース`line-each`
3) 4点以上が同一円上に存在するケース`circle`（$N = 12$）
    - 全ての点が同一円に存在する `circle-all`
     - 1点を除いて全ての点が同一円に存在する `circle-except1`
     - 2点を除いて全ての点が同一円に存在する `circle-except2`
     - 各々の4点が異なる円上に存在するケース`circle-each`
4) すべてが同一点にあるようなケース `same`
   - $N = 2$ `same-2`
   - $N = 4$ `same-4`
5) 一部が同一点であるようなケース（$N = 8$）`partial-same`
   - $2$点が同一点に存在し、$6$点がランダムに存在するケース`partial-same-2`
   - $2$点が同一点に存在し、それとは異なる$2$点が一点に存在し、$6$点がランダムに存在するケース`partial-same-2-2`
   - $4$点が同一点に存在し、$4$点がランダムに存在するケース`partial-same-4`
6) 座標値が巨大なケース（$N = 100$）`huge`
   - 原点からの距離が$R \simeq 1 \times 10^{17}$であるような領域中の点を中心とする、辺長$1000$の正方形上に分布する点群のケース`huge-small`
   - 原点中心・半径$R \simeq 1 \times 10^{17}$の円上に分布する点のケース`huge-large`
7) シャッフルしないと計算量が爆発するケース($N = 300$)
   - `shuffled`
8) 数が多いケース`many`
   - $N = 1 \times 10^4$ `many-1e4-*`
   - $N = 2 \times 10^5$ `many-2e5-*`

### 入力の生成方法
入力の生成は、手による作成・`Generator.hpp`内部で定義されている関数らによって行われています。
具体的には、ケース`few`, `line`, `circle`, `same`, `partial-same`は手動で作成し、`many`, `huge`, `shuffled`はプログラムによって作成しています。
手動で作成する際、その点群の可視化には[Desmos Graphing Calculator](https://www.desmos.com/calculator?lang=ja)を用いました。