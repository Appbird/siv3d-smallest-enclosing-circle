#include <chrono>
#include "SmallestEnclosingCircle.hpp"
#include "Tester.hpp"

/** 実験に使用するパラメータ群 */
constexpr double EPSILON = 1e-8;
constexpr bool VISUALIZE = true;
constexpr int32_t N = 100;


/**
 * @brief `N`点のランダムな点を、`points_generator`を用いて生成し、その生成された点群を返す。
 * 
 * @param rng 乱数生成器
 * @param points_generator 乱数生成器を用いてランダムな点を一つ返す関数
 * @return Array<Vec2> 生成された点群
 */
Array<Vec2> GeneratePoints(
    const int32_t N,
    DefaultRNG& rng,
    std::function<Vec2(DefaultRNG&)> points_generator
);

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
    auto points_generator = [&](DefaultRNG& rng) -> Vec2 {
        NormalDistribution<double> dist_x{center_f.x, center_f.x / 4};
        NormalDistribution<double> dist_y{center_f.x, center_f.x / 4};
        return Vec2{dist_x(rng), dist_y(rng)};
    };
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
            points = GeneratePoints(N, rng, points_generator);
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

void Main() {
    VISUALIZE ? VisualRandomTest() : FastTest();
}




Array<Vec2> GeneratePoints(
    const int32_t N,
    DefaultRNG& rng,
    std::function<Vec2(DefaultRNG&)> points_generator
) {
    Array<Vec2> points;
    for (int32_t i = 0; i < N; i++) {
        points.push_back(points_generator(rng));
    }
    return points;
}