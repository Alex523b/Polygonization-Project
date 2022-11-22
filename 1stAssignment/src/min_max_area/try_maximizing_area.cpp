#include "try_maximizing_area.hpp"
void try_maximizing_area(std::size_t& max_area_edge_index, double& max_area, const std::size_t& size_of_replaceable_edges_of_polygon, const Polygon_2& polygon_2){
    double polygon_2_area = fabs(polygon_2.area());
    if(max_area_edge_index == -1 || polygon_2_area > max_area){
        max_area = polygon_2_area;
        max_area_edge_index = size_of_replaceable_edges_of_polygon;
    }
}