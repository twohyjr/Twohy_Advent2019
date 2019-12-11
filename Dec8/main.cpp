#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

std::vector<int> readPixtureData(std::string filepath) {
    std::vector<int> pitureData;
    std::ifstream inputFile;
    std::string line;
    try {
        inputFile.open(filepath);
        while(getline(inputFile,line)) {
            for(int i=0;i<line.length();i++){
                pitureData.push_back(line[i] - '0');
            }
        }
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::INPUT_FILE::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }
    
    return pitureData;
}

struct Layer {
    int layerNumber = 0;
    int zeros = 0;
    int ones = 0;
    int twos = 0;
};

void runPart1(std::vector<int> pixelData) {
    std::vector<Layer> layers;
    
    int width = 25;
    int height = 6;
    int totalLayerValues = width * height;
    int layerCount = (int)pixelData.size() / (totalLayerValues);
    for(int i = 0; i < layerCount; i++) {
        Layer layer;
        layer.layerNumber = i + 1;
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                int value = pixelData[(i * totalLayerValues) + (y * width + x)];
                switch(value) {
                    case 0:
                        layer.zeros += 1;
                        break;
                    case 1:
                        layer.ones += 1;
                        break;
                    case 2:
                        layer.twos += 1;
                        break;
                }
            }
        }
        layers.push_back(layer);
    }
    
    int result = 0;
    Layer fewestZeroLayer = {
        INT_MAX,
        INT_MAX,
        INT_MAX,
        INT_MAX
    };
    for(auto layer : layers) {
        if(layer.zeros < fewestZeroLayer.zeros) {
            fewestZeroLayer = layer;
            result = layer.ones * layer.twos;
        }
    }
    
    std::cout << result << std::endl;
}

void runPart2(std::vector<int> pixelData) {
    int width = 25;
    int height = 6;
    int totalLayerValues = width * height;
    int layerCount = (int)pixelData.size() / (totalLayerValues);
    
    // Fill the pic with transparent values
    std::vector<int> currentPicture;
    for(int i = 0; i < totalLayerValues; i++) {
        currentPicture.push_back(2);
    }
    
    for(int i = 0; i < layerCount; i++) {
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                int currentLayerIndex = y * width + x;
                
                int currentLayerValue = currentPicture[currentLayerIndex];
                int newLayerValue = pixelData[(i * totalLayerValues) + (y * width + x)];
                
                // new layer black --- current layer transparent
                if(newLayerValue == 0 && currentLayerValue == 2) {
                    currentPicture[currentLayerIndex] = 0;
                }
                
                // new layer white
                if(newLayerValue == 1 && currentLayerValue == 2) {
                    currentPicture[currentLayerIndex] = 1;
                }
            }
        }
    }
    
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int index = y * width + x;
            int value = currentPicture[index];
            if(value == 0) {
                std::cout << " ";
            }else{
                std::cout << value;
            }
        }
        std::cout << std::endl;
    }
}


int main(int argc, const char * argv[]) {
    auto pixelData = readPixtureData("Dec8_input");
    
    runPart1(pixelData);
    runPart2(pixelData);

    return 0;
}
