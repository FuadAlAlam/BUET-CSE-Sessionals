#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <cassert>

using namespace std;

struct SAParams {
    double initialTemperature = 1000.0;
    double coolingRate = 0.995;
    double minTemperature = 0.001;
    int iterationsPerTemp = 100;
    int maxIterations = 100000;
    unsigned int randomSeed = 0;
    bool useGreedyInit = false;
};

struct SAResult {
    vector<int> initialTour;
    double initialCost = 0.0;
    vector<int> bestTour;
    double bestCost = 0.0;
    int totalIterations = 0;
    int acceptedMoves = 0;
    int worseMovesAccepted = 0;
    double executionTime = 0.0;
    double initialTemperature = 1000.0;
    double coolingRate = 0.995;
};

struct GreedyResult {
    vector<int> tour;
    double cost = 0.0;
    double executionTime = 0.0;
};

bool validateTour(const vector<int>& tour, int n) {
    if ((int)tour.size() != n || tour.empty() || tour[0] != 0) {
        return false;
    }
    vector<bool> seen(n, false);
    for (int city : tour) {
        if (city < 0 || city >= n || seen[city]) {
            return false;
        }
        seen[city] = true;
    }
    for (int i = 0; i < n; ++i) {
        if (!seen[i]) return false;
    }
    return true;
}

void readInput(istream& in, int& n, vector<vector<double>>& costMatrix) {
    if (!(in >> n)) return;
    costMatrix.assign(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            in >> costMatrix[i][j];
        }
    }
}

double calculateTourCost(const vector<int>& tour, const vector<vector<double>>& costMatrix) {
    int n = tour.size();
    if (n <= 1) return 0.0;
    double total = 0.0;
    for (int i = 0; i < n - 1; ++i) {
        total += costMatrix[tour[i]][tour[i + 1]];
    }
    total += costMatrix[tour[n - 1]][tour[0]];
    return total;
}

GreedyResult greedyTSP(int n, const vector<vector<double>>& costMatrix) {
    auto t1 = chrono::high_resolution_clock::now();

    vector<int> tour;
    tour.reserve(n);
    vector<bool> visited(n, false);

    int curr = 0;
    tour.push_back(curr);
    visited[curr] = true;

    for (int i = 1; i < n; ++i) {
        int nextCity = -1;
        double bestDist = 1e18;

        for (int candidate = 0; candidate < n; ++candidate) {
            if (!visited[candidate] && costMatrix[curr][candidate] < bestDist) {
                bestDist = costMatrix[curr][candidate];
                nextCity = candidate;
            }
        }

        tour.push_back(nextCity);
        visited[nextCity] = true;
        curr = nextCity;
    }

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = t2 - t1;

    GreedyResult res;
    res.tour = tour;
    res.cost = calculateTourCost(tour, costMatrix);
    res.executionTime = elapsed.count();
    return res;
}

vector<int> generateRandomTour(int n, mt19937& rng) {
    vector<int> tour(n);
    tour[0] = 0;
    for (int i = 1; i < n; ++i) {
        tour[i] = i;
    }
    if (n > 2) {
        shuffle(tour.begin() + 1, tour.end(), rng);
    }
    return tour;
}

vector<int> generateNeighbor(const vector<int>& tour, mt19937& rng) {
    int n = tour.size();
    if (n <= 2) return tour;

    vector<int> nextTour = tour;
    uniform_int_distribution<int> dist(1, n - 1);
    int i = dist(rng);
    int j = dist(rng);
    while (i == j) {
        j = dist(rng);
    }
    if (i > j) {
        swap(i, j);
    }

    reverse(nextTour.begin() + i, nextTour.begin() + j + 1);
    return nextTour;
}

