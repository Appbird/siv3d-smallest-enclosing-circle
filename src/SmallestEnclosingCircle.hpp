#pragma once
# include <Siv3D.hpp> // Siv3D v0.6.15


/** @brief 点`p0`, `p1`, `p2`を含む最小の円を返す。 */
Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2);
/** @brief 点`p0`, `p1`, `p2`, `p3`を含む最小の円を返す。
 * @param tolerance 点が円に含まれているか判定する時に用いる許容誤差。相対誤差か絶対誤差がこの値以下であれば円に点が含まれているものとみなす。
 */
Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const double tolerance);

/** @brief 与えらえた点`points`をすべて含む最小の円`C`を返す。
 * 
 * - 点群の数に対して期待線形時間で動作する。
 * 
 * - 以下の論文で導入されたアルゴリズムを実装している。
 * Emo Welzl. "Smallest enclosing disks (balls and ellipsoids)." New Results and New Trends in Computer Science 555 (1991): 359-370.
 * 
 * @param tolerance 点が円に含まれているか判定する時に用いる許容誤差。相対誤差か絶対誤差がこの値以下であれば円に点が含まれているものとみなす。
 * @param urbg 乱数生成器。このアルゴリズムには点群の順序をシャッフルする処理が含まれるが、その動作に乱数生成器を用いる。
 */
SIV3D_CONCEPT_URBG
Circle SmallestEnclosingCircle(Array<Vec2> points, const double tolerance, URBG&& urbg = GetDefaultRNG());

/**
 * @brief 円`C`内に点`p`が含まれているかを判定する。
 * @note 判定する際、`p`が`C`外にあってもその距離の相対誤差または絶対誤差が`error`以下であれば含まれていると判定する。
 */
inline bool contains(const Circle& C, const Vec2& p, const double tolerance = 1e-8) {
    const double d_sq = (C.center - p).lengthSq();
    const double r_sq = Math::Square(C.r);
    // d_sq < r_sqならば、点`p`は円の内側にあるので一旦絶対誤差を0とおいて下の条件式が通るようにしておく。
    const double abs_err = Max(0., d_sq - r_sq);
    // 相対誤差もしくは絶対誤差のいずれかが許容誤差内ならば許容
    if (r_sq == 0) { return abs_err < tolerance; }
    return abs_err/r_sq < tolerance or abs_err < tolerance;
}

Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2)
{
    // 三角形p0-p1-p2に対して鈍角の存在を判定し、もしあればその対辺（最長辺）を弦とする円が最小の円となる。
    if ((p1 - p0).dot(p2 - p0) <= 0) { return Circle{ p1, p2 }; }
    if ((p0 - p1).dot(p2 - p1) <= 0) { return Circle{ p0, p2 }; }
    if ((p0 - p2).dot(p1 - p2) <= 0) { return Circle{ p0, p1 }; }
    
    // 鋭角三角形の場合は外接円が最小となる。
    return Triangle{p0, p1, p2}.getCircumscribedCircle();
}

// 4 点を含む最小の円を返す by ラクラムシさん
Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const double tolerance)
{
    Circle C = SmallestEnclosingCircle(p0, p1, p2);
    if (not contains(C, p3, tolerance))
    {
        C = SmallestEnclosingCircle(p0, p1, p3);
        if (not contains(C, p2, tolerance))
        {
            C = SmallestEnclosingCircle(p0, p2, p3);
            if (not contains(C, p1, tolerance))
            {
                C = SmallestEnclosingCircle(p1, p2, p3);
            }
        }
    }
    return C;
}


SIV3D_CONCEPT_URBG_
Circle SmallestEnclosingCircle(Array<Vec2> points, const double tolerance, URBG&& urbg)
{
    if (points.size() == 0) { return Circle{}; }
    if (points.size() == 1) { return Circle{points[0], 0}; }
    if (points.size() == 2) { return Circle{points[0], points[1]}; }
    if (points.size() == 3) { return SmallestEnclosingCircle(points[0], points[1], points[2]); }
    if (points.size() == 4) { return SmallestEnclosingCircle(points[0], points[1], points[2], points[3], tolerance); }
    
    // pointsの順序をランダムに並び替える。
    // （ラクラムシさんによって実験的にインデックスをシャッフルするのではなく、配列をコピーして直接シャッフルした方が高速なことがわかった。（キャッシュの恩恵？））
    points.shuffle(std::forward<URBG>(urbg));

    // 適当な 1 点を含む最小包含円Cから始めて、少しずつ広げていく戦略を取る。
    // Cに含まれない点があったら、それが境界上になるように新たに取り直す。
    Circle C = Circle{ points[0], 0.0 };

    for (size_t i = 1; i < points.size(); i++) {
        const Vec2& p0 = points[i];

        if (not contains(C, p0, tolerance)) {
            // i番目の点を最小内包円の境界上の点の一つとして固定して、残りの2点を探る。
            C = Circle{ p0, 0.0 };

            for (size_t j = 0; j < i; j++) {
                const Vec2& p1 = points[j];
                if (not contains(C, p1, tolerance)) {
                    // j番目の点を最小内包円の境界上の点の一つとして固定して、残りの1点を探る。
                    C = Circle{ p0, p1 };

                    for (size_t k = 0; k < j; k++) {
                        const Vec2& p2 = points[k];
                        if (not contains(C, p2, tolerance)) {
                            // fixed by Nachia
                            C = Triangle(p0, p1, p2).getCircumscribedCircle();
                        }
                    }
                }
            }
        }
    }

    return C;
}