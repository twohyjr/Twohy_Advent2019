#include <iostream>
#include <string>
#include <vector>

void permute(std::string a, int l, unsigned long r, std::vector<std::vector<int>> &allPermutations) {
    // Base case
    if (l == r) {
        std::vector<int> permutation;
        for(int i=0;i<a.length();i++){
            permutation.push_back(a[i] - '0');
        }
        allPermutations.push_back(permutation);
    } else {
        // Permutations made
        for (unsigned long i = l; i <= r; i++) {
            // Swapping done
            std::swap(a[l], a[i]);
  
            // Recursion called
            permute(a, l+1, r, allPermutations);
  
            //backtrack
            std::swap(a[l], a[i]);
        }
    }
}

std::vector<std::vector<int>> getAllPermutations(std::string str) {
    std::vector<std::vector<int>> allPermutations;
    
    unsigned long n = str.size();
    permute(str, 0, n-1, allPermutations);
    
    return allPermutations;
}
