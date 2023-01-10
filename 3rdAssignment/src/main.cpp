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
    char *input_file, *output_file;
    double threshold;
    int L;
    bool apply_metropolis_criterion;

    annealing_method annealing;
    extremum_method extremum_method;
    optimization_algorithm opt_alg;
    polygonization_algorithm greedy_alg;

    parse_command_line_arguments(argc, argv, &input_file, &output_file, greedy_alg, opt_alg, threshold, annealing, L, extremum_method, apply_metropolis_criterion);

    std::vector<Point_2> points;
    parse_input_file(input_file, points);

    srand(time(NULL));

    Polygon_2 initial_polygon_2 = compute_initial_polygon(points, greedy_alg), opt_polygon_2;

    clock_t begin = clock();
    if(opt_alg == local_search){
        opt_polygon_2 = optimization_local_search(L, threshold, initial_polygon_2, extremum_method);
    }else{
        opt_polygon_2 = optimization_simulated_annealing(L, initial_polygon_2, extremum_method, annealing, calculate_convex_hull(points).area(), apply_metropolis_criterion);
    }
    clock_t end = clock();
    double construction_time = (double)(end - begin) / CLOCKS_PER_SEC;

    write_results_to_output_file(output_file, points, initial_polygon_2, opt_polygon_2, construction_time, opt_alg, extremum_method);

    delete input_file;
    delete output_file;

    return 0;
}
