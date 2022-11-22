#include <iostream>
#include "edge_selection.hpp"
std::size_t edge_index_to_be_replaced(const visible_edge_selection& edge_select, const std::size_t& number_of_replaceable_edges, const std::size_t& min_area_edge_index, const std::size_t& max_area_edge_index){
    std::size_t edge_index;
    switch (edge_select){
        case rand_edge:
            edge_index = rand() % number_of_replaceable_edges;
            break;
        case minimize_area:
            edge_index = min_area_edge_index;
            break;
        case maximize_area:
            edge_index = max_area_edge_index;
            break;
    }
    return edge_index;
}