#include <CGAL/Polygon_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
#include <iostream>
void try_maximizing_area(std::size_t& max_area_edge_index, double& max_area, const std::size_t& size_of_replaceable_edges_of_polygon, const Polygon_2& polygon_2);
