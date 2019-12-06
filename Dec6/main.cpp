#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

std::vector<std::string> readInOrbitData(std::string filepath) {
    std::vector<std::string> orbitDatas;
    std::ifstream inputFile;
    std::string line;
    
    try {
        inputFile.open(filepath);
        while(getline(inputFile, line)) {
            orbitDatas.push_back(line);
        }
        
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::INPUT_FILE::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }
    return orbitDatas;
}

int getPart1Total(std::vector<std::string> orbitStringDatas) {
    std::map<std::string, std::string> planets;
    for(auto orbitStringData : orbitStringDatas) {
        std::string parentName = orbitStringData.substr(0, orbitStringData.find(")"));
        std::string childName = orbitStringData.substr(orbitStringData.find(")") + 1, orbitStringData.size() - 1);
        planets[childName] = parentName;
    }
    
    int total = 0;
    for(std::map<std::string,std::string>::iterator iter = planets.begin(); iter != planets.end(); ++iter) {
        std::string value =  iter->second; // get value
        while(planets.find(value) != planets.end()) {
            value = planets[value];
            total += 1;
        }
        total += 1;
    }

    return total;
}

// Not 100% proud of this 
int getPart2Total(std::vector<std::string> orbitStringDatas) {
    std::map<std::string, std::string> planets;
    for(auto orbitStringData : orbitStringDatas) {
        std::string parentName = orbitStringData.substr(0, orbitStringData.find(")"));
        std::string childName = orbitStringData.substr(orbitStringData.find(")") + 1, orbitStringData.size() - 1);
        planets[childName] = parentName;
    }
    
    std::string value = planets["SAN"];
    std::vector<std::string> pathToSanta;
    while(planets.find(value) != planets.end()) {
        pathToSanta.push_back(value);
        value = planets[value];
    }
    
    value = planets["YOU"];
    std::map<std::string, bool> pathToYou;
    while(planets.find(value) != planets.end()) {
        pathToYou[value] = true;
        value = planets[value];
    }

    std::string closestConnectedPath;
    for(auto path : pathToSanta) {
        if(pathToYou.find(path) != pathToYou.end()) {
            closestConnectedPath = path;
            break;
        }
    }
    
    // D is the endpoint
    value = planets["SAN"];
    int santaTotal = 0;
    while(planets.find(value) != planets.end()) {
        if(value == closestConnectedPath) {
           break;
        }
        santaTotal += 1;
        value = planets[value];
    }
    
    value = planets["YOU"];
    int youTotal = 0;
    while(planets.find(value) != planets.end()) {
        if(value == closestConnectedPath) {
           break;
        }
        youTotal += 1;
        value = planets[value];
    }

    return santaTotal + youTotal;
}

int main(int argc, const char * argv[]) {
    std::vector<std::string> orbitStringDatas = readInOrbitData("Dec6_input");
    
    int totalPart1 = getPart1Total(orbitStringDatas);
    // Answer -- 333679
    std::cout << "Part 1: The total is: " << totalPart1 << std::endl;

    
    int totalPart2 = getPart2Total(orbitStringDatas);
    // Answer -- 370
    std::cout << "Part 2: The total is: " << totalPart2 << std::endl;
    
    return 0;
}
