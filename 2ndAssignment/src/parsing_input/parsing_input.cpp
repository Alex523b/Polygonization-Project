#include "parsing_input.hpp"

typedef std::string string;
typedef std::ifstream ifstream;
void parse_command_line_arguments(int argc, char **argv, char **input_file, char **output_file, polygonization_algorithm& greedy_alg, optimization_algorithm& algorithm, double& threshold, annealing_method& annealing, int& L, extremum_method& extremum_method){
    for(int i = 1; i < argc; i+=2){
        char *str = argv[i];
        char *value = argv[i+1];
        if(!strcmp(str, "-i")){
            value = argv[i+1];
            *input_file = new char[strlen(value) + 1];
            strcpy(*input_file, value);

        }else if(!strcmp(str, "-o")){
            value = argv[i+1];
            *output_file = new char[strlen(value) + 1];
            strcpy(*output_file, value);

        }else if(!strcmp(str, "-algorithm")){
            value = argv[i+1];
            algorithm = !strcmp(value, "local_search") ? local_search : simulated_annealing;

        }else if(!strcmp(str, "-threshold")){
            value = argv[i+1];
            threshold = atof(value);

        }else if(!strcmp(str, "-annealing")){
            value = argv[i+1];
            if(!strcmp(value, "local")){
                annealing = local;
            }else if(!strcmp(value, "global")){
                annealing = global;
            }else{
                annealing = subdivision;
            }

        }else if(!strcmp(str, "-L")){
            value = argv[i+1];
            L = atoi(value);

        }else if(!strcmp(str, "-greedy_algorithm")){
            value = argv[i+1];
            greedy_alg = !strcmp(str, "convex_hull") ? convex_hull : incremental;

        }
        else{
            extremum_method = !strcmp(str,"-min") ? min : max;
        }
    }
}

Point_2 parse_point_from_file(string line){
    string delimiter = "\t"; // elements of a line are separated by horizontal tabulation
    int x_coordinate, y_coordinate;
    size_t pos = 0;
    string token;
    int counter_tokens = 0;
    while((pos = line.find(delimiter)) != string::npos){
        token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        if(++counter_tokens == 1){ // Ignore the first token, which is the point index
            x_coordinate = std::stoi(line);
        }
    }
    y_coordinate = std::stoi(line);
    return Point_2(x_coordinate, y_coordinate);
}

void parse_input_file(const char *file_name, std::vector<Point_2>& points){
    int counter_lines = 0;
    string line;
    ifstream input_file(file_name);
    while(getline(input_file, line)){
        if(++counter_lines > 2){ // Ignore the first two lines of input file, since we only parse points
            points.push_back(parse_point_from_file(line));
        }
    }

    input_file.close();
}

