#pragma once

#include <math/polygon/polygon.hpp>
#include <math/line_like/segment/segment.hpp>

MATH_NAMESPACE_BEGIN

enum class polygon_point_classification
{
    outside = 0,
    inside = 1,
    on_edge = 2,
};

template<class T>
polygon_point_classification contains(const polygon<T>& poly, const point<T, 2>& p)
{
    auto ray_from_point = ray<T, 2>(p, p + vector<T, 2>({ 1,0 }));

    size_t count_intersections = 0;

    for (size_t i = 0; i < poly.size(); i++)
    {
        auto edge = segment<T, 2>(poly[i], poly[(i + 1) % poly.size()]);

        if (get_side_of_line(edge, p) == side_of_line::on)
        {
            return polygon_point_classification::on_edge;
        }

        auto intersection_point_opt = intersection(ray_from_point, edge);
        if (intersection_point_opt)
        {
            auto& intersection_point = *intersection_point_opt;

            count_intersections++;
        }
    }

    return count_intersections % 2 ? polygon_point_classification::inside : polygon_point_classification::outside;
}

template<class T>
bool intersects_interior(const segment<T, 2>& s, const polygon<T>& poly)
{
    size_t count_intersections = 0;

    for (size_t i = 0; i < poly.size(); i++)
    {
        auto edge = segment<T, 2>(poly[i], poly[(i + 1) % poly.size()]);

        if (get_side_of_line(edge, s[0]) == side_of_line::on ||
            get_side_of_line(edge, s[1]) == side_of_line::on)
        {
            continue;
        }

        auto intersection_point_opt = intersection(s, edge);
        if (intersection_point_opt)
        {
            auto& intersection_point = *intersection_point_opt;

            count_intersections++;
        }
    }

    return count_intersections > 0;
}

MATH_NAMESPACE_END