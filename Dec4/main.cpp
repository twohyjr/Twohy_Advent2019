#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

struct Range {
    int start;
    int end;
};

Range getPasswordRange(std::string filepath) {
    Range r;
    std::ifstream inputFile;
    std::string line;
    try {
        inputFile.open(filepath);
        while(getline(inputFile,line)) {
            std::stringstream  linestream(line);
            std::string rangeString;
            
            int ranges[2];
            int index = 0;
            while(getline(linestream, rangeString, '-')) {
                // convert string to int
                std::stringstream rangeStreamString(rangeString);
                int range;
                rangeStreamString >> range;

                if(index == 0) {
                    ranges[0] = range;
                } else if (index == 1) {
                    ranges[1] = range;
                }
                index += 1;
            }
            r.start = ranges[0];
            r.end = ranges[1];
        }
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::INPUT_FILE::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }
    
    return r;
}

bool passwordIsSixDigits(std::string passwordString) {
    return passwordString.size() == 6;
}

bool passwordContainsAdjacentDigitsPart1(std::string passwordString) {
    // Check for 00
    if(passwordString.find("00") != std::string::npos) {
        return true;
    }
    
    bool containsDoubleDigit = false;
    int currentCheckDigit = 1;
    while(!containsDoubleDigit && currentCheckDigit <= 9) {
        // convert int to double digit
        std::string digits = std::to_string(currentCheckDigit * 11);
        containsDoubleDigit = (passwordString.find(digits) != std::string::npos);
        currentCheckDigit += 1;
    }

    return containsDoubleDigit;
}

bool passwordContainsAdjacentDigitsPart2(std::string passwordString) {
        // Check for 00
    if(passwordString.find("00") != std::string::npos &&
       passwordString.find("000") != std::string::npos) {
        return true;
    }

    bool containsDoubleDigit = false;
    int currentCheckDigit = 1;
    while(!containsDoubleDigit && currentCheckDigit <= 9) {
        // convert int to double digit
        std::string digitsTens = std::to_string(currentCheckDigit * 11);
        std::string digitsHundreds = std::to_string(currentCheckDigit * 111);
        
        containsDoubleDigit = (passwordString.find(digitsTens) != std::string::npos &&
                               passwordString.find(digitsHundreds) == std::string::npos);
        
        currentCheckDigit += 1;
    }

    return containsDoubleDigit;
}

bool passwordIsAlwaysIncreasing(std::string passwordString) {
    bool isIncreasing = true;
    int lastNumber = 0;
    for(int i = 0; i < passwordString.size(); i++) {
        auto character = passwordString[i];
        std::string stringCharacter(1, character);
        std::stringstream stringCharacterStream(stringCharacter);
        int characterNumber;
        stringCharacterStream >> characterNumber;
        
        if(characterNumber < lastNumber) {
            isIncreasing = false;
            break;
        }
        
        lastNumber = characterNumber;
    }
    return isIncreasing;
}

bool validatePasswordIsLegitPart1(std::string passwordString) {
    bool isSixDigits = passwordIsSixDigits(passwordString);
    bool containsDoubleDigit = passwordContainsAdjacentDigitsPart1(passwordString);
    bool isAlwaysIncreasing = passwordIsAlwaysIncreasing(passwordString);
    
    return isSixDigits && containsDoubleDigit && isAlwaysIncreasing;
}

bool validatePasswordIsLegitPart2(std::string passwordString) {
    bool isSixDigits = passwordIsSixDigits(passwordString);
    bool containsDoubleDigit = passwordContainsAdjacentDigitsPart2(passwordString);
    bool isAlwaysIncreasing = passwordIsAlwaysIncreasing(passwordString);
    
    return isSixDigits && containsDoubleDigit && isAlwaysIncreasing;
}

void runPart1Tests() {
    // Tests
    std::cout << "False: " << validatePasswordIsLegitPart1("1111111") << std::endl; // Too many digits
    std::cout << "False: " << validatePasswordIsLegitPart1("11111") << std::endl; // Too few digits
    std::cout << "False: " << validatePasswordIsLegitPart1("133145") << std::endl; // Decreases
    std::cout << "False: " << validatePasswordIsLegitPart1("001122") << std::endl; // No same digits

    std::cout << std::endl;

    std::cout << "True: " << validatePasswordIsLegitPart1("111111") << std::endl;
    std::cout << "True: " << validatePasswordIsLegitPart1("123446") << std::endl;

    std::cout << std::endl;
}

void runPart2Tests() {
    // Tests
    std::cout << "False: " << validatePasswordIsLegitPart2("123444") << std::endl; // Too many digits
    std::cout << "False: " << validatePasswordIsLegitPart2("123456") << std::endl; // Too few digits
    std::cout << std::endl;

    std::cout << "True: " << validatePasswordIsLegitPart2("112233") << std::endl;
    std::cout << "True: " << validatePasswordIsLegitPart2("111122") << std::endl;

    std::cout << std::endl;
}

int main(int argc, const char * argv[]) {
    Range passwordRange = getPasswordRange("Dec4_input");

    // Part 1
    std::vector<std::string> validPasswordsPart1;
    for(int currentPassword = passwordRange.start; currentPassword <= passwordRange.end; currentPassword++) {
        std::string passwordString = std::to_string(currentPassword);
        if(validatePasswordIsLegitPart1(passwordString)) {
            validPasswordsPart1.push_back(passwordString);
        }
    }
    // Answer -- 1246
    std::cout << "Part 1: The number of valid passwords that exist is: " << validPasswordsPart1.size() << std::endl;
//    runPart1Tests();
    
    // Part 1
    std::vector<std::string> validPasswordsPart2;
    for(int currentPassword = passwordRange.start; currentPassword <= passwordRange.end; currentPassword++) {
        std::string passwordString = std::to_string(currentPassword);
        if(validatePasswordIsLegitPart2(passwordString)) {
            validPasswordsPart2.push_back(passwordString);
        }
    }
    // Answer -- 1246
    std::cout << "Part 1: The number of valid passwords that exist is: " << validPasswordsPart2.size() << std::endl;
//    runPart2Tests();
    
    return 0;
}


