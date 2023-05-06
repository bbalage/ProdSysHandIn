#ifndef PROD_SYS_HAND_IN_OPTIMIZER_OPTIMIZER_HPP
#define PROD_SYS_HAND_IN_OPTIMIZER_OPTIMIZER_HPP

#include <memory>

#include "../Planner/Planner.hpp"
#include "../Simulator/Simulator.hpp"
#include "../Evaluator/Evaluator.hpp"
#include "../util/output.hpp"
#include "../Model/Model.hpp"

class Optimizer
{
public:
    Optimizer() {}
    ~Optimizer() {}
    Optimizer(const Optimizer &other) = delete;
    Optimizer &operator=(const Optimizer &other) = delete;
    Optimizer(Optimizer &&other) = default;
    Optimizer &operator=(Optimizer &&other) = default;

    virtual void optimize(const Model &model,
                          const ModelState &modelState,
                          Plan &plan,
                          ModelState &out_modelState,
                          Eval &out_eval,
                          long t_ref) const = 0;
};

class OptimizerLocalSearch : public Optimizer
{
public:
    OptimizerLocalSearch(Simulator &simulator,
                         Evaluator &evaluator,
                         uint steps,
                         uint loops)
        : m_sim{simulator},
          m_eval{evaluator},
          m_steps{steps},
          m_loops{loops} {}

    void optimize(const Model &model,
                  const ModelState &modelState,
                  Plan &plan,
                  ModelState &out_modelState,
                  Eval &out_eval,
                  long t_ref) const override;

private:
    Simulator &m_sim;
    Evaluator &m_eval;
    uint m_steps;
    uint m_loops;
};

Plan mutate(const ModelState &mstate, const Plan &plan_0, long t_ref);

#endif