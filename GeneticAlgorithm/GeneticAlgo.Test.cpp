#include <iostream>
#include <string>
#include "GeneticAlgo.hpp"  // Include the above code in this file

const std::string TARGET = "Hello, World!";

std::vector<char> generateIndividual() {
    std::vector<char> individual;
    for (size_t i = 0; i < TARGET.length(); ++i) {
        individual.push_back(rand() % 128);
    }
    return individual;
}

double fitnessFunction(const std::vector<char>& individual) {
    double fitness = 0;
    for (size_t i = 0; i < TARGET.length(); ++i) {
        fitness += (individual[i] == TARGET[i]) ? 1 : 0;
    }
    return fitness;
}

std::vector<char> crossover(const std::vector<char>& parent1, const std::vector<char>& parent2) {
    std::vector<char> child = parent1;
    size_t split = rand() % TARGET.length();
    for (size_t i = split; i < TARGET.length(); ++i) {
        child[i] = parent2[i];
    }
    return child;
}

void mutate(std::vector<char>& individual) {
    size_t index = rand() % TARGET.length();
    individual[index] = rand() % 128;
}

int main() {
    GeneticAlgorithm<char> ga(generateIndividual, fitnessFunction, crossover, mutate);
    
    std::vector<char> solution = ga.evolve(1000);
    
    std::cout << "Best solution: ";
    for (char c : solution) {
        std::cout << c;
    }
    std::cout << std::endl;
    
    return 0;
}