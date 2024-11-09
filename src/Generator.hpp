#pragma once
#include <Siv3D.hpp>
#include <fstream>

using Points = Array<Vec2>;

NormalDistribution<double> StandardDistribution() {
    const Vec2 center_f = Scene::CenterF();
    return NormalDistribution<double>{center_f.x, center_f.x / 4};
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
void WriteInputToFile(const FilePath& filepath, const Array<Vec2>& points) {
    std::ofstream fout{filepath.toUTF8()};
    assert(fout);
    fout << points.size() << std::endl;
    for (int32_t i = 0; i < points.size(); i++) {
        fout << points[i].x << " " << points[i].y << std::endl;
    }
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

Array<Vec2> GenerateHugeSmall(size_t N, DefaultRNG& rng) {
    Array<Vec2> points(N);
    NormalDistribution<double> dist{1e17, 500};
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

Array<Vec2> GenerateShuffled(size_t N, DefaultRNG& rng) {
    Array<Vec2> points(N);
    NormalDistribution<double> dist{0, 1e17};
    for (int32 i = 0; i < N; ++i)
    {
        points[i] = OffsetCircular{ Scene::CenterF(), (200.0 * i / N), (120_deg * i) };
    }
    return points;
}

void GenerateLargeInputs() {
    DefaultRNG rng = GetDefaultRNG();
    //= GeneratePoints(300, rng);
}