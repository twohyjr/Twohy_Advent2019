#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

std::vector<int> readComputerProgramOpcodes(std::string filepath) {
    std::vector<int> massValues;
    std::ifstream inputFile;
    std::string line;
    try {
        inputFile.open(filepath);
        while(getline(inputFile,line)) {
            std::stringstream   linestream(line);
            std::string opcodeString;
            int opcode;
            while(getline(linestream, opcodeString, ',')) {
                std::stringstream opcodeStream(opcodeString);
                opcodeStream >> opcode;
                massValues.push_back(opcode);
            }
        }
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::INPUT_FILE::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }
    
    return massValues;
}

int intCodeProgramFirstValue(std::vector<int> opcodes, int noun, int verb) {
    bool programIsTerminated = false;
    
    int replaceIndex1Value = noun;
    int replaceIndex2Value = verb;
    
    opcodes[1] = replaceIndex1Value;
    opcodes[2] = replaceIndex2Value;
    
    int currentIndex = 0;
    while(!programIsTerminated) {
        if(currentIndex == opcodes.size() - 1) {
            programIsTerminated = true;
        }
        
        // read next operation
        int opcode = opcodes[currentIndex];
        int parameter1 = opcodes[opcodes[currentIndex + 1]];
        int parameter2 = opcodes[opcodes[currentIndex + 2]];
        int positionToUpdate = opcodes[currentIndex + 3];
        
        // create new value
        int newVal = 0;
        switch (opcode) {
            case 1:
                // ADD
                newVal = parameter1 + parameter2;
                break;
            case 2:
                // MULTIPLY
                newVal = parameter1 * parameter2;
                break;
            case 99:
                programIsTerminated = true;
                break;
            default:
                programIsTerminated = true;
                std::cout << "Something horrible has happened" << std::endl;
                break;
        }
        
        // set value at position to new value
        opcodes[positionToUpdate] = newVal;
        
        currentIndex += 4;
    }
    
    return opcodes[0];
}

int bruteForceTheShitOutOfTheAnswer(std::vector<int> &opcodes, int valueToFind, int iterations) {
    bool foundNounAndVerb = false;
    int noun = 0, verb = 0;
    for(int i = 0; i < iterations; i++) {
        for(int j = 0; j < iterations; j++) {
            int intCodeProgramValue = intCodeProgramFirstValue(opcodes, i, j);
            if(intCodeProgramValue == valueToFind) {
                foundNounAndVerb = true;
                noun = i;
                verb = j;
                break;
            }
        }
        if(foundNounAndVerb) {
            break;
        }
    }
    if(foundNounAndVerb) {
        std::cout << "The noun is: " << noun << "\n" << "The verb is: " << verb << std::endl;
    } else {
        // Increase iterations
        return -1;
    }
    
    return 100 * noun + verb;
}

void printSolution(std::vector<int> &opcodes, int iterationIncrement) {
    int finalValue = -1;
    int iterationCount = 0;
    while(finalValue == -1) {
        finalValue = bruteForceTheShitOutOfTheAnswer(opcodes, 19690720, iterationIncrement * iterationCount + iterationIncrement);
        iterationCount += 1;
    }
    std::cout << "The final answer is: " << finalValue << std::endl;
    std::cout << "It took " << iterationCount << " iteration(s) using " << iterationCount << "!" << std::endl;
    if(iterationCount > 1) {
        std::cout << "You should the increase iteration increment to " << iterationIncrement * iterationCount + iterationIncrement << " for faster results" << std::endl;
    }
}

int main() {
    // increase / decrease this value if you cannot find the value fast enough
    int iterationIncrement = 91;

    auto opcodes = readComputerProgramOpcodes("Dec2/input");
    printSolution(opcodes, iterationIncrement);
    
    return 0;
}
