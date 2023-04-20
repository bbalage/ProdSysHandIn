#include "util.hpp"

uint randBtw(uint min, uint max)
{
    return min + rand() % (max - min);
}