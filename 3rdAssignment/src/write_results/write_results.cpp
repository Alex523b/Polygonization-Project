#include "../calculate_convex_hull/calculate_convex_hull.hpp"
#include "../parsing_input/parsing_input.hpp"
#include "write_results.hpp"

using std::endl;

void write_results_to_output_file(const char* output_file, const struct report& rep){
    std::fstream output;
    output.open(output_file, std::ios::out);
    output << "Size: " << rep.point_set_size << endl;
    for(std::size_t k = 0; k < 2; k++){
        if(k == 0){
            output << "\tAlgorithms: incremental -> simulated_annealing annealing (global) -> simulated_annealing (local)" << endl;
        }else{
            output << "\tAlgorithms: incremental -> simulated_annealing annealing (global) -> local search" << endl;
        }
        output << "\t\tmin_score: " << std::setprecision(2) << rep.alg_perf[k].sum_min / rep.number_of_files_with_point_set_size << endl;
        output << "\t\tmax_score: " << std::setprecision(2) << rep.alg_perf[k].sum_max / rep.number_of_files_with_point_set_size << endl;
        output << "\t\tmin_bound: " << std::setprecision(2) << rep.alg_perf[k].min_bound << endl;
        output << "\t\tmax_bound: " << std::setprecision(2) << rep.alg_perf[k].max_bound << endl;
    }
    output.close();
}
