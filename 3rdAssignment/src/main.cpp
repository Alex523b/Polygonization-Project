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
    double threshold;
    int L;
    bool apply_preprocessing, apply_metropolis_criterion = true;
    clock_t s = clock();
    annealing_method annealing;
    extremum_method extremum_method;
    optimization_algorithm opt_alg;
    polygonization_algorithm greedy_alg;

    parse_command_line_arguments(argc, argv, &input_directory, &output_file, apply_preprocessing);

    std::vector<vectorFileString> file_set;
    parse_input_directory(input_directory, file_set);

    srand(time(NULL));

    std::cout << "Output file: " << output_file << std::endl;

    std::vector<Point_2> points;
    std::vector<report> report_vector;
    double min_score, max_score;
    clock_t start;
    for(std::size_t i = 0; i < file_set.size(); i++){
        parse_input_file(file_set.at(i).fileWithPath, points);
        for(std:: size_t k = 0; k < 2; k++){
            for(std::size_t extremum_method_index = 0; extremum_method_index < 2; extremum_method_index++){
                start = (double) clock();
                Polygon_2 polygon_2 = compute_initial_polygon(points, incremental, start);

                if(k == 0){
                    polygon_2 = optimization_simulated_annealing(polygon_2.size(), polygon_2, extremum_method_index == 0 ? min : max, global, calculate_convex_hull(points).area(), false, start);
                    polygon_2 = optimization_simulated_annealing(polygon_2.size() * 10, polygon_2, extremum_method_index == 0 ? min : max, local, calculate_convex_hull(points).area(), false, start);
                }else{
                    polygon_2 = optimization_simulated_annealing(polygon_2.size(), polygon_2, extremum_method_index == 0 ? min : max, global, calculate_convex_hull(points).area(), false, start);
                    polygon_2 = optimization_local_search(2, (polygon_2.size() / 2) * 10000, polygon_2, extremum_method_index == 0 ? min : max, start);
                }

                if(polygon_2.is_empty()){
                    extremum_method_index == 0 ? min_score = 1 : max_score = 0;
                }else{
                    extremum_method_index == 0 ? min_score = polygon_2.area() / calculate_convex_hull(points).area() : max_score = polygon_2.area() / calculate_convex_hull(points).area();
                }
                clock_t end = (double) clock();
                double construction_time = (double)(end - start) / CLOCKS_PER_SEC;
                std::cout << long(construction_time) << std::endl;
            }
            std::cout << min_score << std::endl;
            std::cout << max_score << std::endl;
            update_report_vector(report_vector, k, points.size(), min_score, max_score);

        }
        points.clear();
        std::cout << "Finished processing file: " << file_set.at(i).fileWithPath << std::endl;
    }

    using std::cout;
    using std::endl;
    for(std::size_t i = 0; i < report_vector.size(); i++){
        write_results_to_output_file(output_file, report_vector.at(i));
    }

    delete input_directory;
    delete output_file;
    clock_t end = (double) clock();
    double construction_time = (double)(end - s) / CLOCKS_PER_SEC;
    std::cout << construction_time << std::endl;
    return 0;
}
