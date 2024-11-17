# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.15

namespace s3d {
    namespace Geometry2D {

        //////////////////////////////////////////////////
		//
		//	SmallestEnclosingCircle
		//
		//////////////////////////////////////////////////

		/// @brief 点 p0, p1, p2 の最小包含円を返します。
		/// @param p0 点 0
		/// @param p1 点 1
		/// @param p2 点 2
		/// @return 点 p0, p1, p2 の最小包含円
		[[nodiscard]]
		Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2);

		/// @brief 点 p0, p1, p2, p3 の最小包含円を返します。
		/// @param p0 点 0
		/// @param p1 点 1
		/// @param p2 点 2
		/// @param p3 点 3
		/// @param tolerance 点が円に含まれているかの判定時の許容誤差。相対誤差または絶対誤差がこの値以下であれば、点が円に含まれているとみなします。
		/// @return 点 p0, p1, p2, p3 の最小包含円
		[[nodiscard]]
		Circle SmallestEnclosingCircle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3, double tolerance = 1e-8);

		/// @brief 点群 points の最小包含円を返します。
		/// @param points 点群
		/// @param tolerance 点が円に含まれているかの判定時の許容誤差。相対誤差または絶対誤差がこの値以下であれば、点が円に含まれているとみなします。
		/// @return 点群 points の最小包含円
		[[nodiscard]]
		Circle SmallestEnclosingCircle(Array<Vec2> points, double tolerance = 1e-8);

		/// @brief 点群 points の最小包含円を返します。
		/// @tparam URBG 乱数生成器の型
		/// @param points 点群
		/// @param tolerance 点が円に含まれているかの判定時の許容誤差。相対誤差または絶対誤差がこの値以下であれば、点が円に含まれているとみなします。
		/// @param urbg 乱数生成器。このアルゴリズムには点群の順序をシャッフルする処理が含まれていて、乱数生成器を使用します。
		/// @return 点群 points の最小包含円
		SIV3D_CONCEPT_URBG
		[[nodiscard]]
		Circle SmallestEnclosingCircle(Array<Vec2> points, double tolerance, URBG&& urbg);

		/// @brief 点群 points の最小包含円を返します。
		/// @tparam URBG 乱数生成器の型
		/// @param points 点群
		/// @param urbg 乱数生成器。このアルゴリズムには点群の順序をシャッフルする処理が含まれていて、乱数生成器を使用します。
		/// @param tolerance 点が円に含まれているかの判定時の許容誤差。相対誤差または絶対誤差がこの値以下であれば、点が円に含まれているとみなします。
		SIV3D_CONCEPT_URBG
		[[nodiscard]]
		Circle SmallestEnclosingCircle(Array<Vec2> points, URBG&& urbg, double tolerance = 1e-8);
		
    }
}

# include "SmallestEnclosingCircle.ipp"

using namespace s3d::Geometry2D;