#include "parsing_input/parsing_input.hpp"
#include "write_results/write_results.hpp"
#include "polygonization_algorithms/polygonization_convex_hull.hpp"
#include "polygonization_algorithms/polygonization_incremental.hpp"
#include "calculate_convex_hull/calculate_convex_hull.hpp"
#include "polygon_optimization_algorithms/simulated_annealing.hpp"
#include "polygon_optimization_algorithms/local_search.hpp"
#include <vector>

using std::vector;

Polygon_2 compute_initial_polygon(vector<Point_2>& points, const polygonization_algorithm& greedy_alg, double start){
    Polygon_2 initial_polygon_2;
    if(greedy_alg == convex_hull){
        do{
            initial_polygon_2 = polygonization_convex_hull(points, start);
        }while(initial_polygon_2.is_empty());
    }else{
        initial_polygon_2 = polygonization_incremental(points, start);
    }
    return initial_polygon_2;
}

void update_report_vector(std::vector<struct report>& report_vector, std::size_t k, std::size_t point_set_size, double min_score, double max_score){
    bool found = false;
    for(std::size_t j = 0; j < report_vector.size() && !found; j++){
        if(report_vector.at(j).point_set_size == point_set_size){
            found = true;
            if(k == 0){
                report_vector.at(j).number_of_files_with_point_set_size++;
            }
            report_vector.at(j).alg_perf[k].sum_min += min_score;
            report_vector.at(j).alg_perf[k].sum_max += max_score;

            if(report_vector.at(j).alg_perf[k].min_bound == -1 || min_score > report_vector.at(j).alg_perf[k].min_bound){
                report_vector.at(j).alg_perf[k].min_bound = min_score;
            }

            if(report_vector.at(j).alg_perf[k].max_bound == -1 || max_score < report_vector.at(j).alg_perf[k].max_bound){
                report_vector.at(j).alg_perf[k].max_bound = max_score;
            }
        }
    }
    if(!found){
        struct report rep;
        rep.number_of_files_with_point_set_size = 1;
        rep.point_set_size = point_set_size;

        rep.alg_perf[0].sum_max = max_score;
        rep.alg_perf[0].sum_min = min_score;
        rep.alg_perf[0].min_bound = min_score;
        rep.alg_perf[0].max_bound = max_score;

        rep.alg_perf[1].min_bound = -1;
        rep.alg_perf[1].max_bound = -1;
        rep.alg_perf[1].sum_min = 0;
        rep.alg_perf[1].sum_max = 0;

        report_vector.push_back(rep);
    }
}

int main(int argc, char** argv){
    char *input_directory, *output_file;
    double threshold = 100000;
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
    initialize_output_file(output_file);
    std::cout << "Output file: " << output_file << std::endl;

    int L_local_search = 5, L_simulated_annealing_global = 1000, L_simulated_annealing_local = L_simulated_annealing_global;
    threshold = 100000;
    if(apply_preprocessing){
        L_local_search = 2;
    }

    std::vector<Point_2> points;
    std::vector<report> report_vector;
    double min_score, max_score;
    time_t start;
    for(std::size_t i = 0; i < file_set.size(); i++){
        parse_input_file(file_set.at(i).fileWithPath, points);
        if(apply_preprocessing){
            L_simulated_annealing_local = points.size() * 10;
            threshold = points.size() * 100000;
        }
        for(std:: size_t k = 0; k < 2; k++){
            for(std::size_t extremum_method_index = 0; extremum_method_index < 2; extremum_method_index++){
                start = time(NULL);
                Polygon_2 polygon_2 = compute_initial_polygon(points, incremental, start);

                if(k == 0){
                    polygon_2 = optimization_simulated_annealing(L_simulated_annealing_global, polygon_2, extremum_method_index == 0 ? min : max, global, calculate_convex_hull(points).area(), false, start);
                    if(!polygon_2.is_empty()){
                        polygon_2 = optimization_simulated_annealing(L_simulated_annealing_local, polygon_2, extremum_method_index == 0 ? min : max, local, calculate_convex_hull(points).area(), false, start);
                    }
                }else{
                    polygon_2 = optimization_simulated_annealing(L_simulated_annealing_global, polygon_2, extremum_method_index == 0 ? min : max, global, calculate_convex_hull(points).area(), false, start);
                    if(!polygon_2.is_empty()){
                        polygon_2 = optimization_local_search(L_local_search, threshold, polygon_2, extremum_method_index == 0 ? min : max, start);
                    }
                }

                if(polygon_2.is_empty()){
                    extremum_method_index == 0 ? min_score = 1 : max_score = 0;
                }else{
                    extremum_method_index == 0 ? min_score = polygon_2.area() / calculate_convex_hull(points).area() : max_score = polygon_2.area() / calculate_convex_hull(points).area();
                }
            }
            update_report_vector(report_vector, k, points.size(), min_score, max_score);
        }
        points.clear();
        std::cout << "Finished processing file: " << file_set.at(i).fileWithPath << std::endl;
    }

    for(std::size_t i = 0; i < report_vector.size(); i++){
        write_results_to_output_file(output_file, report_vector.at(i));
    }

    delete input_directory;
    delete output_file;

    return 0;
}
