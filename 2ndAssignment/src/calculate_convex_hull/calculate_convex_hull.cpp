#include <CGAL/convex_hull_2.h>
#include "calculate_convex_hull.hpp"
Polygon_2 calculate_convex_hull(const std::vector<Point_2>& points){
    std::vector<Point_2> convex_hull_points;
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(convex_hull_points));

    Polygon_2 convex_hull;
    for(const Point_2& convex_hull_point : convex_hull_points){
        convex_hull.push_back(convex_hull_point);
    }
    return convex_hull;
}
