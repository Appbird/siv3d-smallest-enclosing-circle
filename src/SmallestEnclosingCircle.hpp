#pragma once
# include <Siv3D.hpp> // Siv3D v0.6.15

/**
 * @brief 円`C`内に点`p`が含まれているかを判定する。
 * @note 判定する際、`p`が`C`外にあってもその距離の相対誤差が`relative_error`以下であれば含まれていると判定する。
 */
inline bool contains(const Circle& C, const Vec2& p, const double relative_error = 1e-8) {
    const double d_sq = (C.center - p).lengthSq();
    const double r_sq = Math::Square(C.r);
    // d_sq < r_sqならば、点`p`は円の内側にあるので一旦絶対誤差を0とおいて下の条件式が通るようにしておく。
    const double abs_err = Max(0., d_sq - r_sq);
    return abs_err/r_sq  < relative_error;
}

Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2);
Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const double epsilon = 1e-12);
Circle SmallestEnclosingCircle(const Array<Vec2> points, const double error = 1e-12);

// 3 点を含む最小の円を返す
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
Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, const double epsilon)
{
    Circle C = SmallestEnclosingCircle(p0, p1, p2);
    if (not contains(C, p3, epsilon))
    {
        C = SmallestEnclosingCircle(p0, p1, p3);
        if (not contains(C, p2, epsilon))
        {
            C = SmallestEnclosingCircle(p0, p2, p3);
            if (not contains(C, p1, epsilon))
            {
                C = SmallestEnclosingCircle(p1, p2, p3);
            }
        }
    }
    return C;
}

// 与えらえた点をすべて含む最小の円を返す
// DONE コピー渡しにする。
Circle SmallestEnclosingCircle(Array<Vec2> points, const double error)
{
    // 期待線形時間で動作
    // c.f. Emo Welzl. "Smallest enclosing disks (balls and ellipsoids)." New Results and New Trends in Computer Science 555 (1991): 359-370.
    if (points.size() == 0) { return Circle{}; }
    if (points.size() == 1) { return Circle{points[0], 0}; }
    if (points.size() == 2) { return Circle{points[0], points[1]}; }
    if (points.size() == 3) { return SmallestEnclosingCircle(points[0], points[1], points[2]); }
    // DONE N = 4の時に対処する。
    if (points.size() == 4) { return SmallestEnclosingCircle(points[0], points[1], points[2], points[3]); }
    
    // pointsの順序をランダムに並び替える。
    // ラクラムシさんによって実験的にインデックスをシャッフルするのではなく、配列をコピーして直接シャッフルした方が高速なことがわかった。（キャッシュの恩恵？）
    // DONE コピーしなくていい
    points.shuffle();

    // ランダムにとった3点を基底とする最小包含円Cから始めて、少しずつ広げていく戦略を取る。
    // Cに含まれない点があったら、それを含めるように新たに取り直す。
    Circle C = SmallestEnclosingCircle(points[0], points[1], points[2]);
    for (size_t i = 3; i < points.size(); i++) {
        const Vec2& p0 = points[i];

        if (not contains(C, p0, error)) {
            // i番目の点を最小内包円の基底の一つとして固定して、残りの2点を探る。
            C = SmallestEnclosingCircle(p0, points[0], points[1]);
            
            for (size_t j = 2; j < i; j++) {
                const Vec2& p1 = points[j];
                if (not contains(C, p1, error)) {
                    // j番目の点を最小内包円の基底の一つとして固定して、残りの1点を探る。
                    C = SmallestEnclosingCircle(p0, p1, points[0]);
                    
                    for (size_t k = 1; k < j; k++) {
                        const Vec2& p2 = points[k];
                        if (not contains(C, p2, error)) {
                            C = SmallestEnclosingCircle(p0, p1, p2);
                        }
                    }
                }
            }
        }
    }

    return C;
}