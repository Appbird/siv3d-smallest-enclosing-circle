# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.15 
# include <fstream>
# include "SmallestEnclosingCircle.hpp"
# include "Generator.hpp"


struct TestCaseResult {
    /** 与えられた要件（最小性と内包性）を満たした解を出力できているか */
    bool succeeded;
    /** 処理時間 */
    double process_time;
    /** テスト対象の方法で算出された最小包含円 */
    Circle actual;
    /** ナイーブな方法で算出された、正しいと考えられる最小包含円。 */
    Circle expected;
};

// テスト用コード by ラクラムシさん
// 愚直な方法で最小包含円問題を解く
Circle SmallestEnclosingCircleNaive(const Array<Vec2>& points, const double epsilon = 1e-8);

/**
 * @brief ある点群に対して、`SmallestEnclosingCircle`の計算した円が本当に最小かつ内包かを確認する。
 */
TestCaseResult TestSmallestEnclosing(const Array<Vec2>& points, const double epsilon = 1e-8);

// テスト用コード by ラクラムシさん
// 愚直な方法（四重ループ）で最小包含円問題を解く
Circle SmallestEnclosingCircleNaive(const Array<Vec2>& points, const double epsilon)
{
    // #FIXED: convexHullが空だった場合は元のpointsに戻すように修正
    const auto convexHull = Geometry2D::ConvexHull(points).outer();
    const auto eliminated_points = (convexHull.empty()) ? points : convexHull;
    const auto n = eliminated_points.size();

    // #FIXED  by Nachia
    // 点のサイズが2以下だった時に挙動がおかしくなる問題を修正
    if (eliminated_points.size() == 0) { return Circle{}; }
    if (eliminated_points.size() == 1) { return Circle{eliminated_points[0], 0}; }
    if (eliminated_points.size() == 2) { return Circle{eliminated_points[0], eliminated_points[1]}; }
    
    Circle smallest{ 0, 0, Math::Inf };
    for (size_t i = 2; i < n; ++i)
    for (size_t j = 1; j < i; ++j)
    for (size_t k = 0; k < j; ++k)
    {
        const auto c = SmallestEnclosingCircle(eliminated_points[i], eliminated_points[j], eliminated_points[k]);
        if ((c.r < smallest.r) && eliminated_points.all([&](const Vec2& p) { return contains(c, p, epsilon); }))
        {
            smallest = c;
        }
    }
    return smallest;
}

bool EqualRelativeErr(const double expected, const double actual, const double err = 1e-8) {
    const double abs_err = AbsDiff(expected, actual);
    if (expected == 0) { return abs_err < err; }
    return abs_err/expected < err or abs_err < err;
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
        EqualRelativeErr(expected.center.x, actual.center.x, epsilon)
        and EqualRelativeErr(expected.center.y, actual.center.y, epsilon)
        and EqualRelativeErr(expected.r, actual.r, epsilon);
    return TestCaseResult{
        succeeded,
        time,
        actual,
        expected
    };
};

void DrawTestResult(
    const Array<Vec2>& points,
    const TestCaseResult result,
    const String& testcase_name,
    const double EPSILON,
    const double point_scaling = 1
) { 
    // 点群とその最小包含円の描画
    for (const Vec2& point : points)
    {
        const bool is_point_enclosed = contains(result.actual, point, EPSILON);
        Circle{point, 2.5 * point_scaling}.draw(
            is_point_enclosed ? HSV{131, 0.78, 0.90} : HSV{131, 0, 1}
        );
    }
    result.actual.drawFrame(1.0 * point_scaling, HSV{131, 0.78, 0.90}).draw(HSV{131, 0.78, 0.90, 0.05});
    if (not result.succeeded)
    {
        result.expected.drawFrame(1.0 * point_scaling, HSV{35, 0.78, 0.90});
    }
    // 結果の詳細を右上に示す。
    Print << testcase_name;
    Print << U"N = {:3d}"_fmt(points.size());
    Print << U"time: {:.9f}s"_fmt(result.process_time);
    Print << U"Result: {}"_fmt(result.succeeded ? U"OK" : U"NG");
}

Array<Vec2> GetInputFromFile(const FilePath& filepath) {
    std::ifstream fin{filepath.toUTF8()};
    assert(fin);
    int32_t N; fin >> N;
    Array<Vec2> result(N);
    for (int32_t i = 0; i < N; i++) {
        fin >> result[i].x >> result[i].y;
    }
    return result;
}
Array<FilePath> ReadAllTestCases() {
    const Array<FilePath> cases_handmade    = FileSystem::DirectoryContents(U"input-handmade");
    const Array<FilePath> cases_auto        = FileSystem::DirectoryContents(U"input-auto");
    Array<FilePath> all_cases;
    std::ranges::copy(cases_handmade, std::back_inserter(all_cases));
    std::ranges::copy(cases_auto, std::back_inserter(all_cases));
    std::ranges::sort(all_cases);
    return all_cases;
}

