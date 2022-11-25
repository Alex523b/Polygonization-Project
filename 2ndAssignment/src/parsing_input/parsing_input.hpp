#ifndef PARSING_INPUT_HPP
#define PARSING_INPUT_HPP
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef K::Point_2 Point_2;

enum polygonization_algorithm {incremental, convex_hull};
enum annealing_method {local, global, subdivision};
enum extremum_method {min, max};
enum optimization_algorithm {local_search, simulated_annealing};
#endif
void parse_input_file(const char *file_name, std::vector<Point_2>& points);
void parse_command_line_arguments(int argc, char **argv, char **input_file, char **output_file, polygonization_algorithm& greedy_alg, optimization_algorithm& algorithm, double& threshold, annealing_method& annealing, int& L, extremum_method& extremum_method);
