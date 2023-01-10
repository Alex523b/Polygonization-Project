#include "../calculate_convex_hull/calculate_convex_hull.hpp"
#include "../parsing_input/parsing_input.hpp"

using std::cout;
using std::endl;

void initialize_output_file(const char* output_file){
    std::fstream output;
    output.open(output_file, std::ios::app);
    //TODO: format output file
    output.close();
}

void write_results_to_output_file(const char* output_file, const int& polygon_size, const double& min_score, const double& max_score, const double& min_bound, const double& max_bound){
    std::fstream output;
    output.open(output_file, std::ios::app);

    //TODO: format results

    output.close();
}
