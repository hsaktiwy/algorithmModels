#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <ctime>
#include <iostream>
#include <set>
#include <unordered_set>
using namespace std;

template <typename Gene>
class GeneticAlgorithm {
private:
    using Individual = vector<Gene>;
    using Population = vector<Individual>;

    function<Individual()> generateIndividual;
    function<double(const Individual&)> fitnessFunction;
    function<Individual(const Individual&, const Individual&)> crossoverFunction;
    function<void(Individual&)> mutationFunction;

    int populationSize;
    double mutationRate;
    int eliteSize;

    mt19937 rng;

public:
    GeneticAlgorithm(
        function<Individual()> genIndividual,
        function<double(const Individual&)> fitFunc,
        function<Individual(const Individual&, const Individual&)> crossFunc,
        function<void(Individual&)> mutFunc,
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
        rng(time(0))
    {}

    Individual evolve(int generations) {
        Population population = initializePopulation();

        for (int gen = 0; gen < generations; ++gen) {
            vector<pair<double, Individual> > rankedPopulation = rankPopulation(population);
            Population newPopulation = selectElites(rankedPopulation);

            while (newPopulation.size() < populationSize) {
                Individual parent1 = selectParent(rankedPopulation);
                Individual parent2 = selectParent(rankedPopulation);
                Individual child = crossoverFunction(parent1, parent2);
                if (uniform_real_distribution<>(0, 1)(rng) < mutationRate) {
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

    vector<pair<double, Individual> > rankPopulation(const Population& population) {
        vector<pair<double, Individual> > ranked;
        for (const auto& individual : population) {
            ranked.emplace_back(fitnessFunction(individual), individual);
        }
        sort(ranked.begin(), ranked.end(), [](const pair<double, Individual>& a, const pair<double, Individual>& b) {
            return a.first > b.first; // For descending order
        });
        return ranked;
    }

    Population selectElites(const vector<pair<double, Individual> >& ranked) {
        Population elites;
        for (int i = 0; i < eliteSize && i < ranked.size(); ++i) {
            elites.push_back(ranked[i].second);
        }
        return elites;
    }

    Individual selectParent(const vector<pair<double, Individual> >& ranked) {
        double totalFitness = 0;
        for (const auto& [fitness, _] : ranked) {
            totalFitness += fitness;
        }

        double pick = uniform_real_distribution<>(0, totalFitness)(rng);
        double current = 0;
        for (const auto& [fitness, individual] : ranked) {
            current += fitness;
            if (current > pick) {
                return individual;
            }
        }
        return ranked.back().second;
    }

    double CompareFitness(const Individual& a, const Individual& b)
    {
        return fitnessFunction(a) > fitnessFunction(b); // in this case we will try to tack the shortest path
    }

    Individual getBestIndividual(const Population& population) {
        return *max_element(population.begin(), population.end(),
                [this](const Individual& a, const Individual& b) {
                return fitnessFunction(a) > fitnessFunction(b);
            }
            );
    }
};

typedef struct s_point{
    int x;
    int y;
    int index;
}t_point;


vector<t_point> mygenome;
// function  that will generate a random member
vector<t_point> create_memeber()
{
    int vis[10000] = {0};
    int counter = 0;
    vector<t_point> res;
    res.push_back(mygenome[0]);
    vis[0] = 1;
    counter++;
    while (counter < mygenome.size())
    {
        int ran = rand() % mygenome.size();
        //cout << ran << " \n";
        if (!vis[ran])
        {
            res.push_back(mygenome[ran]);
            vis[ran] = 1;
            counter++;
        }
    }
    return res;
}
// fitness function we will try to calculate the totale travaled distance 
double distance(int x, int y, int x1, int y1)
{
    return (sqrt(pow(x - x1, 2) + pow(y - y1, 2)));
}

double fitnessFunction(const vector<t_point>& member)
{
    double res  =0;
    for (int i = 0; i < member.size(); i++)
    {
        if (i + 1 < member.size())
            res += distance(member[i].x, member[i].y, member[i + 1].x, member[i + 1].y);
        if (i == member.size() - 1)
            res += distance(member[0].x, member[0].y, member[i].x, member[i].y);
    }
    return (res);
}
// crossover will tack 2 arrent and return a shield
vector<t_point> crossover(const vector<t_point>& par1, const vector<t_point>& par2) {
    vector<t_point> child;
    unordered_set<int> unique_indices;

    child.push_back(par1[0]);
    unique_indices.insert(0);
    // Add points from the first parent
    while (child.size() < par1.size())
    {
        int r1 = rand()% par1.size();
        int r2 = rand() %2;
       // cout << r1 << " " << ", parents index " << r2 <<endl;
        t_point p = (r2 == 1) ? par2[r1] : par1[r1];
        if (unique_indices.find(p.index) == unique_indices.end())
        {
            child.push_back(p);
            unique_indices.insert(p.index);
        }
    }
    return child;
}
// function that will mutate
void mutation(vector<t_point>& member)
{
    int i = rand() % member.size();
    int i2 = rand() % member.size();

    while (i == i2)
    {
        i = rand() % member.size();
    }
    i = (i == 0 )? 1: i;
    i2 = (i2 == 0) ? 1: i2;
    t_point p;
    p = member[i];
    member[i] = member[i2];
    member[i2] = p;
}
void display(vector<t_point> &res)
{
    int stop = res.size() - 1;
    for (int i = 0; i < res.size(); i++)
    {
        cout << res[i].index << ((i != stop) ?  " " : "");
    }
    cout << endl;
}

int main()
{
    int n; // This variables stores how many nodes are given
    srand(time(NULL)%1000000);
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        t_point p;
        cin >> p.x >> p.y; cin.ignore();
        p.index = i;
        mygenome.push_back(p);
    }

    GeneticAlgorithm<t_point> ga(create_memeber, fitnessFunction, crossover, mutation);
    vector<t_point> solution = ga.evolve(50);
    display(solution);
}