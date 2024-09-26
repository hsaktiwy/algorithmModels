int CompuationIteration = 100000;

float fitnessFunction(Object Obj)
{
    float power = 0;
    // this is our function that identify how powerfull our solution
    // ...
    return (power);
}

void simulationAnnealMadel( Object obj )
{
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_real_distribution<> distr(0.0, 1.0); // Define the range
    vector<_point> bestSolution = obj;
    vector<_point> currentSolution = obj;
    float coolDownRate = 0.99;
    float best_fitness = fitnessFunction(currentSolution);
    float curr_fitness = best_fitness;

    int calculation = CompuationIteration;
    float temperatore = CompuationIteration *  0.1;
 
    for(int x = calculation; x > 0; x--)
    {
        // randomization logic 
        // ...
       
        // function that change our result swap, suffle , inverse , ...
        // .. 
        
        // Calculate our soluation fitness after it n
        float fitness2 = fitnessFunction(currentSolution);

        // identify if we are going with the worst solution or we will drop it or tack it in case it is better then the previous one
        if (fitness2 > curr_fitness)
        {
            double acceptanceProb = exp((curr_fitness - fitness2) / temperatore);
            if (acceptanceProb > distr(eng)) {
                curr_fitness = fitness2;
            }
            else
            {
                // reverse our changes as we will keep the previous solution swap, suffle , inverse , ...
                // .. 
            }
        }
        else
        {
            if (fitness2 < best_fitness)
            {
                bestSolution = currentSolution, best_fitness = fitness2;
            }
            curr_fitness = fitness2;
        }

        // drop our temperatore to drop our probability of accepting worst solution if they appear
        temperatore *= coolDownRate;
    }
    obj = bestSolution;
}