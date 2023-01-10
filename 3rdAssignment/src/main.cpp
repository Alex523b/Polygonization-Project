#include "parsing_input/parsing_input.hpp"
#include "write_results/write_results.hpp"
#include "polygonization_algorithms/polygonization_convex_hull.hpp"
#include "polygonization_algorithms/polygonization_incremental.hpp"
#include "calculate_convex_hull/calculate_convex_hull.hpp"
#include "polygon_optimization_algorithms/simulated_annealing.hpp"
#include "polygon_optimization_algorithms/local_search.hpp"
#include <vector>

using std::vector;

Polygon_2 compute_initial_polygon(vector<Point_2>& points, const polygonization_algorithm& greedy_alg){
    Polygon_2 initial_polygon_2;
    if(greedy_alg == convex_hull){
        do{
            initial_polygon_2 = polygonization_convex_hull(points);
        }while(initial_polygon_2.is_empty());
    }else{
        initial_polygon_2 = polygonization_incremental(points);
    }
    return initial_polygon_2;
}

int main(int argc, char** argv){
    char *input_directory, *output_file;
    double threshold;
    int L;
    bool apply_preprocessing, apply_metropolis_criterion = true;

    annealing_method annealing;
    extremum_method extremum_method;
    optimization_algorithm opt_alg;
    polygonization_algorithm greedy_alg;

    parse_command_line_arguments(argc, argv, &input_directory, &output_file, apply_preprocessing);

    std::vector<vectorFileString> file_set;
    parse_input_directory(input_directory, file_set);

    srand(time(NULL));

    std::cout << "Output file: " << output_file << std::endl;
    initialize_output_file(output_file);

    std::fstream output;
    output.open(output_file, std::ios::app);
    std::vector<Point_2> points;
    for(std::size_t i = 0; i < file_set.size(); i++){
        parse_input_file(file_set.at(i).fileWithPath, points);
        std::cout << file_set.at(i).fileWithPath << ' ' << points.size()  << std::endl;
        points.clear();
    }
    output.close();

    delete input_directory;
    delete output_file;

    return 0;
}
