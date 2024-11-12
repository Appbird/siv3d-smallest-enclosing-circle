#pragma once
#include <Siv3D.hpp>
#include <fstream>

using Points = Array<Vec2>;

NormalDistribution<double> StandardDistribution() {
    const Vec2 center_f = Scene::CenterF();
    return NormalDistribution<double>{center_f.x, center_f.x / 4};
}

void WriteInputToFile(const FilePath& filepath, const Array<Vec2>& points) {
    std::ofstream fout{filepath.toUTF8()};
    // #FIXED: Assertion failed: (fout), function WriteInputToFile, file Generator.hpp, line 24.
    assert(fout);
    fout << points.size() << "\n";
    fout << std::setprecision(16);
    for (int32_t i = 0; i < points.size(); i++) {
        fout << points[i].x << " " << points[i].y << "\n";
    }
    fout << std::flush;
    Console << U"wrote in {}"_fmt(filepath);
}

/**
 * @brief `N`点のランダムな点を生成して返す。
 * 
 * @param rng 乱数生成器
 * @return Array<Vec2> 生成された点群
 */
Points GeneratePoints(
    const int32_t N,
    DefaultRNG& rng
) {
    Points points;
    auto dist = StandardDistribution();
    for (int32_t i = 0; i < N; i++) {
        points.emplace_back(dist(rng), dist(rng));
    }
    return points;
}

Array<Vec2> GenerateHugeSmall(size_t N, DefaultRNG& rng, double average, double variance) {
    Array<Vec2> points(N);
    NormalDistribution<double> dist{average, variance};
    for (size_t i = 0; i < N; i++) {
        points[i] = {dist(rng), dist(rng)};
    }
    return points;
}
Array<Vec2> GenerateHugeLarge(size_t N, DefaultRNG& rng) {
    Array<Vec2> points(N);
    NormalDistribution<double> dist{0, 1e17};
    for (size_t i = 0; i < N; i++) {
        points[i] = {dist(rng), dist(rng)};
    }
    return points;
}

Array<Vec2> GenerateShuffled(size_t N) {
    Array<Vec2> points(N);
    for (int32 i = 0; i < N; ++i)
    {
        points[i] = OffsetCircular{ Scene::CenterF(), (200.0 * i / N), (120_deg * i) };
    }
    return points;
}

void GenerateLargeInputs() {
    DefaultRNG rng = GetDefaultRNG();
    const uint64 seed = 0;
    rng.seed(seed);
    auto tofullpath = [&](const String& filestem){ return U"input-auto/{}.txt"_fmt(filestem); };
    for (int32_t i = 0; i < 30; i++) {
        WriteInputToFile(tofullpath(U"huge-large-{}"_fmt(i)), GenerateHugeLarge(300, rng));
        WriteInputToFile(tofullpath(U"huge-small-{}"_fmt(i)), GenerateHugeSmall(1000, rng, 1e8, 50000));
        WriteInputToFile(tofullpath(U"huge-super-small-{}"_fmt(i)), GenerateHugeSmall(1000, rng, 1e6, 200));
    }
    WriteInputToFile(tofullpath(U"shuffled"),   GenerateShuffled(300));
    for (int32_t i = 0; i < 100; i++) {
        WriteInputToFile(tofullpath(U"many-1e4-{}"_fmt(i)), GeneratePoints(1e4, rng));
    }
    for (int32_t i = 0; i < 100; i++) {
        WriteInputToFile(tofullpath(U"many-1e5-{}"_fmt(i)), GeneratePoints(1e5, rng));
    }
}