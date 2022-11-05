#include "parsing_input/parsing_input.hpp"
#include "write_results/write_results.hpp"
#include "polygonization_algorithms/polygonization_convex_hull.hpp"
using std::cout;
using std::endl;

int main(int argc, char** argv){
    char *input_file, *output_file;
    polygonization_algorithm algorithm;
    Sort_by_coordinate initialization;
    visible_edge_selection edge_select;
    parse_command_line_arguments(argc, argv, &input_file, &output_file, algorithm, edge_select, initialization);

    srand(time(NULL));
    
    std::vector<Point_2> points;
    parse_input_file(input_file, points);
    
    Polygon_2 polygon_2;
    clock_t begin = clock();
    if(algorithm == convex_hull){
        do{
            polygon_2 = polygonization_convex_hull(points, edge_select);
        }while(polygon_2.is_empty());
    }else{
        polygon_2 = polygonization_incremental(points, edge_select, initialization);
    }
    clock_t end = clock();
    double construction_time = (double)(end - begin) / CLOCKS_PER_SEC;
    
    write_results_to_output_file(output_file, points, polygon_2, construction_time, algorithm, edge_select, &initialization);
    
    cout << "debugging" << endl;
    cout << "polygon size: " << polygon_2.size() << endl;
    cout << "check if polygon is simple: " << polygon_2.is_simple() << endl;
    return 0;
}
