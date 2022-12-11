#include "simulated_annealing.hpp"

using std::vector;

double calculate_energy(const double& optimized_polygon_2_area, const double& convex_hull_area, std::size_t optimized_polygon_2_size, const extremum_method& extremum_method){
    double energy;
    if(extremum_method == max){
        energy = optimized_polygon_2_size * (1 - (optimized_polygon_2_area / convex_hull_area));
    }else{
        energy = optimized_polygon_2_size * (optimized_polygon_2_area / convex_hull_area);
    }
    return energy;
}

void local_annealing(Polygon_2& tmp_polygon_2){
    std::size_t point_q_index = rand() % tmp_polygon_2.size(), point_s_index;

    while((point_s_index = rand() % tmp_polygon_2.size()) == point_q_index);

    if(point_q_index < point_s_index){
        for(std::size_t i = point_q_index; i < point_s_index; i++){
            swap(tmp_polygon_2.container().at(i), tmp_polygon_2.container().at(i + 1));
        }
    }else{
        for(std::size_t i = point_q_index; i > point_s_index; i--){
            swap(tmp_polygon_2.container().at(i), tmp_polygon_2.container().at(i - 1));
        }
    }
}

void global_annealing(Polygon_2& tmp_polygon_2){
    std::size_t vertex_index = rand() % tmp_polygon_2.size();
    swap(tmp_polygon_2.container().at(vertex_index), tmp_polygon_2.container().at((vertex_index + 1) % tmp_polygon_2.size()));
}

Polygon_2 optimization_simulated_annealing(const int& L, const Polygon_2& initial_polygon_2, const extremum_method& extremum_method, const annealing_method& annealing_method, const double& convex_hull_area, const bool& apply_metropolis_criterion){
    Polygon_2 optimized_polygon_2 = initial_polygon_2, tmp_polygon_2;

    double temperature = 1.0, energy, previous_energy;
    energy = (previous_energy = calculate_energy(optimized_polygon_2.area(), convex_hull_area, optimized_polygon_2.size(), extremum_method));
    while(temperature > 0){
        tmp_polygon_2 = optimized_polygon_2;

        if(annealing_method == local){
            global_annealing(tmp_polygon_2);
        }else{
            local_annealing(tmp_polygon_2);
        }

        if(tmp_polygon_2.is_simple()){
            previous_energy = energy;
            energy = calculate_energy(tmp_polygon_2.area(), convex_hull_area, tmp_polygon_2.size(), extremum_method);

            bool applicable_transition = false;
            if(energy - previous_energy < 0){
                applicable_transition = true;
            }

            if(apply_metropolis_criterion && !applicable_transition && 1.0 / exp((energy - previous_energy) / temperature / 4.0) > rand() / RAND_MAX){
                applicable_transition = true;
            }

            if(applicable_transition){
                optimized_polygon_2 = tmp_polygon_2;
                if(optimized_polygon_2.orientation() == CGAL::CLOCKWISE){
                    optimized_polygon_2.reverse_orientation();
                }
            }
        }else{
            continue;
        }

        temperature -= 1.0/L;
        std::cout << temperature << std::endl;
    }
    return optimized_polygon_2;
}
