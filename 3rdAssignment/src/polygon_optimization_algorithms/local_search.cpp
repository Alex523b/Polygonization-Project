#include "local_search.hpp"

using std::vector;
typedef K::Segment_2 Segment_2;

struct Optimal_change_info{
    double new_polygon_2_area;
    Polygon_2 new_polygon_2;
};

vector<Point_2> add_vertices_to_path(std::size_t i, int current_path_length, const Polygon_2& optimized_polygon_2, const Segment_2& edge){
    vector<Point_2> path;
    for(std::size_t j = i; j < optimized_polygon_2.size() && path.size() < current_path_length; j++){
        const Point_2& current_vertex = optimized_polygon_2.container().at(j);
        if(current_vertex == edge.source()){
            break;
        }
        path.push_back(current_vertex);
    }
    return path;
}

Polygon_2 remove_path_points_from_polygon(const Polygon_2& optimized_polygon_2, int i, int path_length){
    Polygon_2 tmp_polygon_2 = optimized_polygon_2;
    for(int l = 0; l < path_length; l++){
        tmp_polygon_2.erase(tmp_polygon_2.begin() + i);
    }
    return tmp_polygon_2;
}

void insert_path_points_between_edge(Polygon_2& tmp_polygon_2, const Segment_2& edge, vector<Point_2>& path){
    reverse(path.begin(), path.end());
    for(int l = 0; l < tmp_polygon_2.size(); l++){
        if(tmp_polygon_2.container().at(l) == edge.source() || tmp_polygon_2.container().at(l) == edge.target()){
            for(int k = 0; k < path.size(); k++){
                tmp_polygon_2.insert(tmp_polygon_2.begin() + l, path.at(k));
            }
            break;
        }
    }
}

void add_potential_optimal_polygon(vector<Optimal_change_info>& T, const Polygon_2& tmp_polygon_2, extremum_method extremum_method, const double& optimized_polygon_2_area){
    bool potential_change = false;
    double tmp_polygon_2_area = tmp_polygon_2.area();
    if(tmp_polygon_2.is_simple()){
        if((extremum_method == max && tmp_polygon_2_area > optimized_polygon_2_area) ||
           (extremum_method == min && tmp_polygon_2_area < optimized_polygon_2_area))
        {
            potential_change = true;
        }
    }

    if(potential_change){
        Optimal_change_info t;

        t.new_polygon_2_area = tmp_polygon_2_area;
        t.new_polygon_2 = tmp_polygon_2;

        T.push_back(t);
    }
}

int find_optimal_change(vector<Optimal_change_info> T, extremum_method extremum_method){
    int optimal_change_index = -1;
    double optimal_area;
    for(int i = 0; i < T.size(); i++){
        double current_polygon_2_area = T.at(i).new_polygon_2_area;
        if(optimal_change_index == -1 || (extremum_method == max && current_polygon_2_area > optimal_area) || (extremum_method == min && current_polygon_2_area < optimal_area)){
            optimal_change_index = i;
            optimal_area = current_polygon_2_area;
        }
    }
    return optimal_change_index;
}

Polygon_2 optimization_local_search(const int& L, const double& threshold, const Polygon_2& initial_polygon_2, const extremum_method& extremum_method, clock_t start){
    Polygon_2 optimized_polygon_2 = initial_polygon_2;
    std::size_t optimized_polygon_2_size = optimized_polygon_2.size();
    double optimized_polygon_2_area, area_difference = threshold;
    while(area_difference >= threshold && (((double) clock() - start)/CLOCKS_PER_SEC) < 0.5 * optimized_polygon_2_size){
        area_difference = 0;

        optimized_polygon_2_area = optimized_polygon_2.area();

        vector<Optimal_change_info> T;

        for(const Segment_2& edge: optimized_polygon_2.edges()){
            for(int current_path_length = 1; current_path_length <= L; current_path_length++){
                for(std::size_t i = 0; i < optimized_polygon_2_size; i++){
                    vector<Point_2> path = add_vertices_to_path(i, current_path_length, optimized_polygon_2, edge);

                    if(path.size() == current_path_length){
                        Polygon_2 tmp_polygon_2 = remove_path_points_from_polygon(optimized_polygon_2, i, current_path_length);
                        insert_path_points_between_edge(tmp_polygon_2, edge, path);
                        add_potential_optimal_polygon(T, tmp_polygon_2, extremum_method, optimized_polygon_2_area);
                    }
                }
            }
        }

        int optimal_change_index = find_optimal_change(T, extremum_method);
        if(optimal_change_index != -1){
            optimized_polygon_2 = T.at(optimal_change_index).new_polygon_2;
            area_difference = fabs(optimized_polygon_2.area() - optimized_polygon_2_area);
        }
    }

    if(area_difference >= threshold){
        optimized_polygon_2.clear();
    }

    return optimized_polygon_2;
}
