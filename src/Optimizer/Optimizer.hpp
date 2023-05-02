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
                          long t_ref) const = 0;
};

class OptimizerLocalSearch : public Optimizer
{
public:
    OptimizerLocalSearch(std::unique_ptr<Simulator> simulator,
                         std::unique_ptr<Evaluator> evaluator,
                         uint steps,
                         uint loops)
        : m_sim{std::move(simulator)},
          m_eval{std::move(evaluator)},
          m_steps{steps},
          m_loops{loops} {}

    void optimize(const Model &model,
                  const ModelState &modelState,
                  Plan &plan,
                  long t_ref) const override;

private:
    std::unique_ptr<Simulator> m_sim;
    std::unique_ptr<Evaluator> m_eval;
    uint m_steps;
    uint m_loops;
};

#endif