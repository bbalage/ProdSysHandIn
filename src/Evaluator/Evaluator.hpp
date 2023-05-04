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

Eval analyse(const Model &model, const ModelState &modelState);

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