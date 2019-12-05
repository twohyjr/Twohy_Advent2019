#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

enum InputModes {
    POSITION, // read by index
    IMMEDIATE // read by value
};

struct OpcodeProgram {
    int opcode;
    InputModes param1InputMode;
    InputModes param2InputMode;
    InputModes param3InputMode;
};

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

void processOpcode1(std::vector<int> &opcodes,
                    OpcodeProgram program,
                    int param1,
                    int param2,
                    int param3) {
    int value1 = program.param1InputMode == InputModes::POSITION ? opcodes[param1] : param1;
    int value2 = program.param2InputMode == InputModes::POSITION ? opcodes[param2] : param2;
    opcodes[param3] = value1 + value2;
}

void processOpcode2(std::vector<int> &opcodes,
                    OpcodeProgram program,
                    int param1,
                    int param2,
                    int param3) {
    int value1 = program.param1InputMode == InputModes::POSITION ? opcodes[param1] : param1;
    int value2 = program.param2InputMode == InputModes::POSITION ? opcodes[param2] : param2;
    opcodes[param3] = value1 * value2;
}

// Write value to index
void processOpcode3(std::vector<int> &opcodes,
                    int value,
                    int writeIndex) {
    opcodes[writeIndex] = value;
}

// Returns output at address
int processOpcode4(std::vector<int> const &opcodes,
                   int address) {
    return opcodes[address];
}

void padWithZeros(std::string &value, int desiredLength) {
    while(value.size() < desiredLength) {
        value.insert(0, "0"); // add zero to beginning
    }
}

OpcodeProgram getOpcodeProgram(int programInstruction) {
    // Convert program instruction to string then pad with zeros
    std::string programInstructionString = std::to_string(programInstruction);
    padWithZeros(programInstructionString, 5);
    
    // Get opcode from leftmost 2 digits
    std::string opcodeString = programInstructionString.substr(3, 5);
    int opcode;
    std::stringstream opcodeStringStream(opcodeString);
    opcodeStringStream >> opcode;

    // Get input mode of first parameter
    char inputMode1Char = programInstructionString[2];
    InputModes inputMode1 = inputMode1Char == '0' ? InputModes::POSITION : InputModes::IMMEDIATE;
    
    // Get input mode of second parameter
    char inputMode2Char = programInstructionString[1];
    InputModes inputMode2 = inputMode2Char == '0' ? InputModes::POSITION : InputModes::IMMEDIATE;
    
    // Get input mode of third parameter
    char inputMode3Char = programInstructionString[0];
    InputModes inputMode3 = inputMode3Char == '0' ? InputModes::POSITION : InputModes::IMMEDIATE;

    return OpcodeProgram{
        opcode,
        inputMode1,
        inputMode2,
        inputMode3
    };
}

// Executes the opcodes program
int runProgram(std::vector<int> &opcodes) {
    std::vector<int> outputs;

    int currentIndex = 0;
    bool programIsTerminated = false;
    
    int defaultInput = 1;
    while(!programIsTerminated) {
        if(currentIndex == opcodes.size() - 1) {
            programIsTerminated = true;
        }
        
        OpcodeProgram program = getOpcodeProgram(opcodes[currentIndex]);
        int opcode = program.opcode;
        
        switch (opcode) {
            case 1:
                processOpcode1(opcodes,
                               program,
                               opcodes[currentIndex + 1],
                               opcodes[currentIndex + 2],
                               opcodes[currentIndex + 3]);
                currentIndex += 4;
                break;
            case 2:
                processOpcode2(opcodes,
                               program,
                               opcodes[currentIndex + 1],
                               opcodes[currentIndex + 2],
                               opcodes[currentIndex + 3]);
                currentIndex += 4;
                break;
            case 3:
                opcodes[opcodes[currentIndex + 1]] = defaultInput;
                currentIndex += 2;
                break;
            case 4:
                outputs.push_back(processOpcode4(opcodes, opcodes[currentIndex + 1]));
                currentIndex += 2;
                break;
            case 99:
                programIsTerminated = true;
                break;
            default:
                programIsTerminated = true;
                std::cout << "Something horrible has happened" << std::endl;
                break;
        }
    }
    
    for(int i = 1; i < outputs.size() - 1; i++) {
        if(outputs[i] != 0) {
            std::cout << "Output with the value of: " << outputs[i] << " is incorrect" << std::endl;
            return -1;
        }
        
    }
    
    return outputs[outputs.size() - 1];
}

int main(int argc, const char * argv[]) {
    auto opcodes = readComputerProgramOpcodes("Dec5_input");
    
    // Part 1
    int programOutput = runProgram(opcodes);
    // Answer -- 15386262
    std::cout << "The final program output is: " << programOutput << std::endl;

    return 0;
}
