#include <climits>
#include <cstdlib>
#include <iostream>

#define N 100

// generates a random integer from 0 to floor(LONG_MAX / 2)
long randl() {
    double r = static_cast<double>(std::rand()) / RAND_MAX;
    return static_cast<long>(r * (LONG_MAX / 2));
}

long add(long x, long y)
{
    return x + y;
}

int main()
{
    long x[N], y[N], r[N];

    for (int i = 0; i < N; ++i) {
        x[i] = randl();
        y[i] = randl();
    }

    for (int i = 0; i < N; ++i)
    {
        r[i] = add(x[i], y[i]);
    }

    for (int i = 0; i < N; ++i) {
        std::cout << r[i] << '\n';
    }
    std::cout << std::flush;
}