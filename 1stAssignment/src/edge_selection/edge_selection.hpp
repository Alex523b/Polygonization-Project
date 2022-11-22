#ifndef EDGE_SELECT_HPP
#define EDGE_SELECT_HPP
enum visible_edge_selection {rand_edge = 1, minimize_area = 2, maximize_area = 3};
#endif
std::size_t edge_index_to_be_replaced(const visible_edge_selection& edge_select, const std::size_t& number_of_replaceable_edges, const std::size_t& min_area_edge_index, const std::size_t& max_area_edge_index);
