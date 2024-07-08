#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <ctime>

template <typename Gene>
class GeneticAlgorithm {
private:
    using Individual = std::vector<Gene>;
    using Population = std::vector<Individual>;

    std::function<Individual()> generateIndividual;
    std::function<double(const Individual&)> fitnessFunction;
    std::function<Individual(const Individual&, const Individual&)> crossoverFunction;
    std::function<void(Individual&)> mutationFunction;

    int populationSize;
    double mutationRate;
    int eliteSize;

    std::mt19937 rng;

public:
    GeneticAlgorithm(
        std::function<Individual()> genIndividual,
        std::function<double(const Individual&)> fitFunc,
        std::function<Individual(const Individual&, const Individual&)> crossFunc,
        std::function<void(Individual&)> mutFunc,
        int popSize = 100,
        double mutRate = 0.01,
        int eliteCount = 2
    ) : generateIndividual(genIndividual),
        fitnessFunction(fitFunc),
        crossoverFunction(crossFunc),
        mutationFunction(mutFunc),
        populationSize(popSize),
        mutationRate(mutRate),
        eliteSize(eliteCount),
        rng(std::time(0))
    {}

    Individual evolve(int generations) {
        Population population = initializePopulation();

        for (int gen = 0; gen < generations; ++gen) {
            std::vector<std::pair<double, Individual> > rankedPopulation = rankPopulation(population);
            Population newPopulation = selectElites(rankedPopulation);

            while (newPopulation.size() < populationSize) {
                Individual parent1 = selectParent(rankedPopulation);
                Individual parent2 = selectParent(rankedPopulation);
                Individual child = crossoverFunction(parent1, parent2);
                if (std::uniform_real_distribution<>(0, 1)(rng) < mutationRate) {
                    mutationFunction(child);
                }
                newPopulation.push_back(child);
            }

            population = newPopulation;
        }

        return getBestIndividual(population);
    }

private:
    Population initializePopulation() {
        Population population;
        for (int i = 0; i < populationSize; ++i) {
            population.push_back(generateIndividual());
        }
        return population;
    }

    std::vector<std::pair<double, Individual> > rankPopulation(const Population& population) {
        std::vector<std::pair<double, Individual> > ranked;
        for (const auto& individual : population) {
            ranked.emplace_back(fitnessFunction(individual), individual);
        }
        std::sort(ranked.begin(), ranked.end(), std::greater<>());
        return ranked;
    }

    Population selectElites(const std::vector<std::pair<double, Individual> >& ranked) {
        Population elites;
        for (int i = 0; i < eliteSize && i < ranked.size(); ++i) {
            elites.push_back(ranked[i].second);
        }
        return elites;
    }

    Individual selectParent(const std::vector<std::pair<double, Individual> >& ranked) {
        double totalFitness = 0;
        for (const auto& [fitness, _] : ranked) {
            totalFitness += fitness;
        }

        double pick = std::uniform_real_distribution<>(0, totalFitness)(rng);
        double current = 0;
        for (const auto& [fitness, individual] : ranked) {
            current += fitness;
            if (current > pick) {
                return individual;
            }
        }
        return ranked.back().second;
    }

    Individual getBestIndividual(const Population& population) {
        return *std::max_element(population.begin(), population.end(),
            [this](const Individual& a, const Individual& b) {
                return fitnessFunction(a) < fitnessFunction(b);
            });
    }
};