#include "try_minimizing_area.hpp"
void try_minimizing_area(std::size_t& min_area_edge_index, double& min_area, const std::size_t& size_of_replaceable_edges_of_polygon, const Polygon_2& polygon_2){
    double polygon_2_area = fabs(polygon_2.area());
    if(min_area_edge_index == -1 || polygon_2_area < min_area){
        min_area = polygon_2_area;
        min_area_edge_index = size_of_replaceable_edges_of_polygon;
    }
}