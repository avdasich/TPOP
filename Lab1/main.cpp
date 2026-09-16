#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <omp.h>
using namespace std;

double f(double x) {
    return sqrt(x * (3 - x)) / (x + 1);
}

double simpson(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        if (i % 2 == 1)
            sum += 4 * f(x);
        else
            sum += 2 * f(x);
    }

    return (f(a) + sum + f(b)) * h / 3;
}

int main() {
    double a = 1, b = 1.2, eps = 1e-6;
    int threads[] = {2, 4, 16, 32, 64};
    ofstream file("times.txt");
    if (!file) return 1;
    omp_set_dynamic(0);
    cout << setprecision(12);
    file << setprecision(12);

    for (int p : threads) {
        omp_set_num_threads(p);
        double result = 0;
        int n = 2;
        double start = omp_get_wtime();

        for (int repeat = 0; repeat < 20; repeat++) {
            n = 2;
            double previous = simpson(a, b, n);
            while (true) {
                n *= 2;
                result = simpson(a, b, n);
                if (abs(result - previous) / 15 <= eps) break;
                previous = result;
            }
        }

        double time = (omp_get_wtime() - start) / 20;
        file << p << " " << time << "\n";
        cout << "Потоки: " << p << ", интеграл: " << result
             << ", частей: " << n << ", время: " << time << " с\n";
    }
    return 0;
}
