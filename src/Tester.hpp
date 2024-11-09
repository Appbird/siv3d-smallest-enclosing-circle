# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.15 
# include "SmallestEnclosingCircle.hpp"
# include "Generator.hpp"


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



void VisualRandomTest()
{

    /** 処理合計時間（処理時間の平均の算出に用いる） */
    double sum_cost_time = 0;
    /** テストした回数 */
    int32 test_count = 0;
    /** 何秒で問題例を更新し、解き直すか */
    const double refresh_interval = 1;
    /** 新たな点群が生成されてから何秒経ったか */
    double t = refresh_interval;
    bool draw_expected_circle = false;

    /** 乱数生成器。点群の生成に使用する。 */
    auto& rng = GetDefaultRNG();
    Array<Vec2> points;
    const Vec2 center_f = Scene::CenterF();
    
    const size_t N = 10000;
    constexpr double EPSILON = 1e-8;

    // Smalleset Enclosing Circle
    TestCaseResult result;
    
	while (System::Update())
    {
        ClearPrint();

        // 問題例の更新
        t += Scene::DeltaTime();
        if (t >= refresh_interval)
        {
            t -= refresh_interval;
            points = GeneratePoints(N, rng);
            result = TestSmallestEnclosing(points, EPSILON);
            sum_cost_time += result.process_time;
            test_count++;
            
            // コンソールに結果の詳細を出力する。結果に誤りがあった場合だけ、出力情報の詳細を示しておく。
            Console << U"case {} : [{}] in {} sec"_fmt(test_count, result.succeeded ? U"OK" : U"NG", result.process_time);
            if (not result.succeeded)
            {
                Console << U"expected: (center, r) = ({}, {})"_fmt(result.expected.center, result.expected.r);
                Console << U"actual:   (center, r) = ({}, {})"_fmt(result.actual.center, result.actual.r);
            }
        }

        // 点群とその最小包含円の描画
        for (const Vec2& point : points)
        {
            const bool is_point_enclosed = contains(result.actual, point, EPSILON);
            Circle{point, 5}.draw(
                is_point_enclosed ? HSV{131, 0.78, 0.90} : HSV{131, 0, 1}
            );
        }
        result.actual.drawFrame(3.0, HSV{131, 0.78, 0.90}).draw(HSV{131, 0.78, 0.90, 0.05});
        if (not result.succeeded or draw_expected_circle)
        {
            result.expected.drawFrame(3.0, HSV{35, 0.78, 0.90});
        }

        SimpleGUI::CheckBox(draw_expected_circle, U"Draw Expected", Vec2{ 600, 10 });
        
        // 結果の詳細を右上に示す。
        Print << U"N = {:3d}"_fmt(points.size());
        Print << U"time: {:.9f}s"_fmt(result.process_time);
        Print << U"average_time: {:.9f}s"_fmt(sum_cost_time / test_count);
        Print << U"test_count: {}"_fmt(test_count);
        Print << U"Result: {}"_fmt(result.succeeded ? U"OK" : U"NG");
	}
}
