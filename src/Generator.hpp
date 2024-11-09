#pragma once
#include <Siv3D.hpp>

using Points = Array<Vec2>;

NormalDistribution<double> StandardDistribution() {
    const Vec2 center_f = Scene::CenterF();
    return NormalDistribution<double>{center_f.x, center_f.x / 4};
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
Array<Vec2> GetFromStdin() {
    
}