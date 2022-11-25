#include "parsing_input/parsing_input.hpp"
#include "write_results/write_results.hpp"
#include "polygonization_algorithms/polygonization_convex_hull.hpp"
#include "polygonization_algorithms/polygonization_incremental.hpp"
using std::cout;
using std::endl;

int main(int argc, char** argv){
    char *input_file, *output_file;
    double threshold;
    int L;

    annealing_method annealing;
    extremum_method extremum_method;
    optimization_algorithm opt_alg;
    polygonization_algorithm greedy_alg;
    parse_command_line_arguments(argc, argv, &input_file, &output_file, greedy_alg, opt_alg, threshold, annealing, L, extremum_method);
    std::vector<Point_2> points;
    parse_input_file(input_file, points);
    for(const Point_2& point: points){
        cout << point << endl;
    }
    
    Polygon_2 initial_polygon_2, opt_polygon_2;
    srand(time(NULL));
    if(greedy_alg == convex_hull){
        do{
            initial_polygon_2 = polygonization_convex_hull(points);
        }while(initial_polygon_2.is_empty());
    }else{
        initial_polygon_2 = polygonization_incremental(points);
    }
    double construction_time;
    write_results_to_output_file(output_file, points, initial_polygon_2, opt_polygon_2, construction_time, opt_alg, extremum_method);

    delete input_file;
    delete output_file;

    return 0;
}
