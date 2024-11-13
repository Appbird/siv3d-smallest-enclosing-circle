# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.15

namespace s3d {
    namespace Geometry2D {

        //////////////////////////////////////////////////
		//
		//	SmallestEnclosingCircle
		//
		//////////////////////////////////////////////////

        /// @brief 平面上の3点`p0`, `p1`, `p2`を含む半径最小の円を返します。
        /// @param p0 点0
        /// @param p1 点1
        /// @param p2 点2
        /// @returns 点`p0`, `p1`, `p2`の最小包含円
        [[nodiscard]]
        Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2);

        /// @brief 平面上の4点`p0`, `p1`, `p2`, `p3`を含む半径最小の円を返す。
        /// @param p0 点0
        /// @param p1 点1
        /// @param p2 点2
        /// @param p3 点3
        /// @param tolerance 点が円に含まれているか判定する時に用いる許容誤差。相対誤差か絶対誤差がこの値以下であれば円に点が含まれているものとみなす。
        /// @returns 点`p0`, `p1`, `p2`, `p3`の最小包含円
        [[nodiscard]]
        Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, double tolerance = 1e-8);

        /// @brief 平面上の点群`points`をすべて含む半径最小の円`C`を返す。
        /// @note 時間計算量は点群の数`points.size()`に対して期待線形時間である。
        /// @param points 平面上の点群
        /// @param tolerance 点が円に含まれているか判定する時に用いる許容誤差。相対誤差か絶対誤差がこの値以下であれば円に点が含まれているものとみなす。
        /// @param urbg 乱数生成器。このアルゴリズムには点群の順序をシャッフルする処理が含まれており、乱数生成器が用いられる。
        /// @return 点群`points`の最小包含円
        SIV3D_CONCEPT_URBG
        [[nodiscard]]
        Circle SmallestEnclosingCircle(Array<Vec2> points, double tolerance, URBG&& urbg);

        /// @brief 平面上の点群`points`をすべて含む半径最小の円`C`を返す。
        /// @note 時間計算量は点群の数`points.size()`に対して期待線形時間である。
        /// @param points 平面上の点群
        /// @param urbg 乱数生成器。このアルゴリズムには点群の順序をシャッフルする処理が含まれており、乱数生成器が用いられる。
        /// @param tolerance 点が円に含まれているか判定する時に用いる許容誤差。相対誤差か絶対誤差がこの値以下であれば円に点が含まれているものとみなす。
        /// @return 点群`points`の最小包含円
        SIV3D_CONCEPT_URBG
        [[nodiscard]]
        Circle SmallestEnclosingCircle(const Array<Vec2>& points, URBG&& urbg, double tolerance = 1e-8);

        /// @brief 平面上の点群`points`をすべて含む半径最小の円`C`を返す。
        /// @note 時間計算量は点群の数`points.size()`に対して期待線形時間である。
        /// @param points 平面上の点群
        /// @param tolerance 点が円に含まれているか判定する時に用いる許容誤差。相対誤差か絶対誤差がこの値以下であれば円に点が含まれているものとみなす。
        /// @return 点群`points`の最小包含円
        [[nodiscard]]
        Circle SmallestEnclosingCircle(const Array<Vec2>& points, double tolerance = 1e-8);
    }
}

# include "SmallestEnclosingCircle.ipp"

using namespace s3d::Geometry2D;