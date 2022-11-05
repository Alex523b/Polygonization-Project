#include "../calculate_convex_hull/calculate_convex_hull.hpp"
#include "../structs/Replaceable_edge.hpp"
#include "../edge_selection/edge_selection.hpp"
#include "../min_max_area/try_minimizing_area.hpp"
#include "../min_max_area/try_maximizing_area.hpp"
Replaceable_edge find_closest_inside_point_to_edge(std::vector<Point_2>& inside_points, const Segment_2& edge){
    Replaceable_edge replaceable_edge;
    replaceable_edge.edge = edge;
    replaceable_edge.potential_polygon_point = NULL;

    for(Point_2& point: inside_points){
        if(replaceable_edge.potential_polygon_point == NULL){
            replaceable_edge.potential_polygon_point = &point;
        }
        
        CGAL::Comparison_result comparison_result = CGAL::compare_signed_distance_to_line(edge.source(), edge.target(), point, *replaceable_edge.potential_polygon_point);
        if(comparison_result == CGAL::SMALLER){
            replaceable_edge.potential_polygon_point = &point;
        }
    }
    return replaceable_edge;
}

Polygon_2 polygonization_convex_hull(std::vector<Point_2>& points, const visible_edge_selection& edge_select){
    Polygon_2 polygon_2 = calculate_convex_hull(points);// The initial polygon matches the convex hull
    std::vector<Point_2> inside_points;
    for(const Point_2& point: points){
        if(polygon_2.bounded_side(point) == CGAL::ON_BOUNDED_SIDE){ // Store inside points of convex_hull, to add them to the polygon afterwards
            inside_points.push_back(point);
        }
    }

    int counter_edge;
    Replaceable_edge potential_replaceable_edge;
    double min_area, max_area;
    std::size_t min_area_edge_index, max_area_edge_index;
    while(!inside_points.empty()){
        std::vector<Replaceable_edge> replaceable_edges_of_polygon;

        min_area_edge_index = -1;
        max_area_edge_index = -1;

        counter_edge = 0;
        for(const Segment_2& edge: polygon_2.edges()){
            potential_replaceable_edge = find_closest_inside_point_to_edge(inside_points, edge);
            potential_replaceable_edge.polygon_edge_index = ++counter_edge;
            
            // If-f the polygon remains simple after adding a point, then this point is considered visible from the edge
            Polygon_2 tmp_polygon_2 = polygon_2;
            tmp_polygon_2.insert(tmp_polygon_2.begin() + potential_replaceable_edge.polygon_edge_index, *potential_replaceable_edge.potential_polygon_point); 
            if(tmp_polygon_2.is_simple()){
                try_minimizing_area(min_area_edge_index, min_area, replaceable_edges_of_polygon.size(), tmp_polygon_2);
                try_maximizing_area(max_area_edge_index, max_area, replaceable_edges_of_polygon.size(), tmp_polygon_2);
                replaceable_edges_of_polygon.push_back(potential_replaceable_edge);
            }
        }
        if(replaceable_edges_of_polygon.empty()){// Reached dead end, since we failed to find an edge with the closest inside point being visible; Terminate algorithm
            polygon_2.clear();
            break;
        }
        
        std::size_t edge_index = edge_index_to_be_replaced(edge_select, replaceable_edges_of_polygon.size(), min_area_edge_index, max_area_edge_index);
        polygon_2.insert(polygon_2.begin() + replaceable_edges_of_polygon.at(edge_index).polygon_edge_index, *replaceable_edges_of_polygon.at(edge_index).potential_polygon_point);// Add point to polygon
        
        auto removed_inside_point = remove(inside_points.begin(), inside_points.end(), *replaceable_edges_of_polygon.at(edge_index).potential_polygon_point); // The point is on the polygon boundary; hence, remove it from inside_point vector
        inside_points.erase(removed_inside_point, inside_points.end());
    }
    return polygon_2;
}
