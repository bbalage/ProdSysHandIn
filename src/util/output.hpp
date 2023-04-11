#ifndef PROD_SYS_HAND_IN_UTIL_OUTPUT_HPP
#define PROD_SYS_HAND_IN_UTIL_OUTPUT_HPP

#include <iostream>

#include "../Evaluator/Evaluator.hpp"

void print_eval(const Eval &eval)
{
    std::cout << "Performance report:\n"
              << "Cmax: " << eval.Cmax << "\n"
              << "Csum: " << eval.Csum << "\n"
              << "Lmax: " << eval.Lmax << "\n"
              << "Tsum: " << eval.Tsum << "\n"
              << "Usum: " << eval.Usum << "\n"
              << "Max distance: " << eval.distMax << std::endl;
}

#endif