/** @brief all_casesに渡された問題例が合致しているかを判定し、ログファイル`fulltest.log`・`test_results`にその結果を格納する。 */
void TestAllCases(
    const Array<FilePath>& all_cases,
    Array<TestCaseResult>& test_results,
    const double epsilon
) {
    size_t success = 0;
    TextWriter logger{U"fulltest.log"};
    for (const FilePath& path : all_cases) {
        const Array<Vec2> input = GetInputFromFile(path);
        const String casename = FileSystem::FileName(path);
        
        TestCaseResult result = TestSmallestEnclosing(input, epsilon);
        test_results.push_back(result);
        const String judge_state = result.succeeded ? U"[AC]" : U"[WA]";
        const String judge_state_cmd = result.succeeded ? U"\e[42m\e[37m[AC]\e[0m" : U"\e[43m\e[37m[WA]\e[0m";
        if (result.succeeded) { success++; }
        
        Console << U"\n[{}] {}"_fmt(casename, judge_state_cmd);
        logger << U"\n[{}] {}"_fmt(casename, judge_state);
        logger << U"\ttime: {:.9f}s"_fmt(result.process_time);
        logger << U"\texpected: (center, r) = ({}, {})"_fmt(result.expected.center, result.expected.r);
        logger << U"\tactual:   (center, r) = ({}, {})"_fmt(result.actual.center, result.actual.r);
    }
    logger << U"[AC] x {} / {}"_fmt(success, all_cases.size());
    logger << U"result: {}"_fmt((success == all_cases.size()) ? U"[AC]" : U"[WA]");
}

Mat3x2 RenderedAreaMat(const Array<Vec2>& current_instance, const RectF& view_area, double& point_scale) {
    // 描画すべき範囲を求めておき、後でTransformer2Dに適用する。
    Vec2 min_point = {1e18, 1e18}, max_point = {-1e18, -1e18};
    for (const Vec2& point : current_instance) {
        min_point.x = Min(min_point.x, point.x);
        min_point.y = Min(min_point.y, point.y);
        max_point.x = Max(max_point.x, point.x);
        max_point.y = Max(max_point.y, point.y);
    }
    RectF region_points = RectF::FromPoints(min_point, max_point).stretched(5);
    SizeF size_region_display = SizeF{1., 1.} * region_points.size.maxComponent();
    RectF region_display = RectF{Arg::center = region_points.center(), size_region_display};
    point_scale = view_area.w / region_display.w;
    return
        Mat3x2::Translate(-region_display.center()) *
        Mat3x2::Scale(point_scale) *
        Mat3x2::Translate(view_area.center());
}

void FullTest(const double EPSILON = 1e-8) {
    // テスト処理
    Logger << U"[FullTest at {}]"_fmt(DateTime::Now());
    Array<FilePath> all_cases = ReadAllTestCases();
    Array<TestCaseResult> test_results;
    TestAllCases(all_cases, test_results, EPSILON);
    
    // テスト結果のビジュアライザ
    // ジャッジ状態を含めたテストケース名の列挙
    Array<FilePath> all_cases_title(all_cases.size());
    for (size_t i = 0; i < all_cases.size(); i++) {
        all_cases_title[i] = U"[{}] {}"_fmt(test_results[i].succeeded ? U"AC" : U"WA", FileSystem::FileName(all_cases[i]));
    }
    // 問題例をあらわす点群を格納する配列
    Array<Vec2> current_instance;
    // 現在選択している問題例に対するテスト結果
    TestCaseResult testcase_result;
    // 問題例を選択するためのUI
    ListBoxState case_list { all_cases_title };
    // 現在選んでいる問題例の番号
    Optional<size_t> current_selection = 0;
    const RectF view_area = {0, 0, 600, 600};
    Mat3x2 viewport_affine;
    double point_scale = 1;
    
    while (System::Update()) {
        ClearPrint();
        if (case_list.selectedItemIndex and current_selection != case_list.selectedItemIndex) {
            // 問題例の計算
            current_selection = case_list.selectedItemIndex;
            current_instance = GetInputFromFile(all_cases[*current_selection]);
            testcase_result = test_results[*current_selection];
            viewport_affine = RenderedAreaMat(current_instance, view_area, point_scale);
        }
        if (case_list.selectedItemIndex) {
            Transformer2D transformer{viewport_affine};
            DrawTestResult(current_instance, testcase_result, case_list.items[*current_selection], EPSILON, 1/point_scale);
        }
        SimpleGUI::ListBox(case_list, Vec2{ 500, 30 }, 300, 156);
    }
}


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

    /** 乱数生成器。点群の生成に使用する。 */
    auto& rng = GetDefaultRNG();
    Array<Vec2> points;
    
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

        DrawTestResult(points, result, U"random_tests", EPSILON);
        Print << U"average_time: {:.9f}s"_fmt(sum_cost_time / test_count);
        Print << U"test_count: {}"_fmt(test_count);
        
	}
}