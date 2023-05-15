#ifndef PROD_SYS_HAND_IN_UTIL_OUTPUT_HPP
#define PROD_SYS_HAND_IN_UTIL_OUTPUT_HPP

#include <iostream>

#include "../Evaluator/Evaluator.hpp"
#include "../Simulator/AmountLogger.hpp"

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

inline void print_prodAmountLogs(const AmountLogs &amLogs)
{
    long t = amLogs.start;
    for (size_t i = 0; i < amLogs.prodLogs[0].size(); ++i)
    {
        std::cout << "\tp" << i;
    }
    std::cout << '\n';
    for (const auto &amLogAtT : amLogs.prodLogs)
    {
        std::cout << t << '\t';
        for (const auto &am : amLogAtT)
        {
            std::cout << am << '\t';
        }
        std::cout << '\n';
        t += amLogs.freq;
    }
}

inline void print_matAmountLogs(const AmountLogs &amLogs)
{
    long t = amLogs.start;
    for (size_t i = 0; i < amLogs.matLogs[0].size(); ++i)
    {
        std::cout << "\tm" << i;
    }
    std::cout << '\n';
    for (const auto &amLogAtT : amLogs.matLogs)
    {
        std::cout << t << '\t';
        for (const auto &am : amLogAtT)
        {
            std::cout << am << '\t';
        }
        std::cout << '\n';
        t += amLogs.freq;
    }
}

#endif