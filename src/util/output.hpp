#ifndef PROD_SYS_HAND_IN_UTIL_OUTPUT_HPP
#define PROD_SYS_HAND_IN_UTIL_OUTPUT_HPP

#include <iostream>

#include "../Evaluator/Evaluator.hpp"

inline void print_eval(const Eval &eval)
{
    std::cout << "Performance report:\n"
              << "Cmax: " << eval.Cmax << "\n"
              << "Csum: " << eval.Csum << "\n"
              << "Lmax: " << eval.Lmax << "\n"
              << "Tsum: " << eval.Tsum << "\n"
              << "Usum: " << eval.Usum << std::endl;
}

inline void print_gantt(const ModelState &mstate)
{
    for (const auto &wsOpLogs : mstate.wsOpLogs)
    {
        std::cout << "Workstation: ";
        for (const auto &wsOpLog : wsOpLogs)
        {
            std::cout << " | " << wsOpLog.job
                      << "/" << wsOpLog.op
                      << " " << wsOpLog.startTime
                      << " - " << wsOpLog.endTime;
        }
        std::cout << std::endl;
    }
}

#endif