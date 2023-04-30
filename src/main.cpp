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

    // Build model
    auto model = generateRandomModel();
    ModelState mstate{.orders = generateOrders(5, model.products.size()),
                      .wsOpLogs = std::vector<std::vector<WSOpLog>>(model.workstations.size())};

    // Plan
    PlannerSimple planner;
    Plan plan = planner.plan(model, mstate);

    // Simulate
    SimulatorSimple simulator;
    simulator.simulate(model, mstate, plan);

    // Evaluate
    Eval eval = analyse(model, mstate);

    // Output
    print_eval(eval);
}