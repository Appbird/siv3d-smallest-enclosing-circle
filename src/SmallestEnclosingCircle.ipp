# pragma once
# include <Siv3D.hpp>

namespace s3d {
    namespace detail {
        /// @brief 円`c`内に点`p`が含まれているかを判定する。
        /// @note `p`が`c`外にあっても、`c`の半径に対する 点`p`と`c.r`の間の距離 の **相対誤差または絶対誤差** が`error`以下であれば含まれていると判定する。
        [[nodiscard]]
        inline bool Contains(const Circle& c, const Vec2& p, const double tolerance = 1e-8) {
            const double dSquared = (c.center - p).lengthSq();
            const double rSquared = Math::Square(c.r);
            // d_sq < r_sqならば、点`p`は円の内側にあるので一旦絶対誤差を0とおいて下の条件式が通るようにしておく。
            const double absErr = Max(0.0, dSquared - rSquared);
            // 相対誤差もしくは絶対誤差のいずれかが許容誤差内ならば許容
            if (rSquared == 0) { return absErr < tolerance; }
            return absErr/rSquared < tolerance or absErr < tolerance;
        }
    }

    namespace Geometry2D {
        
        //////////////////////////////////////////////////
		//
		//	SmallestEnclosingCircle
		//
		//////////////////////////////////////////////////

        SIV3D_CONCEPT_URBG_
        Circle SmallestEnclosingCircle(Array<Vec2> points, const double tolerance, URBG&& urbg)
        {
            if (points.size() == 0) { return Circle{}; }
            if (points.size() == 1) { return Circle{points[0], 0.0}; }
            if (points.size() == 2) { return Circle{points[0], points[1]}; }
            if (points.size() == 3) { return SmallestEnclosingCircle(points[0], points[1], points[2]); }
            if (points.size() == 4) { return SmallestEnclosingCircle(points[0], points[1], points[2], points[3], tolerance); }
            
            // 実験的にインデックスをシャッフルするのではなく、配列をコピーして直接シャッフルした方が高速なことがわかった。キャッシュの恩恵によるものと考えられる。
            points.shuffle(std::forward<URBG>(urbg));

            // 適当な 1 点を含む最小包含円Cから始めて、少しずつ広げていく戦略を取る。
            // Cに含まれない点があったら、それが境界上になるように新たに取り直す。
            Circle circle = Circle{ points[0], 0.0 };

            for (size_t i = 1; i < points.size(); ++i)
            {
                const Vec2& p0 = points[i];

                if (not detail::Contains(circle, p0, tolerance))
                {
                    // i番目の点を最小内包円の境界上の点の一つとして固定して、残りの2点を探る。
                    circle = Circle{ p0, 0.0 };

                    for (size_t j = 0; j < i; ++j)
                    {
                        const Vec2& p1 = points[j];
                        if (not detail::Contains(circle, p1, tolerance))
                        {
                            // j番目の点を最小内包円の境界上の点の一つとして固定して、残りの1点を探る。
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
        Circle SmallestEnclosingCircle(const Array<Vec2>& points, URBG&& urbg, const double tolerance)
        {
            return SmallestEnclosingCircle(points, tolerance, urbg);
       }
    }
}