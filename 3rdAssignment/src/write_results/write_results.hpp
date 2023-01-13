#ifndef WRITE_RESULTS_H
#define WRITE_RESULTS_H
struct algorithm_performance{
    double sum_min;
    double min_score;
    double min_bound;

    double sum_max;
    double max_score;
    double max_bound;
};

struct report{
    std::size_t point_set_size;
    int number_of_files_with_point_set_size;
    algorithm_performance alg_perf[2];
};
#endif
void write_results_to_output_file(const char* output_file, const struct report& rep);
