#ifndef PROD_SYS_HAND_IN_EVALUATOR_EVALUATOR_HPP
#define PROD_SYS_HAND_IN_EVALUATOR_EVALUATOR_HPP

#include "../Simulator/Simulator.hpp"
#include <limits>

struct Eval
{
    ulong Cmax;    // completetion time
    ulong Csum;    // sum of completion time of jobs
    ulong Lmax;    // max of lateness
    ulong Tsum;    // sum of tardiness of jobs (lateness but with no negative values)
    ulong Usum;    // number of late jobs
    ulong distMax; // maximal distance travelled by any agent
};

Eval evaluate(const Model &model, const Report &report)
{
    Eval eval{
        .Cmax = std::numeric_limits<ulong>::min(),
        .Csum = 0,
        .Lmax = std::numeric_limits<ulong>::min(),
        .Tsum = 0,
        .Usum = 0,
        .distMax = std::numeric_limits<ulong>::min()};
    // for (uint i = 0; i < model.cargos.size(); ++i)
    // {
    //     const auto &job = model.cargos[i];
    //     eval.Cmax = std::max(job.deliveryTime, eval.Cmax);
    //     eval.Csum += job.deliveryTime;
    //     const auto L = job.deliveryTime - job.dueDelivery;
    //     if (eval.Lmax < L)
    //         eval.Lmax = L;
    //     const auto T = L > 0.0 ? L : 0.0;
    //     eval.Tsum += T;
    //     if (T != 0.0)
    //         ++eval.Usum;
    // }
    // eval.distMax = std::max_element(model.agents.begin(), model.agents.end(), [](const auto &a1, const auto &a2)
    //                                 { return a1.distTravelled < a2.distTravelled; })
    //                    ->distTravelled;
    return eval;
}

#endif