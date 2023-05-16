#ifndef PROD_SYS_HAND_IN_UTIL_OUTPUT_HPP
#define PROD_SYS_HAND_IN_UTIL_OUTPUT_HPP

#include <iostream>

#include "../Evaluator/Evaluator.hpp"
#include "../Simulator/AmountLogger.hpp"

inline void print_eval(std::ostream &ostream, const Eval &eval)
{
    ostream << "Performance report:\n"
            << "Cmax: " << eval.Cmax << "\n"
            << "Csum: " << eval.Csum << "\n"
            << "Lmax: " << eval.Lmax << "\n"
            << "Tsum: " << eval.Tsum << "\n"
            << "Usum: " << eval.Usum << std::endl;
}

inline void print_gantt(std::ostream &ostream, const ModelState &mstate)
{
    for (const auto &wsOpLogs : mstate.wsOpLogs)
    {
        ostream << "Workstation: ";
        for (const auto &wsOpLog : wsOpLogs)
        {
            ostream << " | " << wsOpLog.job
                    << "/" << wsOpLog.op
                    << " " << wsOpLog.startTime
                    << " - " << wsOpLog.endTime;
        }
        ostream << std::endl;
    }
}

inline void print_prodAmountLogs(std::ostream &ostream, const AmountLogs &amLogs)
{
    long t = amLogs.start;
    for (size_t i = 0; i < amLogs.prodLogs[0].size(); ++i)
    {
        ostream << "\tp" << i;
    }
    ostream << '\n';
    for (const auto &amLogAtT : amLogs.prodLogs)
    {
        ostream << t << '\t';
        for (const auto &am : amLogAtT)
        {
            ostream << am << '\t';
        }
        ostream << '\n';
        t += amLogs.freq;
    }
}

inline void print_matAmountLogs(std::ostream &ostream, const AmountLogs &amLogs)
{
    long t = amLogs.start;
    for (size_t i = 0; i < amLogs.matLogs[0].size(); ++i)
    {
        ostream << "\tm" << i;
    }
    ostream << '\n';
    for (const auto &amLogAtT : amLogs.matLogs)
    {
        ostream << t << '\t';
        for (const auto &am : amLogAtT)
        {
            ostream << am << '\t';
        }
        ostream << '\n';
        t += amLogs.freq;
    }
}

#endif