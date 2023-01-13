#ifndef POLY_INC_HPP
#define POLY_INC_HPP
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef K::Point_2 Point_2;

enum Sort_by_coordinate {x_descending = 0, x_ascending, y_descending, y_ascending};
Polygon_2 polygonization_incremental(std::vector<Point_2>& points, clock_t start);
#endif
