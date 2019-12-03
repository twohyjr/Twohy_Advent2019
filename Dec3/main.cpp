#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

struct Direction {
    std::string direction;
    int steps;
};

struct Wire {
    std::vector<Direction> directions;
};

struct Point {
    int x;
    int y;
    
    Point operator +(Point const &p) {
        return {this->x + p.x, this->y + p.y};
    }
};

std::vector<Wire> getCircuitWires(std::string filepath) {
    std::vector<Wire> wires;
    std::ifstream inputFile;
    std::string line;
    try {
        inputFile.open(filepath);
        while(getline(inputFile,line)) {
            std::stringstream  linestream(line);
            std::string directionsString;
            
            std::vector<Direction> directions;
            while(getline(linestream, directionsString, ',')) {
                std::string direction = directionsString.substr(0, 1);
                std::string stepsString = directionsString.substr(1, 5);
                
                // convert string to int
                std::stringstream stepStreamString(stepsString);
                int steps;
                stepStreamString >> steps;
                
                directions.push_back({direction, steps});
            }
            wires.push_back({directions});
        }
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::INPUT_FILE::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }
    
    return wires;
}

// Origin = 0,0
std::map<std::string, bool> getPointsTheWireTouched(std::vector<Direction> wireDirections) {
    std::map<std::string, bool> points;
    Point lastP = {0,0};
    for(auto direction : wireDirections) {
        
        int stepsToTake = direction.steps;
        while (stepsToTake > 0) {
            Point thisP = lastP;
            if(direction.direction == "L") { thisP.x -= 1; }
            if(direction.direction == "R") { thisP.x += 1; }
            if(direction.direction == "D") { thisP.y -= 1; }
            if(direction.direction == "U") { thisP.y += 1; }
            
            std::string key = std::to_string(thisP.x) + " , " + std::to_string(thisP.y);
            points[key] = true;
            lastP = thisP;
            
            stepsToTake -= 1;
        }
    }
    return points;
}
    
std::vector<Point> getIntersectPoints(std::map<std::string, bool> pointsTouchedMap, std::vector<Direction> wireDirections) {
    std::vector<Point> points;
    Point lastP = {0,0};
    for(auto direction : wireDirections) {
        
        int stepsToTake = direction.steps;
        while (stepsToTake > 0) {
            Point thisP = lastP;
            if(direction.direction == "L") { thisP.x -= 1; }
            if(direction.direction == "R") { thisP.x += 1; }
            if(direction.direction == "D") { thisP.y -= 1; }
            if(direction.direction == "U") { thisP.y += 1; }
            
            std::string key = std::to_string(thisP.x) + " , " + std::to_string(thisP.y);
            if(pointsTouchedMap[key] == true) {
                points.push_back(thisP);
            }
            lastP = thisP;
            
            stepsToTake -= 1;
        }
    }
    return points;
}

int getManhattanDistance(Point p1, Point p2) {
    // |x1 - x2| + |y1 - y2|
    return (abs(p1.x - p2.x) + abs(p1.y - p2.y));
}
    
int getClosestManhattanDistance(std::vector<Point> crossedPoints) {
    int closest = INT_MAX;
    for(Point p : crossedPoints) {
        int mDist = getManhattanDistance(p, {0,0});
        if(mDist < closest) {
            closest = mDist;
        }
    }
    return closest;
}

int main(int argc, const char * argv[]) {
    auto wires = getCircuitWires("Dec3/input");
    
    auto wire1 = wires[0];
    auto wire2 = wires[1];
    
    auto pointsTouchedMap = getPointsTheWireTouched(wire1.directions);
    auto intersectionPoints = getIntersectPoints(pointsTouchedMap, wire2.directions);
    
    int closestPoint = INT_MAX;
    for(auto point : intersectionPoints) {
        int mDist = getManhattanDistance(point, {0,0});
        if(mDist < closestPoint) {
            closestPoint = mDist;
        }
    }
    
    std::cout << "The closest intersection of wires to the origin is: " << closestPoint << std::endl;
    
    // Answer -- the closest intersection of wires to the origin is: 221
    return 0;
}
