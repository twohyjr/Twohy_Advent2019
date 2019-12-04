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

// Part 1
int calculateFuel(int massValue) {
    return (massValue / 3) - 2;
}

// Part 2
int calculateFuelRecursive(int massValue, int total) {
    int newFuelLevel = (massValue / 3) - 2;
    if(newFuelLevel <= 0) {
        return total;
    }
    total += newFuelLevel;
    return calculateFuelRecursive(newFuelLevel, total);
}

int main(int argc, const char * argv[]) {
    auto massValues = getMassValuesFromInputFile("Dec1_input");
    
    // Part 1
    int total = 0;
    for(int massValue : massValues) {
        total +=  calculateFuel(massValue);
    }
    // Answer: 3520097
    std::cout << "The total fuel necessary overall is: " << total << std::endl;
    
    
    // Part 2
    total = 0;
    for(int massValue : massValues) {
        total += calculateFuelRecursive(massValue, 0);
    }
    // Answer: 5277255
    std::cout << "The total fuel necessary with the other fuel counted in is: " << total << std::endl;
    
    return 0;
}
