#include "parsing_input.hpp"

typedef std::string string;
typedef std::ifstream ifstream;

void parse_command_line_arguments(int argc, char **argv, char **input_file, char **output_file, polygonization_algorithm& algorithm, visible_edge_selection& edge_select, Sort_by_coordinate& initialization){
    for(int i = 1; i < argc; i+=2){
        char *str = argv[i];
        char *value;
        if(!strcmp(str, "-i")){
            *input_file = new char[strlen(argv[i+1]) + 1];
            value = argv[i+1];
            strcpy(*input_file, value);

        }else if(!strcmp(str, "-o")){
            *output_file = new char[strlen(str) + 1];
            value = argv[i+1];
            strcpy(*output_file, value);

        }else if(!strcmp(str, "-algorithm")){
            value = argv[i+1];
            if(!strcmp(value, "incremental")){
                algorithm = incremental;
            }else{
                algorithm = convex_hull;
            }

        }else if(!strcmp(str, "-edge_selection")){
            value = argv[i+1];
            edge_select = static_cast<visible_edge_selection>((atoi(value)));

        }else if(!strcmp(str, "-initialization")){
            value = argv[i+1];
            if(strcmp(value, "1a") == 0){
                initialization = x_descending;
            }else if(!strcmp(value, "1b")){
                initialization = x_ascending;
            }else if(!strcmp(value, "2a")){
                initialization = y_descending;
            }else if(!strcmp(value,"2b")){
                initialization = y_ascending;
            }
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

