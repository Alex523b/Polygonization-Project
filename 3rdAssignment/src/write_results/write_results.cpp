#include "../calculate_convex_hull/calculate_convex_hull.hpp"
#include "../parsing_input/parsing_input.hpp"
#include "write_results.hpp"

using std::endl;

void initialize_output_file(const char* output_file){
    std::fstream output;
    output.open(output_file, std::ios::trunc | std::ios::out);
    output << "     || incremental -> simulated_annealing annealing (global) -> simulated_annealing (local) || incremental -> simulated_annealing annealing (global) -> local search  ||" << std::endl;
    output << "Size ||" << std::setw(60) << " min score | max score | min bound | max bound" << std::setw(28) << "||" << std::setw(60) << " min score | max score | min bound | max bound" << std::setw(14) << "  ||" << std::endl;
    output.close();
}

void write_results_to_output_file(const char* output_file, const struct report& rep){
    std::fstream output;
    output.open(output_file, std::ios::app);
    output << std::left << std::setw(5) << rep.point_set_size << "||";
    output << std::setprecision(2) << std::fixed;
    for(std::size_t k = 0; k < 2; k++){
        output << "                  " <<  rep.alg_perf[k].sum_min / rep.number_of_files_with_point_set_size << "   |   " <<  rep.alg_perf[k].sum_max / rep.number_of_files_with_point_set_size << "    |  " <<  
        rep.alg_perf[k].min_bound << "     |    " <<  rep.alg_perf[k].max_bound;
        if(k == 0){
            output << "                            ||";
        }else{
            output << "              ||";
        }
    }
    output << endl;
    output.close();
}
