#include <bits/stdc++.h>
using namespace std;

int main() {
    freopen("2.txt", "w", stdout);
    srand(time(0));

    int k = 500; // Earning per hour
    int n = 100; // Number of houses
    int x = 600; // Number of edges

    printf("%d\n%d\n%d\n", k, n, x);

    set<pair<int, int>> used;

    for (int i = 0; i < x; i++) {
        int u = rand() % n + 1;
        int v = rand() % n + 1;
        while (v == u || used.count({u, v}) || used.count({v, u})) {
            u = rand() % n + 1;
            v = rand() % n + 1;
        }
        used.insert({u, v});
        int travel_time = rand() % 10 + 1;    // 1 to 10 hours
        int travel_cost = rand() % 901 + 100; // 100 to 1000
        printf("%d %d %d %d\n", u, v, travel_time, travel_cost);
    }

    int source = rand() % n + 1;
    int dest = rand() % n + 1;
    while (dest == source)
        dest = rand() % n + 1;

    printf("%d\n%d\n", source, dest);

    return 0;
}
