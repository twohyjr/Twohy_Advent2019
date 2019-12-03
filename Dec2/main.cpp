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
                // std::cout << "Program Terminated" << std::endl;
                break;
            default:
                programIsTerminated = true;
                // std::cout << "Something horrible has happened" << std::endl;
                break;
        }
        
        // set value at position to new value
        opcodes[positionToUpdate] = newVal;
        
        currentIndex += 4;
    }
    
    return opcodes[0];
}

int main() {
    auto opcodes = readComputerProgramOpcodes("Dec2/input");
    
    bool foundNounAndVerb = false;
    int noun = 0, verb = 0;
    for(int i = 0; i < 10000; i++) {
        for(int j = 0; j < 10000; j++) {
            int intCodeProgramValue = intCodeProgramFirstValue(opcodes, i, j);
            if(intCodeProgramValue == 19690720) {
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
    std::cout << "NOUN: " << noun << " -- VERB: " << verb << std::endl;
    std::cout << "The final answer is: " << 100 * noun + verb << std::endl;
    
    
}

// Opcodes - read positioins
//       1    -   Adds together numbers read from two positions and stores
//                the result in a third position
//
//       2    -   works exactly like opcode 1, except it multiplies the
//                two inputs instead of adding them
//
//      99    -  program is finished and should immediately halt.
//
// Unknown    -  something went wrong
