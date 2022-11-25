#include "../calculate_convex_hull/calculate_convex_hull.hpp"
#include "../parsing_input/parsing_input.hpp"

using std::cout;
using std::endl;

void write_results_to_output_file(const char* output_file, const std::vector<Point_2>& points, const Polygon_2& initial_polygon_2, const Polygon_2& opt_polygon_2, const double& construction_time, const optimization_algorithm& optimization_algorithm, const extremum_method& extremum_method){
    std::fstream output;
    output.open(output_file, std::ios::out);
    cout << "Output file: " << output_file << endl;

    output << "Vertices of polygon" << endl;
    for(const Point_2& vertex: opt_polygon_2.vertices()){
        output << vertex << endl;
    }

    output << "Edges of polygon" << endl;
    for(const Segment_2& edge: opt_polygon_2.edges()){
        output << edge << endl;
    }

    output << "algorithm: ";
    if(optimization_algorithm == local_search){
        output << "local_search";
    }else{
        output << "simulated_annealing";
    }
    if(extremum_method == min){
        output << "_min";
    }else{
        output << "_max";
    }
    double convex_hull_area = calculate_convex_hull(points).area(), opt_area = opt_polygon_2.area(), area_initial = initial_polygon_2.area();
    output << "area_initial: " << area_initial << endl;
    output << "area: " << opt_area << endl;
    output << "ratio_initial: " << area_initial / convex_hull_area << endl;
    output << "ratio: " << opt_area / convex_hull_area << endl;
    output << "construction time: " << long(construction_time) << " seconds" << endl;

    output.close();
}
