#include "../calculate_convex_hull/calculate_convex_hull.hpp"
#include "polygonization_incremental.hpp"
#include "../structs/Replaceable_edge.hpp"
#include "../min_max_area/try_minimizing_area.hpp"
#include "../min_max_area/try_maximizing_area.hpp"
bool check_for_duplicate_edge(const std::vector<Replaceable_edge> replaceable_edges_of_polygon, const Segment_2& edge){
    bool duplicate_edge = false;
    for(std::size_t l = 0; l < replaceable_edges_of_polygon.size(); l++){
        if(replaceable_edges_of_polygon.at(l).edge == edge){
            duplicate_edge = true;
            break;
        }
    }
    return duplicate_edge;
}

void sort_point_set(std::vector<Point_2>& points, const Sort_by_coordinate& sort_by_coordinate){
    for(std::size_t j = 1; j < points.size(); j++){//Optimised bubble sort in ascending order
        bool swapped = false;
        for(std::size_t i = 0; i < points.size() - j; i++){
            if(sort_by_coordinate == x_descending || sort_by_coordinate == x_ascending){
                if(points.at(i).x() > points.at(i + 1).x()){
                    swap(points.at(i), points.at(i + 1));
                    swapped = true;
                }
            }
            if(sort_by_coordinate == y_descending || sort_by_coordinate == y_ascending){
                if(points.at(i).y() > points.at(i + 1).y()){
                    swap(points.at(i), points.at(i + 1));
                    swapped = true;
                }
            }
        }
        if(!swapped){
            break;
        }
    }
    
    if(sort_by_coordinate == y_descending || sort_by_coordinate == x_descending){
        reverse(points.begin(), points.end());
    }
}

void resolve_collinear_initial_points(Polygon_2& polygon_2, std::vector<Point_2>& polygon_2_points, std::vector<Point_2>& points){
    int attempted_sort = 0;
    Sort_by_coordinate new_sort_by_coordinate[] = {x_descending, x_ascending, y_descending, y_ascending};
    while(CGAL::collinear(points.at(0), points.at(1), points.at(2)) && attempted_sort < 4){ // Try all sorting orders to eliminate the collinear property of the initial polygon points
        sort_point_set(points, new_sort_by_coordinate[attempted_sort++]);
        polygon_2.clear();
        polygon_2_points.clear();
        for(std::size_t i = 0; i < 3; i++){
            polygon_2.push_back(points.at(i));
            polygon_2_points.push_back(points.at(i));
        }
    }
    if(attempted_sort >= 4){
        std::cerr << "Error: failed to find initial triangle" << std::endl;
        polygon_2.clear();
        polygon_2_points.clear();
    }
}

std::vector<Replaceable_edge> find_red_edges(const Polygon_2& convex_hull, Point_2& point){
    std::vector<Replaceable_edge> red_edges;
    int counter_edge = 0;
    for(const Segment_2& edge: convex_hull.edges()){
        Replaceable_edge potential_red_edge;
        potential_red_edge.polygon_edge_index = ++counter_edge;
        potential_red_edge.edge = edge;
        potential_red_edge.potential_polygon_point = &point;
        
        Polygon_2 tmp_convex_hull = convex_hull;
        tmp_convex_hull.insert(tmp_convex_hull.begin() + counter_edge, point);
        if(tmp_convex_hull.is_simple()){
            red_edges.push_back(potential_red_edge); // If-f a point is visible from an edge of the convex_hull, then consider this edge red.
        }
    }
    return red_edges;
}

Polygon_2 polygonization_incremental(std::vector<Point_2>& points, const visible_edge_selection& edge_select, const Sort_by_coordinate& sort_by_coordinate){
    sort_point_set(points, sort_by_coordinate);
    Polygon_2 polygon_2;
    std::vector<Point_2> polygon_2_points;
    for(std::size_t i = 0; i < 3; i++){
        polygon_2.push_back(points.at(i));
        polygon_2_points.push_back(points.at(i));
    }
    
    resolve_collinear_initial_points(polygon_2, polygon_2_points, points);
    if(polygon_2.is_empty()){
        return polygon_2;
    }
    
    double min_area, max_area;
    std::size_t min_area_edge_index, max_area_edge_index;
    for(std::size_t i = 3; i < points.size(); i++){
        Polygon_2 convex_hull = calculate_convex_hull(polygon_2_points);
        std::vector<Replaceable_edge> red_edges = find_red_edges(convex_hull, points.at(i));

        min_area_edge_index = -1;
        max_area_edge_index = -1;

        std::vector<Replaceable_edge> replaceable_edges_of_polygon;
        Replaceable_edge potential_replaceable_edge;
        for(std::size_t j = 0; j < red_edges.size(); j++){ // Find the visible edges from the point that lie behind the red edge, including the red one if-f it's a polygon edge.
            int position_in_polygon = 0;
            int counter_vertices_of_red_edge_found = 0;
            for(const Segment_2& edge: polygon_2.edges()){
                potential_replaceable_edge.edge = edge;
                potential_replaceable_edge.polygon_edge_index = ++position_in_polygon;
                potential_replaceable_edge.potential_polygon_point = &points.at(i);
                
                Segment_2& current_red_edge = red_edges.at(j).edge;
                if(current_red_edge.source() == edge.source() || current_red_edge.source() == edge.target()){
                    counter_vertices_of_red_edge_found++;
                }
                if(current_red_edge.target() == edge.source() || current_red_edge.target() == edge.target()){
                    counter_vertices_of_red_edge_found++;
                }
                
                if(counter_vertices_of_red_edge_found >= 2){ // Look for visible edges of polygon behind the current red edge
                    if(!check_for_duplicate_edge(replaceable_edges_of_polygon, potential_replaceable_edge.edge)){
                        Polygon_2 tmp_polygon_2 = polygon_2;
                        tmp_polygon_2.insert(tmp_polygon_2.begin() + position_in_polygon, points.at(i));
                        if(tmp_polygon_2.is_simple()){
                            try_minimizing_area(min_area_edge_index, min_area, replaceable_edges_of_polygon.size(), tmp_polygon_2);
                            try_maximizing_area(max_area_edge_index, max_area, replaceable_edges_of_polygon.size(), tmp_polygon_2);
                            replaceable_edges_of_polygon.push_back(potential_replaceable_edge);
                        }
                    }
                }
                
                if(counter_vertices_of_red_edge_found >= 3 || current_red_edge == edge){ // Move to next red edge, if we have passed the vertices of the current red edge.
                    break;
                }
            }
        }
        if(replaceable_edges_of_polygon.empty()){ // Reached dead end; Terminate algorithm by returning an empty polygon
            polygon_2.clear();
            break;
        }
        
        std::size_t edge_index = edge_index_to_be_replaced(edge_select, replaceable_edges_of_polygon.size(), min_area_edge_index, max_area_edge_index);
        polygon_2.insert(polygon_2.begin() + replaceable_edges_of_polygon.at(edge_index).polygon_edge_index, points.at(i)); // Add point to polygon
        polygon_2_points.push_back(points.at(i));
        
        if(polygon_2.size() % 1000 == 0){ // Print polygon size per 1000 polygon points
            std::cout << polygon_2.size() << std::endl;
        }
    }
    if(polygon_2.orientation() == CGAL::CLOCKWISE){ // The signed area of a polygon in CW order is negative; thus, reverse its orientation
        polygon_2.reverse_orientation();
    }
    return polygon_2;
}