SAResult simulatedAnnealing(int n, const vector<vector<double>>& costMatrix, const SAParams& params) {
    auto t1 = chrono::high_resolution_clock::now();

    unsigned int seed = params.randomSeed;
    if (seed == 0) {
        seed = random_device{}();
    }
    mt19937 rng(seed);
    uniform_real_distribution<double> realDist(0.0, 1.0);

    SAResult res;
    res.initialTemperature = params.initialTemperature;
    res.coolingRate = params.coolingRate;

    if (params.useGreedyInit) {
        GreedyResult g = greedyTSP(n, costMatrix);
        res.initialTour = g.tour;
    } else {
        res.initialTour = generateRandomTour(n, rng);
    }
    res.initialCost = calculateTourCost(res.initialTour, costMatrix);

    vector<int> currTour = res.initialTour;
    double currCost = res.initialCost;

    res.bestTour = currTour;
    res.bestCost = currCost;

    double temp = params.initialTemperature;
    int iters = 0;
    int accepted = 0;
    int worseAccepted = 0;

    while (temp > params.minTemperature && iters < params.maxIterations) {
        for (int k = 0; k < params.iterationsPerTemp && iters < params.maxIterations; ++k) {
            iters++;

            vector<int> candTour = generateNeighbor(currTour, rng);
            double candCost = calculateTourCost(candTour, costMatrix);
            double delta = candCost - currCost;

            if (delta <= 0) {
                currTour = candTour;
                currCost = candCost;
                accepted++;

                if (currCost < res.bestCost) {
                    res.bestCost = currCost;
                    res.bestTour = currTour;
                }
            } else {
                double prob = exp(-delta / temp);
                if (realDist(rng) < prob) {
                    currTour = candTour;
                    currCost = candCost;
                    accepted++;
                    worseAccepted++;
                }
            }
        }
        temp *= params.coolingRate;
    }

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = t2 - t1;

    res.totalIterations = iters;
    res.acceptedMoves = accepted;
    res.worseMovesAccepted = worseAccepted;
    res.executionTime = elapsed.count();

    return res;
}

void printTour(const vector<int>& tour) {
    for (size_t i = 0; i < tour.size(); ++i) {
        cout << tour[i] << " -> ";
    }
    if (!tour.empty()) {
        cout << tour[0] << "\n";
    }
}

void printCost(double val) {
    if (floor(val) == val) {
        cout << (long long)val;
    } else {
        cout << fixed << setprecision(2) << val;
    }
}

void printResults(int n, const GreedyResult& greedyRes, const SAResult& saRes) {
    cout << "========================================\n";
    cout << "TRAVELLING SALESMAN PROBLEM\n";
    cout << "========================================\n\n";
    cout << "Number of Cities: " << n << "\n\n";

    cout << "---------- GREEDY METHOD ----------\n";
    cout << "Tour:\n";
    printTour(greedyRes.tour);
    cout << "\nTotal Cost: ";
    printCost(greedyRes.cost);
    cout << "\nExecution Time: " << fixed << setprecision(4) << greedyRes.executionTime << " seconds\n\n";

    cout << "---------- SIMULATED ANNEALING ----------\n";
    cout << "Initial Solution:\n";
    printTour(saRes.initialTour);
    cout << "\nInitial Cost: ";
    printCost(saRes.initialCost);
    cout << "\n\nBest Tour Found:\n";
    printTour(saRes.bestTour);
    cout << "\nBest Cost: ";
    printCost(saRes.bestCost);
    cout << "\n\nInitial Temperature: ";
    printCost(saRes.initialTemperature);
    cout << defaultfloat << "\nCooling Rate: " << saRes.coolingRate;
    cout << "\nTotal Iterations: " << saRes.totalIterations;
    cout << "\nAccepted Moves: " << saRes.acceptedMoves;
    cout << "\nWorse Moves Accepted: " << saRes.worseMovesAccepted;
    cout << "\nExecution Time: " << fixed << setprecision(4) << saRes.executionTime << " seconds\n\n";

    cout << "---------- COMPARISON ----------\n";
    cout << "Greedy Cost: ";
    printCost(greedyRes.cost);
    cout << "\nSimulated Annealing Cost: ";
    printCost(saRes.bestCost);
    cout << "\n\nBest Method for This Run:\n";
    if (abs(greedyRes.cost - saRes.bestCost) < 1e-6) {
        cout << "Both methods produced the same cost.\n";
    } else if (saRes.bestCost < greedyRes.cost) {
        cout << "Simulated Annealing found a better tour.\n";
    } else {
        cout << "Greedy found a better tour.\n";
    }
    cout << "========================================\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n = 0;
    vector<vector<double>> costMatrix;
    readInput(cin, n, costMatrix);

    if (n < 2) {
        return 0;
    }

    GreedyResult greedyRes = greedyTSP(n, costMatrix);
    assert(validateTour(greedyRes.tour, n));

    SAParams params;
    params.initialTemperature = 1000.0;
    params.coolingRate = 0.995;
    params.minTemperature = 0.001;
    params.iterationsPerTemp = 100;
    params.maxIterations = 100000;
    params.randomSeed = 0;
    params.useGreedyInit = false;

    SAResult saRes = simulatedAnnealing(n, costMatrix, params);
    assert(validateTour(saRes.bestTour, n));

    printResults(n, greedyRes, saRes);

    return 0;
}
