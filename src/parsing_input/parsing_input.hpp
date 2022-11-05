#ifndef PARSING_INPUT_HPP
#define PARSING_INPUT_HPP
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include "../polygonization_algorithms/polygonization_incremental.hpp"
#include "../edge_selection/edge_selection.hpp"
enum polygonization_algorithm {incremental, convex_hull};
#endif
void parse_input_file(const char *file_name, std::vector<Point_2>& points);
void parse_command_line_arguments(int argc, char **argv, char **input_file, char **output_file, polygonization_algorithm& algorithm, visible_edge_selection& edge_select, Sort_by_coordinate& initialization);
