#include "util.hpp"

uint randBtw(uint min, uint max)
{
    if (min == max)
        return 0;
    return min + rand() % (max - min);
}