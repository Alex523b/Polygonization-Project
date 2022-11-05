#include "../calculate_convex_hull/calculate_convex_hull.hpp"
#include "../parsing_input/parsing_input.hpp"

using std::cout;
using std::endl;

void write_results_to_output_file(const char* output_file, const std::vector<Point_2>& points, const Polygon_2& polygon_2, const double& construction_time, const polygonization_algorithm& algorithm, const visible_edge_selection& edge_select, const Sort_by_coordinate* const initialization){
    std::fstream output;
    output.open(output_file, std::ios::out);
    cout << "Output file: " << output_file << endl;
    
    output << "Vertices of polygon" << endl;
    for(const Point_2& vertex: polygon_2.vertices()){
        output << vertex << endl;
    }

    output << "Edges of polygon" << endl;
    for(const Segment_2& edge: polygon_2.edges()){
        output << edge << endl;
    }

    output << "Algorithm: ";
    if(algorithm == convex_hull){
        output << "convex_hull" << endl;
    }else{
        output << "incremental" << endl;
    }

    output << "Edge selection: " << edge_select << endl;
    
    if(algorithm == incremental){
        output << "Initialization: ";
        switch (*initialization){
            case x_descending:
                output << "1a" << endl;
            break;
            case x_ascending:
                output << "1b" << endl;
            break;
            case y_descending:
                output << "2a" << endl;
            break;
            case y_ascending:
                output << "2b" << endl;
            break;
        }
    }
    
    double convex_hull_area = calculate_convex_hull(points).area(), polygon_2_area = polygon_2.area();
    output << "area: " << long(polygon_2_area) << endl;
    output << "ratio: " << polygon_2_area / convex_hull_area << endl;
    output << "construction time: " << long(construction_time) << " seconds" << endl;
    
    output.close();
}
