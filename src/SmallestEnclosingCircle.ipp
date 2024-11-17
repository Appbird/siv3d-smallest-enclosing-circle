# pragma once
# include <Siv3D.hpp>

namespace s3d {
    namespace detail {
        /// @brief 点 p が円 c に含まれているかを判定します。
		/// @param c 円
		/// @param p 点
		/// @param tolerance 許容誤差（相対誤差または絶対誤差のいずれかが許容誤差以下であれば許容）
		/// @return 点 p が円 c に含まれている場合 true, それ以外の場合は false
		[[nodiscard]]
		inline bool Contains(const Circle& c, const Vec2& p, const double tolerance = 1e-8)
		{
			const double dSquared = (c.center - p).lengthSq();
			const double rSquared = (c.r * c.r);
			const double err = Max(0.0, (dSquared - rSquared));

			if (rSquared == 0)
			{
				return (err <= tolerance);
			}
			
			return (((err / rSquared) <= tolerance) || (err <= tolerance));
		}
    }

    namespace Geometry2D {
        
        SIV3D_CONCEPT_URBG_
		Circle SmallestEnclosingCircle(Array<Vec2> points, const double tolerance, URBG&& urbg)
		{
			if (points.size() == 0)
			{
				return Circle{};
			}

			if (points.size() == 1)
			{
				return Circle{ points[0], 0.0 };
			}

			if (points.size() == 2)
			{
				return Circle{ points[0], points[1] };
			}

			if (points.size() == 3)
			{
				return SmallestEnclosingCircle(points[0], points[1], points[2]);
			}

			if (points.size() == 4)
			{
				return SmallestEnclosingCircle(points[0], points[1], points[2], points[3], tolerance);
			}

			points.shuffle(std::forward<URBG>(urbg));

			// 適当な 1 点を含む最小包含円から始めて、少しずつ広げていく戦略を取る。
			// 含まれない点があったら、それが境界上になるように新たに取り直す。
			Circle circle{ points[0], 0.0 };

			for (size_t i = 1; i < points.size(); ++i)
			{
				const Vec2& p0 = points[i];

				if (not detail::Contains(circle, p0, tolerance))
				{
					circle = Circle{ p0, 0.0 };

					for (size_t j = 0; j < i; ++j)
					{
						const Vec2& p1 = points[j];
						
						if (not detail::Contains(circle, p1, tolerance))
						{
							circle = Circle{ p0, p1 };

							for (size_t k = 0; k < j; ++k)
							{
								const Vec2& p2 = points[k];
								
								if (not detail::Contains(circle, p2, tolerance))
								{
									circle = Triangle(p0, p1, p2).getCircumscribedCircle();
								}
							}
						}
					}
				}
			}

			return circle;
		}

		SIV3D_CONCEPT_URBG_
		Circle SmallestEnclosingCircle(Array<Vec2> points, URBG&& urbg, double tolerance)
		{
			return SmallestEnclosingCircle(std::move(points), tolerance, std::forward<URBG>(urbg));
		}
    }
}