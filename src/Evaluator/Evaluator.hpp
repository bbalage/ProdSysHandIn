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

struct EvalWeights
{
    double wCmax;
    double wCsum;
    double wLmax;
    double wTsum;
    double wUsum;
};

Eval analyse(const Model &model, const ModelState &modelState)
{
    Eval eval{
        .Cmax = std::numeric_limits<long>::min(),
        .Csum = 0,
        .Lmax = std::numeric_limits<long>::min(),
        .Tsum = 0,
        .Usum = 0};
    for (const Order &order : modelState.orders)
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

class Evaluator
{
public:
    Evaluator(EvalWeights evalWeights) : m_evalWeights{evalWeights} {}
    ~Evaluator() {}
    Evaluator(const Evaluator &other) = delete;
    Evaluator &operator=(const Evaluator &other) = delete;
    Evaluator(Evaluator &&other) = default;
    Evaluator &operator=(Evaluator &&other) = default;

    double targetF(const Eval &x, const Eval &y) const;

private:
    EvalWeights m_evalWeights;
};

#endif