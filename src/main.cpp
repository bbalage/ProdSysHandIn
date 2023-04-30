#include <iostream>
#include <vector>

#include "Planner/Planner.hpp"
#include "Simulator/Simulator.hpp"
#include "Evaluator/Evaluator.hpp"
#include "util/output.hpp"
#include "Model/Model.hpp"

int main(int argc, char **argv)
{
    // Initialize
    // std::mt19937 rng(time(0));
    srand(time(0));
    constexpr long t_frozen = 10;

    // Build model
    PlannerSimple planner(t_frozen);
    SimulatorSimple simulator;
    auto model = generateRandomModel();
    long t_ref = 0;

    ModelState mstate{.wsOpLogs = std::vector<std::vector<WSOpLog>>(model.workstations.size())};

    // Receive order...
    // e2e test:
    // generate 2 orders
    auto orders = generateOrders(5, model.products.size());
    // Plan, simulate
    Plan plan = planner.plan(model, mstate, orders, t_ref);
    simulator.simulate(model, mstate, plan);

    // advance time
    // generate 3 more orders
    // advance time

    // Evaluate
    Eval eval = analyse(model, mstate);

    // Output
    print_eval(eval);
}