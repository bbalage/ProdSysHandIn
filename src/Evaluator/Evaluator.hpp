#ifndef PROD_SYS_HAND_IN_EVALUATOR_EVALUATOR_HPP
#define PROD_SYS_HAND_IN_EVALUATOR_EVALUATOR_HPP

#include "../Simulator/Simulator.hpp"
#include <limits>

struct Eval
{
    long Cmax; // completetion time
    long Csum; // sum of completion time of jobs
    long Lmax; // max of lateness
    long Tsum; // sum of tardiness of jobs (lateness but with no negative values)
    long Usum; // number of late jobs
};

Eval evaluate(const Model &model)
{
    Eval eval{
        .Cmax = std::numeric_limits<long>::min(),
        .Csum = 0,
        .Lmax = std::numeric_limits<long>::min(),
        .Tsum = 0,
        .Usum = 0};
    for (const Order &order : model.orders)
    {
        eval.Cmax = std::max(eval.Cmax, order.completionTime);
        eval.Csum += order.completionTime;
        eval.Lmax = std::max(eval.Lmax, order.lateness);
        auto T = std::max(order.lateness, 0L);
        eval.Tsum += T;
        eval.Usum += T > 0 ? 1 : 0;
    }

    return eval;
}

#endif