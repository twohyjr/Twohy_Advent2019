#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<int> getMassValuesFromInputFile(std::string filepath) {
    std::vector<int> massValues;
    std::ifstream inputFile;
    
    try {
        inputFile.open(filepath);
        
        int a;
        while(inputFile >> a) {
            massValues.push_back(a);
        }
        
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::INPUT_FILE::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }
    
    
    return massValues;
}


int main(int argc, const char * argv[]) {
    auto massValues = getMassValuesFromInputFile("Dec1/input");
    
    int total = 0;
    for(int massValue : massValues) {
        total += (massValue / 3) - 2;
    }
    
    std::cout << "The total fuel necessary is: " << total << std::endl;
    // Answer: 3520097
    
    return 0;
}


//Examples

//floor(mass / 3) - 2
//floor(12 / 3) - 2 = 2
//floor(14 / 3) - 2 = 2
//floor(1969 / 3) - 2 = 654
