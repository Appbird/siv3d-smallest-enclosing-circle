# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.15 
# include "SmallestEnclosingCircle.hpp"


struct TestCaseResult {
    /** 与えられた要件（最小性と内包性）を満たした解を出力できているか */
    bool succeeded;
    /** 処理時間 */
    double process_time;
    /** テスト対象の方法で算出された最小包含円 */
    Circle actual;
    /** ナイーブな方法で算出された、正しいと考えられる最小包含円。TestEnclosingで計算されたものはその限りでない。 */
    Circle expected;
};

// テスト用コード by ラクラムシさん
// 愚直な方法で最小包含円問題を解く
Circle SmallestEnclosingCircleNaive(const Array<Vec2>& points, const double epsilon = 1e-12);

/**
 * @brief ある点群に対して、`SmallestEnclosingCircle`の計算した円が本当に最小かつ内包かを確認する。
 */
TestCaseResult TestSmallestEnclosing(const Array<Vec2>& points, const double epsilon = 1e-12);


// テスト用コード by ラクラムシさん
// 愚直な方法（四重ループ）で最小包含円問題を解く
Circle SmallestEnclosingCircleNaive(const Array<Vec2>& points, const double epsilon)
{
    const auto convexHull = Geometry2D::ConvexHull(points).outer();
    const auto n = convexHull.size();
    Circle smallest{ 0, 0, Math::Inf };
    for (size_t i = 2; i < n; ++i)
    for (size_t j = 1; j < i; ++j)
    for (size_t k = 0; k < j; ++k)
    {
        const auto c = SmallestEnclosingCircle(convexHull[i], convexHull[j], convexHull[k]);
        if ((c.r < smallest.r) && convexHull.all([&](const Vec2& p) { return p.distanceFromSq(c.center) <= Math::Square(c.r + epsilon); }))
        {
            smallest = c;
        }
    }
    return smallest;
}

// 最小性と包含性をチェックする。（低速）
TestCaseResult TestSmallestEnclosing(const Array<Vec2>& points, const double epsilon) {
    const Circle expected = SmallestEnclosingCircleNaive(points, epsilon);
    Circle actual;

    const auto start = std::chrono::high_resolution_clock::now();
    {
        actual = SmallestEnclosingCircle(points, epsilon);    
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    const double time = double(duration.count()) / 1e9;

    const bool succeeded =
        actual.center.distanceFromSq(expected.center) < epsilon
        && Math::Abs(actual.r - expected.r) < epsilon;
    return TestCaseResult{
        succeeded,
        time,
        actual,
        expected
    };
};
