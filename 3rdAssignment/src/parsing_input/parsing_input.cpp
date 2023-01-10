extern "C"{
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #include <fcntl.h>
    #include <unistd.h>
}

#include "parsing_input.hpp"

typedef std::string string;
typedef std::ifstream ifstream;

using std::vector;

void parse_command_line_arguments(int argc, char **argv, char **input_directory, char **output_file, bool& apply_preprocessing){
    apply_preprocessing = false;
    for(int i = 1; i < argc; i++){
        char *str = argv[i];
        char *value = argv[i+1];
        if(!strcmp(str, "-i")){
            value = argv[i+1];
            *input_directory = new char[strlen(value) + 1];
            strcpy(*input_directory, value);

        }else if(!strcmp(str, "-o")){
            value = argv[i+1];
            *output_file = new char[strlen(value) + 1];
            strcpy(*output_file, value);

        }else if(!strcmp(str, "-preprocess")){
            apply_preprocessing = true;
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


void parse_input_directory(const char * const TARGET_DIRECTORY, vector<struct vectorFileString> &vec){
    DIR* dirPtr;
    struct dirent *direntPtr;
    if((dirPtr = opendir(TARGET_DIRECTORY)) == NULL){
        perror("opendir error");
        pthread_exit((void *)1);
    }
    struct stat buf;
    while((direntPtr = readdir(dirPtr)) != NULL){
        if(strcmp(direntPtr->d_name, ".") && strcmp(direntPtr->d_name, "..")){//Ignore "." and ".." entities of directory
            char *directoryEntity = NULL;
            directoryEntity = new char[strlen(TARGET_DIRECTORY) + strlen(direntPtr->d_name) + 1 + 1];
            directoryEntity[0] = '\0';
            strcat(directoryEntity, TARGET_DIRECTORY);
            if(TARGET_DIRECTORY[strlen(TARGET_DIRECTORY) - 1] != '/'){
                strcat(directoryEntity, "/");
            }
            strcat(directoryEntity, direntPtr->d_name);//final string: "TARGET_DIRECTORY/fileName"
            stat(directoryEntity, &buf);// Obtain information about entity
            if((buf.st_mode & S_IFMT) == S_IFDIR){// Check whether entity is directory or regular file
                parse_input_directory(directoryEntity, vec);//read contents of sub-directory
            }else{
                //insert file to vector
                struct vectorFileString temp;
                temp.fileWithPath = new char[strlen(directoryEntity)+1];
                strcpy(temp.fileWithPath, directoryEntity);
                vec.push_back(temp);
            }
            delete[] directoryEntity;
        }
    }
    closedir(dirPtr);
}