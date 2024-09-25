#include <vector>
#include <unordered_map>
#include <iostream>

typedef struct s_point
{
    int x,y,index;
}_point;

typedef struct s_grid{
    vector<t_depo> points;
}_grid;

class SpetialHashGrid{
    private:
        int width;
        int height;
        int cellWidth;
        int cellHeight;
        int maxCellWidthNum = 0;
        int maxCellHeightNum = 0;
        int contents;
        unordered_map<int, _grid> map;
    public:
        SpetialHashGrid(int x, int y, int _cellWidth, int _cellHeight): width(x), height(y), cellWidth(_cellWidth), cellHeight(_cellHeight)
        {
            maxCellWidthNum = width/cellWidth;
            maxCellHeightNum = height/cellHeight;
        }

        int hash_x(int x)
        {
            return x / cellWidth;
        }

        int hash_y(int y)
        {
            return y / cellHeight;
        }

        int hash(int x, int y)
        {
            return x * 31 + y;
        }

        void _add(t_depo &point)
        {
            int key = hash(hash_x(point.x), hash_y(point.y));
            map[key].points.push_back(point);
        }

        vector<t_depo>& getCell(t_depo& point)
        {
            int key = hash(hash_x(point.x), hash_y(point.y));
            return (map[key].points);
        }

        vector<t_depo>& getNearByCell(t_depo& base, int x, int y)
        {
            int keyx = hash_x(base.x) + x;
            if (keyx < 0 || keyx >= maxCellWidthNum)
                throw "Boundary Surpace\n";
            int keyy = hash_y(base.y) + y;
            if (keyy < 0 || keyy >= maxCellHeightNum)
                throw "Boundary Surpace\n";
            int key = hash(keyx, keyy);
            return (map[key].points);
        }

        double calculateDistance(vector<t_depo>& toadd)
        {
            double result = 0;
            for (int i = 0; i < toadd.size(); i++)
            {
                if (i + 1 < toadd.size())
                    result += distance(toadd[i].x, toadd[i].y,toadd[i + 1].x, toadd[i + 1].y); 
            }
            return result;
        }

        // this could be used insteed of the swap
        void partialSuffling(vector<t_depo> &vec, int begin, int end)
        {
            random_device rd;
            mt19937 g(rd());

            // Perform partial shuffle
            shuffle(vec.begin() + begin, vec.begin() + end, g);
        }

        // + simulation anneal model to optimize more the code
        void Mini_simulationAnnealing(vector<t_depo>& toadd)
        {         
            random_device rd;  // Obtain a random number from hardware
            mt19937 eng(rd()); // Seed the generator
            uniform_real_distribution<> distr(0.0, 1.0); // Define the range
            double best_result = calculateDistance(toadd);
            vector<t_depo> best_vec = toadd;
            vector<t_depo> curr_vec = toadd;
            double curr_result = best_result;
            float coolDown = 0.99;
            // this line can be odife dependin g on the code and time limitation
            int steps = toadd.size() * 10;
            // applying temperatore in the normal case this must be positive value 100 if the step are 1000 usely we tack 10% or even less, test and try what could help
            float temperator = toadd.size() * 1;
            float initialTemp = temperator;
            for (int i = 0; i < steps; i++)
            {
                int r1 = rand()%curr_vec.size();
                int r2 = rand()%curr_vec.size();
                swap(curr_vec[r1], curr_vec[r2]);
                float fitness = calculateDistance(curr_vec);
                if (fitness > curr_result)
                {
                    double acceptanceProb = exp((curr_result - fitness) / temperator);
                    if (acceptanceProb > distr(eng))
                    {
                        curr_result = fitness;
                    }
                    else
                    {
                        swap(curr_vec[r1], curr_vec[r2]);
                    }
                }
                else
                {
                    if (fitness < best_result)
                        best_vec = curr_vec, best_result = fitness;
                    curr_result = fitness;
                }
                temperator *= coolDown;
            }
            toadd = best_vec;
        }

        void optimizeGrid(vector<t_depo>& toadd)
        {
            Mini_simulationAnnealing(toadd);
        }

        void Optimize( void )
        {
            for (int i = 0; i < maxCellWidthNum; i++)
            {
                for (int j = 0; j < maxCellHeightNum; j++)
                {
                    int key = hash(i, j);
                    cerr << "before <" << i << "," << j << "> " << calculateDistance(map[key].points);
                    optimizeGrid(map[key].points);
                    cerr << " after : " << calculateDistance(map[key].points) << '\n';
                }
            }
        }
};